


#include <rtk/switch.h>
#include <rtk/port.h>
#include <rtk/led.h>
#include <rsd/rsd_port.h>
#include <rtk/stat.h>
#include <board/board.h>
#include <board/poe/poe.h>
#include <board/poe/poe_tps23861.h>
#include <board/poe/poe_rtl8238b.h>
#include <board/poe/poe_rtl8238.h>
#include <board/poe/poe_rtl8239.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <ksi/ksi_core.h>
#include <ksi_core_usr.h>
#ifdef CONFIG_SYS_PLAT_LXKSI
#include <ksi_port.h>
#endif


#if 0
#define POE_DEBUG(a,args...) printk("\n[PoE]%d:"#a"\n", __LINE__, ##args)
#else
#define POE_DEBUG(a,args...){}
#endif
#define WD_DEBUG(a,args...) printk("\n[PoE]%d:"#a"\n", __LINE__, ##args)


static int g_poeDevNum = 0;
static uint32 g_portNum = 0;
static uint32 g_ledEntryNum = 0xFFFF;
static uint32 g_hour = 0;
static uint32 g_sysTimeTZ = 480;
static uint32 g_poeSchedule[POE_MAXPORT_NUM][POE_SCHEDULE_WEEK][POE_SCHEDULE_HOUR] = {{{0}}};
static uint32 g_portMap[POE_MAXPORT_NUM] = {0};
static int g_poeStartPort = 0;

sys_poe_portData_t g_pPortData[POE_DEV_NUM][POE_DEV_PORT];
sys_poe_devConf_t *g_pPoeDevConf = NULL;
sys_poe_portConf_t *g_pPoePortConf = NULL;
sys_poe_portDrv_t *g_pPoePortDrv = NULL;

sys_logic_port_t phy2lPort[POE_DEV_NUM][POE_DEV_PORT];
sys_physic_port_t logic2phyPort[POE_MAXPORT_NUM];

uint32 chip_i2c_dev[POE_DEV_MAX] = {0};
uint32 chip_i2c_dev_two[POE_DEV_MAX] = {0};

sys_gpio_t poe_ip808ResetGpio =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID1,   
    .pin               = 11,                   
    .pinConf = {
        .direction     = GPIO_DIR_OUT,         
        .default_value = GPIO_ACTIVE_HIGH,
    },
    .active            = GPIO_ACTIVE_LOW,
};


extern uint32 poe_tps_23861_get(uint32 devNum);
extern uint32 poe_tps_23880_get(uint32 devNum);
extern uint32 poe_tps_23882_get(uint32 devNum);
extern uint32 poe_tps_23861_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data);
extern uint32 poe_tps_23880_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data);
extern uint32 poe_tps_23882_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data);
extern uint32 board_poe_23861_init(uint32 index, uint32 deviceNum, uint32 globalDev, uint32 tempDev);
extern uint32 board_poe_23880_init(uint32 index);
extern uint32 board_poe_23882_init(uint32 index);
extern uint32 poe_mp_3924_get(uint32 devNum);
extern uint32 poe_mp_3924_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data);
extern uint32 board_poe_mp3924_init(uint32 index);
extern uint32 poe_ip_808_get(uint32 devNum);
extern uint32 poe_ip_808_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data);
extern uint32 board_poe_ip808_init(uint32 index);


int32 poe_logic_to_physic_port(sys_logic_port_t port, sys_physic_port_t *pPhysicPort)
{
    if( 0 > port || port >=  g_portNum)
        return -1;

    osal_memset(pPhysicPort, 0, sizeof(sys_physic_port_t));

    pPhysicPort->dev = logic2phyPort[port].dev;
    pPhysicPort->port = logic2phyPort[port].port;
    

    return 0;
}

int creat_poe_ready_file(char *name)
{
    struct file *filep;
    filep=filp_open(name,O_CREAT | O_RDWR,0);
    if(IS_ERR(filep))
        return -1;
    filp_close(filep,0);
    return 0;
}

