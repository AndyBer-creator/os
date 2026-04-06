
#include <libsal/sal_util.h>
#include <libsal/sal_loopback.h>
#include <libcfg/cfg_loopback.h>

extern int loopback_detectionTime_set(uint32 timeValue);
extern int loopback_resumeTime_set(uint32 timeValue);
extern int loopback_port_enable_set(uint32 port, uint32 isEnable);
extern int loopback_port_resume_mode_set(uint32 port, uint32 reMode);
extern int loopback_port_resume_manual_set(uint32 port,uint32 reManual);
extern int32 loopbackort_status_state_get(uint32 port, uint32 *state);
extern int loopback_proto_enable_set(sys_enable_t enable);
extern int loopback_proto_resume_check_set(sys_enable_t enable);
extern int32 loopback_port_status_state_get(uint32 port, uint32 *state);
extern int loopback_ski_control_vlan_set(int32 bConVlan);

int32 sal_loopback_detectionTime_set(uint32 timeValue)
{
	SYS_ERR_CHK(loopback_detectionTime_set(timeValue));
	SYS_ERR_CHK(cfg_loopback_detectionTime_set(timeValue));
	return SYS_ERR_OK;
}

int32 sal_loopback_detectionTime_get(uint32 *timeValue)
{
	SYS_PARAM_CHK(NULL == timeValue, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_detectionTime_get(timeValue));
	return SYS_ERR_OK;
}

int32 sal_loopback_resumeTime_set(uint32 timeValue)
{
	SYS_ERR_CHK(loopback_resumeTime_set(timeValue));
	SYS_ERR_CHK(cfg_loopback_resumeTime_set(timeValue));
	return SYS_ERR_OK;
}

int32 sal_loopback_resumeTime_get(uint32 *timeValue)
{
	SYS_PARAM_CHK(NULL == timeValue, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_resumeTime_get(timeValue));
	return SYS_ERR_OK;
}

int32 sal_loopback_port_enable_set(uint32 port, uint32 isEnable)
{
	SYS_ERR_CHK(loopback_port_enable_set(port, isEnable));
	SYS_ERR_CHK(cfg_loopback_port_enable_set(port, isEnable));
	
	return SYS_ERR_OK;
}

int32 sal_loopback_port_enable_get(uint32 port, uint32 *isEnable)
{
	SYS_PARAM_CHK(NULL == isEnable, SYS_ERR_NULL_POINTER);
	
	SYS_ERR_CHK(cfg_loopback_port_enable_get(port, isEnable));
	
	return SYS_ERR_OK;
}

int32 sal_loopback_port_resume_mode_set(uint32 port, uint32 reMode)
{
	
	SYS_ERR_CHK(loopback_port_resume_mode_set(port, reMode));
	SYS_ERR_CHK(cfg_loopback_port_resume_mode_set(port, reMode));
	
	return SYS_ERR_OK;
}

int32 sal_loopback_port_resume_mode_get(uint32 port, uint32 *reMode)
{
	SYS_PARAM_CHK(NULL == reMode, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_port_resume_mode_get(port, reMode));
	return SYS_ERR_OK;
}

int32 sal_loopback_port_resume_manual_set(uint32 port, uint32 reManual)
{
	SYS_ERR_CHK(loopback_port_resume_manual_set(port, reManual));
	return SYS_ERR_OK;
}

int32 sal_loopback_control_vlan_set(int32 bConVlan)
{
	SYS_ERR_CHK(cfg_loopback_control_vlan_set(bConVlan));
	SYS_ERR_CHK(loopback_ski_control_vlan_set(bConVlan));
	return SYS_ERR_OK;
}

int32 sal_loopback_control_vlan_get(int32 *controlVlan)
{
	SYS_PARAM_CHK(NULL == controlVlan, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_control_vlan_get(controlVlan));
	return SYS_ERR_OK;
}

int32 sal_loopback_port_status_state_get(uint32 port, uint32 *state)
{
	SYS_PARAM_CHK(NULL == state, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(loopback_port_status_state_get(port, state));
	return SYS_ERR_OK;
}

int32 sal_loopback_proto_enable_set(sys_enable_t enable)
{
	sys_enable_t ori_enable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_ERR_CHK(sal_loopback_proto_enable_get(&ori_enable));
    if (ori_enable != enable)
    {	
		SYS_ERR_CHK(loopback_proto_enable_set(enable));
		SYS_ERR_CHK(cfg_loopback_proto_enable_set(enable));
    }

	return SYS_ERR_OK;
}

int32 sal_loopback_proto_enable_get(sys_enable_t *pEnable)
{
	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_proto_enable_get(pEnable));
	return SYS_ERR_OK;
}

int32 sal_loopback_proto_resume_check_set(sys_enable_t enable)
{
	sys_enable_t ori_enable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_ERR_CHK(sal_loopback_proto_resume_check_get(&ori_enable));
    if (ori_enable != enable)
    {	
		SYS_ERR_CHK(loopback_proto_resume_check_set(enable));
		SYS_ERR_CHK(cfg_loopback_proto_resume_check_set(enable));
    }

	return SYS_ERR_OK;
}

int32 sal_loopback_proto_resume_check_get(sys_enable_t *pEnable)
{
	SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(cfg_loopback_proto_resume_check_get(pEnable));
	return SYS_ERR_OK;
}

#if 0

int32 sal_erps_ringState_set(uint32 instId, sys_enable_t state)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_ringState_set(instId, state));
    SYS_ERR_CHK(cfg_erps_ringState_set(instId, state));
    return SYS_ERR_OK;
}

int32 sal_erps_ringState_get(uint32 instId, uint32 *pState)
{
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ringState_get(instId, pState));

    return SYS_ERR_OK;
}
#endif

