
#ifndef __SAL_LLDP_H__
#define __SAL_LLDP_H__

#ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <libsal/sal_lldp_med.h>
#endif
#include <lldp/proto_lldp.h>

extern int32 sal_lldp_enable_set(sys_enable_t enable);

extern int32 sal_lldp_enable_get(sys_enable_t *pEnable);

extern int32 sal_lldp_port_set(sys_logic_port_t port, uint8 lldpPortState);

extern int32 sal_lldp_port_get(sys_logic_port_t port, uint8 *pLldpPortState);

extern int32 sal_lldp_holdtime_set(uint8 holdtime);

extern int32 sal_lldp_holdtime_get(uint8 *pHoldtime);

extern int32 sal_lldp_tx_interval_set(uint32 tx_interval);

extern int32 sal_lldp_tx_interval_get(uint32 *pTxInterval);

extern int32 sal_lldp_reinit_delay_set(uint8 reinit_delay);

extern int32 sal_lldp_reinit_delay_get(uint8 *pReinitDelay);

extern int32 sal_lldp_tx_delay_set(uint16 txDelay);

extern int32 sal_lldp_tx_delay_get(uint16 *pTxDelay);

extern int32 sal_lldp_portLocalEntryInfo_get(sys_logic_port_t port, display_entry_t *pEntry);

extern int32 sal_lldp_portRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry);

extern int32 sal_lldp_portRemoteEntryInfo_valid_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry);

extern int32 sal_lldp_portRemoteVlanNameTlv_get(sys_logic_port_t port, uint16 index, lldp_vlanName_tlv_t  *pVlanTlv);

extern int32 sal_lldp_portRemoteEntry_clear(sys_logic_port_t port, uint8 index);

extern int32 sal_lldp_statistic_get(lldp_statistic_t *pLldp_statistic);

extern int32 sal_lldp_portEntry_get(sys_logic_port_t logicPort, lldp_port_entry_t *pLldp_portEntry);

extern int32 sal_lldp_globalStatistics_clear(void);

extern int32 sal_lldp_portStatistics_clear(sys_logic_port_t port);

extern int32 sal_lldp_disableAction_set(lldp_disable_action_t action);

extern int32 sal_lldp_disableAction_get(lldp_disable_action_t *pAction);

extern int32 sal_lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable);

extern int32 sal_lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable);

extern int32 sal_lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t vlanmask);

extern int32 sal_lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask);

extern int32 sal_lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel);

extern int32 sal_lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel);

extern int32 sal_lldp_portTlvOverload_get(sys_logic_port_t port, lldp_overLoading_t *pOverload);

extern int32 sal_lldp_remoteTableChangeTime_get(uint32 *pChangeTime);

extern int32 sal_lldp_managementAddressStr_get(manage_address_t manageAddrTlv, uint8 *strAddr);

extern int32 sal_lldp_txOverloadStatus_set(sys_logic_port_t port, uint8 txOverloadStatus);

extern int32 sal_lldp_txOverloadStatus_get(sys_logic_port_t port, uint8 *pTxOverloadStatus);

extern int32 sal_lldp_txOverload_check(sys_logic_port_t port);

#endif 

