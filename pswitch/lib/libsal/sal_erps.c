
#include <libsal/sal_util.h>
#include <libsal/sal_erps.h>

int32 sal_erps_enable_set(sys_enable_t enable)
{
    sys_enable_t ori_enable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_erps_enable_get(&ori_enable));

    if (ori_enable != enable)
    {
        SYS_ERR_CHK(erps_enable_set(enable));
		SYS_ERR_CHK(cfg_erps_enable_set(enable));
    }

    return SYS_ERR_OK;
}

int32 sal_erps_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(cfg_erps_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_erps_inst_create(uint32 instId)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
    SYS_ERR_CHK(erps_inst_create(instId));
    SYS_ERR_CHK(cfg_erps_inst_create(instId));

    return SYS_ERR_OK;
}

int32 sal_erps_inst_get(uint32 instId , uint32 *pBInstance)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
    SYS_ERR_CHK(cfg_erps_inst_get(instId, pBInstance));

    return SYS_ERR_OK;
}

int32 sal_erps_inst_del(uint32 instId)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_inst_delete(instId));
    SYS_ERR_CHK(cfg_erps_inst_del(instId));

    return SYS_ERR_OK;
}

int32 sal_erps_ctrlVlan_set(uint32 instId, uint32 ctrlVlanId)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(ctrlVlanId), SYS_ERR_INPUT);
	if (FALSE == IS_STATIC_VLAN_EXIST(ctrlVlanId))
	{

	    return SYS_ERR_VLAN_NOT_EXIST;
	}

    SYS_ERR_CHK(erps_ctrlVlan_set(instId, ctrlVlanId));
    SYS_ERR_CHK(cfg_erps_ctrlVlan_set(instId, ctrlVlanId));

    return SYS_ERR_OK;
}

int32 sal_erps_ctrlVlan_get(uint32 instId, uint32 *pCtrlVlanId)
{
    SYS_PARAM_CHK((NULL == pCtrlVlanId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ctrlVlan_get(instId, pCtrlVlanId));

    return SYS_ERR_OK;
}

int32 sal_erps_wtrTimer_set(uint32 instId, uint32 wtrTime)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_timer_set(ERPS_WTR_TIME, instId, wtrTime));
    SYS_ERR_CHK(cfg_erps_wtrTimer_set(instId, wtrTime));

    return SYS_ERR_OK;
}

int32 sal_erps_wtrTimer_get(uint32 instId, uint32 *pWtrTime)
{
    SYS_PARAM_CHK((NULL == pWtrTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_wtrTimer_get(instId, pWtrTime));

    return SYS_ERR_OK;
}

int32 sal_erps_guardTimer_set(uint32 instId, uint32 guardTime)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_timer_set(ERPS_GUARD_TIME, instId, guardTime));
    SYS_ERR_CHK(cfg_erps_guardTimer_set(instId, guardTime));

    return SYS_ERR_OK;
}

int32 sal_erps_guardTimer_get(uint32 instId, uint32 *pGuardTime)
{
    SYS_PARAM_CHK((NULL == pGuardTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_guardTimer_get(instId, pGuardTime));

    return SYS_ERR_OK;
}

int32 sal_erps_workMode_set(uint32 instId, uint32 workMode)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_workMode_set(instId, workMode));
    SYS_ERR_CHK(cfg_erps_workMode_set(instId, workMode));

    return SYS_ERR_OK;
}

int32 sal_erps_workMode_get(uint32 instId, uint32 *pWorkMode)
{
    SYS_PARAM_CHK((NULL == pWorkMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_workMode_get(instId, pWorkMode));

    return SYS_ERR_OK;
}

int32 sal_erps_ringId_set(uint32 instId, uint32 ringId)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_ringId_set(instId, ringId));
    SYS_ERR_CHK(cfg_erps_ringId_set(instId, ringId));

    return SYS_ERR_OK;
}

int32 sal_erps_ringId_get(uint32 instId, uint32 *pRingId)
{
    SYS_PARAM_CHK((NULL == pRingId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ringId_get(instId, pRingId));

    return SYS_ERR_OK;
}

int32 sal_erps_ringLevel_set(uint32 instId, uint32 ringLevel)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
    SYS_ERR_CHK(erps_ringLevel_set(instId, ringLevel));
    SYS_ERR_CHK(cfg_erps_ringLevel_set(instId, ringLevel));

    return SYS_ERR_OK;
}

