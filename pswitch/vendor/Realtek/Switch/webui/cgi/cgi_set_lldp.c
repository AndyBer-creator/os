
#include <include/cgi.h>

static int32 cgi_lldp_prop_set(void);
static int32 cgi_lldp_portEdit_set(void);
static int32 cgi_lldp_neighborClear_set(void);
static int32 cgi_lldp_globalStatisticsClear_set(void);
static int32 cgi_lldp_portStatisticsClear_set(void);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 cgi_lldp_medPolicy_set(void);
static int32 cgi_lldp_medPolicyAddEdit_set(void);
static int32 cgi_lldp_medPolicyDel_set(void);
static int32 cgi_lldp_medPortEdit_set(void);
#endif

const cgi_cmdmap_t cmdSetLldpTbl[] =
{
    {CGI_CMD_LLDP_PROP,    cgi_lldp_prop_set},
    {CGI_CMD_LLDP_PORTEDIT,    cgi_lldp_portEdit_set},
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {CGI_CMD_LLDP_MEDPOLICY,    cgi_lldp_medPolicy_set},
    {CGI_CMD_LLDP_MEDPOLICYADD,    cgi_lldp_medPolicyAddEdit_set},
    {CGI_CMD_LLDP_MEDPOLICYEDIT,    cgi_lldp_medPolicyAddEdit_set},
    {CGI_CMD_LLDP_MEDPOLICYDEL,    cgi_lldp_medPolicyDel_set},
    {CGI_CMD_LLDP_MEDPORTEDIT,    cgi_lldp_medPortEdit_set},
  #endif
    {CGI_CMD_LLDP_NEIGHBORCLEAR,    cgi_lldp_neighborClear_set},
    {CGI_CMD_LLDP_GLOBALSTATISTICSCLEAR,    cgi_lldp_globalStatisticsClear_set},
    {CGI_CMD_LLDP_PORTSTATISTICSCLEAR,    cgi_lldp_portStatisticsClear_set},

    {NULL, NULL}
};

static int32 cgi_lldp_prop_set(void)
{
    uint8 holdtime = DFLT_LLDP_HOLDTIME_MUL;
    uint32 tx_interval = DFLT_LLDP_TX_INTERVAL;
    uint16 txDelay = DFLT_LLDP_TX_DELAY;
    uint8 reinit_delay = DFLT_LLDP_REINIT_DELAY;
    int32 ret = SYS_ERR_OK;
    char *val;
    uint8 isTimerConfig = FALSE;

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_FRMHANDLE, val))
    {
        lldp_disable_action_t    disAction = LLDP_DISABLE_ACTION_FILTER;

        if (0 == osal_strcmp(val, "bridge"))
            disAction = LLDP_DISABLE_ACTION_FLOOD_TO_VLAN;
        else if (0 == osal_strcmp(val, "flood"))
            disAction = LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS;
        else if (0 == osal_strcmp(val, "filter"))
            disAction = LLDP_DISABLE_ACTION_FILTER;

        CGI_SET_ERR_HDL(sal_lldp_disableAction_set(disAction), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_STATUS, val))
        CGI_SET_ERR_HDL(sal_lldp_enable_set(ENABLED), ret);
    else
        CGI_SET_ERR_HDL(sal_lldp_enable_set(DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_TLVADVINTV, val))
    {
        tx_interval = SYS_STR2UINT(val);
        isTimerConfig= TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_HOLDMULTI, val))
    {
        holdtime = SYS_STR2UINT(val);
        isTimerConfig= TRUE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_REINITDELAY, val))
    {
        reinit_delay = SYS_STR2UINT(val);
        CGI_SET_ERR_HDL(sal_lldp_reinit_delay_set(reinit_delay), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_TRANSDELAY, val))
    {
        txDelay = SYS_STR2UINT(val);
        isTimerConfig= TRUE;
    }

    if(TRUE == isTimerConfig)
    {
        if(tx_interval < txDelay * 4)
        {
            ret = SYS_ERR_LLDP_TX_DELAY_TX_INTERVAL;
            goto cgi_end;
        }

        if(tx_interval * holdtime > 65535)
        {
            ret = SYS_ERR_LLDP_HOLDTIME_MUL_TX_INTERVAL;
            goto cgi_end;
        }
        CGI_SET_ERR_HDL(sal_lldp_tx_delay_set(BOND_LLDP_TX_DELAY_MIN), ret);
        CGI_SET_ERR_HDL(sal_lldp_holdtime_set(BOND_LLDP_HOLDTIME_MIN), ret);
        CGI_SET_ERR_HDL(sal_lldp_tx_interval_set(BOND_LLDP_TX_INTERVAL_MIN), ret);
        CGI_SET_ERR_HDL(sal_lldp_tx_interval_set(tx_interval), ret);
        CGI_SET_ERR_HDL(sal_lldp_tx_delay_set(txDelay), ret);
        CGI_SET_ERR_HDL(sal_lldp_holdtime_set(holdtime), ret);
    }

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    if (CGI_IS_PARAM_EXIST(CGI_LLDP_STARTCNT, val))
    {
        uint8   fastRepeatCount;
        fastRepeatCount = SYS_STR2UINT(val);
        CGI_SET_ERR_HDL(sal_lldp_medFastRepeatCount_set(fastRepeatCount), ret);
    }
