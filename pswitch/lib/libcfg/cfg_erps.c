
#include <libcfg/cfg.h>

int32 cfg_erps_init(void)
{
    cfg_erps_t *pErps = NULL;

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    CFG_CLOSE(CFG_ID_ERPS);

    return SYS_ERR_OK;
}

int32 cfg_erps_enable_set(sys_enable_t enable)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    pErps->enable = enable;

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_enable_get(sys_enable_t *pEnable)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    *pEnable = pErps->enable;

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_inst_create(uint32 instId)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_VLAN_ID);
    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	if (pErps->cfg_erps_inst[instId].enable != TRUE)
	{
		osal_memset(&pErps->cfg_erps_inst[instId], 0, sizeof(cfg_erps_inst_t));

	    pErps->cfg_erps_inst[instId].enable = TRUE;
		pErps->cfg_erps_inst[instId].workMode = SAL_ERPS_MODE_REVERTIVE;
		pErps->cfg_erps_inst[instId].ctrlVlanId = SAL_ERPS_INVALID_CTRLVLAN_ID;
		pErps->cfg_erps_inst[instId].ringId = SAL_ERPS_DFT_RING_ID;
		pErps->cfg_erps_inst[instId].mel = SAL_ERPS_INVALID_CTRLVLAN_ID;
		pErps->cfg_erps_inst[instId].TxRAPSTime = SAL_ERPS_DFT_TxRAPS_TIMEOUT;
		pErps->cfg_erps_inst[instId].WTBTime = SAL_ERPS_DFT_WTB_TIMEOUT;
		pErps->cfg_erps_inst[instId].WTRTime = SAL_ERPS_DFT_WTR_TIMEOUT;
		pErps->cfg_erps_inst[instId].guardTime = SAL_ERPS_DFT_GUARD_TIMEOUT;
		pErps->cfg_erps_inst[instId].holdoffTime = SAL_ERPS_DFT_HOLDOFF_TIMEOUT;
		pErps->cfg_erps_inst[instId].ringLevel = SAL_ERPS_RING_LEVEL_DEF;
		pErps->cfg_erps_inst[instId].pg_inst = SAL_ERPS_PROTECT_INSTANCE;
		pErps->cfg_erps_inst[instId].port0 = SAL_ERPS_RING_PORT_ID_DEF;
		pErps->cfg_erps_inst[instId].port1 = SAL_ERPS_RING_PORT_ID_DEF;
	}

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_inst_get(uint32 instId, uint32 *pBInstance)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);
	*pBInstance = pErps->cfg_erps_inst[instId].enable;
    CFG_CLOSE(CFG_ID_ERPS);
    return SYS_ERR_OK;
}

int32 cfg_erps_inst_del(uint32 instId)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(IS_VLAN_MSTI_INVALID(instId), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    osal_memset(&pErps->cfg_erps_inst[instId], 0, sizeof(cfg_erps_inst_t));

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ctrlVlan_get(uint32 instId, uint32 *ctrlVlanId)
{
    cfg_erps_t *pErps = NULL;

    SYS_PARAM_CHK(NULL == ctrlVlanId, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    *ctrlVlanId = pErps->cfg_erps_inst[instId].ctrlVlanId;

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ctrlVlan_set(uint32 instId, uint32 ctrlVlanId)
{
    cfg_erps_t *pErps = NULL;

    CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

    pErps->cfg_erps_inst[instId].ctrlVlanId = ctrlVlanId;

    CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_wtrTimer_get(uint32 instId, uint32 *pWtrTime)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pWtrTime, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pWtrTime = pErps->cfg_erps_inst[instId].WTRTime;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_wtrTimer_set(uint32 instId, uint32 wtrTime)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].WTRTime = wtrTime;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_guardTimer_get(uint32 instId, uint32 *pGuardTime)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pGuardTime, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pGuardTime = pErps->cfg_erps_inst[instId].guardTime;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_guardTimer_set(uint32 instId, uint32 guardTime)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].guardTime = guardTime;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_workMode_get(uint32 instId, uint32 *pWorkMode)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pWorkMode, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pWorkMode = pErps->cfg_erps_inst[instId].workMode;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_workMode_set(uint32 instId, uint32 workMode)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].workMode = workMode;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringId_get(uint32 instId, uint32 *pRingId)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pRingId, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pRingId = pErps->cfg_erps_inst[instId].ringId;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringId_set(uint32 instId, uint32 ringId)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].ringId = ringId;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringLevel_get(uint32 instId, uint32 *pRingLevel)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pRingLevel, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pRingLevel = pErps->cfg_erps_inst[instId].ringLevel;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringLevel_set(uint32 instId, uint32 ringLevel)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].ringLevel = ringLevel;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringPort_get(uint32 instId, uint8 isPort0, int32 *portId, uint32 *portRole)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == portId, SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK(NULL == portRole, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	if (isPort0 == TRUE)
	{
		*portId = pErps->cfg_erps_inst[instId].port0;
		*portRole = pErps->cfg_erps_inst[instId].port0Role;
	}
	else
	{
		*portId = pErps->cfg_erps_inst[instId].port1;
		*portRole = pErps->cfg_erps_inst[instId].port1Role;
	}

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringPort_add(uint32 instId, int32 portId, uint32 portRole, uint8 isPort0)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	if (isPort0 == TRUE)
	{
		pErps->cfg_erps_inst[instId].port0 = portId;
		pErps->cfg_erps_inst[instId].port0Role = portRole;
	}
	else
	{
		pErps->cfg_erps_inst[instId].port1 = portId;
		pErps->cfg_erps_inst[instId].port1Role = portRole;
	}

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringPort_del(uint32 instId, uint32 portId, uint8 isPort0)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	if (isPort0 == TRUE)
	{
		pErps->cfg_erps_inst[instId].port0 = SAL_ERPS_RING_PORT_ID_DEF;
		pErps->cfg_erps_inst[instId].port0Role = 0;
	}
	else
	{
		pErps->cfg_erps_inst[instId].port1 = SAL_ERPS_RING_PORT_ID_DEF;
		pErps->cfg_erps_inst[instId].port1Role = 0;
	}

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ring_protected_instance_get(uint32 instId, uint32 *pg_inst)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pg_inst, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pg_inst = pErps->cfg_erps_inst[instId].pg_inst;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ring_protected_instance_add(uint32 instId, uint32 pg_inst)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].pg_inst = pg_inst;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ring_protected_instance_del(uint32 instId, uint32 pg_inst)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].pg_inst = SAL_ERPS_PROTECT_INSTANCE;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_mel_get(uint32 instId, uint32 *pMel)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pMel, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pMel = pErps->cfg_erps_inst[instId].mel;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_mel_set(uint32 instId, uint32 mel)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].mel = mel;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringState_get(uint32 instId, sys_enable_t *pState)
{
	cfg_erps_t *pErps = NULL;

	SYS_PARAM_CHK(NULL == pState, SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	*pState = pErps->cfg_erps_inst[instId].ringEnable;

	CFG_CLOSE(CFG_ID_ERPS);
}

int32 cfg_erps_ringState_set(uint32 instId, sys_enable_t state)
{
	cfg_erps_t *pErps = NULL;

	CFG_OPEN(CFG_ID_ERPS, (cfg_addr)&pErps);

	pErps->cfg_erps_inst[instId].ringEnable = state;

	CFG_CLOSE(CFG_ID_ERPS);
}

