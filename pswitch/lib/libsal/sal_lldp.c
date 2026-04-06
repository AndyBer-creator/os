
#include <libsal/sal_util.h>
#include <libsal/sal_lldp.h>
#ifdef CONFIG_SYS_PROTO_STP
#include <libsal/sal_stp.h>
#endif
#include <libfds/fds_export.h>

const sys_text_t text_lldpPortState[] =
{
    {    "disabled"      },
    {     "Rx only"       },
    {     "Tx only"       },
    {       "Rx & Tx"       },
};

int32 sal_lldp_enable_set(sys_enable_t enable)
{
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    char loaderVer[SYS_BUF16_LEN];
    char loaderDate[SYS_BUF32_LEN];
    char boardname[SYS_BOARD_NAME_LEN], strTmp[SYS_BUF64_LEN];
    char serialnum[CAPA_SYS_SN_LEN];
    char hardwareVer[SYS_BUF64_LEN];
  #endif
  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    uint32 forceVersion;
    lldp_disable_action_t action;
  #endif

  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    sal_lldp_disableAction_get(&action);
    SYS_ERR_CHK(sal_stp_forceVersion_get(&forceVersion));
    if(DISABLED == enable && MSTP_BPDU_VERSION_MSTP == forceVersion && LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS == action)
        return SYS_ERR_LLDP_DISABLE_ACTION_FLOODING_MSTP;
  #endif

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_MEM_CLEAR(loaderVer);
    SYS_MEM_CLEAR(loaderDate);
    SYS_MEM_CLEAR(serialnum);
    SYS_MEM_CLEAR(hardwareVer);

    SYS_ERR_CHK(sal_sys_loaderInfo_get(loaderVer, loaderDate));
    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_FIRMWARE_REVISION, loaderVer));
    osal_strcpy(strTmp, DFLT_SYS_VER);
    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_SOFTWARE_REVISION, strTmp));

    SYS_ERR_CHK(sal_sys_boardModelName_get(boardname));
    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_MODEL_NAME, boardname));

    sal_sys_sysinfoVar_get(DFLT_SYS_SN_VAR, serialnum);
    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_SERIA_NUMBER, serialnum));

    sal_sys_sysinfoVar_get(DFLT_SYS_HWVER_VAR, hardwareVer);
    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_HARDWARE_REVISION, hardwareVer));

    SYS_ERR_CHK(lldp_medInventory_set(LLDP_INVENTORY_TYPE_MANUFACTURER, DFLT_SYS_MANUFACTURER));
  #endif 
    SYS_ERR_CHK(lldp_enable_set(enable));
    SYS_ERR_CHK(cfg_lldp_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_lldp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_lldp_port_set(sys_logic_port_t port, uint8 lldpPortState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lldp_port_set(port, lldpPortState));
    SYS_ERR_CHK(cfg_lldp_port_set(port,lldpPortState));

    return SYS_ERR_OK;
}

int32 sal_lldp_port_get(sys_logic_port_t port, uint8 *pLldpPortState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpPortState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_port_get(port, pLldpPortState));

    return SYS_ERR_OK;
}

int32 sal_lldp_holdtime_set(uint8 holdtime)
{
    uint32 txInterval;
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(holdtime, BOND_LLDP_HOLDTIME_MIN, BOND_LLDP_HOLDTIME_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_lldp_tx_interval_get(&txInterval));
    SYS_PARAM_CHK((65535 < txInterval*holdtime), SYS_ERR_LLDP_HOLDTIME_MUL_TX_INTERVAL);

    SYS_ERR_CHK(lldp_holdtime_set(holdtime));
    SYS_ERR_CHK(cfg_lldp_holdtime_set(holdtime));

    return SYS_ERR_OK;
}