int check_system_ready_file(char *name)
{
    struct file *filep;
    filep=filp_open(name, O_RDONLY,0);
    if(IS_ERR(filep))
        return -1;
    filp_close(filep,0);
    return 0;
}

poe_time_t time1addtime2(poe_time_t stime, poe_time_t dtime)
{
    poe_time_t tmp;

    memset(&tmp, 0, sizeof(poe_time_t));

    tmp.second = stime.second + dtime.second;
    if(tmp.second > 59)
    {
        tmp.minute += 1;
        tmp.second -= 60;
    }
    tmp.minute += stime.minute + dtime.minute;
    if(tmp.minute > 59)
    {
        tmp.hour += 1;
        tmp.minute -= 60;
    }
    tmp.hour += stime.hour + dtime.hour;
    if(tmp.hour > 23)
    {
        tmp.hour -= 24;
        tmp.day += 1;
    }
    return tmp;
}

uint32 poe_time_cmp(poe_time_t stime, poe_time_t dtime)
{
    if(stime.hour == 0 && stime.minute == 0 && stime.second == 0)
    {
        return 1;
    }
    if(stime.hour == dtime.hour && stime.minute == dtime.minute)
    {
        return 0;
    }
    return 1;
}
#ifdef CONFIG_SYS_POE_LED
board_monitor_cb poe_led_detect_monitor(void *pParam)
{
    uint32 lp = 0;
    uint32 data = 0;
    static uint32 delayCount = 0;
    rtk_led_swCtrl_mode_t swMode = 0;
    rtk_enable_t  enable;
    uint32 led_entity = 0;

    if ((delayCount++ % 2) != 0)
    {
        return SYS_ERR_OK;
    }

    if(g_ledEntryNum == 0xFFFF)
    {
        return SYS_ERR_OK;
    }
    else
    {
        led_entity = g_ledEntryNum;
    }


    for (lp = 0; lp < g_portNum; lp++)
    {
        board_poe_msg_get(lp, POE_MSG_GET_PORT_PWGOOD, &data);
        rsd_port_led_entityswCtrl_mode_get(lp, led_entity, PORT_MEDIA_COPPER, &swMode);

        if ((1 == data) && (RTK_LED_SWCTRL_MODE_BLINKING_CONTINUE != swMode))
        {
            rsd_port_led_entityswCtrl_mode_set(lp, led_entity, PORT_MEDIA_COPPER, RTK_LED_SWCTRL_MODE_BLINKING_CONTINUE);
        }
        else if ((0 == data) && (RTK_LED_SWCTRL_MODE_OFF != swMode))
        {
            rsd_port_led_entityswCtrl_mode_set(lp, led_entity, PORT_MEDIA_COPPER, RTK_LED_SWCTRL_MODE_OFF);
        }

        if((delayCount % 3) == 0)
        {
            rsd_port_led_entityswCtrl_enable_get(lp, led_entity, &enable);
            if (ENABLED != enable)
                rsd_port_led_entityswCtrl_enable_set(lp, led_entity, ENABLED);
        }
        rsd_led_swCtrl_start(SYS_DEV_ID_DEFAULT);
    }

    return SYS_ERR_OK;
}
#endif
#ifdef CONFIG_SYS_POE_SWDOG

