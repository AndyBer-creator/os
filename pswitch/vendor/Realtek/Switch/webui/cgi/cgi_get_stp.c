
#include <include/cgi.h>

static int32 cgi_stp_global_get(void);
static int32 cgi_stp_intf_get(void);
static int32 cgi_stp_intfEdit_get(void);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 cgi_stp_msti_get(void);
static int32 cgi_stp_mstiEdit_get(void);
static int32 cgi_stp_mstiIntf_get(void);
static int32 cgi_stp_mstiIntfEdit_get(void);
#endif
static int32 cgi_stp_stats_get(void);
static int32 cgi_stp_statsIntf_get(void);

const cgi_cmdmap_t cmdGetStpTbl[] =
{
    { CGI_CMD_STP_GLOBAL,           cgi_stp_global_get          },
    { CGI_CMD_STP_INTF,             cgi_stp_intf_get            },
    { CGI_CMD_STP_INTF_EDIT,        cgi_stp_intfEdit_get        },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    { CGI_CMD_STP_MSTI,             cgi_stp_msti_get            },
    { CGI_CMD_STP_MSTI_EDIT,        cgi_stp_mstiEdit_get        },
    { CGI_CMD_STP_MSTI_INTF,        cgi_stp_mstiIntf_get        },
    { CGI_CMD_STP_MSTI_INTF_EDIT,   cgi_stp_mstiIntfEdit_get    },
#endif
    { CGI_CMD_STP_STATS,            cgi_stp_stats_get           },
    { CGI_CMD_STP_STATS_INTF,       cgi_stp_statsIntf_get       },
    { NULL,                          NULL                       }
};

static const char *lang_stp_portRole[] =
{
    "txtDisabled",
    "txtMaster",
    "txtRoot",
    "txtDesignated",
    "txtAlternate",
    "txtBackup",
};

static const char *lang_stp_portState[] =
{
    "txtDisabled",
    "txtDiscarding",
    "txtLearning",
    "txtForwarding",
};

static int32 sys_util_bridgeId2Str(sys_stp_bridge_id_t bridgeId, char *pStr)
{
    
    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    osal_sprintf(pStr, "%5u-%02X:%02X:%02X:%02X:%02X:%02X",
                 bridgeId.priority,
                 bridgeId.addr[0], bridgeId.addr[1], bridgeId.addr[2],
                 bridgeId.addr[3], bridgeId.addr[4], bridgeId.addr[5]);

    return SYS_ERR_OK;
}

static int32 sys_util_portId2Str(sys_stp_port_id_t portId, char *pStr)
{
    
    SYS_PARAM_CHK((NULL == pStr), SYS_ERR_NULL_POINTER);

    osal_sprintf(pStr, "%3u-%u",portId.priority, portId.number);

    return SYS_ERR_OK;
} 

