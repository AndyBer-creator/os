
#include <include/cgi.h>

typedef struct mcast_entry_s
{
    multicast_ipType_t ipType;
    sys_vid_t vid;
    uint32 dip;
    uint32 sip;
    char   dipv6[IPV6_ADDR_LEN];
}mcast_entry_t;

static int32 cgi_mcast_gen_prop_set(void);

static int32 cgi_mcast_gen_groupAdd_set(void);
static int32 cgi_mcast_gen_groupEdit_set(void);
static int32 cgi_mcast_gen_groupDel_set(void);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
static int32 cgi_mcast_gen_throtEdit_set(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
static int32 cgi_mcast_gen_routerAdd_set(void);
static int32 cgi_mcast_gen_routerEdit_set(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
static int32 cgi_mcast_gen_fwdAllAdd_set(void);
static int32 cgi_mcast_gen_fwdAllEdit_set(void);
static int32 cgi_mcast_gen_fwdAllDel_set(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 cgi_mcast_gen_profileAdd_set(void);
static int32 cgi_mcast_gen_profileEdit_set(void);
static int32 cgi_mcast_gen_profileDel_set(void);

static int32 cgi_mcast_gen_filterEdit_set(void);
#endif

static int32 cgi_mcast_igmp_prop_set(void);
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
static int32 cgi_mcast_igmp_propEdit_set(void);
static int32 cgi_mcast_igmp_querierEdit_set(void);
#endif
static int32 cgi_mcast_igmp_statClear_set(void);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static int32 cgi_mcast_mld_prop_set(void);
static int32 cgi_mcast_mld_propEdit_set(void);
static int32 cgi_mcast_mld_statClear_set(void);
#endif

#ifdef CONFIG_SYS_PROTO_MVR
static int32 cgi_mcast_mvr_prop_set(void);

static int32 cgi_mcast_mvr_groupAdd_set(void);
static int32 cgi_mcast_mvr_groupEdit_set(void);
static int32 cgi_mcast_mvr_groupDel_set(void);

static int32 cgi_mcast_mvr_portEdit_set(void);
#endif

const cgi_cmdmap_t cmdSetMcastTbl[] =
{
    {CGI_CMD_MCAST_GEN_PROP,                cgi_mcast_gen_prop_set},

    {CGI_CMD_MCAST_GEN_GROUPADD,                cgi_mcast_gen_groupAdd_set},
    {CGI_CMD_MCAST_GEN_GROUPEDIT,                cgi_mcast_gen_groupEdit_set},
    {CGI_CMD_MCAST_GEN_GROUPDEL,                cgi_mcast_gen_groupDel_set},

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    {CGI_CMD_MCAST_GEN_ROURTERADD,            cgi_mcast_gen_routerAdd_set},
    {CGI_CMD_MCAST_GEN_ROURTEREDIT,            cgi_mcast_gen_routerEdit_set},
    
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {CGI_CMD_MCAST_GEN_FWDALLADD,                     cgi_mcast_gen_fwdAllAdd_set},
    {CGI_CMD_MCAST_GEN_FWDALLEDIT,                     cgi_mcast_gen_fwdAllEdit_set},
    {CGI_CMD_MCAST_GEN_FWDALLDEL,                       cgi_mcast_gen_fwdAllDel_set},
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    {CGI_CMD_MCAST_GEN_THROTEDIT,                       cgi_mcast_gen_throtEdit_set},
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {CGI_CMD_MCAST_GEN_PROFILEADD,                cgi_mcast_gen_profileAdd_set},
    {CGI_CMD_MCAST_GEN_PROFILEEDIT,               cgi_mcast_gen_profileEdit_set},
    {CGI_CMD_MCAST_GEN_PROFILEDEL,                cgi_mcast_gen_profileDel_set},

    {CGI_CMD_MCAST_GEN_FILTEREDIT,               cgi_mcast_gen_filterEdit_set},
    #endif

    {CGI_CMD_MCAST_IGMP_PROP,                               cgi_mcast_igmp_prop_set},
    {CGI_CMD_MCAST_IGMP_PROPEDIT,                       cgi_mcast_igmp_propEdit_set},
    {CGI_CMD_MCAST_IGMP_QUERIEREDIT,               cgi_mcast_igmp_querierEdit_set},
    {CGI_CMD_MCAST_IGMP_STATICCLR,                  cgi_mcast_igmp_statClear_set},

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    {CGI_CMD_MCAST_MLD_PROP,               cgi_mcast_mld_prop_set},
    {CGI_CMD_MCAST_MLD_PROPEDIT,               cgi_mcast_mld_propEdit_set},
    {CGI_CMD_MCAST_MLD_STATICCLR,                  cgi_mcast_mld_statClear_set},
    #endif

    #ifdef CONFIG_SYS_PROTO_MVR
    {CGI_CMD_MCAST_MVR_PROP,               cgi_mcast_mvr_prop_set},
    {CGI_CMD_MCAST_MVR_GROUPADD,      cgi_mcast_mvr_groupAdd_set},
    {CGI_CMD_MCAST_MVR_GROUPEDIT,      cgi_mcast_mvr_groupEdit_set},
    {CGI_CMD_MCAST_MVR_GROUPDEL,        cgi_mcast_mvr_groupDel_set},
    {CGI_CMD_MCAST_MVR_PORTEDIT,        cgi_mcast_mvr_portEdit_set},
    #endif

    {NULL, NULL}
};

static int32 cgi_mcast_gen_prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    igmp_unknMcastAction_t action = IGMP_UNKNOWN_MCAST_ACTION_FLOOD;
    igmp_lookup_mode_t fwdMode;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_UNKNOWN_ACT, value))
    {
        if (0 == osal_strcmp (value, CGI_MCAST_UNKNOWN_ACT_DROP))
            action = IGMP_UNKNOWN_MCAST_ACTION_DROP;

        if (0 == osal_strcmp (value, CGI_MCAST_UNKNOWN_ACT_ROUTERPORT))
            action = IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT;

        CGI_SET_ERR_HDL(sal_igmp_unknMcastAction_set(action), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_FWDMETHOD_IPV4, value))
    {
        if (0 == osal_strcmp (value, CGI_MCAST_FWDMETHOD_DMAC))
            fwdMode = IGMP_DMAC_VID_MODE;
        else
            fwdMode = IGMP_DIP_VID_MODE;

        CGI_SET_ERR_HDL(sal_igmp_lookupMode_set(fwdMode), ret);
    }

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_FWDMETHOD_IPV6, value))
    {
        if (0 == osal_strcmp (value, CGI_MCAST_FWDMETHOD_DMAC))
            fwdMode = IGMP_DMAC_VID_MODE;
        else
            fwdMode = IGMP_DIP_VID_MODE;

        CGI_SET_ERR_HDL(sal_mld_lookupMode_set(fwdMode), ret);
    }
    #endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_groupAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    igmp_group_entry_t groupEntry;
    #ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t enable;
    sys_vid_t mvrVlan;
    uint32 groupStart, groupEnd;
    #endif

    SYS_MEM_CLEAR(groupEntry);

    groupEntry.ipType = MULTICAST_TYPE_IPV4;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_IPTYPE_V4))
            groupEntry.ipType = MULTICAST_TYPE_IPV4;
        else
            groupEntry.ipType = MULTICAST_TYPE_IPV6;
    }
    #endif

    if (MULTICAST_TYPE_IPV4 == groupEntry.ipType)
    {
        if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUPADD_ADDR, value))
        {
            sys_util_str2Ip(value, &groupEntry.dip);
            if ((ntohl(groupEntry.dip) >= ntohl(0xE0000000L)) && (ntohl(groupEntry.dip) <= ntohl(0xE00000FFL)))
                CGI_SET_ERR_HDL(SYS_ERR_IGMP_RSV_ADDR, ret);

            #ifdef CONFIG_SYS_PROTO_MVR
            CGI_SET_ERR_HDL(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd), ret);
            if (groupEntry.dip >= groupStart  &&  groupEntry.dip <= groupEnd)
                CGI_SET_ERR_HDL(SYS_ERR_MVR_IGMP_STATIC_GROUP_SET, ret);
            #endif
        }
        else
            goto cgi_end;
    }
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUPADD_ADDRV6, value))
        {
            inet_pton(AF_INET6, value, groupEntry.dipv6.addr);
        }
        else
            goto cgi_end;
    }
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLAN, value))
    {
        groupEntry.vid = SYS_STR2UINT(value);
        #ifdef CONFIG_SYS_PROTO_MVR
        if (MULTICAST_TYPE_IPV4 == groupEntry.ipType)
        {
            CGI_SET_ERR_HDL(sal_igmp_mvr_enable_get(&enable),ret);
            CGI_SET_ERR_HDL(sal_igmp_mvr_vlanId_get(&mvrVlan), ret);
            if ((ENABLED == enable) && (mvrVlan == groupEntry.vid))
                CGI_SET_ERR_HDL(SYS_ERR_STATIC_GROUP_SET, ret);
        }
        #endif
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUP_MBR, value))
    {
        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_GROUP_MBR, groupEntry.mbr);
    }
    else
       CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

    if (IS_LOGIC_PORTMASK_CLEAR(groupEntry.mbr))
        CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

    CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, TRUE), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_groupEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    igmp_group_entry_t groupEntry;
    sys_igmp_db_t mcastDb;
    sys_logic_portmask_t addPmsk, rmPmsk, tmpPmsk;

    SYS_MEM_CLEAR(addPmsk);
    SYS_MEM_CLEAR(rmPmsk);
    SYS_MEM_CLEAR(tmpPmsk);

    SYS_MEM_CLEAR(groupEntry);
    SYS_MEM_CLEAR(mcastDb);

   if (CGI_IS_PARAM_EXIST(CGI_MCAST_IDX, value))
   {
        mcastDb.indx = SYS_STR2UINT(value);
        CGI_SET_ERR_HDL(sal_mcast_database_get(&mcastDb), ret);
        if (IS_VID_INVALID(mcastDb.vid))
            CGI_SET_ERR_HDL(SYS_ERR_ENTRY_NOT_FOUND, ret);

        groupEntry.vid = mcastDb.vid;
        groupEntry.ipType = mcastDb.ipType;
        groupEntry.dip = mcastDb.dip;
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        if (MULTICAST_TYPE_IPV4 != groupEntry.ipType)
            osal_memcpy(groupEntry.dipv6.addr, mcastDb.dipv6, IPV6_ADDR_LEN);
        #endif

        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_GROUP_MBR, tmpPmsk);
        LOGIC_PORTMASK_ANDNOT(addPmsk, tmpPmsk, mcastDb.staticMbr);
        LOGIC_PORTMASK_ANDNOT(rmPmsk, mcastDb.staticMbr, tmpPmsk);

        if (!IS_LOGIC_PORTMASK_CLEAR(addPmsk))
        {
            LOGIC_PORTMASK_COPY(groupEntry.mbr, addPmsk);
            CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, TRUE), ret);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(rmPmsk))
        {
            LOGIC_PORTMASK_COPY(groupEntry.mbr, rmPmsk);
            CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, FALSE), ret);
        }

   }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;
    igmp_group_entry_t delGrpEntry;
    sys_igmp_db_t mcastDb;
    mcast_entry_t   *pdelGrp = NULL;
    uint32 dbSize = 0;
    uint32 delGrpCnt = 0;
    char desStr[SYS_BUF64_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];

    sal_mcast_databaseSize_get(&dbSize);
    if (0 == dbSize)
        return SYS_ERR_OK;

    pdelGrp = (mcast_entry_t *)osal_alloc(dbSize * sizeof(mcast_entry_t));
    if (NULL != pdelGrp)
    {
        if (CGI_GET_PARAM_ARRAY_DATA(CGI_MCAST_IDX, array, &len))
        {
            for (i = 0; i < len; i++)
            {
                SYS_MEM_CLEAR(mcastDb);
                mcastDb.indx = SYS_STR2UINT(array[i]);
                sal_mcast_database_get(&mcastDb);
                if (IS_VID_INVALID(mcastDb.vid))
                    continue;

                 pdelGrp[delGrpCnt].vid = mcastDb.vid;
                 pdelGrp[delGrpCnt].sip = mcastDb.sip;
                 pdelGrp[delGrpCnt].ipType = mcastDb.ipType;
                 pdelGrp[delGrpCnt].dip  = mcastDb.dip;
                 #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                 osal_memcpy(pdelGrp[delGrpCnt].dipv6, mcastDb.dipv6, IPV6_ADDR_LEN);
                 #endif
                 delGrpCnt++;
            }
         }

        for (i = 0; i < delGrpCnt; i++)
        {
            SYS_MEM_CLEAR(delGrpEntry);
            delGrpEntry.vid = pdelGrp[i].vid;
            delGrpEntry.sip = pdelGrp[i].sip;
            delGrpEntry.ipType = pdelGrp[i].ipType;
            delGrpEntry.dip = pdelGrp[i].dip;
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            if (MULTICAST_TYPE_IPV4 != delGrpEntry.ipType)
            {
                osal_memcpy(delGrpEntry.dipv6.addr, pdelGrp[i].dipv6, IPV6_ADDR_LEN);
            }
            #endif

            SYS_MEM_CLEAR(desStr);
            SYS_MEM_CLEAR(ipStr);
            if (MULTICAST_TYPE_IPV4 != delGrpEntry.ipType)
                sys_util_ip2Str(delGrpEntry.dip, ipStr);
            else
                inet_ntop(AF_INET6, delGrpEntry.dipv6.addr, ipStr, sizeof(ipStr));
            osal_sprintf(desStr, "vlan:%d, group address: %s\n", delGrpEntry.vid, ipStr);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_mcast_static_group_clear(&delGrpEntry), desStr);
        }

        osal_free(pdelGrp);
    }

    if (array)
        free(array);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
