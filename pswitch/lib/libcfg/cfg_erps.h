
#ifndef __CFG_ERPS_H__
#define __CFG_ERPS_H__

#include <libsal/sal_erps.h>

typedef struct cfg_erps_inst_s{
	 uint8 enable;		 
	 uint32 mel;			   
	 int32 workMode;			  
	 int32 TxRAPSTime;
	 int32 WTRTime;
	 int32 WTBTime;
	 int32 guardTime;
	 int32 holdoffTime;
	 int32 pg_inst;
	 int32 ringId;							  
	 int32 ringLevel;						  
	 int32 ctrlVlanId;						  
	 sys_enable_t ringEnable;					 
	 int8 port0;					  
	 int32 port0Role;
	 int8 port1;					  
	 int32 port1Role;
	 
 }cfg_erps_inst_t;

 typedef struct cfg_erps_s
{
    
    sys_enable_t enable;
	cfg_erps_inst_t cfg_erps_inst[ERPS_MAX_RING_INS];
} cfg_erps_t;

extern int32 cfg_erps_init(void);

extern int32 cfg_erps_enable_set(sys_enable_t enable);

extern int32 cfg_erps_enable_get(sys_enable_t *pEnable);

extern int32 cfg_erps_inst_create(uint32 instId);
extern int32 cfg_erps_inst_get(uint32 instId, uint32 *pBInstance);

extern int32 cfg_erps_inst_del(uint32 instId);

extern int32 cfg_erps_ctrlVlan_get(uint32 instId, uint32 *ctrlVlanId);

extern int32 cfg_erps_ctrlVlan_set(uint32 instId, uint32 ctrlVlanId);

extern int32 cfg_erps_wtrTimer_get(uint32 instId, uint32 *pWtrTime);

extern int32 cfg_erps_wtrTimer_set(uint32 instId, uint32 wtrTime);

extern int32 cfg_erps_guardTimer_get(uint32 instId, uint32 *pGuardTime);

extern int32 cfg_erps_guardTimer_set(uint32 instId, uint32 guardTime);

extern int32 cfg_erps_workMode_get(uint32 instId, uint32 *pWorkMode);

extern int32 cfg_erps_workMode_set(uint32 instId, uint32 workMode);

extern int32 cfg_erps_ringId_get(uint32 instId, uint32 *pRingId);

extern int32 cfg_erps_ringId_set(uint32 instId, uint32 ringId);

extern int32 cfg_erps_ringLevel_get(uint32 instId, uint32 *pRingLevel);

extern int32 cfg_erps_ringLevel_set(uint32 instId, uint32 ringLevel);

extern int32 cfg_erps_ringPort_get(uint32 instId, uint8 isPort0, int32 *portId, uint32 *portRole);

extern int32 cfg_erps_ringPort_add(uint32 instId, int32 portId, uint32 portRole, uint8 isPort0);

extern int32 cfg_erps_ringPort_del(uint32 instId, uint32 portId, uint8 isPort0);

extern int32 cfg_erps_ring_protected_instance_get(uint32 instId, uint32 *pg_inst);

extern int32 cfg_erps_ring_protected_instance_add(uint32 instId, uint32 pg_inst);

extern int32 cfg_erps_ring_protected_instance_del(uint32 instId, uint32 pg_inst);

extern int32 cfg_erps_mel_get(uint32 instId, uint32 *pMel);

extern int32 cfg_erps_mel_set(uint32 instId, uint32 mel);

int32 cfg_erps_ringState_get(uint32 instId, sys_enable_t *pState);

int32 cfg_erps_ringState_set(uint32 instId, sys_enable_t state);

#endif

