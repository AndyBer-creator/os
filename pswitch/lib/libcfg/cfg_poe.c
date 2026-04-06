
#include <libcfg/cfg.h>

int32 cfg_poe_init(void)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	CFG_CLOSE(CFG_ID_POE);

	return SYS_ERR_OK;
}

int32 cfg_poe_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->enable[port] = enable;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*pEnable = pPOE->enable[port];

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portIcut_set(sys_logic_port_t port, uint32 icut)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(icut, BOND_POE_PORT_ICUT_MIN, BOND_POE_PORT_ICUT_MAX), SYS_ERR_OUT_OF_RANGE);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->portIcut[port] = icut;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portIcut_get(sys_logic_port_t port, uint32 *pIcut)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(NULL == pIcut, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*pIcut = pPOE->portIcut[port];

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portSchedule_set(sys_logic_port_t port, uint32 week, uint32 hour)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((week >= POE_WEEK_MAX), SYS_ERR_INPUT);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->portSchedule[port].time[week] = hour;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portSchedule_get(sys_logic_port_t port, uint32 week, uint32 *hour)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(NULL == hour, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((week >= POE_WEEK_MAX), SYS_ERR_INPUT);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*hour = pPOE->portSchedule[port].time[week];

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_port_total_set(uint32 num)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->poe_num = num;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_port_total_get(uint32 *num)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*num = pPOE->poe_num;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_currentPower_sum_set(uint32 sum)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->poe_currentPower = sum;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_currentPower_sum_get(uint32 *pSum)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*pSum = pPOE->poe_currentPower;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_info_port_set(sys_logic_port_t port, uint32 *pStatus, uint32* pCurrent, uint32* pVolatge, uint32* pPower)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->portCurrent[port] = *pCurrent;
	pPOE->portVolatge[port] = *pVolatge;
	pPOE->portStatus[port] = *pStatus;
	pPOE->portPower[port] = *pPower;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_info_port_get(sys_logic_port_t port, uint32 *pStatus, uint32* pCurrent, uint32* pVolatge, uint32* pPower)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*pStatus = pPOE->portStatus[port];
	*pPower = pPOE->portPower[port];
	*pCurrent = pPOE->portCurrent[port];
	*pVolatge = pPOE->portVolatge[port];

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_status_port_get(sys_logic_port_t port, uint32 *pData)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);
	*pData = pPOE->portStatus[port];
	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_power_port_get(sys_logic_port_t port, uint32 *pData)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);
	*pData = pPOE->portPower[port];
	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_current_port_get(sys_logic_port_t port, uint32 *pData)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);
	*pData = pPOE->portCurrent[port];
	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_sw_volatge_port_get(sys_logic_port_t port, uint32 *pData)
{
	cfg_poe_t *pPOE = NULL;

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);
	*pData = pPOE->portVolatge[port];
	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_port_watchdog_Enable_set(sys_logic_port_t port, sys_enable_t enable)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	pPOE->watchdog[port] = enable;

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_port_watchdog_Enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	*pEnable = pPOE->watchdog[port];

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portAssignTime_work_set(sys_logic_port_t port, cfg_poe_time_t rebootTime, cfg_poe_time_t delayTime)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	osal_memcpy(&pPOE->rebootTimes[port], &rebootTime, sizeof(cfg_poe_time_t));
	osal_memcpy(&pPOE->delayTimes[port], &delayTime, sizeof(cfg_poe_time_t));

	CFG_CLOSE(CFG_ID_POE);
}

int32 cfg_poe_portAssignTime_work_get(sys_logic_port_t port, cfg_poe_time_t* pRebootTime, cfg_poe_time_t* pDelayTime)
{
	cfg_poe_t *pPOE = NULL;

	SYS_PARAM_CHK(IS_NORMAL_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK(NULL == pRebootTime, SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK(NULL == pDelayTime, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_POE, (cfg_addr)&pPOE);

	osal_memcpy(pRebootTime, &pPOE->rebootTimes[port], sizeof(cfg_poe_time_t));
	osal_memcpy(pDelayTime, &pPOE->delayTimes[port], sizeof(cfg_poe_time_t));

	CFG_CLOSE(CFG_ID_POE);
}