int32 sal_lldp_holdtime_get(uint8 *pHoldtime)
{
    SYS_PARAM_CHK((NULL == pHoldtime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_holdtime_get(pHoldtime));

    return SYS_ERR_OK;
}

int32 sal_lldp_tx_interval_set(uint32 tx_interval)
{
    uint8 holdtime;
    uint16 txDelay;
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(tx_interval, BOND_LLDP_TX_INTERVAL_MIN, BOND_LLDP_TX_INTERVAL_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_lldp_holdtime_get(&holdtime));
    SYS_PARAM_CHK((65535 < tx_interval*holdtime), SYS_ERR_LLDP_HOLDTIME_MUL_TX_INTERVAL);

    SYS_ERR_CHK(sal_lldp_tx_delay_get(&txDelay));
    SYS_PARAM_CHK((txDelay > tx_interval/4), SYS_ERR_LLDP_TX_DELAY_TX_INTERVAL);

    SYS_ERR_CHK(lldp_tx_interval_set(tx_interval));
    SYS_ERR_CHK(cfg_lldp_txInterval_set(tx_interval));

    return SYS_ERR_OK;
}

int32 sal_lldp_tx_interval_get(uint32 *pTxInterval)
{
    SYS_PARAM_CHK((NULL == pTxInterval), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_txInterval_get(pTxInterval));

    return SYS_ERR_OK;
}

int32 sal_lldp_reinit_delay_set(uint8 reinit_delay)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(reinit_delay, BOND_LLDP_REINIT_DELAY_MIN, BOND_LLDP_REINIT_DELAY_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(lldp_reinit_delay_set(reinit_delay));
    SYS_ERR_CHK(cfg_lldp_reinitDelay_set(reinit_delay));

    return SYS_ERR_OK;
}

int32 sal_lldp_reinit_delay_get(uint8 *pReinitDelay)
{
    SYS_PARAM_CHK((NULL == pReinitDelay), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_reinitDelay_get(pReinitDelay));

    return SYS_ERR_OK;
}

int32 sal_lldp_tx_delay_set(uint16 txDelay)
{
    uint32 txInterval;

    SYS_ERR_CHK(sal_lldp_tx_interval_get(&txInterval));
    SYS_PARAM_CHK((BOND_LLDP_TX_DELAY_MIN > txDelay), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((txDelay > txInterval/4), SYS_ERR_LLDP_TX_DELAY_TX_INTERVAL);

    SYS_ERR_CHK(lldp_tx_delay_set(txDelay));
    SYS_ERR_CHK(cfg_lldp_tx_delay_set(txDelay));

    return SYS_ERR_OK;
}

int32 sal_lldp_tx_delay_get(uint16 *pTxDelay)
{
    SYS_PARAM_CHK((NULL == pTxDelay), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_tx_delay_get(pTxDelay));

    return SYS_ERR_OK;
}

int32 sal_lldp_portLocalEntryInfo_get(sys_logic_port_t port, display_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portLocalEntryInfo_get(port, pEntry));

    return SYS_ERR_OK;
}

int32 sal_lldp_portRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portRemoteEntryInfo_get(port, index, pEntry));

    return SYS_ERR_OK;
}

int32 sal_lldp_portRemoteEntryInfo_valid_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry)
{
    display_entry_t empEntry;
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    osal_memset(&empEntry, 0, sizeof(display_entry_t));
    SYS_ERR_CHK(lldp_portRemoteEntryInfo_get(port, index, pEntry));

    if(osal_memcmp(&empEntry, pEntry, sizeof(display_entry_t)) == 0)
    {
        return SYS_ERR_ENTRY_NOT_FOUND;
    }
    return SYS_ERR_OK;
}

int32 sal_lldp_portRemoteVlanNameTlv_get(sys_logic_port_t port, uint16 index, lldp_vlanName_tlv_t  *pVlanTlv)
{
    SYS_PARAM_CHK((NULL == pVlanTlv), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portRemoteVlanNameTlv_get(port, index, pVlanTlv));

    return SYS_ERR_OK;
}

int32 sal_lldp_portRemoteEntry_clear(sys_logic_port_t port, uint8 index)
{
    SYS_ERR_CHK(lldp_portRemoteEntry_clear(port, index));

    return SYS_ERR_OK;
}

int32 sal_lldp_statistic_get(lldp_statistic_t *pLldp_statistic)
{
    SYS_PARAM_CHK((NULL == pLldp_statistic), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_statistic_get(pLldp_statistic));

    return SYS_ERR_OK;
}

int32 sal_lldp_portEntry_get(sys_logic_port_t logicPort, lldp_port_entry_t *pLldp_portEntry)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldp_portEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portEntry_get(logicPort, pLldp_portEntry));

    return SYS_ERR_OK;
}

int32 sal_lldp_globalStatistics_clear(void)
{
    SYS_ERR_CHK(lldp_globalStatistics_clear());

    return SYS_ERR_OK;
}

int32 sal_lldp_portStatistics_clear(sys_logic_port_t port)
{
    SYS_ERR_CHK(lldp_portStatistics_clear(port));

    return SYS_ERR_OK;
}

