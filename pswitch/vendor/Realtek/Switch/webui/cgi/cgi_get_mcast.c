
#include <include/cgi.h>

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
const char *cgiMaxGrpAct[] =
{
    "lblDeny",
    "lblReplace",
};

const char *cgiMaxGrpActVal[] =
{
    "deny",
    "replace",
};
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
const char *cgiProfileAct[] =
{
    "lblDeny",
    "lblAllow",
};

const char *cgiProfileActVal[] =
{
    "deny",
    "allow",
};
#endif

#ifdef CONFIG_SYS_PROTO_MVR
const char *cgiMvrPortType[] =
{
    "lblNone",
    "lblReceiver",
    "lblSource"
};
#endif

static int32 cgi_mcast_gen_prop_get(void);

static int32 cgi_mcast_gen_group_get(void);
static int32 cgi_mcast_gen_groupAdd_get(void);
static int32 cgi_mcast_gen_groupEdit_get(void);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
static int32 cgi_mcast_gen_throt_get(void);
static int32 cgi_mcast_gen_throtEdit_get(void);
#endif

static int32 cgi_mcast_gen_router_get(void);
#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
static int32 cgi_mcast_gen_routerAdd_get(void);
static int32 cgi_mcast_gen_routerEdit_get(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
static int32 cgi_mcast_gen_fwdAll_get(void);
static int32 cgi_mcast_gen_fwdAllAdd_get(void);
static int32 cgi_mcast_gen_fwdAllEdit_get(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 cgi_mcast_gen_profile_get(void);
static int32 cgi_mcast_gen_profileAdd_get(void);
static int32 cgi_mcast_gen_profileEdit_get(void);

static int32 cgi_mcast_gen_filter_get(void);
static int32 cgi_mcast_gen_filterEdit_get(void);
#endif

static int32 cgi_mcast_igmp_prop_get(void);
static int32 cgi_mcast_igmp_propEdit_get(void);
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
static int32 cgi_mcast_igmp_querier_get(void);
static int32 cgi_mcast_igmp_querierEdit_get(void);
#endif
static int32 cgi_mcast_igmp_stat_get(void);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static int32 cgi_mcast_mld_prop_get(void);
static int32 cgi_mcast_mld_propEdit_get(void);
static int32 cgi_mcast_mld_stat_get(void);
#endif

#ifdef CONFIG_SYS_PROTO_MVR
static int32 cgi_mcast_mvr_prop_get(void);
static int32 cgi_mcast_mvr_group_get(void);
static int32 cgi_mcast_mvr_groupAdd_get(void);
static int32 cgi_mcast_mvr_groupEdit_get(void);
static int32 cgi_mcast_mvr_port_get(void);
static int32 cgi_mcast_mvr_portEdit_get(void);
#endif

const cgi_cmdmap_t cmdGetMcastTbl[] =
{
    {CGI_CMD_MCAST_GEN_PROP,                        cgi_mcast_gen_prop_get},

    {CGI_CMD_MCAST_GEN_GROUP,                      cgi_mcast_gen_group_get},
    {CGI_CMD_MCAST_GEN_GROUPADD,                cgi_mcast_gen_groupAdd_get},
    {CGI_CMD_MCAST_GEN_GROUPEDIT,                cgi_mcast_gen_groupEdit_get},

    {CGI_CMD_MCAST_GEN_ROURTER,                           cgi_mcast_gen_router_get},

#ifdef  CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    {CGI_CMD_MCAST_GEN_ROURTERADD,                    cgi_mcast_gen_routerAdd_get},
    {CGI_CMD_MCAST_GEN_ROURTEREDIT,                    cgi_mcast_gen_routerEdit_get},
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {CGI_CMD_MCAST_GEN_FWDALL,                              cgi_mcast_gen_fwdAll_get},
    {CGI_CMD_MCAST_GEN_FWDALLADD,                       cgi_mcast_gen_fwdAllAdd_get},
    {CGI_CMD_MCAST_GEN_FWDALLEDIT,                       cgi_mcast_gen_fwdAllEdit_get},
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    {CGI_CMD_MCAST_GEN_THROT,                       cgi_mcast_gen_throt_get},
    {CGI_CMD_MCAST_GEN_THROTEDIT,                       cgi_mcast_gen_throtEdit_get},
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {CGI_CMD_MCAST_GEN_PROFILE,                        cgi_mcast_gen_profile_get},
    {CGI_CMD_MCAST_GEN_PROFILEADD,                 cgi_mcast_gen_profileAdd_get},
    {CGI_CMD_MCAST_GEN_PROFILEEDIT,                 cgi_mcast_gen_profileEdit_get},

    {CGI_CMD_MCAST_GEN_FILTER,                       cgi_mcast_gen_filter_get},
    {CGI_CMD_MCAST_GEN_FILTEREDIT,                cgi_mcast_gen_filterEdit_get},
#endif

    {CGI_CMD_MCAST_IGMP_PROP,                       cgi_mcast_igmp_prop_get},
    {CGI_CMD_MCAST_IGMP_PROPEDIT,                 cgi_mcast_igmp_propEdit_get},
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    {CGI_CMD_MCAST_IGMP_QUERIER,                    cgi_mcast_igmp_querier_get},
    {CGI_CMD_MCAST_IGMP_QUERIEREDIT,             cgi_mcast_igmp_querierEdit_get},
#endif
    {CGI_CMD_MCAST_IGMP_STATIC,                       cgi_mcast_igmp_stat_get},

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {CGI_CMD_MCAST_MLD_PROP,                       cgi_mcast_mld_prop_get},
    {CGI_CMD_MCAST_MLD_PROPEDIT,                cgi_mcast_mld_propEdit_get},
    {CGI_CMD_MCAST_MLD_STATIC,                    cgi_mcast_mld_stat_get},
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    {CGI_CMD_MCAST_MVR_PROP,                       cgi_mcast_mvr_prop_get},
    {CGI_CMD_MCAST_MVR_GROUP,                       cgi_mcast_mvr_group_get},
    {CGI_CMD_MCAST_MVR_GROUPADD,                       cgi_mcast_mvr_groupAdd_get},
    {CGI_CMD_MCAST_MVR_GROUPEDIT,                       cgi_mcast_mvr_groupEdit_get},
    {CGI_CMD_MCAST_MVR_PORT,                       cgi_mcast_mvr_port_get},
    {CGI_CMD_MCAST_MVR_PORTEDIT,                       cgi_mcast_mvr_portEdit_get},
#endif

    {NULL, NULL}
};

static int32 cgi_mcast_ipTypeArry_set(cJSON *data)
{
  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    cJSON *ipTypes = NULL;

    if (NULL == data)
        return SYS_ERR_FAILED;

    ipTypes = cJSON_CreateArray();
    cJSON_AddItemToArray(ipTypes, cJSON_CreateString(CGI_MCAST_IPTYPE_V4));
    cJSON_AddItemToArray(ipTypes, cJSON_CreateString(CGI_MCAST_IPTYPE_V6));

    cJSON_AddItemToObject(data, CGI_MCAST_IPTYPES, ipTypes);
#endif
    return SYS_ERR_OK;
}

static int32 cgi_mcast_routerMaxLife_get(uint16 *pTimeArry, uint16 arryNum, uint16 *pMaxLife)
{
    uint16 maxLife = 0;
    int32 i;

    if (NULL == pTimeArry || NULL == pMaxLife)
        return SYS_ERR_FAILED;

    for (i = 0; i < arryNum; i++)
    {
        if (pTimeArry[i] > maxLife)
            maxLife = pTimeArry[i];
    }

    *pMaxLife = maxLife;
    return SYS_ERR_OK;
}

static int32 cgi_mcast_portMbr_set(cJSON *data, const char *pNotMbrParm,  const char *pMbrParm, sys_logic_portmask_t notMbr,  sys_logic_portmask_t mbr)
{
    cJSON *notMbrs = NULL;
    cJSON *mbrs = NULL;
    cJSON *port = NULL;
    sys_logic_port_t lp;
    char portStr[CAPA_PORT_STR_LEN + 1];

    if (NULL == data)
        return SYS_ERR_FAILED;

    if (!IS_LOGIC_PORTMASK_CLEAR(notMbr))
    {
        notMbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, pNotMbrParm, notMbrs);
        FOR_EACH_PORT_IN_LOGIC_PORTMASK (lp, notMbr)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_PORT, portStr);
            cJSON_AddItemToArray(notMbrs, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(mbr))
    {
        mbrs = cJSON_CreateArray();
        cJSON_AddItemToObject(data, pMbrParm, mbrs);
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, mbr)
        {
            SYS_MEM_CLEAR(portStr);
            LP2WEBSTR(lp, portStr);

            port = cJSON_CreateObject();
            cJSON_AddStringToObject(port, CGI_PORT, portStr);
            cJSON_AddItemToArray(mbrs, port);
        }
    }

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    igmp_unknMcastAction_t   unknAct;
    igmp_lookup_mode_t fwdMode;

    sal_igmp_lookupMode_get(&fwdMode);
    sal_igmp_unknMcastAction_get(&unknAct);

    CGI_GET_JSON_CREATE(root, data);

    if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == unknAct)
        cJSON_AddStringToObject(data, CGI_MCAST_UNKNOWN_ACT, CGI_MCAST_UNKNOWN_ACT_FLOOD);
    else if (IGMP_UNKNOWN_MCAST_ACTION_DROP == unknAct)
        cJSON_AddStringToObject(data, CGI_MCAST_UNKNOWN_ACT, CGI_MCAST_UNKNOWN_ACT_DROP);
    else
        cJSON_AddStringToObject(data, CGI_MCAST_UNKNOWN_ACT, CGI_MCAST_UNKNOWN_ACT_ROUTERPORT);

    cJSON_AddStringToObject(data, CGI_MCAST_FWDMETHOD_IPV4, (IGMP_DIP_VID_MODE == fwdMode) ? CGI_MCAST_FWDMETHOD_DIP : CGI_MCAST_FWDMETHOD_DMAC);
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    sal_mld_lookupMode_get(&fwdMode);
    cJSON_AddStringToObject(data, CGI_MCAST_FWDMETHOD_IPV6, (IGMP_DIP_VID_MODE == fwdMode) ? CGI_MCAST_FWDMETHOD_DIP : CGI_MCAST_FWDMETHOD_DMAC);
    #endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *group = NULL;

    char *value = NULL;
    uint32 entry = 0;

    uint32 idx;
    sys_igmp_db_t mcastDb;
    uint32 dbSize = 0;
    char groupStr[CAPA_IPV6_STR_LEN];
    char portStr[CAPA_PORTMASK_STR_LEN];

    multicast_ipType_t  filterIpType = MULTICAST_TYPE_IPV4;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        filterIpType = SYS_STR2UINT(value);
    }

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    groups = cJSON_CreateArray();
    sal_mcast_databaseSize_get(&dbSize);
    for (idx = 0; idx < dbSize; idx++)
    {
        SYS_MEM_CLEAR(mcastDb);
        mcastDb.indx = idx;
        sal_mcast_database_get(&mcastDb);

        if (filterIpType != mcastDb.ipType)
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (sal_igmp_mvr_group_ret(&mcastDb))
            continue;
        #endif

        group = cJSON_CreateObject();
        cJSON_AddNumberToObject(group, CGI_MCAST_IDX, idx);
        cJSON_AddNumberToObject(group, CGI_MCAST_VLAN, mcastDb.vid);

        SYS_MEM_CLEAR(groupStr);
        if (MULTICAST_TYPE_IPV4 == mcastDb.ipType)
        {
            sys_util_ip2Str(mcastDb.dip, groupStr);
        }
        else
        {
            inet_ntop(AF_INET6, mcastDb.dipv6, groupStr, sizeof(groupStr));
        }

        cJSON_AddStringToObject(group, CGI_MCAST_GROUPADDR, groupStr);

        SYS_MEM_CLEAR(portStr);
        LPM2WEBSTR(&mcastDb.portmask, portStr);
        cJSON_AddStringToObject(group, CGI_MCAST_MBR, portStr);

        if (IGMP_GROUP_ENTRY_STATIC ==  mcastDb.form)
        {
            cJSON_AddNumberToObject(group, CGI_MCAST_GRPSTATIC_TYPE, 1);
            cJSON_AddStringToObject(group, CGI_MCAST_GRP_TYPE,  CGI_LANG("mcast",  "lblStatic"));
        }
        else if (IGMP_GROUP_ENTRY_DYNAMIC == mcastDb.form)
        {
            cJSON_AddNumberToObject(group, CGI_MCAST_GRPSTATIC_TYPE, 0);
            cJSON_AddStringToObject(group, CGI_MCAST_GRP_TYPE,  CGI_LANG("mcast",  "lblDynamic"));
            cJSON_AddNumberToObject(group, CGI_MCAST_LIFETIME, mcastDb.life);
        }

        cJSON_AddItemToArray(groups, group);
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_GROUPS, groups);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_groupAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;

    sys_vlanmask_t vlanMask;
    sys_vid_t vid;

    SYS_MEM_CLEAR(vlanMask);
    
    CGI_GET_JSON_CREATE(root, data);

    sal_vlan_adminEntryExist_get(&vlanMask);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        cJSON_AddItemToArray(vlans, cJSON_CreateNumber(vid));
    }
    cJSON_AddItemToObject(data, CGI_MCAST_VIDS, vlans);

    cgi_mcast_ipTypeArry_set(data);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_groupEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    char *value = NULL;
    sys_igmp_db_t mcastDb;
    int32 idx;
    sys_logic_portmask_t notMbr;

    CGI_GET_JSON_CREATE(root, data);

    LOGIC_PORTMASK_SET_ALL(notMbr);
    SYS_MEM_CLEAR(mcastDb);
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IDX, value))
    {
        idx = SYS_STR2UINT(value);
        mcastDb.indx = idx;
        sal_mcast_database_get(&mcastDb);
        LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,mcastDb.staticInMbr);
    }

    cgi_mcast_portMbr_set(data, CGI_MCAST_NOTMBR, CGI_MCAST_GROUP_MBR, notMbr, mcastDb.staticInMbr);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_router_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *routers = NULL;
    cJSON *router = NULL;
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    char *value;
#endif

    sys_vlanmask_t vlanMask;
    sys_vid_t vid;
    igmp_router_entry_t routerEntry;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    char portStr[CAPA_PORTMASK_STR_LEN];
    uint16 maxLife = 0;
    int32 entry = 0;
    sys_logic_portmask_t dynPmsk;

    SYS_MEM_CLEAR(vlanMask);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    cJSON_AddNumberToObject(data, CGI_MCAST_IS_ST_ROUTER,1);
    #endif

    routers = cJSON_CreateArray();

    sal_vlan_operEntryExist_get(&vlanMask);
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(routerEntry);
        routerEntry.vid = vid;
        routerEntry.ipType = ipType;
        sal_mcast_router_get(&routerEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_pmsk) && IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_forbid_pmsk))
            continue;

        router = cJSON_CreateObject();
        cJSON_AddNumberToObject(router, CGI_MCAST_VLAN, vid);

        if (!IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_pmsk))
        {
            SYS_MEM_CLEAR(portStr);
            LPM2WEBSTR(&routerEntry.router_pmsk, portStr);
            cJSON_AddStringToObject(router, CGI_MCAST_MBR, portStr);
        }

        #ifdef  CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        if (!IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_static_pmsk))
        {
            SYS_MEM_CLEAR(portStr);
            LPM2WEBSTR(&routerEntry.router_static_pmsk, portStr);
            cJSON_AddStringToObject(router, CGI_MCAST_MBR_ST, portStr);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_forbid_pmsk))
        {
            SYS_MEM_CLEAR(portStr);
            LPM2WEBSTR(&routerEntry.router_forbid_pmsk, portStr);
            cJSON_AddStringToObject(router, CGI_MCAST_MBR_FB, portStr);
        }
        #endif

        SYS_MEM_CLEAR(dynPmsk);
        LOGIC_PORTMASK_ANDNOT(dynPmsk, routerEntry.router_pmsk, routerEntry.router_static_pmsk);
        if (!IS_LOGIC_PORTMASK_CLEAR(dynPmsk))
        {
             cgi_mcast_routerMaxLife_get(routerEntry.portTimer, SYS_PORT_NUM_MAX, &maxLife);
             cJSON_AddNumberToObject(router, CGI_MCAST_LIFETIME, maxLife);
        }
        cJSON_AddItemToArray(routers, router);
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_ROUTERS, routers);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
static int32 cgi_mcast_gen_routerAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    cJSON *vlansv6 = NULL;
    #endif

    igmp_router_entry_t routerEntry;
    sys_vlanmask_t vlanMask;
    sys_vid_t vid;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    int32 entry = 0;

    SYS_MEM_CLEAR(vlanMask);
    
    CGI_GET_JSON_CREATE(root, data);

    cgi_mcast_ipTypeArry_set(data);

    sal_vlan_adminEntryExist_get(&vlanMask);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(routerEntry);
        routerEntry.vid = vid;
        routerEntry.ipType = ipType;
        sal_mcast_router_get(&routerEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_static_pmsk) && IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_forbid_pmsk))
        {
            cJSON_AddItemToArray(vlans, cJSON_CreateNumber(vid));
            entry++;
        }
    }
    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_AVLVLANS, vlans);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    entry = 0;
    ipType = MULTICAST_TYPE_IPV6;
    vlansv6 = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(routerEntry);
        routerEntry.vid = vid;
        routerEntry.ipType = ipType;
        sal_mcast_router_get(&routerEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_static_pmsk) && IS_LOGIC_PORTMASK_CLEAR(routerEntry.router_forbid_pmsk))
        {
            cJSON_AddItemToArray(vlansv6, cJSON_CreateNumber(vid));
            entry++;
        }
    }
    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_AVLVLANS_V6, vlansv6);
    #endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_routerEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    char *value = NULL;
    sys_logic_portmask_t notMbr;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    igmp_router_entry_t routerEntry;
    sys_vid_t vid = 1;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IDX, value))
        vid = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    LOGIC_PORTMASK_SET_ALL(notMbr);
    SYS_MEM_CLEAR(routerEntry);
    routerEntry.vid = vid;
    routerEntry.ipType = ipType;
    sal_mcast_router_get(&routerEntry);
    LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,routerEntry.router_static_pmsk);
    LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,routerEntry.router_forbid_pmsk);
    cgi_mcast_portMbr_set(data, CGI_MCAST_MBR_NOTST, CGI_MCAST_MBR_ST, notMbr, routerEntry.router_static_pmsk);

    LOGIC_PORTMASK_SET_ALL(notMbr);
    LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,routerEntry.router_forbid_pmsk);
    cgi_mcast_portMbr_set(data, CGI_MCAST_MBR_NOTFB, CGI_MCAST_MBR_FB, notMbr, routerEntry.router_forbid_pmsk);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
