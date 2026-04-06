


#include <rtk/switch.h>
#include <rtk/port.h>
#include <rtk/led.h>
#include <rsd/rsd_port.h>
#include <rtk/stat.h>
#include <board/poe/poe.h>
#include <board/poe/poe_rtl8238.h>
#include <linux/timer.h>
#include <linux/rtc.h>
extern int g_portPoeEnable[POE_MAXPORT_NUM];
extern sys_poe_devConf_t *g_pPoeDevConf;
extern sys_poe_portConf_t *g_pPoePortConf;
extern sys_poe_portData_t g_pPortData[POE_DEV_NUM][POE_DEV_PORT];



#if 0
#define POE_RTL8238_DEBUG(a,args...) \
do { \
	printk("\r\nRTL8238 %s %d "#a"\n", __func__, __LINE__, ##args); \
} while (0)
#else 
#define POE_RTL8238_DEBUG(a,args...){}
#endif


 
uint32 rtl8238_init_setDevPowerOff(uint32 index, uint32 deviceNum)
{
    int32 rtn = 0;
	int32  i = index;
	int  portIndex ;
    uint8 value[4] ={0x00, 0xF0, 0x00, 0x00};  
	uint32 data = 0;
	
	
	{
		for(portIndex = 0; portIndex < 8; portIndex++)
		{
			data = 0;
			rtn += i2c_two_byte_get(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), &data);
			POE_RTL8238_DEBUG("port:%d 0x%x", portIndex, data);
    		rtn += i2c_two_byte_set(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), value);
    		data = 0;
    		rtn += i2c_two_byte_get(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), &data);
			POE_RTL8238_DEBUG("port:%d 0x%x", portIndex, data);
    	}
    	i++;
    }
    return (rtn);
}

uint32 rtl8238_init_setDevPowerOn(uint32 index, uint32 deviceNum)
{
    int32 rtn = 0;
	int32  i = index;
	int  portIndex ;
    uint8 value[4] ={0x01, 0xF0, 0x00, 0x00};  
	uint32 data = 0;
	
	while(i < deviceNum)
	{
		for(portIndex = 0; portIndex < 8; portIndex++)
		{
			data = 0;
			i2c_two_byte_get(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), &data);
			POE_RTL8238_DEBUG("start port:%d 0x%x", portIndex, data);
    		rtn  += i2c_two_byte_set(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), value);
    		data =0;
    		i2c_two_byte_get(&g_pPoeDevConf[i].i2c, RTL8238_PORT_CTRL_COMMAND+(0x4*portIndex), &data);
			POE_RTL8238_DEBUG("end port:%d 0x%x", portIndex, data);
    	}
    	i++;
    }
    return (rtn);
}


uint32 rtl8238_init_setDevDetectClassEnable(uint32 index, uint32 deviceNum)
{
    uint32 rtn = 0;
    uint32 i = index;
    int  portIndex ;
	uint8 value[4] ={0x00, 0x00, 0x00, 0x03};  

    while(i < deviceNum)
    {
    	for(portIndex = 0; portIndex < 8; portIndex++)
    	{
    		rtn  += i2c_two_byte_set(&g_pPoeDevConf[i].i2c, RTL8238_DETECT_CLASS_ENABLE_COMMAND+(0x4*portIndex), value);
    	}
    	i++;
    }
    return (rtn);
}




uint32 rtl8238_init_setPowerModeAuto(uint32 index, uint32 deviceNum)
{
    uint32 rtn = 0;
    uint32 value;
	int32 i = index;
	
    value = 0xFF; 

    while(i < deviceNum)
    {
   		
   		i++;
    }
    return (rtn);
}



int32 rtl8238_SetDeviceInterruptMask (uint32 index, uint32 deviceNum)
{
	int32 rtn = 0;
	uint32 value;
	int i = index;

	value = 0x00; 
	while(i < deviceNum)
	{
    	
    	i++;
    }
    return (rtn);
}



int32 rtl8238_SetDeviceDisconnectEnable (uint32 index, uint32 deviceNum)
{
	int32 rtn = 0;
	int i = index;

	while(i < deviceNum)
	{
    	
    	i++;
	}
    return (rtn);
}


