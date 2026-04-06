


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


static uint32 i2cAddList[POE_DEV_NUM] = {0};
static uint32 i2cAddListAll[ ] = {
        PSE_I2C_ADDR_UNIT0,
        PSE_I2C_ADDR_UNIT1,
        PSE_I2C_ADDR_UNIT2,
        PSE_I2C_ADDR_UNIT3,
        PSE_I2C_ADDR_UNIT4,
        PSE_I2C_ADDR_UNIT5,
        PSE_I2C_ADDR_UNIT6,
        PSE_I2C_ADDR_UNIT7,
        PSE_I2C_ADDR_UNIT8,
        PSE_I2C_ADDR_UNIT9,
        PSE_I2C_ADDR_UNIT10,
        PSE_I2C_ADDR_UNIT11,
        PSE_I2C_ADDR_UNIT12,
        PSE_I2C_ADDR_UNIT13,
        PSE_I2C_ADDR_UNIT14,
        PSE_I2C_ADDR_UNIT15,
        PSE_I2C_ADDR_UNIT16,
        PSE_I2C_ADDR_UNIT17,
        PSE_I2C_ADDR_END
    };
extern int g_portPoeEnable[POE_MAXPORT_NUM];

extern sys_poe_devConf_t *g_pPoeDevConf;
extern sys_poe_portConf_t *g_pPoePortConf;
extern sys_poe_portData_t g_pPortData[POE_DEV_NUM][POE_DEV_PORT] ;

#if 0
#define POE_TPS23861_DEBUG(a,args...) \
do { \
	printk("\r\nRTL23861 %s %d "#a"\n", __func__, __LINE__, ##args); \
} while (0)
#else 
#define POE_TPS23861_DEBUG(a,args...){}
#endif


uint32 tps_init_setI2CAddr(uint32 numDev, uint32 *listAddr, i2c_devConf_t *pGlobalConf, i2c_devConf_t *pTempConf)
{
    uint32 rtn = SYS_ERR_OK;
    uint32 tempAddr = 0;
    uint32 value;
    uint32 i;

    tempAddr = pTempConf->dev_addr;
    rtn = i2c_byte_set(pGlobalConf, TPS238X_UNLOCK_CODE, tempAddr);

    
    for (i = 0; i < numDev; i++)
    {
        value = listAddr[i];
        rtn |= i2c_byte_set(pTempConf, TPS238X_I2C_SLAVE_ADDRESS_COMMAND, value);
    }

    
    
    osal_time_mdelay(300);

    return (rtn);
}

int tps_init_getI2CAddr_check(uint32 index, uint32 numDev)
{
    uint32 rtn = SYS_ERR_OK;
    uint32 current_address;
    uint32 i;

    for (i = 0; i < numDev; i++)
    {
        rtn |= i2c_byte_get(&g_pPoeDevConf[index + i].i2c, TPS238X_I2C_SLAVE_ADDRESS_COMMAND, &current_address);
        current_address &= 0x7F; 

        if (current_address != i2cAddList[i])
        {
            printk("\r\naddress config failed. dev: %d, now:0x%x, set:0x%x\r\n", i, current_address, i2cAddList[i]);
            rtn |= TPS_ERR_I2C_ADDRESS_MISMATCH;
        }
    }
    
    return (rtn);
}

uint32 tps_init_setDevlegacyDetectMode(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value = 0xaa;
    uint32 i = 0;

    for(i = 0; i < numDev; i++)
    {
        rtn = i2c_byte_set (&g_pPoeDevConf[index + i].i2c, TPS238X_LEGACY_DETECT_MODE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_getDevInterStatus(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    for(i = 0; i < numDev; i++)
    {
        rtn = i2c_byte_get (&g_pPoeDevConf[index + i].i2c, TPS238X_INTERRUPT_COMMAND, &value);
    }
    return (rtn);
}

uint32 tps_init_clearDevAllInterEvents(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;
    uint32 clearEvent = 1;

    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_get (&g_pPoeDevConf[index + i].i2c, TPS238X_POWER_EVENT_COMMAND + clearEvent, &value);
        rtn += i2c_byte_get (&g_pPoeDevConf[index + i].i2c, TPS238X_DETECTION_EVENT_COMMAND + clearEvent, &value);
        rtn += i2c_byte_get (&g_pPoeDevConf[index + i].i2c, TPS238X_FAULT_EVENT_COMMAND + clearEvent, &value);
        rtn += i2c_byte_get (&g_pPoeDevConf[index + i].i2c, TPS238X_INRUSH_LIMIT_EVENT_COMMAND + clearEvent, &value);
    }
    return (rtn);
}

uint32 tps_init_setDevInterMask(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0x00; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_INTERRUPT_MASK_COMMAND, value);
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_CLASS_FIVE_TIMER_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setPowerModeAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xFF; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_OPERATING_MODE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setPowerModeSemiAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xAA; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_OPERATING_MODE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setPortPowerModeAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xFF; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_OPERATING_MODE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setPortPowerModeSemiAuto(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xAA; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_OPERATING_MODE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setDevPowerOff(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xF0; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_POWER_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setDevDisconnectEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value1 = 0;
    uint32 value2 = 0;
    uint32 i = 0;

    value1 = 0x0F; 
    value2 = 0x00; 
    for(i = 0; i < numDev; i++)
    {
        rtn =  i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_DISCONNECT_ENABLE_COMMAND, value1);
        rtn += i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_DISCONNECT_THRESHOLD_COMMAND, value2);
    }
    return (rtn);
}

uint32 tps_init_setDevDetectClassEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0xFF; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, value);
    }
    return (rtn);
}

