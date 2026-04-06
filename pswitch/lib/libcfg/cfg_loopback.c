
#include <libcfg/cfg.h>

int32 cfg_loopback_init(void)
{
    cfg_loopback_t *pLoopback = NULL;

    CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);
	pLoopback->detectionTime = SAL_DEFAULT_DETECTIONTIME;
	pLoopback->resumeTime = SAL_DEFAULT_RESUMETIME;
	pLoopback->protoEnable = DISABLED;
	pLoopback->protoResumeCheckEnable = DISABLED;
	pLoopback->bControlVlanAll = SAL_DEFAULT_BCONTROL_VLAN;

	{

		osal_memset(pLoopback->port_value.port_enable, 0x00, sizeof(pLoopback->port_value.port_enable));
		osal_memset(pLoopback->port_value.resume_mode, 0x1, sizeof(pLoopback->port_value.resume_mode));
	}

    CFG_CLOSE(CFG_ID_LOOPBACK);

    return SYS_ERR_OK;
}

int32 cfg_loopback_detectionTime_set(uint32 timeValue)
{
	cfg_loopback_t *pLoopback = NULL;

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->detectionTime = timeValue;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_detectionTime_get(uint32 *timeValue)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == timeValue, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*timeValue = pLoopback->detectionTime;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_port_enable_set(uint32 port, uint32 isEnable)
{
	cfg_loopback_t *pLoopback = NULL;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->port_value.port_enable[port] = isEnable;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_port_enable_get(uint32 port, uint32 *isEnable)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == isEnable, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*isEnable = pLoopback->port_value.port_enable[port];

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_resumeTime_set(uint32 timeValue)
{
	cfg_loopback_t *pLoopback = NULL;

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->resumeTime = timeValue;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_resumeTime_get(uint32 *timeValue)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == timeValue, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*timeValue = pLoopback->resumeTime;
     CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_control_vlan_set(int32 bAdd)
{
	cfg_loopback_t *pLoopback = NULL;

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);
	
	pLoopback->bControlVlanAll = bAdd;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_control_vlan_get(int32 *controlVlan)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == controlVlan, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);
	*controlVlan = pLoopback->bControlVlanAll;

     CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_port_resume_mode_set(uint32 port, uint32 reMode)
{
	cfg_loopback_t *pLoopback = NULL;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->port_value.resume_mode[port] = reMode;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_port_resume_mode_get(uint32 port, uint32 *reMode)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == reMode, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*reMode = pLoopback->port_value.resume_mode[port];

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_proto_enable_set(sys_enable_t enable)
{
	cfg_loopback_t *pLoopback = NULL;

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->protoEnable = enable;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_proto_enable_get(sys_enable_t *pEnable)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*pEnable = pLoopback->protoEnable;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_proto_resume_check_set(sys_enable_t enable)
{
	cfg_loopback_t *pLoopback = NULL;

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	pLoopback->protoResumeCheckEnable = enable;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

int32 cfg_loopback_proto_resume_check_get(sys_enable_t *pEnable)
{
	cfg_loopback_t *pLoopback = NULL;

	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_LOOPBACK, (cfg_addr)&pLoopback);

	*pEnable = pLoopback->protoResumeCheckEnable;

	CFG_CLOSE(CFG_ID_LOOPBACK);
}

#if 0

int32 cfg_erps_init(void)
{
    cfg_erps_t *pErps = NULL;

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    CFG_CLOSE(CFG_ID_ERPS);

    return SYS_ERR_OK;
}

int32 cfg_erps_ringState_get(uint32 instId, sys_enable_t *pState)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pState, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pState = pErps->cfg_erps_inst[instId]->ringEnable;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringState_set(uint32 instId, sys_enable_t state)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId]->ringEnable = state;

	CFG_CLOSE(CFG_ID_ERPS);
}

#endif
