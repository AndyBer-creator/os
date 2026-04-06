
#ifndef __SAL_ERPS_H__
#define __SAL_ERPS_H__

#include <libsal/sal_type.h>
#include <erps/proto_sys_erps.h>

#define SAL_ERPS_DEFAULT_CTRLVLAN_ID 1
#define SAL_ERPS_INVALID_CTRLVLAN_ID 0
#define SAL_ERPS_DFT_INST_MEL        0
#define SAL_ERPS_DFT_RING_ID         1
#define SAL_ERPS_PROTECT_INSTANCE    -1
#define SAL_ERPS_RING_LEVEL_DEF      0
#define SAL_ERPS_RING_PORT_ID_DEF    -1

#define SAL_ERPS_DFT_TxRAPS_TIMEOUT  2
#define SAL_ERPS_DFT_WTB_TIMEOUT     6   
#define SAL_ERPS_DFT_WTR_TIMEOUT     5   
#define SAL_ERPS_DFT_GUARD_TIMEOUT   500 
#define SAL_ERPS_DFT_HOLDOFF_TIMEOUT 0   

enum sal_erps_work_mode{
    SAL_ERPS_MODE_REVERTIVE = 0,            
    SAL_ERPS_MODE_NON_REVERTIVE         
};

enum sal_erps_port_role{
	SAL_ERPS_RPL_PORT,
	SAL_ERPS_PORT_OWNER,
	SAL_ERPS_PORT_NEIGHBOUR,
	SAL_ERPS_PORT_NEXT_NEIGHBOUR,
	SAL_ERPS_PORT_NONE
};

int32 sal_erps_enable_set(sys_enable_t enable);

int32 sal_erps_enable_get(sys_enable_t *pEnable);

int32 sal_erps_inst_create(uint32 instId);

int32 sal_erps_inst_get(uint32 instId , uint32 *pBInstance);

int32 sal_erps_inst_del(uint32 instId);

int32 sal_erps_ctrlVlan_set(uint32 instId, uint32 ctrlVlanId);

int32 sal_erps_ctrlVlan_get(uint32 instId, uint32 *pCtrlVlanId);

int32 sal_erps_wtrTimer_set(uint32 instId, uint32 wtrTime);

int32 sal_erps_wtrTimer_get(uint32 instId, uint32 *pWtrTime);

int32 sal_erps_guardTimer_set(uint32 instId, uint32 guardTime);

int32 sal_erps_guardTimer_get(uint32 instId, uint32 *pGuardTime);

int32 sal_erps_workMode_set(uint32 instId, uint32 workMode);

int32 sal_erps_workMode_get(uint32 instId, uint32 *pWorkMode);

int32 sal_erps_ringId_set(uint32 instId, uint32 ringId);

int32 sal_erps_ringId_get(uint32 instId, uint32 *pRingId);

int32 sal_erps_ringLevel_set(uint32 instId, uint32 ringLevel);

int32 sal_erps_ringPort_add(uint32 instId, int32 portId, uint32 portRole, uint8 isPort0);

 int32 sal_erps_ringPort_del(uint32 instId, uint8 isPort0);

 int32 sal_erps_ringPort_get(uint32 instId, uint8 isPort0, int32 *portId, uint32 *portRole);

 int32 sal_erps_ring_protected_instance_add(uint32 instId, uint32 gp_inst);

 int32 sal_erps_ring_protected_instance_del(uint32 instId, uint32 gp_inst);

 int32 sal_erps_ring_protected_instance_get(uint32 instId, uint32 *gp_inst);

 int32 sal_erps_mel_set(uint32 instId, uint32 mel);

 int32 sal_erps_mel_get(uint32 instId, uint32 *pMel);

 int32 sal_erps_ringState_set(uint32 instId, sys_enable_t state);

 int32 sal_erps_get_inst_state_status(uint32 instId, int32 *state);

int32 sal_erps_get_inst_port_state_status(uint32 portId, int32 *state);

 int32 sal_erps_ringState_get(uint32 instId, uint32 *pState);

int32 sal_erps_ringLevel_get(uint32 instId, uint32 *pRingLevel);

#endif 

