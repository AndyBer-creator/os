
#ifndef __CFG_POE_H__
#define __CFG_POE_H__

#define POE_WEEK_MAX         (7)

typedef struct cfg_poe_time_s
{
	uint8           hour;
	uint8           minute;
	uint8           second;	
}cfg_poe_time_t;

typedef enum poe_week_type_t
{
	POE_TIME_WEEK_MON = 0,
	POE_TIME_WEEK_TUE,
	POE_TIME_WEEK_WED,
	POE_TIME_WEEK_THU,
	POE_TIME_WEEK_FRI,
	POE_TIME_WEEK_SAT,
	POE_TIME_WEEK_SUN,
	POE_TIME_WEEK_ALL
} poe_week_type_t;

typedef struct poe_time_range_s
{
	uint32 time[POE_WEEK_MAX];
}poe_time_range_t;

typedef struct cfg_poe_s
{
	uint32 poePortNum;   
    sys_enable_t enable[SYS_LOGICPORT_NUM_MAX];
    uint32 portIcut[SYS_LOGICPORT_NUM_MAX];
	poe_time_range_t portSchedule[SYS_LOGICPORT_NUM_MAX];
	
	uint32 portStatus[SYS_LOGICPORT_NUM_MAX];
	uint32 portVolatge[SYS_LOGICPORT_NUM_MAX];
	uint32 portCurrent[SYS_LOGICPORT_NUM_MAX]; 
	uint32 portPower[SYS_LOGICPORT_NUM_MAX];
	uint32 poe_currentPower;
	uint32 poe_num;
    sys_enable_t watchdog[SYS_LOGICPORT_NUM_MAX];
    cfg_poe_time_t rebootTimes[SYS_LOGICPORT_NUM_MAX];
    cfg_poe_time_t delayTimes[SYS_LOGICPORT_NUM_MAX];
} cfg_poe_t;

extern int32 cfg_poe_init(void);

extern int32 cfg_poe_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_poe_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_poe_portIcut_set(sys_logic_port_t port, uint32 icut);

extern int32 cfg_poe_portIcut_get(sys_logic_port_t port, uint32 *pIcut);

extern int32 cfg_poe_portSchedule_set(sys_logic_port_t port, uint32 week, uint32 hour);
extern int32 cfg_poe_portSchedule_get(sys_logic_port_t port, uint32 week, uint32 *hour);
extern int32 cfg_poe_port_total_set(uint32 num);
extern int32 cfg_poe_port_total_get(uint32 *num);
extern int32 cfg_poe_currentPower_sum_set(uint32 sum);
extern int32 cfg_poe_currentPower_sum_get(uint32 *pSum);
extern int32 cfg_poe_sw_info_port_set(sys_logic_port_t port, uint32 *pStatus, uint32* pCurrent, uint32* pVolatge, uint32* pPower);
extern int32 cfg_poe_sw_info_port_get(sys_logic_port_t port, uint32 *pStatus, uint32* pCurrent, uint32* pVolatge, uint32* pPower);
extern int32 cfg_poe_sw_status_port_get(sys_logic_port_t port, uint32 *pData);
extern int32 cfg_poe_sw_power_port_get(sys_logic_port_t port, uint32 *pData);
extern int32 cfg_poe_sw_current_port_get(sys_logic_port_t port, uint32 *pData);
extern int32 cfg_poe_sw_volatge_port_get(sys_logic_port_t port, uint32 *pData);

extern int32 cfg_poe_port_watchdog_Enable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 cfg_poe_port_watchdog_Enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 cfg_poe_portAssignTime_work_set(sys_logic_port_t port, cfg_poe_time_t rebootTime, cfg_poe_time_t delayTime);
extern int32 cfg_poe_portAssignTime_work_get(sys_logic_port_t port, cfg_poe_time_t* pRebootTime, cfg_poe_time_t* pDelayTime);
#endif