board_monitor_cb poe_watch_dog_monitor(void *pParam)
{
    uint32 i = 0;
    uint32 data = 0;
    uint64 conter64 = 0;
    uint32 portstatus = 0;
    uint32 portMachine = 0;
    static uint32 delayCount = 0;
	sys_physic_port_t phyPort;

    if ((delayCount++ % POE_WATCH_SCAN_TIME) != 0)
    {
        return SYS_ERR_OK;
    }

    for (i = 0, portMachine = 0; i < g_portNum; i++)
    {
        portMachine = 0;
        sys_logic_to_physic_port(i, &phyPort);
        board_poe_msg_get(i, POE_MSG_GET_PORT_WATCH_DOG_ENABLE, &data);
        if(!data)
        {
            continue;
        }

        board_poe_msg_get(i, POE_MSG_GET_PORT_PWGOOD, &data);
        if(data == 0)
        {
            
            if(g_pPoePortConf[i].portPoeWatchDog.rebootFlag == 1)
            {
                board_poe_msg_set(i, POE_MSG_SET_PORT_ENABLE, 1);
                osal_time_sleep(4);
                board_poe_msg_set(i, POE_MSG_SET_PORT_ENABLE, 1);
                g_pPoePortConf[i].portPoeWatchDog.rebootFlag = 0;
                g_pPoePortConf[i].portPoeWatchDog.scanTimes ++;
                
            }
            else
            {
                g_pPoePortConf[i].portPoeWatchDog.rebootTimes = 0;
                g_pPoePortConf[i].portPoeWatchDog.stateMachine = portMachine;
                g_pPoePortConf[i].portPoeWatchDog.scanTimes = 0;
                
            }
            continue;
        }

        portMachine |= POE_WATCH_MACHINE_POWER;

        if((rtk_port_link_get(phyPort.dev, phyPort.port, &portstatus) == RT_ERR_OK) && portstatus)
        {
            
            portMachine |= POE_WATCH_MACHINE_UP;

            rtk_stat_port_get(phyPort.dev, phyPort.port, IF_HC_IN_OCTETS_INDEX, &conter64);
            if(g_pPoePortConf[i].portPoeWatchDog.ingStat != conter64)
            {
                g_pPoePortConf[i].portPoeWatchDog.ingStat = conter64;
                portMachine |= POE_WATCH_MACHINE_FLOW ;
                POE_DEBUG(" status=(%d) poe port=(%d) flow conter2=(%d)", portMachine, i, (int)conter64);
            }
        }

        if(portMachine == POE_WATCH_DOG_011)
        {
            if(g_pPoePortConf[i].portPoeWatchDog.stateMachine == POE_WATCH_DOG_111)
            {
            }
            else if(g_pPoePortConf[i].portPoeWatchDog.stateMachine == POE_WATCH_DOG_011)
            {
                g_pPoePortConf[i].portPoeWatchDog.scanTimes ++;
                if(g_pPoePortConf[i].portPoeWatchDog.scanTimes % (POE_WATCH_REBOOT_INTERVAL / POE_WATCH_SCAN_TIME - 1) == 0)
                {
                    POE_DEBUG(" status=(0x011) poe port=(%d) 2 min reboot scanTimes=%d", i, g_pPoePortConf[i].portPoeWatchDog.scanTimes);
                    board_poe_msg_set(i, POE_MSG_SET_PORT_ENABLE, 0);
                    g_pPoePortConf[i].portPoeWatchDog.rebootFlag = 1;
                }
            }
        }

        if(portMachine == POE_WATCH_DOG_001)
        {
            if(g_pPoePortConf[i].portPoeWatchDog.stateMachine == POE_WATCH_DOG_111)
            {
                g_pPoePortConf[i].portPoeWatchDog.rebootTimes ++;
                POE_DEBUG(" status=(0x001) poe port=(%d) reboot scanTimes=%d", i, g_pPoePortConf[i].portPoeWatchDog.scanTimes);
            }
            else if(g_pPoePortConf[i].portPoeWatchDog.stateMachine == POE_WATCH_DOG_001 && g_pPoePortConf[i].portPoeWatchDog.rebootTimes)
            {
                
                g_pPoePortConf[i].portPoeWatchDog.scanTimes ++;
                if(g_pPoePortConf[i].portPoeWatchDog.scanTimes % (POE_WATCH_REBOOT_INTERVAL / POE_WATCH_SCAN_TIME - 1) == 0)
                {
                    if(g_pPoePortConf[i].portPoeWatchDog.rebootTimes > POE_WATCH_REBOOT_TIMES)
                    {
                        g_pPoePortConf[i].portPoeWatchDog.rebootFlag = 0;
                        continue;
                    }
                    POE_DEBUG(" status=(0x001) poe port=(%d) 2 min reboot; rebootTime=(%d) scanTimes=%d", i, g_pPoePortConf[i].portPoeWatchDog.rebootTimes, g_pPoePortConf[i].portPoeWatchDog.scanTimes);
                    board_poe_msg_set(i, POE_MSG_SET_PORT_ENABLE, 0);
                    g_pPoePortConf[i].portPoeWatchDog.rebootTimes ++;
                    g_pPoePortConf[i].portPoeWatchDog.rebootFlag = 1;
                }
            }
        }

        if(portMachine == POE_WATCH_DOG_111)
        {
            g_pPoePortConf[i].portPoeWatchDog.scanTimes = 0;
            g_pPoePortConf[i].portPoeWatchDog.rebootTimes = 0;
            g_pPoePortConf[i].portPoeWatchDog.rebootFlag = 0;
            POE_DEBUG(" status=(0x111) poe port=(%d) recovery", i);
        }
        g_pPoePortConf[i].portPoeWatchDog.stateMachine = portMachine;
    }

    return SYS_ERR_OK;
}
#endif

