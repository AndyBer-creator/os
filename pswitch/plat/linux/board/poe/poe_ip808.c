


#include <rtk/switch.h>
#include <rtk/port.h>
#include <rtk/led.h>
#include <rsd/rsd_port.h>
#include <rtk/stat.h>
#include <board/poe/poe.h>
#include <board/poe/poe_ip808.h>
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


uint32 ip808_set_page(uint32 numDev, uint32 page)
{
    uint32 rtn = 0;
    uint32 value = 0;
    uint32 tmp_page = 0;

    tmp_page = (g_pPoeDevConf[numDev].i2c.dev_addr & 0x7);
    value = (tmp_page | ((page & 0x1) << 6));
    rtn  = i2c_byte_set(&g_pPoeDevConf[numDev].i2c, PAGE_N_DEVICE_ADDR, value);
    return (rtn);
}

uint32 ip808_init_getI2CAddr_check(uint32 index)
{
    uint32 rtn = 0;
    uint32 value = 0;

    ip808_set_page(index, IP808_REG_PAGE0);

    rtn  = i2c_byte_get(&g_pPoeDevConf[index].i2c, PAGE_N_DEVICE_ADDR, &value);
    if(0 != rtn)
    {
        osal_printf("\nDevice %d read fail.\n", index);
        return (rtn);
    }
    else
    {
        if((g_pPoeDevConf[index].i2c.dev_addr & 0x7) != (value & 0x7))
        {
            osal_printf("\nDevice %d address check fail. %x, %x\n", index, g_pPoeDevConf[index].i2c.dev_addr, value);
            return SYS_ERR_FAILED;
        }
    }

    ip808_set_page(index, IP808_REG_PAGE1);
    rtn  = i2c_byte_get(&g_pPoeDevConf[index].i2c, SYS_CONFIG_CONTROL, &value);
    if(0 != rtn)
    {
        osal_printf("\nDevice %d read fail.\n", index);
        return (rtn);
    }
    else
    {
        if(((value >> 6) & 0x3) != 0x1)
        {
            osal_printf("\nDevice %d mode not manual. %x\n", index, value);
            return SYS_ERR_FAILED;
        }
    }
    return SYS_ERR_OK;
}

uint32 ip808_init_setReset(uint32 index)
{
    uint32 rtn = SYS_ERR_OK;
    ip808_set_page(index, IP808_REG_PAGE1);
    rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, RESET_FLAG_REG_IP808A, RESET_FLAG);
    return (rtn);
}

uint32 ip808_init_setPortPowerOn(uint32 index)
{
    uint32 rtn = 0;
    uint32 i = 0;

    ip808_set_page(index, IP808_REG_PAGE1);
    for(i = 0; i < g_pPoeDevConf[index].portNum; i++)
    {
        rtn = i2c_byte_set(&g_pPoeDevConf[index].i2c, (POWER_CONTROL_P0 + i), 1);
    }
    return (rtn);
}

uint32 ip808_init_setIvtAutoPoll(uint32 index)
{
    uint32 rtn = 0;
    uint32 value;

    ip808_set_page(index, IP808_REG_PAGE0);
    rtn  = i2c_byte_get(&g_pPoeDevConf[index].i2c, IVT_POLL_CONTROL, &value);
    value |= 0x20;
    rtn  = i2c_byte_set(&g_pPoeDevConf[index].i2c, IVT_POLL_CONTROL, value);
    return (rtn);
}