#endif

cgi_end:

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_portEdit_set(void)
{
    int32 ret = SYS_ERR_OK, i, idx;
    uint32 num = 0;
    uint8 lldpPortState = LLDP_PORT_TXRX;
    char *val;
    char **array = NULL;
    sys_logic_port_t lp;
    sys_logic_portmask_t        lpm;
    lldp_tlv_sel_t          lldpTlvSel;
    sys_enable_t            pvidTlvEbl = DISABLED;
    sys_vlanmask_t          vlanmask;
    lldp_overLoading_t      pre_overload, cur_overload;

    SYS_MEM_CLEAR(lpm);
    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, val))
        STR2LPM(val, &lpm);
    else
    {
        goto cgi_end;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_ADMINSTATUS, val))
    {
        if (0 == osal_strcmp(val, "txrx"))
            lldpPortState = LLDP_PORT_TXRX;
        else if (0 == osal_strcmp(val, "rx"))
            lldpPortState = LLDP_PORT_RXONLY;
        else if (0 == osal_strcmp(val, "tx"))
            lldpPortState = LLDP_PORT_TXONLY;
        else if (0 == osal_strcmp(val, "disable"))
            lldpPortState = LLDP_PORT_DISABLE;
    }

    osal_memset(&lldpTlvSel, 0, sizeof(lldpTlvSel));
    if(CGI_GET_PARAM_ARRAY_DATA(CGI_LLDP_SELOPTS, array, &num))
    {
        for(i = 0; i < num; i++)
        {
            idx = SYS_STR2UINT(array[i]);

            switch(idx)
            {
                case 0:
                    lldpTlvSel.portDesciptionSel = TRUE;
                    break;
                case 1:
                    lldpTlvSel.sysNameSel = TRUE;
                    break;
                case 2:
                    lldpTlvSel.sysDescriptionSel = TRUE;
                    break;
                case 3:
                    lldpTlvSel.sysCapabilitySel = TRUE;
                    break;
                case 4:
                    lldpTlvSel.macPhySel = TRUE;
                    break;
                case 5:
                    lldpTlvSel.linkAggregationSel = TRUE;
                    break;
                case 6:
                    lldpTlvSel.maxFrameSizeSel = TRUE;
                    break;
                case 7:
                    lldpTlvSel.managementAddrSel = TRUE;
                    break;
                case 8:
                    pvidTlvEbl = ENABLED;
                    break;
                default:
                    break;
            }
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, val))
        STR2VMSK(val, &vlanmask);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_ERR_CHK(sal_lldp_portTlvOverload_get(lp, &pre_overload));
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_port_set(lp, lldpPortState), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portTlvSel_set(lp, lldpTlvSel), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portTlvPvidEnable_set(lp, pvidTlvEbl), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portTlvVlanmask_set(lp, vlanmask), lp);
        SYS_ERR_CHK(sal_lldp_portTlvOverload_get(lp, &cur_overload));

        if (FALSE == pre_overload.isOverload && TRUE == cur_overload.isOverload)
            SYS_LOG(LOG_SEV_NOTICE, LOG_CAT_LLDP, "LLDP TLVsTxOverloading: Enter Overloading state on port %s\n", LPSTR(lp));
        else if (TRUE == pre_overload.isOverload && FALSE == cur_overload.isOverload)
            SYS_LOG(LOG_SEV_NOTICE, LOG_CAT_LLDP, "LLDP TLVsTxOverloading: Exit Overloading state on port %s\n", LPSTR(lp));
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 cgi_lldp_medPolicy_set(void)
{
    int32                   ret = SYS_ERR_OK;
    char                    *val;

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_POLICY4VOICE, val))
    {
        CGI_SET_ERR_HDL(sal_lldp_medNetworkPolicyAuto_set(ENABLED), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_lldp_medNetworkPolicyAuto_set(DISABLED), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPolicyAddEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *val;
    uint32 idx = 0;
    lldp_med_networkPolicy_t policy;

    osal_memset(&policy, 0, sizeof(policy));
    policy.valid = TRUE;

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_POLICYNUM, val))
    {
        idx = SYS_STR2UINT(val) - 1;
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_APP, val))
    {
        policy.app = SYS_STR2UINT(val);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_VID, val))
    {
        policy.vid = SYS_STR2UINT(val);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_VLANTAG, val))
    {
        if (0 == osal_strcmp(val, "tag"))
            policy.vlanTagged = TRUE;
        else
            policy.vlanTagged = FALSE;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_PRI, val))
    {
        policy.userPriority = SYS_STR2UINT(val);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_DSCP, val))
    {
        policy.dscpValue = SYS_STR2UINT(val);
    }

    CGI_SET_ERR_HDL(sal_lldp_medNetworkPolicy_set(idx, policy), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPolicyDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 idx = 0;
    char **array = NULL;
    uint32 len = 0;
    char  entryDescp[20] = "Police ID ";
    lldp_med_networkPolicy_t policy;

    osal_memset(&policy, 0, sizeof(policy));

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_LLDP_POLICY, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            idx = SYS_STR2UINT(array[i]) - 1;
            strlcat(entryDescp, array[i], sizeof(entryDescp));
            CGI_SET_ENTRY_ERR_CONTINUE(sal_lldp_medNetworkPolicy_set(idx, policy), entryDescp);
        }
    }

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
    }

    return SYS_ERR_OK;
}