static int32 cgi_mcast_gen_throtEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    uint16 getMaxGrp,setMaxGrp;
    igmp_maxGroupAct_t getAct, setAct;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
    {
        STR2LPM(value, &lpm);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }

    if (CGI_IS_PARAM_EXIST (CGI_MCAST_MAXGRP_NUM, value))
    {
        setMaxGrp = SYS_STR2UINT(value);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_igmp_portGroup_limit_get(lp, &getMaxGrp), lp);
                if (setMaxGrp != getMaxGrp)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_igmp_portGroup_limit_set(lp, setMaxGrp), lp);
                }
            }
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_mld_portGroup_limit_get(lp, &getMaxGrp), lp);
                if (setMaxGrp != getMaxGrp)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_mld_portGroup_limit_set(lp, setMaxGrp), lp);
                }
            }
            #endif
        }
    }

    if (CGI_IS_PARAM_EXIST (CGI_MCAST_MAXGRP_ACT, value))
    {
        if (0 == osal_strcmp(value, "Deny"))
            setAct = IGMP_ACT_DENY;
        else
            setAct = IGMP_ACT_REPLACE;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_igmp_portGroup_excessAct_get(lp, &getAct), lp);
                if (setAct != getAct)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_igmp_portGroup_excessAct_set(lp, setAct), lp);
                }
            }
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_mld_portGroup_excessAct_get(lp, &getAct), lp);
                if (setAct != getAct)
                {
                    CGI_SET_PORT_ERR_CONTINUE(sal_mld_portGroup_excessAct_set(lp, setAct), lp);
                }
            }
            #endif
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
static int32 cgi_mcast_gen_routerAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    sys_vlanmask_t vlanmask;
    sys_logic_portmask_t lpm;
    sys_vid_t vid;
    uint8 portType = 0;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_IPTYPE_V4))
            ipType = MULTICAST_TYPE_IPV4;
        else
            ipType = MULTICAST_TYPE_IPV6;
    }
    #endif

    SYS_MEM_CLEAR(vlanmask);
    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &vlanmask);
    else
        goto cgi_end;

    if (IS_VLANMASK_CLEAR(vlanmask))
        CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_VLAN_SEL, ret);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROUTER_PORTTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_PORTTYPE_FB))
            portType = 1;

        if (1 == portType)
        {
            CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR, lpm);
            if (!IS_LOGIC_PORTMASK_CLEAR(lpm))
            {
                FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
                {
                    CGI_SET_VID_ERR_CONTINUE(sal_mcast_router_forbid_port_set(vid, ipType, lpm), vid);
                }
            }
            else
                CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);
        }
        else
        {
            CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR, lpm);
            if (!IS_LOGIC_PORTMASK_CLEAR(lpm))
            {
                FOR_EACH_VID_IN_VLANMASK(vid,vlanmask)
                {
                    CGI_SET_VID_ERR_CONTINUE(sal_mcast_router_static_port_set(vid, ipType, lpm), vid);
                }
            }
            else
                CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

        }
    }
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_routerEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    igmp_router_entry_t  routerEntry;
    sys_logic_portmask_t staticPmsk, forbidPmsk, lpm, tmp;
    sys_vid_t  vlanId;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    uint8 portType = 0;

    SYS_MEM_CLEAR(routerEntry);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLAN, value))
    {
        vlanId = SYS_STR2UINT(value);
        routerEntry.vid = vlanId;
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
        {
            ipType = SYS_STR2UINT(value);
        }
        #endif
        routerEntry.ipType = ipType;
        CGI_SET_ERR_HDL(sal_mcast_router_get(&routerEntry), ret);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROUTER_PORTTYPE, value))
        {
            if (0 == osal_strcmp(value, CGI_MCAST_PORTTYPE_FB))
                portType = 1;

            SYS_MEM_CLEAR(lpm);
            if (1 == portType)
            {
                SYS_MEM_CLEAR(forbidPmsk);
                CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR_FB, lpm);
                LOGIC_PORTMASK_ANDNOT(tmp,routerEntry.router_forbid_pmsk,lpm);
                LOGIC_PORTMASK_OR(forbidPmsk,routerEntry.router_forbid_pmsk,lpm);
                LOGIC_PORTMASK_ANDNOT(forbidPmsk,forbidPmsk,tmp);
                CGI_SET_ERR_HDL(sal_mcast_router_forbid_port_set(vlanId, ipType, forbidPmsk), ret);
            }
            else
            {
                SYS_MEM_CLEAR(staticPmsk);
                CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR_ST, lpm);
                LOGIC_PORTMASK_ANDNOT(tmp,routerEntry.router_static_pmsk,lpm);
                LOGIC_PORTMASK_OR(staticPmsk,routerEntry.router_static_pmsk,lpm);
                LOGIC_PORTMASK_ANDNOT(staticPmsk,staticPmsk,tmp);
                CGI_SET_ERR_HDL(sal_mcast_router_static_port_set(vlanId, ipType, staticPmsk), ret);
            }
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
static int32 cgi_mcast_gen_fwdAllAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    sys_vlanmask_t vlanmask;
    sys_vlanmask_t vmExist;
    sys_logic_portmask_t lpm;
    sys_vid_t vid;
    uint8 portType = 0;
    mcast_vlan_entry_t mcastVlan;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_IPTYPE_V4))
            ipType = MULTICAST_TYPE_IPV4;
        else
            ipType = MULTICAST_TYPE_IPV6;
    }
    #endif

    SYS_MEM_CLEAR(vlanmask);
    SYS_MEM_CLEAR(lpm);

    CGI_SET_ERR_HDL(sal_mcast_vlanMskConfig_get(ipType,&vmExist), ret);

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &vlanmask);
    else
        goto cgi_end;

    if (IS_VLANMASK_CLEAR(vlanmask))
        CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_VLAN_SEL, ret);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_FWDALL_PORTTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_PORTTYPE_FB))
            portType = 1;

        if (0 == portType)
        {
            CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR, lpm);
            if (IS_LOGIC_PORTMASK_CLEAR(lpm))
                CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

            FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
            {
                SYS_MEM_CLEAR(mcastVlan);
                mcastVlan.vid = vid;
                mcastVlan.ipType = ipType;
                CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan), vid);
                if (!VLANMASK_IS_VIDSET(vmExist, vid))
                {
                    CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_add(&mcastVlan), vid);
                }
                LOGIC_PORTMASK_OR(mcastVlan.staticMbr, mcastVlan.staticMbr, lpm);
                CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_set(&mcastVlan), vid);
            }
        }
        else
        {
            CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR, lpm);
            if (IS_LOGIC_PORTMASK_CLEAR(lpm))
                CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

            FOR_EACH_VID_IN_VLANMASK(vid,vlanmask)
            {
                SYS_MEM_CLEAR(mcastVlan);
                mcastVlan.vid = vid;
                mcastVlan.ipType = ipType;
                CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan), vid);
                if (!VLANMASK_IS_VIDSET(vmExist, vid))
                {
                    CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_add(&mcastVlan), vid);
                }
                LOGIC_PORTMASK_OR(mcastVlan.forbidMbr, mcastVlan.forbidMbr, lpm);
                CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_set(&mcastVlan), vid);
            }
        }
    }
    else
        goto cgi_end;

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_fwdAllEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    mcast_vlan_entry_t  mcastVlan;
    sys_logic_portmask_t lpm, tmp;
    sys_vid_t  vlanId;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    uint8 portType = 0;

    SYS_MEM_CLEAR(mcastVlan);
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLAN, value))
    {
        vlanId = SYS_STR2UINT(value);
        mcastVlan.vid = vlanId;
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
        {
            ipType = SYS_STR2UINT(value);
        }
        #endif
        mcastVlan.ipType = ipType;
        CGI_SET_ERR_HDL(sal_mcast_vlan_get(&mcastVlan), ret);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_FWDALL_PORTTYPE, value))
        {
            if (0 == osal_strcmp(value, CGI_MCAST_PORTTYPE_FB))
                portType = 1;

            SYS_MEM_CLEAR(lpm);
            if (1 == portType)
            {
                CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR_FB, lpm);
                LOGIC_PORTMASK_ANDNOT(tmp,mcastVlan.forbidMbr,lpm);
                LOGIC_PORTMASK_OR(mcastVlan.forbidMbr,mcastVlan.forbidMbr,lpm);
                LOGIC_PORTMASK_ANDNOT(mcastVlan.forbidMbr,mcastVlan.forbidMbr,tmp);
                LOGIC_PORTMASK_ANDNOT(mcastVlan.staticMbr, mcastVlan.staticMbr, lpm);
            }
            else
            {
                CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_MBR_ST, lpm);
                LOGIC_PORTMASK_ANDNOT(tmp,mcastVlan.staticMbr,lpm);
                LOGIC_PORTMASK_OR(mcastVlan.staticMbr,mcastVlan.staticMbr,lpm);
                LOGIC_PORTMASK_ANDNOT(mcastVlan.staticMbr,mcastVlan.staticMbr,tmp);
                LOGIC_PORTMASK_ANDNOT(mcastVlan.forbidMbr, mcastVlan.forbidMbr, lpm);
            }

            CGI_SET_ERR_HDL(sal_mcast_vlan_set(&mcastVlan), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_fwdAllDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;

    uint32 i, len=0;
    mcast_vlan_entry_t  mcastVlan;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    char *value = NULL;
    if (CGI_IS_POSTURL_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_MCAST_VLAN, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(mcastVlan);
            mcastVlan.vid = SYS_STR2UINT(array[i]);
            mcastVlan.ipType = ipType;
            CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan),mcastVlan.vid);
            LOGIC_PORTMASK_CLEAR_ALL(mcastVlan.staticMbr);
            LOGIC_PORTMASK_CLEAR_ALL(mcastVlan.forbidMbr);
            CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_set(&mcastVlan), mcastVlan.vid);
        }
     }

    if (array)
        free(array);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 cgi_mcast_gen_profileAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_igmp_profile_t profileEntry, tmpPrf;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    SYS_MEM_CLEAR(profileEntry);

    profileEntry.valid = 1;
    profileEntry.profileType = MCAST_PROFILE_TYPE_FILTER;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_IPTYPE_V4))
            ipType = MULTICAST_TYPE_IPV4;
        else
            ipType = MULTICAST_TYPE_IPV6;
    }
    #endif

    profileEntry.ipType  = ipType;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_IDX, value))
    {
        profileEntry.profileId = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ACT, value))
        {
            if (0== osal_strcmp(value, "Allow"))
                profileEntry.act = MCAST_PROFILE_ACT_PERMIT;
            else
                profileEntry.act = MCAST_PROFILE_ACT_DENY;
        }
        else
            goto cgi_end;

        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (SYS_ERR_OK == sal_igmp_profile_get(profileEntry.profileId, &tmpPrf))
                CGI_SET_ERR_HDL(SYS_ERR_ENTRY_IS_EXIST , ret);

          if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_STARTADDR_V4, value))
                sys_util_str2Ip(value, &profileEntry.lowIp);

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ENDADDR_V4, value))
                sys_util_str2Ip(value, &profileEntry.highIp);

            if (profileEntry.lowIp > profileEntry.highIp)
                CGI_SET_ERR_HDL(SYS_ERR_RANGE , ret);

            CGI_SET_ERR_HDL(sal_igmp_profile_set(profileEntry.profileId, &profileEntry), ret);
        }
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
            if (SYS_ERR_OK == sal_mld_profile_get(profileEntry.profileId, &tmpPrf))
                CGI_SET_ERR_HDL(SYS_ERR_ENTRY_IS_EXIST , ret);

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_STARTADDR_V6, value))
            {
               if (1 != inet_pton(AF_INET6, value, profileEntry.ipv6Low))
                    CGI_SET_ERR_HDL(SYS_ERR_IPV6_ADDR , ret);
            }

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ENDADDR_V6, value))
            {
               if (1 != inet_pton(AF_INET6, value, profileEntry.ipv6High))
                    CGI_SET_ERR_HDL(SYS_ERR_IPV6_ADDR , ret);
            }

            if (osal_memcmp(profileEntry.ipv6High, profileEntry.ipv6Low, CAPA_IPV6_BYTE_NUM) < 0)
                CGI_SET_ERR_HDL(SYS_ERR_RANGE , ret);

            CGI_SET_ERR_HDL(sal_mld_profile_set(profileEntry.profileId, &profileEntry), ret);
        }
        #endif
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_profileEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_port_t port;
    sys_logic_portmask_t lpmsk;
    sys_igmp_profile_t profileEntry;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    sys_igmp_profileId_t  tmpIdx, setIdx;
    mcast_profileAct_t Act = MCAST_PROFILE_ACT_PERMIT;

    SYS_MEM_CLEAR(profileEntry);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_IDX, value))
    {
        setIdx = SYS_STR2UINT(value);

        SYS_MEM_CLEAR(lpmsk);
        FOR_EACH_LOGIC_PORT(port)
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                sal_igmp_port_filter_get(port, &tmpIdx);
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                sal_mld_port_filter_get(port, &tmpIdx);
            #endif

            if (tmpIdx == setIdx)
            {
                LOGIC_PORTMASK_SET_PORT(lpmsk, port);
            }
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(lpmsk))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                CGI_SET_ERR_HDL(SYS_ERR_IGMP_PROFILE_USED, ret);
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                CGI_SET_ERR_HDL(SYS_ERR_MLD_PROFILE_USED, ret);
            #endif
        }

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ACT, value))
        {
            if (0== osal_strcmp(value, "Allow"))
                Act = MCAST_PROFILE_ACT_PERMIT;
             else
                Act = MCAST_PROFILE_ACT_DENY;
        }

        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            CGI_SET_ERR_HDL(sal_igmp_profile_get(setIdx, &profileEntry), ret);

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_STARTADDR_V4, value))
                sys_util_str2Ip(value, &profileEntry.lowIp);

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ENDADDR_V4, value))
                sys_util_str2Ip(value, &profileEntry.highIp);

            if (profileEntry.lowIp > profileEntry.highIp)
                CGI_SET_ERR_HDL(SYS_ERR_RANGE , ret);

            profileEntry.act = Act;
            CGI_SET_ERR_HDL(sal_igmp_profile_set(profileEntry.profileId, &profileEntry), ret);
        }
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
            CGI_SET_ERR_HDL(sal_mld_profile_get(setIdx, &profileEntry), ret);

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_STARTADDR_V6, value))
            {
               if (1 != inet_pton(AF_INET6, value, profileEntry.ipv6Low))
                    CGI_SET_ERR_HDL(SYS_ERR_IPV6_ADDR , ret);
            }

            if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_ENDADDR_V6, value))
            {
               if (1 != inet_pton(AF_INET6, value, profileEntry.ipv6High))
                    CGI_SET_ERR_HDL(SYS_ERR_IPV6_ADDR , ret);
            }

            if (osal_memcmp(profileEntry.ipv6High, profileEntry.ipv6Low, CAPA_IPV6_BYTE_NUM) < 0)
                CGI_SET_ERR_HDL(SYS_ERR_RANGE , ret);

            profileEntry.act = Act;

            CGI_SET_ERR_HDL(sal_mld_profile_set(profileEntry.profileId, &profileEntry), ret);
          }
         #endif
     }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_profileDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    char *value = NULL;
    #endif
    char **array = NULL;
    uint32 i, len = 0;

    sys_igmp_profile_t profileEntry;
    sys_logic_port_t port;
    sys_logic_portmask_t lpmsk;
    sys_igmp_profileId_t tmpIdx, setIdx;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    char proIdxStr[SYS_BUF16_LEN];

    SYS_MEM_CLEAR(profileEntry);

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_POSTURL_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
    {
        ipType = SYS_STR2UINT(value);
    }
    #endif

    if (CGI_GET_PARAM_ARRAY_DATA(CGI_MCAST_PROFILE_IDX, array, &len))
    {
        for (i = 0; i < len; i++)
        {
            setIdx =  SYS_STR2UINT(array[i]);

            SYS_MEM_CLEAR(lpmsk);
            FOR_EACH_LOGIC_PORT(port)
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                    sal_igmp_port_filter_get(port, &tmpIdx);
                #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    sal_mld_port_filter_get(port, &tmpIdx);
                #endif

                if (tmpIdx == setIdx)
                {
                    LOGIC_PORTMASK_SET_PORT(lpmsk, port);
                }
            }

            SYS_MEM_CLEAR(proIdxStr);
            osal_sprintf(proIdxStr,"Profile Idx %d ", setIdx);
            if (!IS_LOGIC_PORTMASK_CLEAR(lpmsk))
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    CGI_SET_ENTRY_ERR_CONTINUE(SYS_ERR_IGMP_PROFILE_USED, proIdxStr);
                }
                #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                {
                    CGI_SET_ENTRY_ERR_CONTINUE(SYS_ERR_MLD_PROFILE_USED, proIdxStr);
                }
                #endif
            }

            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                CGI_SET_ENTRY_ERR_CONTINUE(sal_igmp_profile_set(setIdx, &profileEntry), proIdxStr);
            }
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                CGI_SET_ENTRY_ERR_CONTINUE(sal_mld_profile_set(setIdx, &profileEntry), proIdxStr);
            }
            #endif
        }
     }

    if (array)
        free(array);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_gen_filterEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    sys_igmp_profileId_t setIdx = INVALID_PROFILE_ID, getIdx;

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_IPTYPE, value))
        ipType = SYS_STR2UINT(value);
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
    {
        STR2LPM(value, &lpm);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_PROFILE_IDX, value))
    {
        setIdx = SYS_STR2UINT(value);
    }

    if (!CGI_IS_PARAM_EXIST(CGI_MCAST_FILTER, value))
    {
        setIdx = INVALID_PROFILE_ID;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_igmp_port_filter_get(lp, &getIdx), lp);
            if (setIdx != getIdx)
                CGI_SET_PORT_ERR_CONTINUE(sal_igmp_port_filter_set(lp, setIdx), lp);
        }
    }
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_mld_port_filter_get(lp, &getIdx), lp);
            if (setIdx != getIdx)
                CGI_SET_PORT_ERR_CONTINUE(sal_mld_port_filter_set(lp, setIdx), lp);
        }
    }
   #endif

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_mcast_igmp_prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 operVersion;

   if (CGI_IS_PARAM_EXIST(CGI_MCAST_STATE, value))
        CGI_SET_ERR_HDL(sal_igmp_enable_set(ENABLED), ret);
   else
        CGI_SET_ERR_HDL(sal_igmp_enable_set(DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VER, value))
    {
        operVersion = SYS_STR2UINT(value);
        if (3 == operVersion)
            operVersion = IGMP_VERSION_V3_BASIC;
        else
            operVersion = IGMP_VERSION_V2;
        CGI_SET_ERR_HDL(sal_igmp_operVersion_set(operVersion),ret);
    }

   if (CGI_IS_PARAM_EXIST(CGI_MCAST_REPORTSUP, value))
        CGI_SET_ERR_HDL(sal_igmp_suppre_enable_set(ENABLED), ret);
   else
        CGI_SET_ERR_HDL(sal_igmp_suppre_enable_set(DISABLED), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_igmp_propEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_vid_t vid;
    sys_vlanmask_t vlanMask;
    mcast_vlan_entry_t vlanEntry;

    SYS_MEM_CLEAR(vlanMask);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLANLIST, value))
        STR2VMSK(value, &vlanMask);
    else
        goto cgi_end;

    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = MULTICAST_TYPE_IPV4;
        CGI_SET_ERR_HDL(sal_mcast_vlan_get(&vlanEntry), ret);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ADMIN_STS, value))
            vlanEntry.enable = ENABLED;
        else
            vlanEntry.enable = DISABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROUTER_AUTOLEARN, value))
            vlanEntry.pimDvmrpLearn = ENABLED;
        else
            vlanEntry.pimDvmrpLearn = DISABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROBUST, value))
            vlanEntry.robustnessVar = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_INTVAL, value))
            vlanEntry.queryIntv = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_MAXRESPONSE, value))
            vlanEntry.responseTime = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_LASTMBRCNT, value))
            vlanEntry.lastMmbrQueryCnt = SYS_STR2UINT(value);

         if (CGI_IS_PARAM_EXIST(CGI_MCAST_LASTMBRINTVAL, value))
            vlanEntry.grpSpecificQueryIntv = SYS_STR2UINT(value);

         if (CGI_IS_PARAM_EXIST(CGI_MCAST_FASTLEAVE, value))
            vlanEntry.fastleave_enable = ENABLED;
         else
            vlanEntry.fastleave_enable = DISABLED;

         CGI_SET_ERR_HDL(sal_mcast_vlan_set(&vlanEntry), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
static int32 cgi_mcast_igmp_querierEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_vid_t vid;
    sys_vlanmask_t vlanMask;
    sys_igmp_querier_t  querierEntry;
    uint8 qerVer = IGMP_QUERY_V2;
    sys_enable_t queStats = DISABLED;
    sys_mcast_info_t info;

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLANLIST, value))
        STR2VMSK(value, &vlanMask);
    else
        goto cgi_end;

    SYS_MEM_CLEAR(info);
    #ifdef CONFIG_SYS_PROTO_IGMPV3_BASIC
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_QUERIER_VER, value))
        qerVer = (uint8)SYS_STR2UINT(value);

    sal_igmp_info_get(&info);
    if (info.operVersion < IGMP_VERSION_V3_BASIC && qerVer == IGMP_QUERY_V3)
        CGI_SET_ERR_HDL(SYS_ERR_IGMP_QUERIER_VERSION, ret);
    #endif

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_QUERYSTATE, value))
        queStats = ENABLED;
    else
        queStats = DISABLED;

    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(querierEntry);
        querierEntry.vid = vid;
        querierEntry.ipType = MULTICAST_TYPE_IPV4;

        CGI_SET_VID_ERR_CONTINUE(sal_igmp_querier_get(&querierEntry), vid);
        if (ENABLED == queStats)
        {
            if (qerVer != querierEntry.version || queStats != querierEntry.enabled)
            {
                querierEntry.enabled = ENABLED;
                querierEntry.version = qerVer;
                CGI_SET_VID_ERR_CONTINUE(sal_igmp_querier_set(&querierEntry), vid);
            }
        }
        else
        {
            if (queStats != querierEntry.enabled)
            {
                querierEntry.enabled = DISABLED;
                CGI_SET_VID_ERR_CONTINUE(sal_igmp_querier_set(&querierEntry), vid);
            }
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

static int32 cgi_mcast_igmp_statClear_set(void)
{
    int32 ret = SYS_ERR_OK;

    CGI_SET_ERR_HDL(sal_igmp_statistics_clear(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static int32 cgi_mcast_mld_prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    uint32 operVersion;

   if (CGI_IS_PARAM_EXIST(CGI_MCAST_STATE, value))
        CGI_SET_ERR_HDL(sal_mld_enable_set(ENABLED), ret);
   else
        CGI_SET_ERR_HDL(sal_mld_enable_set(DISABLED), ret);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VER, value))
    {
        operVersion = SYS_STR2UINT(value);
        if (2 == operVersion)
            operVersion = MLD_VERSION_V2_BASIC;
        else
            operVersion = MLD_VERSION_V1;
        CGI_SET_ERR_HDL(sal_mld_operVersion_set(operVersion),ret);
    }

   if (CGI_IS_PARAM_EXIST(CGI_MCAST_REPORTSUP, value))
        CGI_SET_ERR_HDL(sal_mld_suppre_enable_set(ENABLED), ret);
   else
        CGI_SET_ERR_HDL(sal_mld_suppre_enable_set(DISABLED), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mld_propEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_vid_t vid;
    sys_vlanmask_t vlanMask;
    mcast_vlan_entry_t vlanEntry;

    SYS_MEM_CLEAR(vlanMask);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_VLANLIST, value))
        STR2VMSK(value, &vlanMask);
    else
        goto cgi_end;

    FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
    {
        SYS_MEM_CLEAR(vlanEntry);
        vlanEntry.vid = vid;
        vlanEntry.ipType = MULTICAST_TYPE_IPV6;
        CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_get(&vlanEntry), vid);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ADMIN_STS, value))
            vlanEntry.enable = ENABLED;
        else
            vlanEntry.enable = DISABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROUTER_AUTOLEARN, value))
            vlanEntry.pimDvmrpLearn = ENABLED;
        else
            vlanEntry.pimDvmrpLearn = DISABLED;

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_ROBUST, value))
            vlanEntry.robustnessVar = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_INTVAL, value))
            vlanEntry.queryIntv = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_MAXRESPONSE, value))
            vlanEntry.responseTime = SYS_STR2UINT(value);

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_LASTMBRCNT, value))
            vlanEntry.lastMmbrQueryCnt = SYS_STR2UINT(value);

         if (CGI_IS_PARAM_EXIST(CGI_MCAST_LASTMBRINTVAL, value))
            vlanEntry.grpSpecificQueryIntv = SYS_STR2UINT(value);

         if (CGI_IS_PARAM_EXIST(CGI_MCAST_FASTLEAVE, value))
            vlanEntry.fastleave_enable = ENABLED;
         else
            vlanEntry.fastleave_enable = DISABLED;

         CGI_SET_VID_ERR_CONTINUE(sal_mcast_vlan_set(&vlanEntry), vid);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mld_statClear_set(void)
{
    int32 ret = SYS_ERR_OK;

    CGI_SET_ERR_HDL(sal_mld_statistics_clear(), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR
static int32 cgi_mcast_mvr_prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_enable_t     mvrEnable = DISABLED, mvrOldEn;
    uint32    mvrVlan, mvrOldVlan;
    uint32    mvrGrpStart = 0,mvrGrpEnd = 0, mvrOldGrpStart, mvrOldGrpEnd;
    uint32    grpNum;
    uint32    mvrQryTime, mvrOldQryTime;
    mvr_mode_t mvrMode, mvrOldMode;

    sal_igmp_mvr_enable_get(&mvrOldEn);
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_STATE, value))
        mvrEnable = ENABLED;

    if (mvrEnable != mvrOldEn)
    {
        CGI_SET_ERR_HDL(sal_igmp_mvr_enable_set(mvrEnable), ret);
        if (DISABLED == mvrEnable)
            goto cgi_end;
    }

    sal_igmp_mvr_vlanId_get(&mvrOldVlan);
    sal_igmp_mvr_groupRange_get(&mvrOldGrpStart, &mvrOldGrpEnd);
    sal_igmp_mvr_querytime_get(&mvrOldQryTime);
    sal_igmp_mvr_mode_get(&mvrOldMode);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_MVR_VID, value))
    {
        mvrVlan = SYS_STR2UINT(value);
        if (mvrVlan != mvrOldVlan)
        {
            CGI_SET_ERR_HDL(sal_igmp_mvr_vlanId_set(mvrVlan), ret);
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_MVR_MODE, value))
    {
        if (0 == osal_strcmp(value, CGI_MCAST_MVR_MODE_COMP))
            mvrMode = MVR_MODE_COMPATABLE;
        else
            mvrMode = MVR_MODE_DYNAMIC;
        if (mvrMode != mvrOldMode)
        {
            CGI_SET_ERR_HDL(sal_igmp_mvr_mode_set(mvrMode), ret);
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_MVR_GRPSTART, value))
    {
        SYS_ERR_CHK(sys_util_str2Ip(value, &mvrGrpStart));
        if ((ntohl(mvrGrpStart) >= ntohl(0xE0000000L)) && (ntohl(mvrGrpStart) <= ntohl(0xE00000FFL)))
        {
            CGI_SET_ERR_HDL(SYS_ERR_IGMP_RSV_ADDR, ret);
        }

        if (CGI_IS_PARAM_EXIST(CGI_MCAST_MVR_SEQNUM, value))
        {
            grpNum = SYS_STR2UINT(value);
            mvrGrpEnd = mvrGrpStart + grpNum - 1;
        }

        if ((mvrGrpStart != mvrOldGrpStart) || (mvrGrpEnd != mvrOldGrpEnd))
        {
            CGI_SET_ERR_HDL(sal_igmp_mvr_groupRange_set(mvrGrpStart,mvrGrpEnd), ret);
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_MVR_QRYTIME, value))
    {
        mvrQryTime = SYS_STR2UINT(value);
        if (mvrQryTime != mvrOldQryTime)
        {
            CGI_SET_ERR_HDL(sal_igmp_mvr_querytime_set(mvrQryTime), ret);
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
static int32 cgi_mcast_mvr_groupAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_vid_t mvrVid;
    igmp_group_entry_t groupEntry;
    sys_enable_t mvrEnable;
    uint32 grpStart,grpEnd;
    uint32 mvrGrp;

    sys_igmp_db_t db;
    uint32 i, dbSize = 0;

    SYS_MEM_CLEAR(groupEntry);

    sal_igmp_mvr_enable_get(&mvrEnable);
    if (DISABLED == mvrEnable)
    {
        CGI_SET_ERR_HDL(SYS_ERR_MVR_ENABLE, ret);
    }

    sal_igmp_mvr_vlanId_get(&mvrVid);
    sal_igmp_mvr_groupRange_get(&grpStart, &grpEnd);

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUPADD_ADDR, value))
    {
        CGI_SET_ERR_HDL(sys_util_str2Ip(value, &mvrGrp), ret);
        if (mvrGrp < grpStart || mvrGrp > grpEnd)
            CGI_SET_ERR_HDL(SYS_ERR_MVR_STATIC_GROUP_CONFLICT, ret);
    }
    else
        goto cgi_end;

    sal_mcast_databaseSize_get(&dbSize);
    for (i = 0; i < dbSize; i++)
    {
        SYS_MEM_CLEAR(db);
        db.indx = i;
        sal_mcast_database_get(&db);

        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        if ((IGMP_GROUP_ENTRY_STATIC == db.form) && (db.dip == mvrGrp) && (mvrVid == db.vid))
            CGI_SET_ERR_HDL(SYS_ERR_IGMP_STATIC_ENTRY_EXIST, ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUP_MBR, value))
    {
        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_GROUP_MBR, groupEntry.mbr);
    }
    else
        CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

    if (IS_LOGIC_PORTMASK_CLEAR(groupEntry.mbr))
        CGI_SET_ERR_HDL(CGI_ERR_MCAST_NO_PORT_SEL, ret);

    groupEntry.dip = mvrGrp;
    groupEntry.ipType = MULTICAST_TYPE_IPV4;
    groupEntry.vid = mvrVid;
    CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, TRUE), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