uint32 poe_port_enable_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portEnable;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_pwgood_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portPwGood;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_status_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portStatus;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_classsts_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portClassSts;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_detclass_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portDetClass;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_current_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portCurrent;

    if(g_pPoePortConf[logicPort].mode == PSE_MODE_BT)
    {
        value += g_pPortData[phyPort.dev][phyPort.port+1].portCurrent;
    }
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_voltage_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portVoltage;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_wdogen_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portWDogEn;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}

uint32 poe_port_temperature_get(uint32 logicPort)
{
    uint32 value = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;

    value = g_pPortData[phyPort.dev][phyPort.port].portTemperature;
    POE_DEBUG("d:%u-p:%u~l:%u get v:%u", phyPort.dev, phyPort.port, logicPort, value);

    return value;
}


int32 board_poe_msg_get(uint32 logicPort, sys_poe_msg_t cmd, uint32 *pData)
{
    switch (cmd)
    {
        case POE_MSG_GET_PORT_ENABLE:
            *pData = poe_port_enable_get(logicPort);
            break;

        case POE_MSG_GET_PORT_ABILITY:
            *pData = 1;
            break;

        case POE_MSG_GET_PORT_PWGOOD:
            *pData = poe_port_pwgood_get(logicPort);
            break;

        case POE_MSG_GET_PORT_STATUS:
            *pData = poe_port_status_get(logicPort);
            break;

        case POE_MSG_GET_PORT_CURRENT:
            *pData = poe_port_current_get(logicPort);
            break;

        case POE_MSG_GET_PORT_VOLTAGE:
            *pData = poe_port_voltage_get(logicPort);
            break;

        case POE_MSG_GET_PORT_ICUT:
            break;

        case POE_MSG_GET_DEV_POWER_STATUS:
            break;

        case POE_MSG_GET_DEV_INTR_STATUS:
            break;

        case POE_MSG_GET_DEV_CLASS_STATUS:
            *pData = poe_port_classsts_get(logicPort);
            break;

        case POE_MSG_GET_DEV_DETECT_STATUS:
            *pData = poe_port_detclass_get(logicPort);
            break;

        case POE_MSG_GET_DEV_DISC_STATUS:
            break;

        case POE_MSG_GET_DEV_TEMPERATURE:
            *pData = poe_port_temperature_get(logicPort);
            break;

#ifdef CONFIG_SYS_POE_SWDOG
        case POE_MSG_GET_PORT_WATCH_DOG_ENABLE:
            *pData = poe_port_wdogen_get(logicPort);
            break;
#endif
        default:
            break;
    }

    return SYS_ERR_OK;
}