static int32 cgi_lldp_medPortEdit_set(void)
{
    int32 ret = SYS_ERR_OK, i, idx;
    uint32 num = 0;
    char *val;
    char **array = NULL;
    sys_logic_port_t lp;
    sys_logic_portmask_t        lpm;
    sys_enable_t enable = ENABLED;
    lldp_med_tlv_sel_t lldpMedTlvSel;
    lldp_med_networkPolicy_bind_t policyBind;
    lldp_med_location_t location;
    uint16 strLen;
    uint16 size;
    uint8 byteArray[256];

    SYS_MEM_CLEAR(lpm);
    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, val))
    {
        STR2LPM(val, &lpm);
    }
    else
    {
        goto cgi_end;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_MEDSTATUS, val))
        enable = ENABLED;
    else
        enable = DISABLED;

    osal_memset(&lldpMedTlvSel, 0, sizeof(lldpMedTlvSel));
    if(CGI_GET_PARAM_ARRAY_DATA(CGI_LLDP_SELOPTS, array, &num))
    {
        for(i = 0; i < num; i++)
        {
            idx = SYS_STR2UINT(array[i]);

            switch(idx)
            {
                case 0:
                    lldpMedTlvSel.networkPolicySel = TRUE;
                    break;
                case 1:
                    lldpMedTlvSel.locationSel = TRUE;
                    break;
                case 2:
                    lldpMedTlvSel.poePseSel = TRUE;
                    break;
                case 3:
                    lldpMedTlvSel.inventorySel = TRUE;
                    break;
                default:
                    break;
            }
        }
    }

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    osal_memset(&policyBind, 0, sizeof(policyBind));
    if(CGI_GET_PARAM_ARRAY_DATA(CGI_LLDP_SELPOLICIES, array, &num))
    {
        for(i = 0; i < num; i++)
        {
            idx = SYS_STR2UINT(array[i]);

            policyBind.bindMask |= 1 << (idx - 1);
        }
    }

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    osal_memset(&location, 0, sizeof(location));
    if (CGI_IS_PARAM_EXIST(CGI_LLDP_COORDINATE, val))
    {
        strLen = osal_strlen(val);

        size = strLen/2;
        if(1 == (strLen % 2))
            size += 1;

        osal_memset(byteArray, 0, sizeof(byteArray));
        ret = sys_util_hexStr2byteArray(val, byteArray);

        if(SYS_ERR_OK != ret)
        {
            ret = SYS_ERR_LLDP_LOCATION_FORMAT;
            goto cgi_end;
        }

        osal_memcpy(location.coordinate, byteArray, sizeof(location.coordinate));
        location.coorLen = size;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_CIVIC, val))
    {
        strLen = osal_strlen(val);

        size = strLen/2;
        if(1 == (strLen % 2))
            size += 1;

        osal_memset(byteArray, 0, sizeof(byteArray));
        ret = sys_util_hexStr2byteArray(val, byteArray);

        if(SYS_ERR_OK != ret)
        {
            ret = SYS_ERR_LLDP_LOCATION_FORMAT;
            goto cgi_end;
        }

        osal_memcpy(location.civicAddress, byteArray, sizeof(location.civicAddress));
        location.civicLen = size;
    }

    if (CGI_IS_PARAM_EXIST(CGI_LLDP_ECS, val))
    {
        strLen = osal_strlen(val);

        size = strLen/2;
        if(1 == (strLen % 2))
            size += 1;

        osal_memset(byteArray, 0, sizeof(byteArray));
        ret = sys_util_hexStr2byteArray(val, byteArray);

        if(SYS_ERR_OK != ret)
        {
            ret = SYS_ERR_LLDP_LOCATION_FORMAT;
            goto cgi_end;
        }

        osal_memcpy(location.ecsElin, byteArray, sizeof(location.ecsElin));
        location.ecsLen = size;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portMedEnable_set(lp, enable), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portMedTlvSel_set(lp, lldpMedTlvSel), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portMedNetworkPolicyBind_set(lp, policyBind), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portMedLocation_set(lp, location), lp);

        CGI_SET_PORT_ERR_CONTINUE(sal_lldp_txOverload_check(lp), lp);
    }