static int32 cgi_mcast_mvr_groupEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_igmp_db_t  db;
    uint32 mvrGrp = 0;

    sys_logic_portmask_t inMbr;
    sys_logic_portmask_t delMbr, addMbr;
    uint8 isAdd;
    igmp_group_entry_t  groupEntry;
    sys_vid_t mvrVlan;
    uint32 dbsize= 0, i;

    LOGIC_PORTMASK_CLEAR_ALL(inMbr);
    SYS_MEM_CLEAR(groupEntry);

    sal_igmp_mvr_vlanId_get(&mvrVlan);
    if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUPADD_ADDR, value))
    {
        CGI_SET_ERR_HDL(sys_util_str2Ip(value, &mvrGrp), ret);
    }
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_mcast_databaseSize_get(&dbsize), ret);
    for(i = 0; i < dbsize; i++)
    {
        SYS_MEM_CLEAR(db);
        db.indx = i;
        sal_mcast_database_get(&db);
        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        if (db.vid != mvrVlan)
            continue;

        if((db.dip == mvrGrp) && (IGMP_GROUP_ENTRY_STATIC == db.form))
            break;
    }

    if(i == dbsize)
    {
        CGI_SET_ERR_HDL(SYS_ERR_IGMP_GROUP_ENTRY_NOT_EXIST, ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_MCAST_GROUP_MBR, value))
    {
        CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MCAST_GROUP_MBR, inMbr);
    }

    LOGIC_PORTMASK_ANDNOT(delMbr, db.staticMbr, inMbr);
    LOGIC_PORTMASK_ANDNOT(addMbr, inMbr, db.staticMbr);

    groupEntry.dip = mvrGrp;
    groupEntry.vid = mvrVlan;
    groupEntry.ipType = MULTICAST_TYPE_IPV4;
    LOGIC_PORTMASK_COPY(groupEntry.mbr, delMbr);
    isAdd = FALSE;
    CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, isAdd), ret);

    LOGIC_PORTMASK_COPY(groupEntry.mbr, addMbr);
    isAdd = TRUE;
    CGI_SET_ERR_HDL(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE,isAdd), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    char **array = NULL;
    uint32 i, len = 0;

    igmp_group_entry_t delGrpEntry;
    sys_igmp_db_t mcastDb;
    mcast_entry_t   *pdelGrp = NULL;
    uint32 dbSize = 0;
    uint32 delGrpCnt = 0;
    char desStr[SYS_BUF64_LEN];
    char ipStr[CAPA_IPV6_STR_LEN];

    sal_mcast_databaseSize_get(&dbSize);
    if (0 == dbSize)
    {
        cgi_set_respGeneral_send(ret);
        return SYS_ERR_OK;
    }

    pdelGrp = (mcast_entry_t *)osal_alloc(dbSize * sizeof(mcast_entry_t));
    if (NULL != pdelGrp)
    {
        if (CGI_GET_PARAM_ARRAY_DATA(CGI_MCAST_IDX, array, &len))
        {
            for (i = 0; i < len; i++)
            {
                SYS_MEM_CLEAR(mcastDb);
                mcastDb.indx = SYS_STR2UINT(array[i]);
                sal_mcast_database_get(&mcastDb);
                if (!sal_igmp_mvr_group_ret(&mcastDb))
                    continue;

                 pdelGrp[delGrpCnt].vid = mcastDb.vid;
                 pdelGrp[delGrpCnt].sip = mcastDb.sip;
                 pdelGrp[delGrpCnt].ipType = mcastDb.ipType;
                 pdelGrp[delGrpCnt].dip  = mcastDb.dip;
                 delGrpCnt++;
            }
         }

        for (i = 0; i < delGrpCnt; i++)
        {
            SYS_MEM_CLEAR(delGrpEntry);
            delGrpEntry.vid = pdelGrp[i].vid;
            delGrpEntry.sip = pdelGrp[i].sip;
            delGrpEntry.ipType = pdelGrp[i].ipType;
            delGrpEntry.dip = pdelGrp[i].dip;

            SYS_MEM_CLEAR(desStr);
            SYS_MEM_CLEAR(ipStr);
            sys_util_ip2Str(delGrpEntry.dip, ipStr);
            osal_sprintf(desStr, "vlan:%d, group address: %s\n", delGrpEntry.vid, ipStr);
            CGI_SET_ENTRY_ERR_CONTINUE(sal_mcast_static_group_clear(&delGrpEntry), desStr);
        }

        osal_free(pdelGrp);
    }

    if (array)
        free(array);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_mcast_mvr_portEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    mvr_portType_t  portType, oldType;
    sys_enable_t enable, oldEn;
    sys_enable_t mvrEnable;

    sal_igmp_mvr_enable_get(&mvrEnable);
    if (DISABLED == mvrEnable)
    {
        CGI_SET_ERR_HDL(SYS_ERR_MVR_ENABLE, ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
    {
        STR2LPM(value, &lpm);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST (CGI_MCAST_MVR_IMMDLEAVE, value))
    {
        enable = ENABLED;
    }
    else
        enable = DISABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_igmp_mvr_portImmediate_get(lp, &oldEn),lp);
        if (oldEn != enable)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_igmp_mvr_portImmediate_set(lp, enable), lp);
        }
    }

    if (CGI_IS_PARAM_EXIST (CGI_MCAST_MVR_PORTTYPE, value))
    {
        if (0 == osal_strcmp(value, "None"))
            portType = MVR_TYPE_NONE;
        else if (0 == osal_strcmp(value, "Receiver"))
            portType = MVR_TYPE_RECEIVER;
        else
            portType = MVR_TYPE_SOURCE;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_igmp_mvr_portType_get(lp, &oldType),lp);
            if (portType != oldType)
            {
                CGI_SET_PORT_ERR_CONTINUE(sal_igmp_mvr_portType_set(lp, portType), lp);
            }
        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
#endif