int32 board_poe_msg_set(uint32 logicPort, sys_poe_msg_t cmd, uint32 Data)
{
    
    
    uint32 weekday = 0;
    uint32 j;
    uint32 tmpVal = 0;
    sys_physic_port_t phyPort;

    if(poe_logic_to_physic_port(logicPort, &phyPort) != 0)
        return -1;
    POE_DEBUG("dev:%u port:%u lport:%u", phyPort.dev, phyPort.port, logicPort);

    switch (cmd)
    {
        case POE_MSG_SET_PORT_ENABLE:
            g_pPoePortDrv[logicPort].poe_portEnable_set(phyPort.dev, phyPort.port, Data);
            break;
        case POE_MSG_SET_PORT_ICUT:
            break;
        case POE_MSG_SET_PORT_SCHEDULE:
            tmpVal = Data;

            weekday = tmpVal >> 24;
            for (j = 0; j < 24; j++)
            {
                g_pPoePortConf[logicPort].poeTime.portSchedule[weekday][j] = (tmpVal >> j) & 0x1;
            }
            break;
        case POE_MSG_SET_TIMEZONE:
            break;
        case POE_MSG_SET_PORT_POEP:
            break;
        case POE_MSG_SET_PORT_MODE:
            break;
        case POE_MSG_SET_PORT_WATCH_DOG_ENABLE:
            g_pPortData[phyPort.dev][phyPort.port].portWDogEn = Data;
            break;
        case POE_MSG_SET_PORT_TIME_OPEN:
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.openTime.hour =  tmpVal >> 24;
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.openTime.minute =(tmpVal >> 16) & 0x00FF;
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.openTime.second =tmpVal & 0x000000FF;
            g_pPoePortConf[logicPort].poeTime.openTime = time1addtime2(g_pPoePortConf[logicPort].poeTime.closeTime, g_pPoePortConf[logicPort].poeTime.openTime);
            break;
        case POE_MSG_SET_PORT_TIME_CLOSE:
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.closeTime.hour =  tmpVal >> 24;
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.closeTime.minute =(tmpVal >> 16) & 0x00FF;
            tmpVal = Data;
            g_pPoePortConf[logicPort].poeTime.closeTime.second = tmpVal & 0x000000FF;
            break;
        default:
            break;
    }
    return 0;
}


void board_poe_time_thread(void *pParam)
{
    struct timex txc;
    struct rtc_time tm;
    int i = 0;
    uint32 oldVal = 0;
    poe_time_t cur_time;
    uint32 enable;
    
    for (;;)
    {
        do_gettimeofday(&(txc.time));
        rtc_time_to_tm(txc.time.tv_sec + 60*g_sysTimeTZ, &tm);
        if (g_hour < tm.tm_hour)
        {
            g_hour = tm.tm_hour;
            
        }
        cur_time.hour = tm.tm_hour;
        cur_time.minute = tm.tm_min;
        cur_time.second = tm.tm_sec;
        for (i = 0; i < g_portNum; i++)
        {
            board_poe_msg_get(i, POE_MSG_GET_PORT_ENABLE, &enable);
            if(enable == 0 && g_pPoePortConf[i].poeTime.portstat == 0 )
            {
                
                continue;
            }


            
            oldVal = enable;
            if(1 == oldVal)
            {
                if(0 == g_pPoePortConf[i].poeTime.portSchedule[tm.tm_wday][tm.tm_hour])
                {
                    board_poe_msg_set(g_pPoePortConf[i].port, POE_MSG_SET_PORT_ENABLE, 0);
                    g_pPoePortConf[i].poeTime.portstat = 1;
                    POE_DEBUG("  port= %d status=%d portSchedule close ", i, oldVal);
                }
                if(0 == poe_time_cmp(g_pPoePortConf[i].poeTime.closeTime, cur_time))
                {
                    board_poe_msg_set(g_pPoePortConf[i].port, POE_MSG_SET_PORT_ENABLE, 0);
                    g_pPoePortConf[i].poeTime.portstat = 1;
                    g_pPoePortConf[i].poeTime.rebootflag = 1;
                    POE_DEBUG("  port= %d status=%d reboot close ", i, oldVal);
                }
            }
            else if(1 != oldVal)
            {
                if(1 == g_pPoePortConf[i].poeTime.portSchedule[tm.tm_wday][tm.tm_hour] && g_pPoePortConf[i].poeTime.rebootflag == 0)
                {
                    board_poe_msg_set(g_pPoePortConf[i].port, POE_MSG_SET_PORT_ENABLE, 1);
                    g_pPoePortConf[i].poeTime.portstat = 0;
                    POE_DEBUG("  port= %d status=%d portSchedule open ", i, oldVal);
                }
                if(0 == poe_time_cmp(g_pPoePortConf[i].poeTime.openTime, cur_time) && g_pPoePortConf[i].poeTime.rebootflag == 1)
                {
                    board_poe_msg_set(g_pPoePortConf[i].port, POE_MSG_SET_PORT_ENABLE, 1);
                    g_pPoePortConf[i].poeTime.portstat = 0;
                    g_pPoePortConf[i].poeTime.rebootflag = 0;
                    POE_DEBUG("  port= %d status=%d reboot open", i, oldVal);
                }
            }
        }
        
        osal_time_sleep(50);    
    }
    osal_thread_exit(0);
}