static int32 cgi_mcast_gen_fwdAll_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *fwdAll = NULL;
    cJSON *fwdAlls = NULL;
    char *value;

    sys_vlanmask_t vlanMask;
    sys_vid_t vid;
    char portStr[CAPA_PORTMASK_STR_LEN];
    int32 entry =  0;

    mcast_vlan_entry_t vlanEntry;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    SYS_MEM_CLEAR(vlanMask);
    SYS_MEM_CLEAR(vlanEntry);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    fwdAlls = cJSON_CreateArray();

    sal_vlan_operEntryExist_get(&vlanMask);
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = ipType;
        sal_mcast_vlan_get(&vlanEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(vlanEntry.staticMbr) && IS_LOGIC_PORTMASK_CLEAR(vlanEntry.forbidMbr))
            continue;

        fwdAll = cJSON_CreateObject();
        cJSON_AddNumberToObject(fwdAll, CGI_MCAST_VLAN, vid);

        if (!IS_LOGIC_PORTMASK_CLEAR(vlanEntry.staticMbr))
        {
            SYS_MEM_CLEAR(portStr);
            LPM2WEBSTR(&vlanEntry.staticMbr, portStr);
            cJSON_AddStringToObject(fwdAll, CGI_MCAST_MBR_ST, portStr);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(vlanEntry.forbidMbr))
        {
            SYS_MEM_CLEAR(portStr);
            LPM2WEBSTR(&vlanEntry.forbidMbr, portStr);
            cJSON_AddStringToObject(fwdAll, CGI_MCAST_MBR_FB, portStr);
        }

        cJSON_AddItemToArray(fwdAlls, fwdAll);
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_FWDALLS, fwdAlls);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_fwdAllAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    cJSON *vlansv6 = NULL;
    #endif

    mcast_vlan_entry_t vlanEntry;
    sys_vlanmask_t vlanMask;
    sys_vid_t vid;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    int32 entry = 0;

    SYS_MEM_CLEAR(vlanMask);
    
    CGI_GET_JSON_CREATE(root, data);

    cgi_mcast_ipTypeArry_set(data);

    sal_vlan_adminEntryExist_get(&vlanMask);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = ipType;
        sal_mcast_vlan_get(&vlanEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(vlanEntry.staticMbr) && IS_LOGIC_PORTMASK_CLEAR(vlanEntry.forbidMbr))
        {
            cJSON_AddItemToArray(vlans, cJSON_CreateNumber(vid));
            entry++;
        }
    }
    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_AVLVLANS, vlans);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    entry = 0;
    ipType = MULTICAST_TYPE_IPV6;
    vlansv6 = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = ipType;
        sal_mcast_vlan_get(&vlanEntry);

        if (IS_LOGIC_PORTMASK_CLEAR(vlanEntry.staticMbr) && IS_LOGIC_PORTMASK_CLEAR(vlanEntry.forbidMbr))
        {
            cJSON_AddItemToArray(vlansv6, cJSON_CreateNumber(vid));
            entry++;
        }
    }
    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_AVLVLANS_V6, vlansv6);
    #endif

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_fwdAllEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    char *value = NULL;
    sys_logic_portmask_t notMbr;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t vlanEntry;
    sys_vid_t vid = 1;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        return SYS_ERR_OK;

    CGI_GET_JSON_CREATE(root, data);

    LOGIC_PORTMASK_SET_ALL(notMbr);
    SYS_MEM_CLEAR(vlanEntry);
    vlanEntry.vid = vid;
    vlanEntry.ipType = ipType;
    sal_mcast_vlan_get(&vlanEntry);
    LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,vlanEntry.staticMbr);
    cgi_mcast_portMbr_set(data, CGI_MCAST_MBR_NOTST, CGI_MCAST_MBR_ST, notMbr, vlanEntry.staticMbr);

    LOGIC_PORTMASK_SET_ALL(notMbr);
    LOGIC_PORTMASK_ANDNOT(notMbr,notMbr,vlanEntry.forbidMbr);
    cgi_mcast_portMbr_set(data, CGI_MCAST_MBR_NOTFB, CGI_MCAST_MBR_FB, notMbr, vlanEntry.forbidMbr);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