static int32 cgi_stp_global_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;

    sys_enable_t            enable;
    uint32                  forceVersion;
    uint32                  method;
    uint32                  val;
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    char                    name[MSTP_CONF_NAME_LEN + 1];
#endif

    sys_stp_mst_info_t      mstInfo;
    char                    bridgeId[SYS_BUF32_LEN];
    char                    rootId[SYS_BUF32_LEN];
    uint32 tcdays;
    uint32 tchrs;
    uint32 tcmins;
    uint32 tcsecs;

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_stp_enable_get(&enable));
    cJSON_AddBoolToObject(data, CGI_STP_ENABLE, enable);

    SYS_ERR_CHK(sal_stp_forceVersion_get(&forceVersion));

    switch (forceVersion)
    {
        case STP_VERSION_STP:
            cJSON_AddStringToObject(data, CGI_STP_VERSION, "stp");
            break;
        case STP_VERSION_RSTP:
            cJSON_AddStringToObject(data, CGI_STP_VERSION, "rstp");
            break;
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        case STP_VERSION_MSTP:
            cJSON_AddStringToObject(data, CGI_STP_VERSION, "mstp");
            break;
#endif
        default:
            break;
    }

    SYS_ERR_CHK(sal_stp_pathcostMethod_get(&method));

    switch (method)
    {
        case STP_PATHCOST_METHOD_LONG:
            cJSON_AddStringToObject(data, CGI_STP_PATHCOST_METHOD, "long");
            break;
        case STP_PATHCOST_METHOD_SHORT:
            cJSON_AddStringToObject(data, CGI_STP_PATHCOST_METHOD, "short");
            break;
        default:
            break;
    }

    SYS_ERR_CHK(sal_stp_forwardBpdu_get(&enable));

    cJSON_AddStringToObject(data, CGI_STP_BPDU_FORWARD, ENABLED == enable ? "flooding" : "filtering");

    SYS_ERR_CHK(sal_stp_priority_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY, val);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_MAX, BOND_STP_PRI_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_DFLT, DFLT_STP_PRI);

    SYS_ERR_CHK(sal_stp_helloTime_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_HELLO_TIME, val);
    cJSON_AddNumberToObject(data, CGI_STP_HELLO_TIME_MIN, BOND_STP_HELLO_TIME_MIN);
    cJSON_AddNumberToObject(data, CGI_STP_HELLO_TIME_MAX, BOND_STP_HELLO_TIME_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_HELLO_TIME_DFLT, DFLT_STP_HELLO_TIME);

    SYS_ERR_CHK(sal_stp_maximumAge_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_MAX_AGE, val);
    cJSON_AddNumberToObject(data, CGI_STP_MAX_AGE_MIN, BOND_STP_MAX_AGE_MIN);
    cJSON_AddNumberToObject(data, CGI_STP_MAX_AGE_MAX, BOND_STP_MAX_AGE_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_MAX_AGE_DFLT, DFLT_STP_MAX_AGE);

    SYS_ERR_CHK(sal_stp_forwardDelay_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_FORWARD_DELAY, val);
    cJSON_AddNumberToObject(data, CGI_STP_FORWARD_DELAY_MIN, BOND_STP_FORWARD_DELAY_MIN);
    cJSON_AddNumberToObject(data, CGI_STP_FORWARD_DELAY_MAX, BOND_STP_FORWARD_DELAY_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_FORWARD_DELAY_DFLT, DFLT_STP_FORWARD_DELAY);

    SYS_ERR_CHK(sal_stp_txHoldCount_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_TX_HOLD_COUNT, val);
    cJSON_AddNumberToObject(data, CGI_STP_TX_HOLD_COUNT_MIN, BOND_STP_TX_HOLD_COUNT_MIN);
    cJSON_AddNumberToObject(data, CGI_STP_TX_HOLD_COUNT_MAX, BOND_STP_TX_HOLD_COUNT_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_TX_HOLD_COUNT_DFLT, DFLT_STP_TX_HOLD_COUNT);

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    SYS_MEM_CLEAR(name);
    SYS_ERR_CHK(sal_stp_mstConfigName_get(name));

    cJSON_AddStringToObject(data, CGI_STP_MSTP_REGION_NAME, name);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REGION_NAME_MAXLEN, MSTP_CONF_NAME_LEN);

    SYS_ERR_CHK(sal_stp_mstConfigRevision_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REVISION, val);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REVISION_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REVISION_MAX, BOND_STP_MST_CFG_REV_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REVISION_DFLT, DFLT_STP_MST_CFG_REV);

    SYS_ERR_CHK(sal_stp_maxHops_get(&val));

    cJSON_AddNumberToObject(data, CGI_STP_MSTP_MAXHOP, val);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_MAXHOP_MIN, BOND_STP_MAX_HOPS_MIN);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_MAXHOP_MAX, BOND_STP_MAX_HOPS_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_MAXHOP_DFLT, DFLT_STP_MAX_HOPS);