int32 board_poe_time_init(void)
{
    if ((osal_thread_t)NULL == (osal_thread_create("Borad POE Thread",
                                                     32768,
                                                     DFLT_THREAD_PRI_BOARD_EXTGPIO,
                                                     (void *)board_poe_time_thread,
                                                     NULL)))
    {
        SYS_PRINTF("Board POE Thread create failed\n");
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}
void board_poe_chip_monitor_thread(void *pParam)
{
    uint32 i = 0;

    while(check_system_ready_file("/var/system_ready") == -1)
    {
        osal_time_sleep(3);
    }

    
    for (;;)
    {
        for (i = 0; i < g_poeDevNum; i++)
        {
            switch(g_pPoeDevConf[i].devType)
            {
                case PSE_TYPE_TI23861:
                    poe_tps_23861_get(i);
                    break;
                case PSE_TYPE_TI23880:
                    poe_tps_23880_get(i);
                    break;
                case PSE_TYPE_TI23882:
                    poe_tps_23882_get(i);
                    break;
                case PSE_TYPE_RTL8238B_L:
                case PSE_TYPE_RTL8238B_H:
                    poe_rtl_8238b_get(i);
                    break;
                case PSE_TYPE_RTL8238:
                    poe_rtl_8238_get(i);
                    break;
                case PSE_TYPE_RTL8239_L:
                case PSE_TYPE_RTL8239_H:
                    poe_rtl8239_get(i);
                    break;
                case PSE_TYPE_MP3924:
                    poe_mp_3924_get(i);
                    break;
                case PSE_TYPE_IP808:
                    poe_ip_808_get(i);
                    break;
                default:
                    break;
            }
            if(i % 3 == 2)
            {
                osal_time_sleep(1);
            }
            else
            {
                if(g_poeDevNum <= 2)
                {
                    osal_time_sleep(1);
                }
            }
        }
    }
    osal_thread_exit(0);
}

int32 board_poe_chip_monitor_init(void)
{
    if ((osal_thread_t)NULL == (osal_thread_create("PSE Chip Monitor",
                                                     32768,
                                                     DFLT_THREAD_PRI_BOARD_EXTGPIO,
                                                     (void *)board_poe_chip_monitor_thread,
                                                     NULL)))
    {
        SYS_PRINTF("Board POE Thread create failed\n");
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int board_poe_config_init(void)
{
    sys_board_t *pBoardConf = NULL;
    sys_poe_config_t *pPoeConf = NULL;
    int i;

    SYS_ERR_CHK(board_conf_get(&pBoardConf));
    pPoeConf = (sys_poe_config_t *) pBoardConf->pPoeConf;
    if (NULL == pPoeConf)
        return SYS_ERR_FAILED;
    g_pPoeDevConf = (sys_poe_devConf_t *) pPoeConf->pPoeDevConf;
    g_pPoePortConf = (sys_poe_portConf_t *) pPoeConf->pPoePortConf;

    g_poeDevNum = pPoeConf->poeDevNum;
    g_portNum = pPoeConf->portNum;
    g_poeStartPort = pPoeConf->startport;
    g_ledEntryNum = pPoeConf->poeLedEntity;

    for(i = 0; i < g_portNum; i++)
    {
        g_portMap[i] = i + g_poeStartPort;
    }

    return SYS_ERR_OK;
}

uint32 board_poe_port_init(void)
{
    uint32 i = 0;
    int devIdx =0, port = 0;

    for(devIdx = 0; devIdx < POE_DEV_NUM; devIdx ++)
    {
        for(port = 0; port < POE_DEV_PORT; port++)
        {
            phy2lPort[devIdx][port] = 0xFFFF;
            memset(&g_pPortData[devIdx][port], 0, sizeof(sys_poe_portData_t));
        }
    }

    for(port = 0; port < POE_DEV_PORT; port++)
    {
        logic2phyPort[port].dev = 0xFFFF;
        logic2phyPort[port].port = 0xFFFF;
    }

    for(i = 0; i < g_portNum; i++)
    {
        logic2phyPort[i].dev = g_pPoePortConf[i].devNum;
        logic2phyPort[i].port = g_pPoePortConf[i].devPort-1;
        phy2lPort[logic2phyPort[i].dev][logic2phyPort[i].port] = i;
        POE_DEBUG("dev:%u port:%u lport:%u", logic2phyPort[i].dev, logic2phyPort[i].port, phy2lPort[logic2phyPort[i].dev][logic2phyPort[i].port]);
    }

    g_pPoePortDrv = osal_alloc(sizeof(sys_poe_portDrv_t)*(g_portNum + 1));
    if(NULL == g_pPoePortDrv)
    {
        return SYS_ERR_FAILED;
    }
    memset(g_pPoePortDrv, 0, sizeof(sys_poe_portDrv_t)*(g_portNum + 1));

    for(i = 0; i < g_portNum; i++)
    {
        g_pPoePortDrv[i].poe_portIndex = i;
        switch(g_pPoeDevConf[g_pPoePortConf[i].devNum].devType)
        {
            case PSE_TYPE_TI23861:
                g_pPoePortDrv[i].poe_portEnable_set = poe_tps_23861_portEnable_set;
                break;
            case PSE_TYPE_TI23880:
                g_pPoePortDrv[i].poe_portEnable_set = poe_tps_23880_portEnable_set;
                break;
            case PSE_TYPE_TI23881:
                break;
            case PSE_TYPE_TI23882:
                g_pPoePortDrv[i].poe_portEnable_set = poe_tps_23882_portEnable_set;
                break;
            case PSE_TYPE_RTL8238B_L:
            case PSE_TYPE_RTL8238B_H:
                g_pPoePortDrv[i].poe_portEnable_set = poe_rtl_8238b_portEnable_set;
                break;
            case PSE_TYPE_RTL8239_L:
            case PSE_TYPE_RTL8239_H:
                if(g_pPoePortConf[i].mode == PSE_MODE_BT)
                    g_pPoePortDrv[i].poe_portEnable_set = poe_rtl8239_BT_portEnable_set;
                else
                    g_pPoePortDrv[i].poe_portEnable_set = poe_rtl8239_portEnable_set;
                break;
            case PSE_TYPE_RTL8238:
                g_pPoePortDrv[i].poe_portEnable_set = poe_rtl_8238_portEnable_set;
                break;
            case PSE_TYPE_MP3924:
                g_pPoePortDrv[i].poe_portEnable_set = poe_mp_3924_portEnable_set;
                break;
            case PSE_TYPE_IP808:
                g_pPoePortDrv[i].poe_portEnable_set = poe_ip_808_portEnable_set;
                break;
            default:
                break;
        }
    }
    return SYS_ERR_OK;
}


int32 board_poe_init(void)
{
    uint32 i;
    uint32 ret = 0;
    uint32 globalDev = 0;
    uint32 tempDev = 0;
    uint32 daisyChainNum = 0;
    uint32 rtl8238BNum = 0;
    uint32 rtl8238Num = 0;
    uint32 rtl8239Num = 0;

    ret = board_poe_config_init();
    if(ret != SYS_ERR_OK)
        return SYS_ERR_OK;


    osal_memset(g_poeSchedule, 0, sizeof(g_poeSchedule));

    for (i = 0; g_pPoeDevConf[i].devType != PSE_TYPE_END; i++)
    {
        i2c_dev_init(&g_pPoeDevConf[i].i2c);

        if(g_pPoeDevConf[i].devNum == 254)
        {
            globalDev = i;
        }
        else if(g_pPoeDevConf[i].devNum == 255)
        {
            tempDev = i;
        }
        else
        {
            if(PSE_TYPE_TI23861 == g_pPoeDevConf[i].devType)
            {
                daisyChainNum++;
            }

            if(PSE_TYPE_RTL8238B_L == g_pPoeDevConf[i].devType)
            {
                chip_i2c_dev[rtl8238BNum] = i;
                rtl8238BNum++;
            }

            if(PSE_TYPE_RTL8239_L == g_pPoeDevConf[i].devType)
            {
                chip_i2c_dev_two[rtl8239Num] = i;
                rtl8239Num++;
            }

            if(PSE_TYPE_RTL8238 == g_pPoeDevConf[i].devType)
            {
                rtl8238Num++;
            }
        }
    }

    if(rtl8238BNum)
    {
        ret = board_poe_rtl8238b_firmware_init(chip_i2c_dev, rtl8238BNum, globalDev);
        if(ret != SYS_ERR_OK)
            return SYS_ERR_OK;
    }

    if(rtl8239Num)
    {
        board_poe_rtl8239_firmware_init(chip_i2c_dev_two, rtl8239Num, tempDev);
        if(ret != SYS_ERR_OK)
            return SYS_ERR_OK;
    }

    for (i = 0; i < g_poeDevNum; i++)
    {
        switch(g_pPoeDevConf[i].devType)
        {
            case PSE_TYPE_TI23861:
                board_poe_23861_init(i, daisyChainNum, globalDev, tempDev);
                i = i + daisyChainNum;
                break;
            case PSE_TYPE_TI23880:
                board_poe_23880_init(i);
                break;
            case PSE_TYPE_TI23881:
                break;
            case PSE_TYPE_TI23882:
                board_poe_23882_init(i);
                break;
            case PSE_TYPE_RTL8238B_L:
            case PSE_TYPE_RTL8238B_H:
                board_poe_rtl8238b_init(i);
                break;

            case PSE_TYPE_RTL8238:
                ret = board_poe_rtl8238_init(i, rtl8238Num);
                if(ret != SYS_ERR_OK)
                    return SYS_ERR_OK;
                break;
            case PSE_TYPE_RTL8239_L:
            case PSE_TYPE_RTL8239_H:
                ret = board_poe_rtl8239_init(i);
                if(ret != SYS_ERR_OK)
                    return SYS_ERR_OK;
                break;
            case PSE_TYPE_MP3924:
                ret = board_poe_mp3924_init(i);
                if(ret != SYS_ERR_OK)
                    return SYS_ERR_OK;
                break;
            case PSE_TYPE_IP808:
                {
                    gpio_init(&poe_ip808ResetGpio);
                    osal_time_mdelay(50);
                    gpio_data_set(&poe_ip808ResetGpio, 1);
                    osal_time_mdelay(100);
                    gpio_data_set(&poe_ip808ResetGpio, 0);
                    osal_time_mdelay(100);
                    gpio_data_set(&poe_ip808ResetGpio, 1);
                }
                ret = board_poe_ip808_init(i);
                if(ret != SYS_ERR_OK)
                    return SYS_ERR_OK;
                break;
            default:
                break;
        }
    }

    if (ret == 0)
    {
        SYS_ERR_CHK(board_poe_port_init());

        SYS_ERR_CHK(board_poe_chip_monitor_init());

    #ifdef CONFIG_SYS_POE_LED
        board_monitor_register((board_monitor_cb)poe_led_detect_monitor);
    #endif
    #ifdef CONFIG_SYS_POE_SWDOG
        memset(&g_pPoePortConf->portPoeWatchDog, 0, sizeof(poe_watch_dog_t));
        board_monitor_register((board_monitor_cb)poe_watch_dog_monitor);
    #endif
        memset(&g_pPoePortConf->poeTime, 0, sizeof(poe_timed_reboot_t));
        SYS_ERR_CHK(board_poe_time_init());
    }

    creat_poe_ready_file("/var/system_POE_ready");
    return SYS_ERR_OK;
}

