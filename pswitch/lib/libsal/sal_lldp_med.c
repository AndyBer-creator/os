
#include <libsal/sal_util.h>
#include <libsal/sal_lldp.h>
#include <libfds/fds_export.h>

int32 sal_lldp_medFastRepeatCount_set(uint8 fastRepeatCount)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(fastRepeatCount, BOND_LLDP_MED_FAST_REPEAT_COUNT_MIN, BOND_LLDP_MED_FAST_REPEAT_COUNT_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(lldp_medFastRepeatCount_set(fastRepeatCount));
    SYS_ERR_CHK(cfg_lldp_medFastRepeatCount_set(fastRepeatCount));

    return SYS_ERR_OK;
}

int32 sal_lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount)
{
    SYS_PARAM_CHK((NULL == pFastRepeatCount), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_medFastRepeatCount_get(pFastRepeatCount));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedLocalEntryInfo_get(sys_logic_port_t port, display_med_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portMedLocalEntryInfo_get(port, pEntry));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_med_entry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(lldp_portMedRemoteEntryInfo_get(port, index, pEntry));

    return SYS_ERR_OK;
}

int32 sal_lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy)
{
    char policyStr[512], strTmp[32];
    sys_logic_port_t port;
    lldp_med_networkPolicy_bind_t policyBind;
    sys_enable_t enable;

    SYS_PARAM_CHK((idx >= CAPA_LLDP_MED_NETWORK_POLICY_NUM), SYS_ERR_OUT_OF_RANGE);
    if(TRUE == policy.valid)
    {
        SYS_PARAM_CHK((policy.app >= LLDP_APP_END), SYS_ERR_INPUT);
        SYS_PARAM_CHK(((((policy.vid) < BOND_LLDP_MED_NETWORK_POLICY_VID_MIN) ||
            ((policy.vid) > BOND_LLDP_MED_NETWORK_POLICY_VID_MAX)) ? 1 : 0), SYS_ERR_VLAN_ID);
        SYS_PARAM_CHK((policy.userPriority > BOND_LLDP_MED_USER_PRI_MAX), SYS_ERR_OUT_OF_RANGE);
        SYS_PARAM_CHK((policy.dscpValue > BOND_LLDP_MED_DSCP_PRI_MAX), SYS_ERR_OUT_OF_RANGE);
    }

    SYS_ERR_CHK(sal_lldp_medNetworkPolicyAuto_get(&enable));
    if((ENABLED == enable) && (1 == policy.valid) && (LLDP_APP_VOICE == policy.app))
    {
        return SYS_ERR_LLDP_NETWORK_POLICY_VOICE_WHEN_AUTO;
    }

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        SYS_ERR_TRK_CHK(sal_lldp_portMedNetworkPolicyBind_get(port, &policyBind));
        if(policyBind.bindMask & (1 << idx))
        {
            return SYS_ERR_LLDP_NETWORK_POLICY_BINDED;
        }
    }

    osal_memset(policyStr, 0, sizeof(policyStr));

    if(TRUE == policy.valid)
    {
        osal_strcat(policyStr, " valid, app:");
        switch(policy.app)
        {
            case LLDP_APP_VOICE:
                osal_strcat(policyStr, "voice");
                osal_strcpy(policy.appStr, "Voice");
                break;
            case LLDP_APP_VOICE_SIGNALING:
                osal_strcat(policyStr, "voice-signaling");
                osal_strcpy(policy.appStr, "Voice Signaling");
                break;
            case LLDP_APP_GUEST_VOICE:
                osal_strcat(policyStr, "guest-voice");
                osal_strcpy(policy.appStr, "Guest Voice");
                break;
            case LLDP_APP_GUEST_VOICE_SIGNALING:
                osal_strcat(policyStr, "guest-voice-signaling");
                osal_strcpy(policy.appStr, "Guest Voice Signaling");
                break;
            case LLDP_APP_SOFTPHONE_VOICE:
                osal_strcat(policyStr, "softphone-voice");
                osal_strcpy(policy.appStr, "Softphone Voice");
                break;
            case LLDP_APP_VIDEO_CONFERENCING:
                osal_strcat(policyStr, "video-conferencing");
                osal_strcpy(policy.appStr, "Video Conferencing");
                break;
            case LLDP_APP_STREAMING_VIDEO:
                osal_strcat(policyStr, "streaming-video");
                osal_strcpy(policy.appStr, "Streaming Video");
                break;
            case LLDP_APP_VIDEO_SIGNALING:
                osal_strcat(policyStr, "video-signaling");
                osal_strcpy(policy.appStr, "Video Signaling");
                break;
            default:
                break;
        }
        osal_sprintf(strTmp, ", vid:%d", policy.vid);
        osal_strcat(policyStr, strTmp);
        osal_sprintf(strTmp, ", vlanTag:%s", policy.vlanTagged?"tag":"untag");
        osal_strcat(policyStr, strTmp);
        osal_sprintf(strTmp, ", user priority:%d", policy.userPriority);
        osal_strcat(policyStr, strTmp);
        osal_sprintf(strTmp, ", dscp priority:%d", policy.dscpValue);
        osal_strcat(policyStr, strTmp);
    }
    else
        osal_strcat(policyStr, " invalid");

    SYS_ERR_CHK(lldp_medNetworkPolicy_set(idx, policy));
    SYS_ERR_CHK(cfg_lldp_medNetworkPolicy_set(idx, policy));

    return SYS_ERR_OK;
}