static int32 cgi_mcast_gen_throt_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char *value = NULL;

    sys_logic_port_t lp;
    uint16 maxGrpNum;
    igmp_maxGroupAct_t maxGrpAct;
    multicast_ipType_t  filterIpType = MULTICAST_TYPE_IPV4;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        filterIpType = SYS_STR2UINT(value);
    }

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (MULTICAST_TYPE_IPV4 == filterIpType)
        {
            sal_igmp_portGroup_limit_get(lp, &maxGrpNum);
            sal_igmp_portGroup_excessAct_get(lp, &maxGrpAct);
        }
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
            sal_mld_portGroup_limit_get(lp, &maxGrpNum);
            sal_mld_portGroup_excessAct_get(lp, &maxGrpAct);
        }
        #endif
        port = cJSON_CreateObject();
        cJSON_AddNumberToObject(port, CGI_MCAST_MAXGRP_NUM,maxGrpNum);
        cJSON_AddLangStringToObject(port, CGI_MCAST_MAXGRP_ACT, "mcast",  cgiMaxGrpAct[maxGrpAct]);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_throtEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    uint16 maxGrp;
    igmp_maxGroupAct_t maxGrpAct;
    sys_logic_port_t lp = 0;
    multicast_ipType_t  filterIpType = MULTICAST_TYPE_IPV4;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
     
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        filterIpType = SYS_STR2UINT(value);
    }
    #endif

    CGI_GET_JSON_CREATE(root, data);

    if (MULTICAST_TYPE_IPV4 == filterIpType)
    {
        sal_igmp_portGroup_limit_get(lp, &maxGrp);
        sal_igmp_portGroup_excessAct_get(lp, &maxGrpAct);
    }
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        sal_mld_portGroup_limit_get(lp, &maxGrp);
        sal_mld_portGroup_excessAct_get(lp, &maxGrpAct);
    }
    #endif

    cJSON_AddNumberToObject(data, CGI_MCAST_GROUPNUM_MAX, CAPA_MCAST_PORT_LIMIT_NUM);
    cJSON_AddNumberToObject(data, CGI_MCAST_GROUPNUM_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_MCAST_GROUPNUM, maxGrp);
    cJSON_AddLangStringToObject(data, CGI_MCAST_MAXGRP_ACT,"mcast", cgiMaxGrpAct[maxGrpAct]);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 cgi_mcast_gen_profile_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *profiles = NULL;
    cJSON *profile = NULL;

    char *value = NULL;
    uint32 entry = 0;
    uint32 idx = 0;
    sys_igmp_profile_t profileEntry;
    multicast_ipType_t  filterIpType = MULTICAST_TYPE_IPV4;
    char grpAddrStartStr[CAPA_IPV6_STR_LEN];
    char grpAddrEndStr[CAPA_IPV6_STR_LEN];

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        filterIpType = SYS_STR2UINT(value);
    }

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    profiles = cJSON_CreateArray();
    for (idx = BOND_MCAST_PROFILE_MIN; idx <= BOND_MCAST_PROFILE_MAX; idx++)
    {
        if (MULTICAST_TYPE_IPV4 == filterIpType)
        {
            if (SYS_ERR_OK != sal_igmp_profile_get(idx, &profileEntry))
                continue;

            SYS_MEM_CLEAR(grpAddrStartStr);
            SYS_MEM_CLEAR(grpAddrEndStr);
            sys_util_ip2Str(profileEntry.lowIp, grpAddrStartStr);
            sys_util_ip2Str(profileEntry.highIp, grpAddrEndStr);
        }
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
            if (SYS_ERR_OK != sal_mld_profile_get(idx, &profileEntry))
                continue;

            SYS_MEM_CLEAR(grpAddrStartStr);
            SYS_MEM_CLEAR(grpAddrEndStr);
            inet_ntop(AF_INET6, profileEntry.ipv6Low, grpAddrStartStr, sizeof(grpAddrStartStr));
            inet_ntop(AF_INET6, profileEntry.ipv6High, grpAddrEndStr, sizeof(grpAddrEndStr));
        }
        #endif

        profile = cJSON_CreateObject();
        cJSON_AddNumberToObject(profile, CGI_MCAST_PROFILE_IDX, idx);

        cJSON_AddStringToObject(profile, CGI_MCAST_PROFILE_STARTADDR, grpAddrStartStr);
        cJSON_AddStringToObject(profile, CGI_MCAST_PROFILE_ENDADDR, grpAddrEndStr);
        cJSON_AddLangStringToObject(profile, CGI_MCAST_PROFILE_ACT, "mcast",  cgiProfileAct[profileEntry.act]);
        cJSON_AddItemToArray(profiles, profile);
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_PROFILES, profiles);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_profileAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);
    
    cgi_mcast_ipTypeArry_set(data);

    cJSON_AddNumberToObject(data, CGI_MCAST_PROFILE_MIN_IDX, BOND_MCAST_PROFILE_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_PROFILE_MAX_IDX, BOND_MCAST_PROFILE_MAX);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_profileEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_filter_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    sys_logic_port_t lp;
    sys_igmp_profileId_t prfIdx;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    char *value = NULL;
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
            sal_igmp_port_filter_get(lp, &prfIdx);
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
            sal_mld_port_filter_get(lp, &prfIdx);
        #endif

        port = cJSON_CreateObject();
        if (INVALID_PROFILE_ID == prfIdx)
        {
            cJSON_AddBoolToObject(port, CGI_MCAST_FILTER, FALSE);
        }
        else
        {
            cJSON_AddBoolToObject(port, CGI_MCAST_FILTER, TRUE);
            cJSON_AddNumberToObject(port, CGI_MCAST_PROFILE_IDX, prfIdx);
        }

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_filterEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *idxs = NULL;
    char *value = NULL;

    sys_logic_port_t lp = 0;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    sys_igmp_profileId_t prfIdx;
    sys_igmp_profile_t tmpPrf;
    int32 idx;
    uint32 entry = 0;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
        ipType = SYS_STR2UINT(value);
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    if (MULTICAST_TYPE_IPV4 == ipType)
        sal_igmp_port_filter_get(lp, &prfIdx);
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
        sal_mld_port_filter_get(lp, &prfIdx);
    #endif

    if (INVALID_PROFILE_ID == prfIdx)
        cJSON_AddBoolToObject(data, CGI_MCAST_FILTER, FALSE);
    else
        cJSON_AddBoolToObject(data, CGI_MCAST_FILTER, TRUE);

    cJSON_AddNumberToObject(data, CGI_MCAST_PROFILE_IDX, prfIdx);

    idxs = cJSON_CreateArray();
    for (idx = BOND_MCAST_PROFILE_MIN; idx <= BOND_MCAST_PROFILE_MAX; idx++)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (SYS_ERR_OK != sal_igmp_profile_get(idx, &tmpPrf))
                continue;
        }
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
             if (SYS_ERR_OK != sal_mld_profile_get(idx, &tmpPrf))
                continue;
        }
        #endif
        cJSON_AddItemToArray(idxs , cJSON_CreateNumber(idx));
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data,CGI_MCAST_PROFILE_IDXS,idxs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_mcast_igmp_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;
    cJSON *vlan = NULL;

    sys_mcast_info_t mcastInfo;
    igmp_lookup_mode_t mode;
    sys_vlanmask_t vlanMsk;
    sys_vid_t vid;
    uint32 entry = 0;

    mcast_vlan_entry_t vlanEntry;

    SYS_MEM_CLEAR(mcastInfo);
    SYS_MEM_CLEAR(vlanMsk);

    CGI_GET_JSON_CREATE(root, data);

    sal_igmp_info_get(&mcastInfo);
    sal_igmp_lookupMode_get(&mode);

    cJSON_AddBoolToObject(data,CGI_MCAST_STATE, mcastInfo.enable);
    #ifdef CONFIG_SYS_PROTO_IGMPV3_BASIC
    cJSON_AddNumberToObject(data, CGI_MCAST_VER, (mcastInfo.operVersion <= IGMP_VERSION_V2) ? 2:3);
    #endif
    cJSON_AddBoolToObject(data, CGI_MCAST_REPORTSUP, mcastInfo.suppreEnable);

    sal_vlan_operEntryExist_get(&vlanMsk);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMsk)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = MULTICAST_TYPE_IPV4;
        sal_mcast_vlan_get(&vlanEntry);

        vlan = cJSON_CreateObject();
        cJSON_AddNumberToObject(vlan, CGI_MCAST_VLAN, vid);
        cJSON_AddBoolToObject(vlan, CGI_MCAST_OPERSTS, (vlanEntry.enable & mcastInfo.enable));
        cJSON_AddBoolToObject(vlan, CGI_MCAST_ROUTER_AUTOLEARN, vlanEntry.pimDvmrpLearn);

        cJSON_AddNumberToObject(vlan, CGI_MCAST_ROBUST, vlanEntry.operRobNessVar);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_INTVAL, vlanEntry.operQueryIntv);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_MAXRESPONSE, vlanEntry.operRespTime);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_LASTMBRCNT, vlanEntry.operLastMmbrQueryCnt);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_LASTMBRINTVAL, vlanEntry.operGsqueryIntv);

        cJSON_AddBoolToObject(vlan, CGI_MCAST_FASTLEAVE, vlanEntry.fastleave_enable);

         cJSON_AddItemToArray(vlans, vlan);
         entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