static int32 SupplyVolVal = -1;
static int32 portGood[POE_DEV_PORT] = {0};
uint32 poe_ip_808_get(uint32 devNum)
{
    int32  ret = 0;
    uint32 i = 0;
    uint32 pStatusVal = 0;
    uint32 pControlVal = 0;
    uint32 pDetectVal = 0;
    uint32 tmpLsbVal = 0;
    uint32 tmpMsbVal = 0;
    sys_poe_portData_t portData[POE_DEV_PORT];

    SYS_MEM_CLEAR(portData);

    if(-1 == SupplyVolVal)
    {
        ip808_set_page(devNum, IP808_REG_PAGE0);
        osal_time_mdelay(10);
        SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (SUPPLY_VOLTAGE_MSB), &tmpMsbVal), failread, ret);
        SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (SUPPLY_VOLTAGE_LSB), &tmpLsbVal), failread, ret);
        SupplyVolVal = (((tmpMsbVal & 0x0F) << 8) | (tmpLsbVal & 0xFF)) >> 4;
        if((SupplyVolVal > 54)||(SupplyVolVal < 48))
        {
            SupplyVolVal = -1;
            return 0;
        }
    }
    
    ip808_set_page(devNum, IP808_REG_PAGE1);
    osal_time_mdelay(10);
    SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, PORT_POWER_STATUS, &pStatusVal), failread, ret);
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        if((pStatusVal >> i & 0x1) == 1)
        {
            portData[i].portEnable = 1;
            portData[i].portPwGood = 1;
            portGood[i] = 1;
        }
        else
        {
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, POWER_CONTROL_P0 + i, &pControlVal), failread, ret);
            if(0 == (pControlVal & 0x3))
            {
                portData[i].portEnable = 0;
            }
            else
            {
                portData[i].portEnable = 1;
            }
            portData[i].portPwGood = 0;
            portGood[i] = 0;
        }
    }

    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        if(1 == portGood[i])
        {
            ip808_set_page(devNum, IP808_REG_PAGE0);
            osal_time_mdelay(10);
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (CURRENT_MSB0 + i * 2), &tmpMsbVal), failread, ret);
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (CURRENT_LSB0 + i * 2), &tmpLsbVal), failread, ret);
            portData[i].portCurrent = (((tmpMsbVal & 0x0F) << 8) | (tmpLsbVal & 0xFF)) >> 2;
            portData[i].portVoltage = SupplyVolVal;
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (CURRENT_MSB0 + i * 2), &tmpMsbVal), failread, ret);
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (CURRENT_LSB0 + i * 2), &tmpLsbVal), failread, ret);
            portData[i].portTemperature = (((tmpMsbVal & 0x1F) << 8) | (tmpLsbVal & 0xFF)) >> 4;
            SYS_ERR_HDL(i2c_byte_get(&g_pPoeDevConf[devNum].i2c, (DETECT_CLASS0_1 + i/2), &pDetectVal), failread, ret);
            if(i % 2 == 0)
            {
                portData[i].portStatus = (pDetectVal & 0x7);
            }
            else
            {
                portData[i].portStatus = ((pDetectVal >> 4) & 0x7);
            }
        }
        else
        {
            portData[i].portCurrent = 0;
            portData[i].portVoltage = SupplyVolVal;
            portData[i].portTemperature = 0;
            portData[i].portStatus = 5;
        }
    }

    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
        if((portData[i].portCurrent <= 3) 
            && ((portData[i].portStatus >= 5)||(portData[i].portStatus == 0))
            && (portData[i].portPwGood == 1))
        {
            
            portData[i].portPwGood = 0;
            
        }
        else if(portData[i].portCurrent >= 680)
        {
            
            portData[i].portCurrent = 0;
            portData[i].portPwGood = 0;
            
        }
        else
        {
            
        }
    }
    osal_memcpy(g_pPortData[devNum], portData, sizeof(portData));
    return 0;

failread:
    
    return 0;
}

uint32 poe_ip_808_portEnable_set(uint32 devNum, uint32 devPort, uint32 Data)
{
    uint32 tmpVal = 0;
    
    tmpVal = (Data & 0x1);
    
    ip808_set_page(devNum, IP808_REG_PAGE1);
    SYS_ERR_CHK(i2c_byte_set(&g_pPoeDevConf[devNum].i2c, (POWER_CONTROL_P0 + (devPort & 0xF)), tmpVal));
    
    return 0;
}

uint32 board_poe_ip808_init(uint32 index)
{
    uint32 ret = 0;
    
    

    ret = ip808_init_getI2CAddr_check(index);
    if(ret == 0)
    {
        
        osal_time_mdelay(100);
        ip808_init_setIvtAutoPoll(index);
        
        osal_time_mdelay(300);
        ip808_init_setReset(index);
        
        osal_time_mdelay(500);
        ip808_init_setPortPowerOn(index);
        
        osal_time_mdelay(500);
    }
    else
    {
        osal_printf("\nDevice %d check failed.\n", index);
    }
    return ret;
}
    