uint32 tps_init_setTwoEventClassEnable(uint32 index, uint32 numDev)
{
    uint32 rtn = 0;
    uint32 value;
    uint32 i = 0;

    value = 0x55; 
    for(i = 0; i < numDev; i++)
    {
        rtn  = i2c_byte_set(&g_pPoeDevConf[index + i].i2c, TPS238X_TWO_EVENT_CLASSIFICATION_COMMAND, value);
    }
    return (rtn);
}

uint32 poe_tps_23861_get(uint32 devNum)
{
    uint32 i = 0;
    
    uint32 tmpVal = 0;
    uint32 pwTmpVal = 0;
    uint32 tmpLsbVal = 0;
    uint32 tmpMsbVal = 0;
    
    
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_STATUS_COMMAND, &pwTmpVal));
    SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_TEMPERATURE_COMMAND, &tmpVal));
    tmpVal = CONVERT_TEMP(tmpVal);
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        g_pPortData[devNum][i].portEnable = (pwTmpVal & (1 << i)) >> i;
        g_pPortData[devNum][i].portPwGood = (pwTmpVal & (0x10 << i)) >> (i + 4);
        
        g_pPortData[devNum][i].portTemperature = tmpVal;
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

        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_CURRENT_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_CURRENT_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portCurrent = ((((tmpMsbVal & 0x3F) << 8) | tmpLsbVal) * TPS_CURRENT_SCALE_FACTOR)/1000000;
        
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_VOLTAGE_LSB_COMMAND + i * 4), &tmpLsbVal));
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (TPS238X_PORT_1_VOLTAGE_MSB_COMMAND + i * 4), &tmpMsbVal));
        g_pPortData[devNum][i].portVoltage = ((((tmpMsbVal & 0x3F) << 8) | tmpLsbVal) * TPS_VOLTAGE_SCALE_FACTOR)/1000000;
        
    }
    


    return 0;
}

uint32 poe_tps_23861_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data)
{
    uint32 tmpVal = 0;
    uint32 oldVal = 0;
    
    tmpVal = (Data & 0x1);
    
    if (0 == tmpVal)
    {
        tmpVal = (1 << (devPort - 1)) << 4;
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_POWER_ENABLE_COMMAND, tmpVal));
    }
    else
    {
        SYS_ERR_CHK(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, &oldVal));
        tmpVal = (1 << (devPort - 1)) << 4 | (1 << (devPort - 1));
        tmpVal = oldVal | tmpVal;
        SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, TPS238X_DETECT_CLASS_ENABLE_COMMAND, tmpVal));
    }
    return 0;
}

uint32 board_poe_23861_init(uint32 index, uint32 deviceNum, uint32 globalDev, uint32 tempDev)
{
    uint32 i = 0;
    uint32 ret = 0;

    
    
    for(i = 0; i < deviceNum; i++)
    {
		i2cAddList[i] = i2cAddListAll[i];
		if(i2cAddListAll[i] == 0xFF)
		{
			printk("\r\n %s %d i2cdev over max dev, please check\r\n", __func__, __LINE__);
		}
    }

    
    ret = tps_init_setI2CAddr(deviceNum,  i2cAddList, &g_pPoeDevConf[globalDev].i2c, &g_pPoeDevConf[tempDev].i2c);
    
    ret += tps_init_getI2CAddr_check(index, deviceNum);
    
    ret += tps_init_getDevInterStatus(index, deviceNum);
    
    ret += tps_init_clearDevAllInterEvents(index, deviceNum);
    
    ret += tps_init_setDevInterMask(index, deviceNum);
    
    ret += tps_init_setPowerModeAuto(index, deviceNum);
    
    ret += tps_init_setDevPowerOff(index, deviceNum);
    
    ret += tps_init_setDevDisconnectEnable(index, deviceNum);
    
    ret += tps_init_setDevDetectClassEnable(index, deviceNum);
    
    ret += tps_init_setTwoEventClassEnable(index, deviceNum);
	
    return ret;
}