int32 sal_erps_ringLevel_get(uint32 instId, uint32 *pRingLevel)
{
    SYS_PARAM_CHK((NULL == pRingLevel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ringLevel_get(instId, pRingLevel));

    return SYS_ERR_OK;
}

int32 sal_erps_ringPort_add(uint32 instId, int32 portId, uint32 portRole, uint8 isPort0)
{	
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
    SYS_ERR_CHK(erps_ringPort_set(instId, portId, portRole, isPort0, TRUE));
    SYS_ERR_CHK(cfg_erps_ringPort_add(instId, portId, portRole, isPort0));
    
    return SYS_ERR_OK;
}

int32 sal_erps_ringPort_del(uint32 instId, uint8 isPort0)
{
	int32 portId = 0;
	uint32 portRole = 0;

	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

	sal_erps_ringPort_get(instId, isPort0, &portId, &portRole);
    SYS_ERR_CHK(erps_ringPort_set(instId, portId, portRole, isPort0, FALSE));
    SYS_ERR_CHK(cfg_erps_ringPort_del(instId, isPort0, portId));

    return SYS_ERR_OK;
}

int32 sal_erps_ringPort_get(uint32 instId, uint8 isPort0, int32 *portId, uint32 *portRole)
{
    SYS_PARAM_CHK((NULL == portId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ringPort_get(instId, isPort0, portId, portRole));

    return SYS_ERR_OK;
}

int32 sal_erps_ring_protected_instance_add(uint32 instId, uint32 gp_inst)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_inst_pgInst_set(instId, gp_inst, Erps_Protected_Inst_Add));
    SYS_ERR_CHK(cfg_erps_ring_protected_instance_add(instId, gp_inst));

    return SYS_ERR_OK;
}

int32 sal_erps_ring_protected_instance_del(uint32 instId, uint32 gp_inst)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_inst_pgInst_set(instId, gp_inst, Erps_Protected_Inst_Del));
    SYS_ERR_CHK(cfg_erps_ring_protected_instance_del(instId, gp_inst));

    return SYS_ERR_OK;
}

int32 sal_erps_ring_protected_instance_get(uint32 instId, uint32 *gp_inst)
{
    SYS_PARAM_CHK((NULL == gp_inst), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ring_protected_instance_get(instId, gp_inst));

    return SYS_ERR_OK;
}

int32 sal_erps_mel_set(uint32 instId, uint32 mel)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_mel_set(instId, mel));
    SYS_ERR_CHK(cfg_erps_mel_set(instId, mel));

    return SYS_ERR_OK;
}

int32 sal_erps_mel_get(uint32 instId, uint32 *pMel)
{
    SYS_PARAM_CHK((NULL == pMel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_mel_get(instId, pMel));

    return SYS_ERR_OK;
}

int32 sal_erps_ringState_set(uint32 instId, sys_enable_t state)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);

    SYS_ERR_CHK(erps_ringState_set(instId, state));
    SYS_ERR_CHK(cfg_erps_ringState_set(instId, state));
    return SYS_ERR_OK;
}

int32 sal_erps_get_inst_state_status(uint32 instId, int32 *state)
{
	SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_INPUT);
	SYS_PARAM_CHK((NULL == state), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(erps_get_inst_state_status(instId, state));
    return SYS_ERR_OK;
}

int32 sal_erps_get_inst_port_state_status(uint32 portId, int32 *state)
{
	SYS_PARAM_CHK((NULL == state), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(erps_get_inst_port_state_status(portId, state));
    return SYS_ERR_OK;
}

int32 sal_erps_ringState_get(uint32 instId, uint32 *pState)
{
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_erps_ringState_get(instId, pState));

    return SYS_ERR_OK;
}

#if 0

int32 sal_stp_info_get(sys_stp_info_t *pStpInfo)
{
    SYS_PARAM_CHK((NULL == pStpInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_info_get(pStpInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_portInfo_get(sys_logic_port_t port, sys_stp_port_info_t *pStpPortInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStpPortInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_portInfo_get(port, pStpPortInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_portCounter_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portCounter_clear(port));

    return SYS_ERR_OK;
}

int32 sal_stp_mstInfo_get(uint32 instance, sys_stp_mst_info_t *pMstInfo)
{
    SYS_PARAM_CHK((NULL == pMstInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstInfo_get(instance, pMstInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortInfo_get(uint32 instance, sys_logic_port_t port, sys_stp_mst_port_info_t *pMstPortInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pMstPortInfo, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstPortInfo_get(instance, port, pMstPortInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortMsg_get(uint32 instance, sys_logic_port_t port, sys_stp_mstPortMsg_t *pMstPortMsg)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pMstPortMsg, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstPortMsg_get(instance, port, pMstPortMsg));

    return SYS_ERR_OK;
}
#endif
