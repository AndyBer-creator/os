


#include <rtk/switch.h>
#include <rtk/port.h>
#include <rtk/led.h>
#include <rsd/rsd_port.h>
#include <rtk/stat.h>
#include <board/poe/poe.h>
#include <board/poe/poe_mp3924.h>
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
extern sys_poe_portData_t g_pPortData[POE_DEV_NUM][POE_DEV_PORT] ;


uint32 mp_init_setI2CAddr(uint32 numDev, uint32 *listAddr, i2c_devConf_t *pGlobalConf, i2c_devConf_t *pTempConf)
{
    uint32 rtn = SYS_ERR_OK;
    return (rtn);
}

int mp_init_getI2CAddr_check(uint32 index, uint32 numDev)
{
    uint32 rtn = SYS_ERR_OK;
    return (rtn);
}

uint32 mp_init_setDevlegacyDetectMode(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value = 0xaa;
    uint32 i = 0;

    for(i = 0; i < numDev; i++)
    {
        rtn = i2c_byte_set (&g_pPoeDevConf[index + i].i2c, MP3924_LEGACY_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_getDevInterStatus(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    for(i = 0; i < numDev; i++)
    {
        rtn = i2c_byte_get (&g_pPoeDevConf[index + i].i2c, MP3924_INTERRUPT_COMMAND, &value);
    }
    return (rtn);
}

uint32 mp_init_setDevInterMask(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0x00; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_INTERRUPT_MASK_COMMAND, value);
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_INTERRUPT_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setPowerModeAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xFF; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_MODE_SETTING_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setPowerModeSemiAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xAA; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_MODE_SETTING_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setPortPowerModeAuto(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    value = 0xFF; 
    rtn  = i2c_byte_set(&g_pPoeDevConf[index].i2c, MP3924_MODE_SETTING_COMMAND, value);
    return (rtn);
}

uint32 mp_init_setPortPowerModeSemiAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xAA; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_MODE_SETTING_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setDevPowerOff(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xF0; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_PORT_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setDevDisconnectEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value1 = 0;
    uint32 i = 0;

    value1 = 0x0F; 
    for(i = 0; i < numDev; i++)
    {
        rtn =  i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_DISCONNECT_ENABLE_COMMAND, value1);
    }
    return (rtn);
}

uint32 mp_init_setDevDetectClassEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xFF; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_DET_CLS_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 mp_init_setTwoEventClassEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0x55; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, MP3924_2_EVENT_CLASS_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 poe_mp_3924_get(uint32 devNum)
{
    uint32 i = 0;
    uint32 pwTmpVal = 0;
    uint32 tmpVal = 0;
    uint32 tmpLsbVal = 0;
    uint32 tmpMsbVal = 0;

    
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_POWER_STATUS_COMMAND, &pwTmpVal));
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_JUNCTION_TEMPERATURE_LSB_COMMAND, &tmpLsbVal));
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_JUNCTION_TEMPERATURE_MSB_COMMAND, &tmpMsbVal));
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        g_pPortData[devNum][i].portEnable = (pwTmpVal & (1 << i)) >> i;
        g_pPortData[devNum][i].portPwGood = (pwTmpVal & (0x10 << i)) >> (i + 4);
        g_pPortData[devNum][i].portTemperature = (((tmpMsbVal & 0xFF) << 1) | (tmpLsbVal & 0x1))*4/10;
        if(g_pPortData[devNum][i].portTemperature >= 40)
            g_pPortData[devNum][i].portTemperature = g_pPortData[devNum][i].portTemperature - 40;
        else
            g_pPortData[devNum][i].portTemperature = 0;
    }

    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (MP3924_DET_CLS_RESULT1_COMMAND + i), &tmpVal));
        g_pPortData[devNum][i].portDetClass = (tmpVal & 0x7);
        g_pPortData[devNum][i].portClassSts = (tmpVal >> 4) & 0xF;
        if(0 == g_pPortData[devNum][i].portClassSts)
        {
            g_pPortData[devNum][i].portStatus = 0xE;
        }
        else if ((1 <= g_pPortData[devNum][i].portClassSts) && (g_pPortData[devNum][i].portClassSts <= 4))
        {
            g_pPortData[devNum][i].portStatus = g_pPortData[devNum][i].portClassSts;
        }
        else if ((5 <= g_pPortData[devNum][i].portClassSts) && (g_pPortData[devNum][i].portClassSts <= 6))
        {
            g_pPortData[devNum][i].portStatus = 0;
        }
        else
        {
            g_pPortData[devNum][i].portStatus = 0xF;
        }

        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (MP3924_PORT1_CURRENT_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (MP3924_PORT1_CURRENT_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portCurrent = (((tmpMsbVal & 0xFF) << 1) | (tmpLsbVal & 0x1))*24/10;
        
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (MP3924_PORT1_VOLTAGE_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (MP3924_PORT1_VOLTAGE_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portVoltage = 52 - (((tmpMsbVal & 0xFF) << 1) | (tmpLsbVal & 0x1))*15/100;

    }

    return 0;
}

uint32 poe_mp_3924_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data)
{
    uint32 tmpVal = 0;
    uint32 oldVal = 0;
    
    tmpVal = (Data & 0x1);
    
    if (0 == tmpVal)
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_PORT_ENABLE_COMMAND, &oldVal));
        tmpVal = ~(1 << (devPort));
        tmpVal = oldVal & tmpVal;
        
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, MP3924_PORT_ENABLE_COMMAND, tmpVal));
    }
    else
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_PORT_ENABLE_COMMAND, &oldVal));
        tmpVal = (1 << (devPort));
        tmpVal = oldVal | tmpVal;
        
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, MP3924_PORT_ENABLE_COMMAND, tmpVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, MP3924_DET_CLS_ENABLE_COMMAND, &oldVal));
        tmpVal = ((1 << devPort) << 4)|(1 << devPort);
        tmpVal = oldVal | tmpVal;
        
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, MP3924_DET_CLS_ENABLE_COMMAND, tmpVal));
    }
    
    return 0;
}

uint32 board_poe_mp3924_init(uint32 index)
{
    uint32 ret = 0;

    mp_init_setPortPowerModeAuto(index);

    return ret;
}