uint32 poe_rtl_8238_get(uint32 devNum)
{
    uint32 i = 0;
    uint32 tmpVal = 0;
    uint32 tmpLsbVal = 0;
    uint32 tmpMsbVal = 0;

	
 
    /* ȡ����оƬ�Ķ˿�״̬ */
    for(i = 0; i < g_pPoeDevConf[devNum].portNum; i++)
    {
		SYS_ERR_CHK(i2c_two_byte_get(&g_pPoeDevConf[devNum].i2c, RTL8238_PORT_STATE_COMMAND+(i*0x4), &tmpVal));
		
        g_pPortData[devNum][i].portEnable = ((tmpVal >> 24) & 0x01);
        g_pPortData[devNum][i].portPwGood = ((tmpVal >> 16 ) & 0x0004);

		if(g_pPortData[devNum][i].portPwGood && g_pPortData[devNum][i].portEnable)
		{
	        
	        tmpLsbVal = 0;
	        tmpMsbVal = 0;
	        SYS_ERR_CHK(i2c_two_byte_get(&g_pPoeDevConf[devNum].i2c, (RTL8238_PORT_CURRENT_LSB_COMMAND + i * 0x4), &tmpVal));
	        tmpMsbVal = (tmpVal >> 16) & 0x00FF;
	        tmpLsbVal = tmpVal >> 24 & 0xFF;
	        
	        g_pPortData[devNum][i].portCurrent = ((((tmpMsbVal << 8) | tmpLsbVal)* 1000)/8192);

			
	        tmpLsbVal = 0;
	        tmpMsbVal = 0;
	        SYS_ERR_CHK(i2c_two_byte_get(&g_pPoeDevConf[devNum].i2c, (RTL8238_PORT_VOLTAGE_LSB_COMMAND + i * 0x4), &tmpVal));
	        tmpMsbVal = tmpVal >> 16 & 0x00FF;
	        tmpLsbVal = tmpVal >> 24 & 0xFF;
	        
	        g_pPortData[devNum][i].portVoltage = (((tmpMsbVal << 8) | tmpLsbVal)* 64/4096); 
		}
        else
        {
			g_pPortData[devNum][i].portCurrent = 0;
			g_pPortData[devNum][i].portVoltage = 0;
        }
        g_pPortData[devNum][i].portTemperature = 0;
    }

    return 0;
}


uint32 poe_rtl_8238_portEnable_set(uint32 devNum, uint32 devPort,uint32 Data)
{
    uint32 tmpVal = 0;
	uint8  disable[4] ={0x00, 0xF0, 0x00, 0x00};
	uint8  enable[4] ={0x01, 0xF0, 0xF0, 0x00};
	uint32 data = 0;

    tmpVal = (Data & 0x1);
    POE_RTL8238_DEBUG("    dev :%d, port :%d, Data :%d", devNum, devPort, Data);

    data = 0;
	i2c_two_byte_get(&g_pPoeDevConf[devNum].i2c, RTL8238_PORT_CTRL_COMMAND + (devPort*4), &data);
	POE_RTL8238_DEBUG("port:%d 0x%x", devPort, data);
    if (0 == tmpVal)
    {
        SYS_ERR_CHK(i2c_two_byte_set(&g_pPoeDevConf[devNum].i2c, RTL8238_PORT_CTRL_COMMAND + (devPort*4), disable));
    }
    else
    {
        SYS_ERR_CHK(i2c_two_byte_set(&g_pPoeDevConf[devNum].i2c, RTL8238_PORT_CTRL_COMMAND + (devPort*4), enable));
    }
    data = 0;
    i2c_two_byte_get(&g_pPoeDevConf[devNum].i2c, RTL8238_PORT_CTRL_COMMAND + (devPort*4), &data);
	POE_RTL8238_DEBUG("port:%d 0x%x", devPort, data);
    return 0;
}


uint32 board_poe_rtl8238_init(uint32 index, uint32 deviceNum)
{
    uint32 ret = 0;
	POE_RTL8238_DEBUG(" index:%u deviceNum:%u ", index, deviceNum);

	
	ret += rtl8238_init_setDevPowerOff(index, deviceNum);
	

	
	
	
	
    

    
   
    
    
    
    
    
    
	if(ret != SYS_ERR_OK)
		POE_RTL8238_DEBUG(" Init RTL8238 Fail!!! \n");

    return ret;
}

