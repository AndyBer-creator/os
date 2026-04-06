
#ifndef __CFG_LLDP_H__
#define __CFG_LLDP_H__

#include <libsal/sal_lldp.h>

typedef struct cfg_lldp_s
{
    sys_enable_t enable;
    uint32       lldpPortState[SYS_LOGICPORT_NUM_MAX];
    uint32       holdtime;
    uint32       reinit_delay;
    uint32       tx_interval;
    uint16       tx_delay;
    lldp_disable_action_t action;
    lldp_tlv_sel_t lldpTlvSel[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t    tlvPvidEnable[SYS_LOGICPORT_NUM_MAX];
    sys_vlanmask_t  tlvVlanmask[SYS_LOGICPORT_NUM_MAX];
    uint8           txOverloading[SYS_LOGICPORT_NUM_MAX];
} cfg_lldp_t;

extern int32 cfg_lldp_enable_set(sys_enable_t enable);

extern int32 cfg_lldp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_lldp_port_set(sys_logic_port_t port, uint8 lldpPortState);

extern int32 cfg_lldp_port_get(sys_logic_port_t port ,uint8 *pLldpPortState);

extern int32 cfg_lldp_holdtime_set(uint8 holdtime);

extern int32 cfg_lldp_holdtime_get(uint8 *pHoldtime);

extern int32 cfg_lldp_txInterval_set(uint32 tx_interval);

extern int32 cfg_lldp_txInterval_get(uint32 *pTxInterval);

extern int32 cfg_lldp_reinitDelay_set(uint8 reinit_delay);

extern int32 cfg_lldp_reinitDelay_get(uint8 *pReinitDelay);

extern int32 cfg_lldp_tx_delay_set(uint16 txDelay);

extern int32 cfg_lldp_tx_delay_get(uint16 *pTxDelay);

extern int32 cfg_lldp_txOverloadStatus_set(sys_logic_port_t port, uint8 txOverloadStatus);

extern int32 cfg_lldp_txOverloadStatus_get(sys_logic_port_t port ,uint8 *pTxOverloadStatus);

extern int32 cfg_lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable);

extern int32 cfg_lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable);

extern int32 cfg_lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t  vlanmask);

extern int32 cfg_lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 cfg_lldp_disableAction_set(lldp_disable_action_t action);

extern int32 cfg_lldp_disableAction_get(lldp_disable_action_t *pAction);

extern int32 cfg_lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel);

extern int32 cfg_lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel);

#endif 