#endif

    SYS_MEM_CLEAR(bridgeId);
    SYS_MEM_CLEAR(rootId);

    SYS_ERR_CHK(sal_stp_mstInfo_get(0, &mstInfo));

    sys_util_bridgeId2Str(mstInfo.bridgeId, bridgeId);
    sys_util_bridgeId2Str(mstInfo.rootPriority.rootBridgeId, rootId);

    tcdays = (int) mstInfo.tcTimer / (60 * 60 * 24);
    tcmins = (int) mstInfo.tcTimer / 60;
    tcsecs = mstInfo.tcTimer - tcmins * 60;
    tchrs = (tcmins / 60) % 24;
    tcmins %= 60;

    cJSON_AddStringToObject(data, CGI_STP_BRIDGE_ID, bridgeId);
    cJSON_AddStringToObject(data, CGI_STP_ROOT_BRIDGE_ID,rootId);

    if (mstInfo.rootPortId.number != 0)
        cJSON_AddStringToObject(data, CGI_STP_ROOT_PORT, LPSTR(mstInfo.rootPortId.number - 1));

    cJSON_AddNumberToObject(data, CGI_STP_ROOT_COST, mstInfo.rootPriority.externalRootPathCost);

    cJSON_AddNumberToObject(data, CGI_STP_ROOT_TC_CNT, mstInfo.tcTimes);
    cJSON_AddLangStringArgToObject(data, CGI_STP_ROOT_LAST_TC, "stp","txtDayHrMinSecArg",
                                   "%d,%d,%d,%d", tcdays, tchrs, tcmins, tcsecs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_intf_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    sys_enable_t            enable;
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    sys_enable_t            bpduGuard;
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    sys_enable_t            bpduFilter;
#endif
    char                    bridgeId[SYS_BUF32_LEN];
    char                    desgPort[SYS_BUF16_LEN];

    sys_stp_port_info_t     stpPortInfo;
    sys_stp_mst_port_info_t mstPortInfo;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        enable = DISABLED;
#ifdef CONFIG_SYS_STP_BPDU_GUARD
        bpduGuard = DISABLED;
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
        bpduFilter = DISABLED;
#endif

        SYS_MEM_CLEAR(stpPortInfo);
        SYS_MEM_CLEAR(mstPortInfo);

        port = cJSON_CreateObject();

        SYS_ERR_CHK(sal_stp_portAdminEnable_get(lp, &enable));
        SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));
        SYS_ERR_CHK(sal_stp_mstPortInfo_get(0, lp, &mstPortInfo));
#ifdef CONFIG_SYS_STP_BPDU_GUARD
        SYS_ERR_CHK(sal_stp_portBpduGuard_get(lp, &bpduGuard));
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
        SYS_ERR_CHK(sal_stp_portBpduFilter_get(lp, &bpduFilter));
#endif

        cJSON_AddBoolToObject(port, CGI_STP_PORT_ENABLE, enable);

        cJSON_AddNumberToObject(port, CGI_STP_PORT_PRIORITY, mstPortInfo.portId.priority);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_PATHCOST, stpPortInfo.operExtPathCost);

#ifdef CONFIG_SYS_STP_BPDU_GUARD
        cJSON_AddBoolToObject(port, CGI_STP_PORT_BPDU_GUARD, bpduGuard);
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
        cJSON_AddBoolToObject(port, CGI_STP_PORT_BPDU_FILTER, bpduFilter);