static int32 cgi_mcast_igmp_propEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    sys_vlanmask_t vlanMask;
    sys_vid_t getVid = 1;
    char *value = NULL;

    mcast_vlan_entry_t vlanEntry;
    sys_enable_t   igmpEn;

    SYS_MEM_CLEAR(vlanMask);
    SYS_MEM_CLEAR(vlanEntry);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        getVid = SYS_STR2UINT(value);

    sal_igmp_enable_get(&igmpEn);
    vlanEntry.vid = getVid;
    vlanEntry.ipType = MULTICAST_TYPE_IPV4;
    sal_mcast_vlan_get(&vlanEntry);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_MCAST_ADMIN_STS, vlanEntry.enable);
    cJSON_AddBoolToObject(data, CGI_MCAST_OPER_STS, (vlanEntry.enable & igmpEn) );
    cJSON_AddBoolToObject(data, CGI_MCAST_ROUTER_AUTOLEARN, vlanEntry.pimDvmrpLearn);

    cJSON_AddNumberToObject(data, CGI_MCAST_ROBUST, vlanEntry.robustnessVar);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_ROBUST, vlanEntry.operRobNessVar);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_ROBUST,  BOND_MCAST_ROBUST_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_ROBUST, BOND_MCAST_ROBUST_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_ROBUST, DFLT_IGMP_ROBUSTNESS_VAR);

    cJSON_AddNumberToObject(data, CGI_MCAST_INTVAL, vlanEntry.queryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_INTVAL, vlanEntry.operQueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_INTVAL,  BOND_MCAST_QRYINTV_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_INTVAL, BOND_MCAST_QRYINTV_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_INTVAL, DFLT_IGMP_QUERY_INTV);

    cJSON_AddNumberToObject(data, CGI_MCAST_MAXRESPONSE, vlanEntry.responseTime);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_MAXRESPONSE, vlanEntry.operRespTime);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_MAXRESPONSE,  BOND_MCAST_RESPONSE_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_MAXRESPONSE, BOND_MCAST_RESPONSE_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_MAXRESPONSE, DFLT_IGMP_QUERY_RESPONSE_INTV);

    cJSON_AddNumberToObject(data, CGI_MCAST_LASTMBRCNT, vlanEntry.lastMmbrQueryCnt);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_LASTMBRCNT, vlanEntry.operLastMmbrQueryCnt);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_LASTMBRCNT,  BOND_MCAST_LASTMBRCNT_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_LASTMBRCNT, BOND_MCAST_LASTMBRCNT_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_LASTMBRCNT, DFLT_IGMP_ROBUSTNESS_VAR);

    cJSON_AddNumberToObject(data, CGI_MCAST_LASTMBRINTVAL, vlanEntry.grpSpecificQueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_LASTMBRINTVAL, vlanEntry.operGsqueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_LASTMBRINTVAL,  BOND_MCAST_LASTMBRINTV_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_LASTMBRINTVAL, BOND_MCAST_LASTMBRINTV_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_LASTMBRINTVAL, DFLT_IGMP_GSQUERY_RESPONSE_INTV);

    cJSON_AddBoolToObject(data, CGI_MCAST_FASTLEAVE, vlanEntry.fastleave_enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
static int32 cgi_mcast_igmp_querier_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;
    cJSON *vlan = NULL;

    sys_vlanmask_t vlanMsk;
    sys_vid_t vid;
    uint32 entry = 0;
    char ipStr[CAPA_IPV4_STR_LEN];
    sys_igmp_querier_t  querierEntry;
    mcast_vlan_entry_t  mcastVlan;
    sys_enable_t igmpEn;

    SYS_MEM_CLEAR(vlanMsk);

    CGI_GET_JSON_CREATE(root, data);
    sal_vlan_operEntryExist_get(&vlanMsk);
    sal_igmp_enable_get(&igmpEn);

    vlans = cJSON_CreateArray();

    FOR_EACH_VID_IN_VLANMASK(vid, vlanMsk)
    {
        SYS_MEM_CLEAR(querierEntry);
        querierEntry.vid =  vid;
        querierEntry.ipType = MULTICAST_TYPE_IPV4;
        sal_igmp_querier_get(&querierEntry);

        vlan = cJSON_CreateObject();
        cJSON_AddNumberToObject(vlan, CGI_MCAST_VLAN, vid);
        cJSON_AddBoolToObject(vlan, CGI_MCAST_QUERYSTATE, querierEntry.enabled);
        cJSON_AddBoolToObject(vlan, CGI_MCAST_QUERIERSTS, querierEntry.status);

        if (ENABLED == querierEntry.enabled)
        {
            mcastVlan.vid =  vid;
            mcastVlan.ipType = MULTICAST_TYPE_IPV4;
            sal_mcast_vlan_get(&mcastVlan);

            cJSON_AddNumberToObject(vlan, CGI_MCAST_QUERIER_VER, querierEntry.version);
            if (ENABLED == mcastVlan.enable &&  ENABLED == igmpEn)
            {
                SYS_MEM_CLEAR(ipStr);
                sys_util_ip2Str(querierEntry.ipAddr, ipStr);
                cJSON_AddStringToObject(vlan, CGI_MCAST_QUERIER_IP, ipStr);
            }
        }

         cJSON_AddItemToArray(vlans, vlan);
         entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

static int32 cgi_mcast_igmp_querierEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    sys_igmp_querier_t  querierEntry;
    sys_vid_t  getVid = 1;

    SYS_MEM_CLEAR(querierEntry);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        getVid = SYS_STR2UINT(value);

    querierEntry.vid = getVid;
    querierEntry.ipType = MULTICAST_TYPE_IPV4;
    sal_igmp_querier_get(&querierEntry);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddBoolToObject(data, CGI_MCAST_QUERYSTATE, querierEntry.enabled);
    if (ENABLED == querierEntry.enabled)
        cJSON_AddNumberToObject(data,  CGI_MCAST_QUERIER_VER, querierEntry.version);
    else
        cJSON_AddNumberToObject(data,  CGI_MCAST_QUERIER_VER, IGMP_QUERY_V2);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_mcast_igmp_stat_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_mcast_info_t info;

    SYS_MEM_CLEAR(info);
    sal_igmp_info_get(&info);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_TOTAL, info.totStat[MCAST_STAT_TOTAL_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_VALID, info.totStat[MCAST_STAT_VALID_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_INVALID,  info.totStat[MCAST_STAT_INVALID_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_OTHER,  info.totStat[MCAST_STAT_OTHER_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_LEAVE,  info.totStat[MCAST_STAT_LEAVE_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_REPORT, info.totStat[MCAST_STAT_REPORT_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_GENQRY, info.totStat[MCAST_STAT_GENQRY_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_SPECGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_SPECSRCGRPQRY,  info.totStat[MCAST_STAT_SPEC_GROUP_SRC_QRY_RX]);

    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_LEAVE, info.totStat[MCAST_STAT_LEAVE_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_REPORT, info.totStat[MCAST_STAT_REPORT_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_GENQRYR,  info.totStat[MCAST_STAT_GENQRY_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_SPECGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_SPECSRCGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_TX]);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

     return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static int32 cgi_mcast_mld_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;
    cJSON *vlan = NULL;

    sys_mcast_info_t mcastInfo;
    igmp_lookup_mode_t mode;
    sys_vlanmask_t vlanMsk;
    sys_vid_t vid;
    uint32 entry = 0;

    mcast_vlan_entry_t vlanEntry;

    SYS_MEM_CLEAR(mcastInfo);
    SYS_MEM_CLEAR(vlanMsk);

    CGI_GET_JSON_CREATE(root, data);

    sal_mld_info_get(&mcastInfo);
    sal_mld_lookupMode_get(&mode);

    cJSON_AddBoolToObject(data,CGI_MCAST_STATE, mcastInfo.enable);
    cJSON_AddNumberToObject(data, CGI_MCAST_VER, (mcastInfo.oper_version == MLD_VERSION_V1) ? 1:2);
    cJSON_AddBoolToObject(data, CGI_MCAST_REPORTSUP, mcastInfo.suppreEnable);

    sal_vlan_operEntryExist_get(&vlanMsk);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMsk)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = MULTICAST_TYPE_IPV6;
        sal_mcast_vlan_get(&vlanEntry);

        vlan = cJSON_CreateObject();
        cJSON_AddNumberToObject(vlan, CGI_MCAST_VLAN, vid);
        cJSON_AddBoolToObject(vlan, CGI_MCAST_OPERSTS, (vlanEntry.enable & mcastInfo.enable));
        cJSON_AddBoolToObject(vlan, CGI_MCAST_ROUTER_AUTOLEARN, vlanEntry.pimDvmrpLearn);

        cJSON_AddNumberToObject(vlan, CGI_MCAST_ROBUST, vlanEntry.robustnessVar);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_INTVAL, vlanEntry.queryIntv);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_MAXRESPONSE, vlanEntry.responseTime);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_LASTMBRCNT, vlanEntry.lastMmbrQueryCnt);
        cJSON_AddNumberToObject(vlan, CGI_MCAST_LASTMBRINTVAL, vlanEntry.grpSpecificQueryIntv);

        cJSON_AddBoolToObject(vlan, CGI_MCAST_FASTLEAVE, vlanEntry.fastleave_enable);

         cJSON_AddItemToArray(vlans, vlan);
         entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_VLANS, vlans);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

static int32 cgi_mcast_mld_propEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;

    sys_vlanmask_t vlanMask;
    sys_vid_t getVid = 1;
    char *value = NULL;

    mcast_vlan_entry_t vlanEntry;
    sys_enable_t   igmpEn;

    SYS_MEM_CLEAR(vlanMask);
    SYS_MEM_CLEAR(vlanEntry);

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        getVid = SYS_STR2UINT(value);

    sal_mld_enable_get(&igmpEn);
    vlanEntry.vid = getVid;
    vlanEntry.ipType = MULTICAST_TYPE_IPV6;
    sal_mcast_vlan_get(&vlanEntry);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_MCAST_ADMIN_STS, vlanEntry.enable);
    cJSON_AddBoolToObject(data, CGI_MCAST_OPER_STS, (vlanEntry.enable & igmpEn) );
    cJSON_AddBoolToObject(data, CGI_MCAST_ROUTER_AUTOLEARN, vlanEntry.pimDvmrpLearn);

    cJSON_AddNumberToObject(data, CGI_MCAST_ROBUST, vlanEntry.robustnessVar);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_ROBUST, vlanEntry.operRobNessVar);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_ROBUST,  BOND_MCAST_ROBUST_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_ROBUST, BOND_MCAST_ROBUST_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_ROBUST, DFLT_IGMP_ROBUSTNESS_VAR);

    cJSON_AddNumberToObject(data, CGI_MCAST_INTVAL, vlanEntry.queryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_INTVAL, vlanEntry.operQueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_INTVAL,  BOND_MCAST_QRYINTV_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_INTVAL, BOND_MCAST_QRYINTV_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_INTVAL, DFLT_IGMP_QUERY_INTV);

    cJSON_AddNumberToObject(data, CGI_MCAST_MAXRESPONSE, vlanEntry.responseTime);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_MAXRESPONSE, vlanEntry.operRespTime);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_MAXRESPONSE,  BOND_MCAST_RESPONSE_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_MAXRESPONSE, BOND_MCAST_RESPONSE_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_MAXRESPONSE, DFLT_IGMP_QUERY_RESPONSE_INTV);

    cJSON_AddNumberToObject(data, CGI_MCAST_LASTMBRCNT, vlanEntry.lastMmbrQueryCnt);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_LASTMBRCNT, vlanEntry.operLastMmbrQueryCnt);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_LASTMBRCNT,  BOND_MCAST_LASTMBRCNT_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_LASTMBRCNT, BOND_MCAST_LASTMBRCNT_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_LASTMBRCNT, DFLT_IGMP_ROBUSTNESS_VAR);

    cJSON_AddNumberToObject(data, CGI_MCAST_LASTMBRINTVAL, vlanEntry.grpSpecificQueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_OPER_LASTMBRINTVAL, vlanEntry.operGsqueryIntv);
    cJSON_AddNumberToObject(data, CGI_MCAST_MIN_LASTMBRINTVAL,  BOND_MCAST_LASTMBRINTV_MIN);
    cJSON_AddNumberToObject(data, CGI_MCAST_MAX_LASTMBRINTVAL, BOND_MCAST_LASTMBRINTV_MAX);
    cJSON_AddNumberToObject(data, CGI_MCAST_DFLT_LASTMBRINTVAL, DFLT_IGMP_GSQUERY_RESPONSE_INTV );

    cJSON_AddBoolToObject(data, CGI_MCAST_FASTLEAVE, vlanEntry.fastleave_enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mld_stat_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_mcast_info_t info;

    SYS_MEM_CLEAR(info);
    sal_mld_info_get(&info);

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_TOTAL, info.totStat[MCAST_STAT_TOTAL_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_VALID, info.totStat[MCAST_STAT_VALID_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_INVALID,  info.totStat[MCAST_STAT_INVALID_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_OTHER,  info.totStat[MCAST_STAT_OTHER_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_LEAVE,  info.totStat[MCAST_STAT_LEAVE_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_REPORT, info.totStat[MCAST_STAT_REPORT_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_GENQRY, info.totStat[MCAST_STAT_GENQRY_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_SPECGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_RX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_RX_SPECSRCGRPQRY,  info.totStat[MCAST_STAT_SPEC_GROUP_SRC_QRY_RX]);

    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_LEAVE, info.totStat[MCAST_STAT_LEAVE_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_REPORT, info.totStat[MCAST_STAT_REPORT_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_GENQRYR,  info.totStat[MCAST_STAT_GENQRY_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_SPECGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_TX]);
    cJSON_AddNumberToObject(data,  CGI_MCAST_STAT_TX_SPECSRCGRPQRY, info.totStat[MCAST_STAT_SPEC_GROUP_QRY_TX]);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

     return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_PROTO_MVR
static int32 cgi_mcast_mvr_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *vlans = NULL;

    sys_enable_t enable;
    sys_vid_t mvrVlanId;
    uint32 maxGroupNum = 0, curGroupNum = 0;
    uint32 startIp = 0, endIp = 0;
    uint32 responseTime;
    mvr_mode_t mode;
    char startIpStr[CAPA_IPV4_STR_LEN];
    sys_vlanmask_t vlanMask;
    sys_vid_t vid;
    int32 entry = 0;

    sal_igmp_mvr_enable_get(&enable);
    sal_igmp_mvr_mode_get(&mode);
    sal_igmp_mvr_querytime_get(&responseTime);
    sal_igmp_mvr_vlanId_get(&mvrVlanId);
    sal_igmp_mvr_groupRange_get(&startIp, &endIp);
    sal_igmp_mvrGroupNum_get(&curGroupNum);
    maxGroupNum = CAPA_IGMP_MVR_GROUP_NUM;

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddBoolToObject(data,CGI_MCAST_STATE, enable);
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_VID, mvrVlanId);

    SYS_MEM_CLEAR(vlanMask);
    sal_vlan_adminEntryExist_get(&vlanMask);
    vlans = cJSON_CreateArray();
    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        cJSON_AddItemToArray(vlans, cJSON_CreateNumber(vid));
        entry++;
    }
    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_MVR_VLANS, vlans);

    SYS_MEM_CLEAR(startIpStr);
    sys_util_ip2Str(startIp, startIpStr);
    cJSON_AddStringToObject(data, CGI_MCAST_MVR_GRPSTART, startIpStr);
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_SEQNUM, endIp-startIp+1);
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_QRYTIME, responseTime);
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_QRYTIME_MAX, 10);
    
    cJSON_AddStringToObject(data, CGI_MCAST_MVR_MODE, (MVR_MODE_COMPATABLE == mode ) ? "Compatible" : "Dynamic");
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_NUM_MAX, maxGroupNum);
    cJSON_AddNumberToObject(data, CGI_MCAST_MVR_NUM_CUR, curGroupNum);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *group = NULL;
    uint32 entry = 0;

    uint32 idx;
    sys_igmp_db_t mcastDb;
    uint32 dbSize = 0;
    char groupStr[CAPA_IPV4_STR_LEN];
    char portStr[CAPA_PORTMASK_STR_LEN];

    CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    groups = cJSON_CreateArray();
    sal_mcast_databaseSize_get(&dbSize);
    for (idx = 0; idx < dbSize; idx++)
    {
        SYS_MEM_CLEAR(mcastDb);
        mcastDb.indx = idx;
        sal_mcast_database_get(&mcastDb);

        if (!sal_igmp_mvr_group_ret(&mcastDb))
            continue;

        group = cJSON_CreateObject();
        cJSON_AddNumberToObject(group, CGI_MCAST_IDX, idx);
        cJSON_AddNumberToObject(group, CGI_MCAST_VLAN, mcastDb.vid);

        SYS_MEM_CLEAR(groupStr);
        sys_util_ip2Str(mcastDb.dip, groupStr);
        cJSON_AddStringToObject(group, CGI_MCAST_GROUPADDR, groupStr);

        SYS_MEM_CLEAR(portStr);
        LPM2WEBSTR(&mcastDb.portmask, portStr);
        cJSON_AddStringToObject(group, CGI_MCAST_MBR, portStr);

        if (IGMP_GROUP_ENTRY_STATIC ==  mcastDb.form)
        {
            cJSON_AddNumberToObject(group, CGI_MCAST_GRPSTATIC_TYPE, 1);
            cJSON_AddStringToObject(group, CGI_MCAST_GRP_TYPE,  CGI_LANG("mcast",  "lblStatic"));
        }
        else if (IGMP_GROUP_ENTRY_DYNAMIC == mcastDb.form)
        {
            cJSON_AddNumberToObject(group, CGI_MCAST_GRPSTATIC_TYPE, 0);
            cJSON_AddStringToObject(group, CGI_MCAST_GRP_TYPE,  CGI_LANG("mcast",  "lblDynamic"));
            cJSON_AddNumberToObject(group, CGI_MCAST_LIFETIME, mcastDb.life);
        }

        cJSON_AddItemToArray(groups, group);
        entry++;
    }

    if (entry > 0)
        cJSON_AddItemToObject(data, CGI_MCAST_GROUPS, groups);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_groupAdd_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    sys_logic_portmask_t notMbr;
    sys_logic_portmask_t grpMbr;
    mvr_mode_t mvrMode;
    sys_vid_t mvrVlanId;
    sys_logic_port_t lp;
    mvr_portType_t  portType;
    char startIpStr[CAPA_IPV4_STR_LEN];
    char endIpStr[CAPA_IPV4_STR_LEN];
    uint32 startIp = 0, endIp = 0;

    CGI_GET_JSON_CREATE(root, data);

    sal_igmp_mvr_vlanId_get(&mvrVlanId);
    sal_igmp_mvr_groupRange_get(&startIp, &endIp);
    SYS_MEM_CLEAR(startIpStr);
    SYS_MEM_CLEAR(endIpStr);
    sys_util_ip2Str(startIp, startIpStr);
    sys_util_ip2Str(endIp, endIpStr);
    cJSON_AddNumberToObject(data, CGI_MCAST_VLAN, mvrVlanId);
    cJSON_AddStringToObject(data, CGI_MCAST_MVR_GRPSTART, startIpStr);
    cJSON_AddStringToObject(data, CGI_MCAST_MVR_GRPEND, endIpStr);

    LOGIC_PORTMASK_CLEAR_ALL(notMbr);
    LOGIC_PORTMASK_CLEAR_ALL(grpMbr);

    sal_igmp_mvr_mode_get(&mvrMode);
    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_igmp_mvr_portType_get(lp, &portType);
        if (MVR_TYPE_RECEIVER == portType)
            LOGIC_PORTMASK_SET_PORT(notMbr, lp);
        if (MVR_MODE_DYNAMIC == mvrMode)
        {
            if (MVR_TYPE_SOURCE == portType)
                LOGIC_PORTMASK_SET_PORT(notMbr, lp);
        }
    }

    cgi_mcast_portMbr_set(data, CGI_MCAST_NOTMBR, CGI_MCAST_GROUP_MBR, notMbr, grpMbr);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_groupEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    sys_igmp_db_t mcastDb;
    sys_logic_portmask_t notMbr;
    mvr_mode_t mvrMode;
    sys_vid_t mvrVlanId;
    sys_logic_port_t lp;
    mvr_portType_t  portType;

    CGI_GET_JSON_CREATE(root, data);
    sal_igmp_mvr_vlanId_get(&mvrVlanId);
    cJSON_AddNumberToObject(data, CGI_MCAST_VLAN, mvrVlanId);

    SYS_MEM_CLEAR(mcastDb);
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IDX, value))
    {
        mcastDb.indx = SYS_STR2UINT(value);
        sal_mcast_database_get(&mcastDb);
    }

    LOGIC_PORTMASK_CLEAR_ALL(notMbr);
    sal_igmp_mvr_mode_get(&mvrMode);
    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_igmp_mvr_portType_get(lp, &portType);
        if (MVR_TYPE_RECEIVER == portType)
            LOGIC_PORTMASK_SET_PORT(notMbr, lp);
        if (MVR_MODE_DYNAMIC == mvrMode)
        {
            if (MVR_TYPE_SOURCE == portType)
                LOGIC_PORTMASK_SET_PORT(notMbr, lp);
        }
    }

    LOGIC_PORTMASK_ANDNOT(notMbr, notMbr, mcastDb.staticInMbr);

    cgi_mcast_portMbr_set(data, CGI_MCAST_NOTMBR, CGI_MCAST_GROUP_MBR, notMbr, mcastDb.staticInMbr);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_port_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;

    sys_logic_port_t lp;
    mvr_portType_t  portType;
    sys_enable_t enable;

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_igmp_mvr_portType_get(lp, &portType);
        sal_igmp_mvr_portImmediate_get(lp, &enable);

        port = cJSON_CreateObject();
        cJSON_AddLangStringToObject(port, CGI_MCAST_MVR_PORTTYPE, "mvr",  cgiMvrPortType[portType]);
        cJSON_AddBoolToObject(port, CGI_MCAST_MVR_IMMDLEAVE,enable);

        cJSON_AddItemToArray(ports, port);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_portEdit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char *value = NULL;

    sys_logic_port_t lp = 0;
    mvr_portType_t  portType;
    sys_enable_t enable;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

    CGI_GET_JSON_CREATE(root, data);

    sal_igmp_mvr_portType_get(lp, &portType);
    sal_igmp_mvr_portImmediate_get(lp, &enable);

    cJSON_AddLangStringToObject(data, CGI_MCAST_MVR_PORTTYPE,"mvr", cgiMvrPortType[portType]);
    cJSON_AddBoolToObject(data, CGI_MCAST_MVR_IMMDLEAVE,enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#endif