cgi_end:

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_lldp_neighborClear_set(void)
{
    sys_logic_port_t        lp;
    uint8                   index;
    int32                   ret = SYS_ERR_OK, i;
    uint32 num = 0;
    char **array = NULL;
    char                *p;
    char                *key = "_";

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_LLDP_SELENTRIES, array, &num))
    {
        for(i = 0; i < num; i++)
        {
            p = strtok(array[i], key);
            STR2LP(p, &lp);
            p = strtok(NULL, key);
            index = SYS_STR2UINT(p);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_lldp_portRemoteEntry_clear(lp, index), p);
        }
    }
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

static int32 cgi_lldp_globalStatisticsClear_set(void)
{
    int32 ret = SYS_ERR_OK;

    CGI_SET_ERR_HDL(sal_lldp_globalStatistics_clear(), ret);

cgi_end:

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_lldp_portStatisticsClear_set(void)
{
    int32 ret = SYS_ERR_OK;
    sys_user_port_t uPort;
    sys_logic_port_t lPort;
    uint32            num, i;
    char **array = NULL;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_PORT, array, &num))
    {
        for(i = 0; i < num; i++)
        {
            
            uPort = SYS_STR2UINT(array[i]) + 1;
            lPort = UP2LP(uPort);

            CGI_SET_PORT_ERR_CONTINUE(sal_lldp_portStatistics_clear(lPort), lPort);
        }
    }
    else
    {
        goto cgi_end;
    }

cgi_end:

    cgi_set_respGeneral_send(ret);

    if (NULL != array)
    {
        osal_free(array);
        array = NULL;
    }

    return SYS_ERR_OK;
}

