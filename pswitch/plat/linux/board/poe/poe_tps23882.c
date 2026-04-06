


#include <rtk/switch.h>
#include <rtk/port.h>
#include <rtk/led.h>
#include <rsd/rsd_port.h>
#include <rtk/stat.h>
#include <board/poe/poe.h>
#include <board/poe/poe_tps23861.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <ksi/ksi_core.h>
#include <ksi_core_usr.h>
#ifdef CONFIG_SYS_PLAT_LXKSI
#include <ksi_port.h>
#endif


extern int g_portPoeEnable[POE_MAXPORT_NUM];
extern sys_poe_devConf_t *g_pPoeDevConf;
extern sys_poe_portConf_t *g_pPoePortConf;
extern sys_poe_portData_t g_pPortData[POE_DEV_NUM][POE_DEV_PORT];





uint32 tps_init_setDevInterMask_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    value = 0x00; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_OPERATING_MODE_COMMAND, value);
    
    return (rtn);
}

uint32 tps_init_setPowerModeAuto_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    value = 0xFF; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_OPERATING_MODE_COMMAND, value);

    return (rtn);
}

uint32 tps_init_setDevDisconnectEnable_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value = 0;

    value = 0x0F; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_DISCONNECT_ENABLE_COMMAND, value);

    return (rtn);
}

uint32 tps_init_setDev4PPowerAllocation_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value = 0;

    value = 0xFF; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_4PWIRED_POWER_ALLOCATION_CONFIG_COMMAND, value);

    return (rtn);
}

uint32 tps_init_setDev2PPowerAllocation_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value = 0;

    value = 0x33; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_4PWIRED_POWER_ALLOCATION_CONFIG_COMMAND, value);

    return (rtn);
}

uint32 tps_init_setDevPowerOff_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    value = 0xF0; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_POWER_ENABLE_COMMAND, value);

    return (rtn);
}

uint32 tps_init_setDevDetectClassEnable_23882(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    value = 0xFF; 
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, value);

    return (rtn);
}

uint32 poe_tps_23882_get(uint32 devNum)
{
    uint32 i = 0;
    uint32 port[4] = {0};
    uint32 tmpVal = 0;
    uint32 tmpLsbVal = 0;
    uint32 tmpMsbVal = 0;

    /* ȡ�ø�оƬ��Ӧ�Ķ˿ں� */
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        port[i] = g_pPoeDevConf[devNum].startp + i;
    }
    
    /* ȡ����оƬ�Ķ˿�״̬ */
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_STATUS_COMMAND, &tmpVal));
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        g_pPortData[devNum][i].portEnable = (tmpVal & (1 << i)) >> i;
        g_pPortData[devNum][i].portPwGood = (tmpVal & (0x10 << i)) >> (i + 4);
    }

    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_STATUS_COMMAND + i), &tmpVal));
        g_pPortData[devNum][i].portDetClass = (tmpVal & 0xF);
        g_pPortData[devNum][i].portClassSts = (tmpVal >> 4) & 0xF;
        tmpVal = (tmpVal >> 4) & 0xF;
        if(0 == tmpVal)
        {
            g_pPortData[devNum][i].portStatus = 0xE;
        }
        else if ((1 <= tmpVal) && (tmpVal <= 4))
        {
            g_pPortData[devNum][i].portStatus = tmpVal;
        }
        else if ((5 <= tmpVal) && (tmpVal <= 6))
        {
            g_pPortData[devNum][i].portStatus = 0;
        }
        else
        {
            g_pPortData[devNum][i].portStatus = 0xF;
        }
    }
    
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_CURRENT_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_CURRENT_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portCurrent = ((((tmpMsbVal & 0x3F) << 8) | tmpLsbVal) * TPS_CURRENT_SCALE_FACTOR)/1000000;
        
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_VOLTAGE_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_VOLTAGE_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portVoltage = ((((tmpMsbVal & 0x3F) << 8) | tmpLsbVal) * TPS_VOLTAGE_SCALE_FACTOR)/1000000;
    }

    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_TEMPERATURE_COMMAND, &tmpVal));
    tmpVal = CONVERT_TEMP(tmpVal);
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        g_pPortData[devNum][i].portTemperature = tmpVal;
    }

    return 0;
}

uint32 poe_tps_23882_portEnable_set(uint32 devNum, uint32 devPort,uint32 Data)
{
    uint32 tmpVal = 0;
    uint32 oldVal = 0;

    tmpVal = (Data & 0x1);
    
    if (0 == tmpVal)
    {
        tmpVal = (1 << (devPort - 1)) << 4;
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_ENABLE_COMMAND, tmpVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, &oldVal));
        tmpVal = ~((1 << (devPort - 1)) << 4 | (1 << (devPort - 1)));
        tmpVal = oldVal & tmpVal;
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, tmpVal));
    }
    else
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_ENABLE_COMMAND, &oldVal));
        tmpVal = oldVal & ~((1 << (devPort - 1)) << 4);
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_ENABLE_COMMAND, tmpVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, &oldVal));
        tmpVal = (1 << (devPort - 1)) << 4 | (1 << (devPort - 1));
        tmpVal = oldVal | tmpVal;
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, tmpVal));
    }
    return 0;
}


uint32 board_poe_23882_init(uint32 index)
{
    uint32 ret = 0;

    
    ret = tps_init_setDevInterMask_23882(index);
    
    ret = tps_init_setPowerModeAuto_23882(index);
    
    ret = tps_init_setDevDisconnectEnable_23882(index);
    
    ret = tps_init_setDev2PPowerAllocation_23882(index);
    
    ret = tps_init_setDevPowerOff_23882(index);
    
    ret = tps_init_setDevDetectClassEnable_23882(index);

    return SYS_ERR_OK;
}