int32 sal_lldp_disableAction_set(lldp_disable_action_t action)
{
  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    uint32 forceVersion;
    sys_enable_t enable;
  #endif

    SYS_PARAM_CHK((action >= LLDP_DISABLE_ACTION_END), SYS_ERR_INPUT);

  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    SYS_ERR_CHK(sal_lldp_enable_get(&enable));
    SYS_ERR_CHK(sal_stp_forceVersion_get(&forceVersion));
    if(DISABLED == enable && MSTP_BPDU_VERSION_MSTP == forceVersion && LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS == action)
        return SYS_ERR_LLDP_DISABLE_ACTION_FLOODING_MSTP;
  #endif

    SYS_ERR_CHK(lldp_disableAction_set(action));
    SYS_ERR_CHK(cfg_lldp_disableAction_set(action));

    switch(action)
    {
        case LLDP_DISABLE_ACTION_FILTER:
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            break;
        case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
            break;
        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_lldp_disableAction_get(lldp_disable_action_t *pAction)
{
    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_disableAction_get(pAction));

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(pvidEnable), SYS_ERR_INPUT);

    SYS_ERR_CHK(lldp_portTlvPvidEnable_set(port, pvidEnable));
    SYS_ERR_CHK(cfg_lldp_portTlvPvidEnable_set(port, pvidEnable));

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPvidEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portTlvPvidEnable_get(port, pPvidEnable));

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t vlanmask)
{
    char strVlanList[CAPA_VLANLIST_STR_LEN];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_ERR_CHK(lldp_portTlvVlanmask_set(port, vlanmask));
    SYS_ERR_CHK(cfg_lldp_portTlvVlanmask_set(port, vlanmask));

    osal_memset(strVlanList, 0, sizeof(strVlanList));
    VMSK2STR(&vlanmask, strVlanList);

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portTlvVlanmask_get(port, pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel)
{
    char tlvStr[256];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lldp_portTlvSel_set(port, lldpTlvSel));
    SYS_ERR_CHK(cfg_lldp_portTlvSel_set(port,lldpTlvSel));

    osal_memset(tlvStr, 0, sizeof(tlvStr));

    if(TRUE == lldpTlvSel.portDesciptionSel)
        osal_strcat(tlvStr, " port-desc");
    if(TRUE == lldpTlvSel.sysNameSel)
        osal_strcat(tlvStr, " sys-name");
    if(TRUE == lldpTlvSel.sysDescriptionSel)
        osal_strcat(tlvStr, " sys-desc");
    if(TRUE == lldpTlvSel.sysCapabilitySel)
        osal_strcat(tlvStr, " sys-cap");
    if(TRUE == lldpTlvSel.macPhySel)
        osal_strcat(tlvStr, " mac-phy");
    if(TRUE == lldpTlvSel.linkAggregationSel)
        osal_strcat(tlvStr, " lag");
    if(TRUE == lldpTlvSel.maxFrameSizeSel)
        osal_strcat(tlvStr, " max-frame-size");
    if(TRUE == lldpTlvSel.managementAddrSel)
        osal_strcat(tlvStr, " management-addr");

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpTlvSel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portTlvSel_get(port, pLldpTlvSel));

    return SYS_ERR_OK;
}

int32 sal_lldp_portTlvOverload_get(sys_logic_port_t port, lldp_overLoading_t *pOverload)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pOverload), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portTlvOverload_get(port, pOverload));

    return SYS_ERR_OK;
}

int32 sal_lldp_remoteTableChangeTime_get(uint32 *pChangeTime)
{
    SYS_PARAM_CHK((NULL == pChangeTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_remoteTableChangeTime_get(pChangeTime));

    return SYS_ERR_OK;
}

int32 sal_lldp_managementAddressStr_get(manage_address_t manageAddrTlv, uint8 *strAddr)
{
    SYS_PARAM_CHK((NULL == strAddr), SYS_ERR_NULL_POINTER);

    switch(manageAddrTlv.iSubtype)
    {
        case 1: 
            inet_ntop(AF_INET, manageAddrTlv.addr, (char *)strAddr, CAPA_IPV4_STR_LEN);
            break;
        case 2: 
            inet_ntop(AF_INET6, manageAddrTlv.addr, (char *)strAddr, CAPA_IPV6_STR_LEN);
            break;
        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_lldp_txOverloadStatus_set(sys_logic_port_t port, uint8 txOverloadStatus)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_lldp_txOverloadStatus_set(port, txOverloadStatus));

    SYS_LOG(LOG_SEV_DEBUG, LOG_CAT_LLDP, "Port %u LLDP operation status is set to %s",
            LP2UP(port),
            txOverloadStatus == TRUE ? "Overloading" : "Not Overloading");

    return SYS_ERR_OK;
}

int32 sal_lldp_txOverloadStatus_get(sys_logic_port_t port, uint8 *pTxOverloadStatus)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTxOverloadStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_txOverloadStatus_get(port, pTxOverloadStatus));

    return SYS_ERR_OK;
}

int32 sal_lldp_txOverload_check(sys_logic_port_t port)
{
    uint8 pre_overload;
    lldp_overLoading_t cur_overload;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(sal_lldp_txOverloadStatus_get(port, &pre_overload));

    SYS_ERR_CHK(sal_lldp_portTlvOverload_get(port, &cur_overload));

    if (FALSE == pre_overload && TRUE == cur_overload.isOverload)
    {
        SYS_ERR_CHK(sal_lldp_txOverloadStatus_set(port, cur_overload.isOverload));
        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TXOVERLOAD, "Enter", LPSTR(port));
    }
    else if (TRUE == pre_overload && FALSE == cur_overload.isOverload)
    {
        SYS_ERR_CHK(sal_lldp_txOverloadStatus_set(port, cur_overload.isOverload));
        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TXOVERLOAD, "Exit", LPSTR(port));
    }

    return SYS_ERR_OK;
}