#endif
        cJSON_AddBoolToObject(port, CGI_STP_PORT_EDGE_OPER,
                              (ENABLED == stpPortInfo.operEdge) ? ENABLED : DISABLED);
        cJSON_AddBoolToObject(port, CGI_STP_PORT_P2P_OPER,
                              stpPortInfo.operPointToPointMac % 2 ? ENABLED : DISABLED);

        cJSON_AddLangStringToObject(port, CGI_STP_PORT_ROLE,
                                    "stp", lang_stp_portRole[mstPortInfo.portRole]);
        cJSON_AddLangStringToObject(port, CGI_STP_PORT_STATE,
                                    "stp", lang_stp_portState[mstPortInfo.portState]);

        SYS_MEM_CLEAR(bridgeId);
        SYS_MEM_CLEAR(desgPort);

        sys_util_portId2Str(mstPortInfo.portId,desgPort);
        sys_util_bridgeId2Str(mstPortInfo.portPriority.designatedBridgeId,bridgeId);

        cJSON_AddStringToObject(port, CGI_STP_PORT_DESG_BRIDGE, bridgeId);
        cJSON_AddStringToObject(port, CGI_STP_PORT_DESG_PORT_ID, desgPort);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_DESG_COST, stpPortInfo.operExtPathCost);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_intfEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    sys_logic_port_t        lp = 0;
    uint32                  pri = 0;
    uint32                  method;
    sys_enable_t            enable = DISABLED;
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    sys_enable_t            bpduGuard = DISABLED;
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    sys_enable_t            bpduFilter = DISABLED;
#endif
    char                    bridgeId[SYS_BUF32_LEN];
    char                    desgPort[SYS_BUF16_LEN];
    char                    *pStrValue;

    sys_stp_port_info_t     stpPortInfo;
    sys_stp_mst_port_info_t mstPortInfo;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, pStrValue))
        lp = SYS_STR2UINT(pStrValue);

    CGI_GET_JSON_CREATE(root, data);

    SYS_MEM_CLEAR(stpPortInfo);
    SYS_MEM_CLEAR(bridgeId);
    SYS_MEM_CLEAR(desgPort);

    SYS_ERR_CHK(sal_stp_pathcostMethod_get(&method));
    SYS_ERR_CHK(sal_stp_portAdminEnable_get(lp, &enable));
    SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));
    SYS_ERR_CHK(sal_stp_mstPortInfo_get(0, lp, &mstPortInfo));
    SYS_ERR_CHK(sal_stp_portPriority_get(lp, &pri));
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    SYS_ERR_CHK(sal_stp_portBpduGuard_get(lp, &bpduGuard));
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    SYS_ERR_CHK(sal_stp_portBpduFilter_get(lp, &bpduFilter));
#endif

    cJSON_AddBoolToObject(data, CGI_STP_PORT_ENABLE, enable);
    
	switch (stpPortInfo.confEdge)
    {
        case STP_PORT_CONF_FALSE:
            cJSON_AddStringToObject(data, CGI_STP_PORT_EDGE, "disable");
            break;
        case STP_PORT_CONF_TRUE:
            cJSON_AddStringToObject(data, CGI_STP_PORT_EDGE, "enable");
            break;
        case STP_PORT_CONF_AUTO:
            cJSON_AddStringToObject(data, CGI_STP_PORT_EDGE, "auto");
            break;
    }
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST, stpPortInfo.confExtPathCost);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST_MAX,
                            (STP_PATHCOST_METHOD_LONG == method) ? BOND_STP_PATHCOST_LONG_MAX : BOND_STP_PATHCOST_SHORT_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY, pri);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY_MAX, BOND_STP_PORT_PRI_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY_DFLT, DFLT_STP_PORT_PRI);

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    cJSON_AddBoolToObject(data, CGI_STP_PORT_BPDU_GUARD, bpduGuard);
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    cJSON_AddBoolToObject(data, CGI_STP_PORT_BPDU_FILTER, bpduFilter);
#endif

    switch (stpPortInfo.confPointToPointMac)
    {
        case STP_PORT_CONF_FALSE:
            cJSON_AddStringToObject(data, CGI_STP_PORT_P2P, "disable");
            break;
        case STP_PORT_CONF_TRUE:
            cJSON_AddStringToObject(data, CGI_STP_PORT_P2P, "enable");
            break;
        case STP_PORT_CONF_AUTO:
            cJSON_AddStringToObject(data, CGI_STP_PORT_P2P, "auto");
            break;
    }

    cJSON_AddLangStringToObject(data, CGI_STP_PORT_STATE,
                                "stp", lang_stp_portState[mstPortInfo.portState]);

    sys_util_portId2Str(mstPortInfo.portId,desgPort);
    sys_util_bridgeId2Str(mstPortInfo.portPriority.designatedBridgeId,bridgeId);

    cJSON_AddStringToObject(data, CGI_STP_PORT_DESG_BRIDGE, bridgeId);
    cJSON_AddStringToObject(data, CGI_STP_PORT_DESG_PORT_ID, desgPort);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_DESG_COST, stpPortInfo.operExtPathCost);

    cJSON_AddBoolToObject(data, CGI_STP_PORT_EDGE_OPER,
                          (ENABLED == stpPortInfo.operEdge) ? ENABLED : DISABLED);
    cJSON_AddBoolToObject(data, CGI_STP_PORT_P2P_OPER,
                          stpPortInfo.operPointToPointMac % 2 ? ENABLED : DISABLED);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP
static int32 cgi_stp_msti_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *instances = NULL;
    cJSON               *instance = NULL;
    char                bridgeId[SYS_BUF32_LEN];
    char                rootId[SYS_BUF32_LEN];

    uint32              i = 0;
    uint32              pri = 0;
    uint32              pathcost = 0;
    char                strVlanList[CAPA_VLANLIST_STR_LEN];

    sys_stp_mst_info_t  mstInfo;

    CGI_GET_JSON_CREATE(root, data);

    instances = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_STP_MST_INSTANCES, instances);

    for (i = 0; i < MSTP_INSTANCE_NUM; i++)
    {
        pri = 0;
        pathcost = 0;

        SYS_MEM_CLEAR(mstInfo);
        SYS_MEM_CLEAR(bridgeId);
        SYS_MEM_CLEAR(rootId);

        instance = cJSON_CreateObject();

        SYS_ERR_CHK(sal_stp_mstInfo_get(i, &mstInfo));

        VMSK2STR(&mstInfo.vlanmask, strVlanList);

        cJSON_AddStringToObject(instance, CGI_STP_MST_VLANS, strVlanList);

        SYS_ERR_CHK(sal_stp_mstPriority_get(i, &pri));

        cJSON_AddNumberToObject(instance, CGI_STP_PRIORITY, pri);

#if 0
        if (MSTP_INSTANCE_CIST != i && IS_VLANMASK_CLEAR(mstInfo.vlanmask))
        {
            cJSON_AddItemToArray(instances, instance);
            continue;
        }
#endif

        sys_util_bridgeId2Str(mstInfo.bridgeId, bridgeId);

        cJSON_AddStringToObject(instance, CGI_STP_BRIDGE_ID, bridgeId);

        if (MSTP_INSTANCE_CIST == i)
        {
            pathcost = mstInfo.rootPriority.externalRootPathCost;
            sys_util_bridgeId2Str(mstInfo.rootPriority.rootBridgeId, rootId);
        }
        else
        {
            pathcost = mstInfo.rootPriority.internalRootPathCost;
            sys_util_bridgeId2Str(mstInfo.rootPriority.regionalRootBridgeId, rootId);
        }

        sys_util_bridgeId2Str(mstInfo.bridgeId, bridgeId);

        cJSON_AddStringToObject(instance, CGI_STP_BRIDGE_ID, bridgeId);
        cJSON_AddStringToObject(instance, CGI_STP_ROOT_BRIDGE_ID,rootId);

        if (mstInfo.rootPortId.number != 0)
            cJSON_AddStringToObject(data, CGI_STP_ROOT_PORT, LPSTR(mstInfo.rootPortId.number - 1));

        cJSON_AddNumberToObject(instance, CGI_STP_ROOT_COST, pathcost);
        cJSON_AddNumberToObject(instance, CGI_STP_MSTP_REMAINING_HOP, mstInfo.remainingHops);

        cJSON_AddItemToArray(instances, instance);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_mstiEdit_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;
    cJSON               *availVlans = NULL;
    cJSON               *selVlans = NULL;

    uint32              pri = 0;
    uint32              msti = 0;
    uint32              pathcost = 0;
    char                bridgeId[SYS_BUF32_LEN];
    char                rootId[SYS_BUF32_LEN];
    char                *pStrValue;

    sys_vid_t           vid;
    sys_vlanmask_t      vlanmask;
    sys_stp_mst_info_t  mstInfo;

    SYS_MEM_CLEAR(bridgeId);
    SYS_MEM_CLEAR(rootId);

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTI, pStrValue))
        msti = SYS_STR2UINT(pStrValue);

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_stp_mstVlanmask_get(msti, &vlanmask));

    SYS_ERR_CHK(sal_stp_mstPriority_get(msti, &pri));
    SYS_ERR_CHK(sal_stp_mstInfo_get(msti, &mstInfo));

    FOR_EACH_VID(vid)
    {
        if (VLANMASK_IS_VIDSET(vlanmask, vid))
        {
            if (NULL == selVlans)
                selVlans = cJSON_CreateArray();

            cJSON_AddItemToArray(selVlans, cJSON_CreateNumber(vid));
        }
        else
        {
            if (NULL == availVlans)
                availVlans = cJSON_CreateArray();

            cJSON_AddItemToArray(availVlans, cJSON_CreateNumber(vid));
        }
    }

    if (NULL == selVlans)
        selVlans = cJSON_CreateNull();

    if (NULL == availVlans)
        availVlans = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_STP_MST_VLAN_LIST, availVlans);
    cJSON_AddItemToObject(data, CGI_STP_MST_VLAN_SEL, selVlans);

    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY, pri);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_MAX, BOND_STP_PRI_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_PRIORITY_DFLT, DFLT_STP_MST_PRI);

    sys_util_bridgeId2Str(mstInfo.bridgeId, bridgeId);

    cJSON_AddStringToObject(data, CGI_STP_BRIDGE_ID, bridgeId);

    if (MSTP_INSTANCE_CIST == msti)
    {
        pathcost = mstInfo.rootPriority.externalRootPathCost;
        sys_util_bridgeId2Str(mstInfo.rootPriority.rootBridgeId, rootId);
    }
    else
    {
        pathcost = mstInfo.rootPriority.internalRootPathCost;
        sys_util_bridgeId2Str(mstInfo.rootPriority.regionalRootBridgeId, rootId);
    }

    sys_util_bridgeId2Str(mstInfo.bridgeId, bridgeId);

    cJSON_AddStringToObject(data, CGI_STP_BRIDGE_ID, bridgeId);
    cJSON_AddStringToObject(data, CGI_STP_ROOT_BRIDGE_ID,rootId);

    if (mstInfo.rootPortId.number != 0)
        cJSON_AddStringToObject(data, CGI_STP_ROOT_PORT, LPSTR(mstInfo.rootPortId.number - 1));

    cJSON_AddNumberToObject(data, CGI_STP_ROOT_COST, pathcost);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REMAINING_HOP, mstInfo.remainingHops);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_mstiIntf_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;

    uint32                  msti = 0;
    uint32                  priority;
    char                    bridgeId[SYS_BUF32_LEN];
    char                    desgPort[SYS_BUF16_LEN];
    char                    *pStrValue;

    sys_logic_port_t        lp;
    sys_stp_port_info_t     stpPortInfo;
    sys_stp_mst_port_info_t mstPortInfo;

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTI, pStrValue))
        msti = SYS_STR2UINT(pStrValue);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_STP_MSTI_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_MSTI_MAX, BOND_STP_MST_INSTANCE_MAX);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        priority = 0;

        SYS_MEM_CLEAR(stpPortInfo);
        SYS_MEM_CLEAR(mstPortInfo);

        port = cJSON_CreateObject();

        SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));
        SYS_ERR_CHK(sal_stp_mstPortInfo_get(msti, lp, &mstPortInfo));
        SYS_ERR_CHK(sal_stp_mstPortPriority_get(msti, lp, &priority));

        cJSON_AddNumberToObject(port, CGI_STP_PORT_PATHCOST, mstPortInfo.operIntPathCost);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_PRIORITY ,priority);
        cJSON_AddLangStringToObject(port, CGI_STP_PORT_ROLE,
                                    "stp", lang_stp_portRole[mstPortInfo.portRole]);
        cJSON_AddLangStringToObject(port, CGI_STP_PORT_STATE, "stp", lang_stp_portState[mstPortInfo.portState]);

        switch (stpPortInfo.mode)
        {
            case STP_PORT_MODE_STP:
                cJSON_AddLangStringToObject(port, CGI_STP_PORT_MODE, "stp", "txtSTP");
                break;

            case STP_PORT_MODE_RSTP:
                cJSON_AddLangStringToObject(port, CGI_STP_PORT_MODE, "stp", "txtRSTP");
                break;

            default:
                cJSON_AddLangStringToObject(port, CGI_STP_PORT_MODE, "stp", "txtMSTP");
                break;

        }

        if(STP_PORT_TYPE_BOUNDARY == stpPortInfo.type)
            cJSON_AddLangStringToObject(port, CGI_STP_PORT_TYPE, "stp", "txtBoundary");
        else
            cJSON_AddLangStringToObject(port, CGI_STP_PORT_TYPE, "stp", "txtInternal");

        SYS_MEM_CLEAR(bridgeId);
        SYS_MEM_CLEAR(desgPort);

        sys_util_bridgeId2Str(mstPortInfo.portPriority.designatedBridgeId, bridgeId);
        sys_util_portId2Str(mstPortInfo.portId, desgPort);

        cJSON_AddStringToObject(port, CGI_STP_PORT_DESG_BRIDGE, bridgeId);
        cJSON_AddStringToObject(port, CGI_STP_PORT_DESG_PORT_ID, desgPort);

        cJSON_AddNumberToObject(port, CGI_STP_PORT_DESG_COST, mstPortInfo.portPriority.externalRootPathCost);
        cJSON_AddNumberToObject(port, CGI_STP_MSTP_REMAINING_HOP, mstPortInfo.portTimes.remainingHops);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_mstiIntfEdit_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;

    uint32                  msti = 0;
    uint32                  priority = 0;
    uint32                  cost = 0;
    uint32                  method;
    char                    bridgeId[SYS_BUF32_LEN];
    char                    desgPort[SYS_BUF16_LEN];
    char                    *pStrValue;

    sys_logic_port_t        lp = 0;
    sys_stp_port_info_t     stpPortInfo;
    sys_stp_mst_port_info_t mstPortInfo;

    if (CGI_IS_PARAM_EXIST(CGI_STP_MSTI, pStrValue))
        msti = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, pStrValue))
        lp = SYS_STR2UINT(pStrValue);

    SYS_MEM_CLEAR(stpPortInfo);
    SYS_MEM_CLEAR(mstPortInfo);

    CGI_GET_JSON_CREATE(root, data);

    SYS_ERR_CHK(sal_stp_pathcostMethod_get(&method));
    SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));
    SYS_ERR_CHK(sal_stp_mstPortInfo_get(msti, lp, &mstPortInfo));
    SYS_ERR_CHK(sal_stp_mstPortPathCost_get(msti, lp, &cost));
    SYS_ERR_CHK(sal_stp_mstPortPriority_get(msti, lp, &priority));

    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST, cost);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PATHCOST_MAX,
                            STP_PATHCOST_METHOD_LONG == method ? BOND_STP_PATHCOST_LONG_MAX : BOND_STP_PATHCOST_SHORT_MAX);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY, priority);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_PRIORITY_MAX, BOND_STP_PORT_PRI_MAX);
    cJSON_AddLangStringToObject(data, CGI_STP_PORT_ROLE,
                                "stp", lang_stp_portRole[mstPortInfo.portRole]);
    cJSON_AddLangStringToObject(data, CGI_STP_PORT_STATE, "stp", lang_stp_portState[mstPortInfo.portState]);

    switch (stpPortInfo.mode)
    {
        case STP_PORT_MODE_STP:
            cJSON_AddLangStringToObject(data, CGI_STP_PORT_MODE, "stp", "txtSTP");
            break;

        case STP_PORT_MODE_RSTP:
            cJSON_AddLangStringToObject(data, CGI_STP_PORT_MODE, "stp", "txtRSTP");
            break;

        default:
            cJSON_AddLangStringToObject(data, CGI_STP_PORT_MODE, "stp", "txtMSTP");
            break;

    }

    if(STP_PORT_TYPE_BOUNDARY == stpPortInfo.type)
        cJSON_AddLangStringToObject(data, CGI_STP_PORT_TYPE, "stp", "txtBoundary");
    else
        cJSON_AddLangStringToObject(data, CGI_STP_PORT_TYPE, "stp", "txtInternal");

    SYS_MEM_CLEAR(bridgeId);
    SYS_MEM_CLEAR(desgPort);

    sys_util_bridgeId2Str(mstPortInfo.portPriority.designatedBridgeId, bridgeId);
    sys_util_portId2Str(mstPortInfo.portId, desgPort);

    cJSON_AddStringToObject(data, CGI_STP_PORT_DESG_BRIDGE, bridgeId);
    cJSON_AddStringToObject(data, CGI_STP_PORT_DESG_PORT_ID, desgPort);

    cJSON_AddNumberToObject(data, CGI_STP_PORT_DESG_COST, mstPortInfo.operIntPathCost);
    cJSON_AddNumberToObject(data, CGI_STP_MSTP_REMAINING_HOP, mstPortInfo.portTimes.remainingHops);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_stp_stats_get(void)
{
    cJSON                   *root = NULL;
    cJSON                   *data = NULL;
    cJSON                   *ports = NULL;
    cJSON                   *port = NULL;
    sys_logic_port_t        lp;
    sys_stp_port_info_t     stpPortInfo;

    CGI_GET_JSON_CREATE(root, data);

    ports = cJSON_CreateArray();

    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(stpPortInfo);

        port = cJSON_CreateObject();

        SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));

        cJSON_AddNumberToObject(port, CGI_STP_PORT_CONFIG_BPDU_RX, stpPortInfo.pktCount.rxConfig);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_TCN_BPDU_RX, stpPortInfo.pktCount.rxTcn);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_CONFIG_BPDU_TX, stpPortInfo.pktCount.txConfig);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_TCN_BPDU_TX, stpPortInfo.pktCount.txTcn);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
        cJSON_AddNumberToObject(port, CGI_STP_PORT_MSTP_BPDU_RX, stpPortInfo.pktCount.rxMstp);
        cJSON_AddNumberToObject(port, CGI_STP_PORT_MSTP_BPDU_TX, stpPortInfo.pktCount.txMstp);
#endif

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_stp_statsIntf_get(void)
{
    cJSON               *root = NULL;
    cJSON               *data = NULL;

    char                *pStrValue = NULL;

    sys_logic_port_t    lp = 0;
    sys_stp_port_info_t stpPortInfo;

    SYS_MEM_CLEAR(stpPortInfo);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
        lp = SYS_STR2UINT(pStrValue);

    SYS_ERR_CHK(sal_stp_portInfo_get(lp, &stpPortInfo));

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_STP_PORT_CONFIG_BPDU_RX, stpPortInfo.pktCount.rxConfig);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_TCN_BPDU_RX, stpPortInfo.pktCount.rxTcn);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_CONFIG_BPDU_TX, stpPortInfo.pktCount.txConfig);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_TCN_BPDU_TX, stpPortInfo.pktCount.txTcn);
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    cJSON_AddNumberToObject(data, CGI_STP_PORT_MSTP_BPDU_RX, stpPortInfo.pktCount.rxMstp);
    cJSON_AddNumberToObject(data, CGI_STP_PORT_MSTP_BPDU_TX, stpPortInfo.pktCount.txMstp);
#endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