int32 sal_lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy)
{
    SYS_PARAM_CHK((idx >= CAPA_LLDP_MED_NETWORK_POLICY_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pPolicy), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_medNetworkPolicy_get(idx, pPolicy));
    return SYS_ERR_OK;
}

int32 sal_lldp_medNetworkPolicyAuto_set(sys_enable_t enable)
{
    int32 i;
    lldp_med_networkPolicy_t policy;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    for(i = 0; i < CAPA_LLDP_MED_NETWORK_POLICY_NUM; i ++)
    {
        SYS_ERR_CHK(sal_lldp_medNetworkPolicy_get(i, &policy));
        if(TRUE == policy.valid && LLDP_APP_VOICE == policy.app && ENABLED == enable)
            return SYS_ERR_LLDP_NETWORK_POLICY_AUTO_WHEN_VOICE;
    }

    SYS_ERR_CHK(lldp_medNetworkPolicyAuto_set(enable));
    SYS_ERR_CHK(cfg_lldp_medNetworkPolicyAuto_set(enable));

    return SYS_ERR_OK;
}

int32 sal_lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_medNetworkPolicyAuto_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel)
{
    char tlvStr[256];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (IS_NOT_POE_PORT(port))
        lldpMedTlvSel.poePseSel = FALSE;

    SYS_ERR_CHK(lldp_portMedTlvSel_set(port, lldpMedTlvSel));
    SYS_ERR_CHK(cfg_lldp_portMedTlvSel_set(port,lldpMedTlvSel));

    osal_memset(tlvStr, 0, sizeof(tlvStr));

    if (TRUE == lldpMedTlvSel.networkPolicySel)
        osal_strcat(tlvStr, " network-policy");
    if (TRUE == lldpMedTlvSel.locationSel)
        osal_strcat(tlvStr, " location");
    if (TRUE == lldpMedTlvSel.poePseSel)
        osal_strcat(tlvStr, " poe-pse");
    if (TRUE == lldpMedTlvSel.inventorySel)
        osal_strcat(tlvStr, " inventory");

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpMedTlvSel), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portMedTlvSel_get(port, pLldpMedTlvSel));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lldp_portMedEnable_set(port, enable));
    SYS_ERR_CHK(cfg_lldp_portMedEnable_set(port,enable));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portMedEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind)
{
    char bindStr[128];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lldp_portMedNetworkPolicyBind_set(port, policyBind));
    SYS_ERR_CHK(cfg_lldp_portMedNetworkPolicyBind_set(port,policyBind));

    osal_memset(bindStr, 0, sizeof(bindStr));

    sys_util_ulMask2Str(policyBind.bindMask, bindStr);

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPolicyBind), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portMedNetworkPolicyBind_get(port, pPolicyBind));

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location)
{
    char locationStr[512], strTmp0[512], strTmp1[512], strTmp2[512];

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(lldp_portMedLocation_set(port, location));
    SYS_ERR_CHK(cfg_lldp_portMedLocation_set(port, location));

    osal_memset(locationStr, 0, sizeof(locationStr));
    osal_strcpy(strTmp0, "null");
    osal_strcpy(strTmp1, "null");
    osal_strcpy(strTmp2, "null");

    if(0 != osal_strcmp((char*)location.coordinate, ""))
    {
        sys_util_byteArray2hexStr(location.coordinate, location.coorLen, locationStr);
        osal_strcpy(strTmp0, locationStr);
    }
    if(0 != osal_strcmp((char*)location.civicAddress, ""))
    {
        sys_util_byteArray2hexStr(location.civicAddress, location.civicLen, locationStr);
        osal_strcpy(strTmp1, locationStr);
    }
    if(0 != osal_strcmp((char*)location.ecsElin, ""))
    {
        sys_util_byteArray2hexStr(location.ecsElin, location.ecsLen, locationStr);
        osal_strcpy(strTmp2, locationStr);
    }

    return SYS_ERR_OK;
}

int32 sal_lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLocation), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_lldp_portMedLocation_get(port, pLocation));

    return SYS_ERR_OK;
}

