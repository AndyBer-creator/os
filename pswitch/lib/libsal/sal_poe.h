
#ifndef __SAL_POE_H__
#define __SAL_POE_H__

#include <libsal/sal_util.h>
#include <rsd/rsd_poe.h>

typedef enum sal_pd_mode_e{
    PD_MODE_AF,
    PD_MODE_AT,
    PD_MODE_BT,
    PD_MODE_NONE,
    PD_MODE_END
}sal_pd_mode_t;

typedef enum sal_pd_class_e{
	PD_CLASS_UNKNOWN,
    PD_CLASS1_3_84W,
    PD_CLASS2_6_49W,
    PD_CLASS3_13W,
    PD_CLASS4_25_5W,
    PD_CLASS5_40W,
    PD_CLASS6_51W,
    PD_CLASS7_62W,
    PD_CLASS8_71W,
    PD_CLASS0_13W,
    PD_CLASS_END
}sal_pd_class_t;

typedef struct sal_poe_ioctl_s
{
	sys_logic_port_t port;
	uint32 portAbility;     
    uint32 portEnable;      
    uint32 portStatus;      
    uint32 portType;         
    uint32 portLevel;       
    uint32 portPowerLimit;  
    uint32 portPower;       
    uint32 portVolatge;     
    uint32 portCurrent;     
    uint32 portWatchDog;    
}sal_poe_ioctl_t;
typedef struct sal_poe_time_s
{
	uint8           hour;
	uint8           minute;
	uint8           second;	
}sal_poe_time_t;

int32 sal_strtime_to_hour_min_sec(char* pStrtime, uint8* pHour, uint8* pMinute, uint8* pSecond);
int32 sal_poe_portEnable_set(sys_logic_port_t port, uint32 Enable);
int32 sal_poe_portAbility_get(sys_logic_port_t port, uint32 *pEnable);
int32 sal_poe_portIcut_set(sys_logic_port_t port, uint32 Icut);
int32 sal_poe_portSchedule_set(sys_logic_port_t port, uint32 week, uint32 hour);
int32 sal_poe_timeZone_set(uint32 timeZone);
int32 sal_poe_portStatus_get(sys_logic_port_t port, uint32 *pStatus);
int32 sal_poe_portEnable_get(sys_logic_port_t port, uint32 *pEnable);
int32 sal_poe_portPwGood_get(sys_logic_port_t port, uint32 *pPwGood);
int32 sal_poe_portCurrent_get(sys_logic_port_t port, uint32 *pCurrent);
int32 sal_poe_portVoltage_get(sys_logic_port_t port, uint32 *pVoltage);
int32 sal_poe_portIcut_get(sys_logic_port_t port, uint32 *pIcut);
int32 sal_poe_devPower_get(sys_logic_port_t port, uint32 *pPower);
int32 sal_poe_devTemperature_get(sys_logic_port_t port, uint32 *pTemperature);
int32 sal_poe_portSchedule_get(sys_logic_port_t port, uint32 week, uint32 *hour);
int32 sal_poe_portInfo_get(sys_logic_port_t port, sal_poe_ioctl_t *pInfo);
int32 sal_poe_port_total_num_set(uint32 num);
int32 sal_poe_port_total_num_get(uint32 *num);
int32 sal_poe_status_info_updata(void);
int32 sal_poe_currentPower_sum_get(uint32 *pSum);
int32 sal_poe_port_info_get(sys_logic_port_t port, sal_poe_ioctl_t *pData);
int32 sal_poe_portPower_get(sys_logic_port_t port, uint32 *pPower);

#if defined(CONFIG_SYS_POE_SWDOG)
int32 sal_poe_port_watch_dog_Enable_set(sys_logic_port_t port, uint32 Enable);
int32 sal_poe_port_watch_dog_Enable_get(sys_logic_port_t port, uint32 *pEnable);
#endif
int32 sal_poe_portAssignTime_work_set(sys_logic_port_t port, sal_poe_time_t rebootTime, sal_poe_time_t delayTime);
int32 sal_poe_portAssignTime_work_get(sys_logic_port_t port, sal_poe_time_t* pRebootTime, sal_poe_time_t* pDelayTime);

#endif 

