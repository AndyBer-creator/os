
#include <libsal/sal_igmp.h>
#include <libcmd/cmd.h>

sys_igmp_profileId_t g_profileIdx = 1;

int32 cmd_igmp_static_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 groupAddr =0;
    uint32 sourceAddr =0;
    sys_vlanmask_t  vlanmask;
    igmp_group_entry_t groupEntry;
    sys_vid_t vid;

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sys_util_str2Ip(argv[1], &groupAddr));

    if (!IGMP_IN_MULTICAST(ntohl(groupAddr)))
    {
        return SYS_ERR_MCAST_ADDR;
    }

    if (argc == 3)
        SYS_ERR_CHK(sys_util_str2Ip(argv[2], &sourceAddr));

    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
        osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
        groupEntry.vid = vid;
        groupEntry.sip = sourceAddr;
        groupEntry.ipType = MULTICAST_TYPE_IPV4;
        groupEntry.dip = groupAddr;

        SYS_ERR_CONTINUE(sal_mcast_static_group_clear(&groupEntry));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mcast_group_type_t type = IGMP_GROUP_ENTRY_ALL;

    if (1 == argc)
    {
        switch(argv[0][0])
        {
            case 'd' :
                type = IGMP_GROUP_ENTRY_DYNAMIC;
                break;

            case 's':
                type = IGMP_GROUP_ENTRY_STATIC;
                break;

            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_igmp_groupByType_clear(type));

    return SYS_ERR_OK;

}

int32 cmd_igmp_statistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_igmp_statistics_clear());

    return SYS_ERR_OK;
}

int32 cmd_igmp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t        enable;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t unknMcastAction;
    int32 i;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));
    SYS_ERR_CHK(sal_igmp_info_get(&info));
    SYS_ERR_CHK(sal_igmp_enable_get(&enable));
    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&unknMcastAction));

    XMORE("\n\n\t\tIGMP Snooping Status\n");
    XMORE("\t\t--------------------\n\n");

    XMORE("    Snooping                        : %sabled\n", enable ? "En" : "Dis");
    XMORE("    Report Suppression              : %sabled\n", info.suppreEnable ? "En" : "Dis");
    XMORE("    Operation Version               : %s \n", (info.operVersion >= IGMP_VERSION_V3_BASIC) ? "v3" : "v2" );
    XMORE("    Forward Method                  : %s \n", gLookupMode[mode]);

    XMORE("    Unknown IP Multicast Action     : %s\n", gUnknownMcastAct[unknMcastAction]);

    vty_out_done();
    XMORE("\n\n\t\tPacket Statistics\n");
    for (i = 0; i < MCAST_STAT_END; i++)
    {
        XMORE ("    %-32s :  %d \n",  mcastStatCntName[i], info.totStat[i]);
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_router_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char port_str[CAPA_PORTMASK_STR_LEN];
    uint32 isShowDynamic = FALSE;
#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    uint32 isShowStatic = FALSE;
    uint32 isShowForbbiden = FALSE;
#endif
    uint32 cnt = 0;
    sys_vid_t vid;
    sys_vlanmask_t vlanMask;
    igmp_router_entry_t router;
    sys_logic_port_t port;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    VLANMASK_CLEAR_ALL(vlanMask);

    if(1 == argc)
    {
        if(('d' == argv[0][0]))
            isShowDynamic = TRUE;
#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        if(('s' == argv[0][0]))
            isShowStatic = TRUE;

        if(('f' == argv[0][0]))
            isShowForbbiden = TRUE;
#endif
    }
    else
    {
        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        isShowStatic = TRUE;
        isShowDynamic = TRUE;
        isShowForbbiden = TRUE;
        #else
        isShowDynamic = TRUE;
        #endif
    }

   FOR_EACH_VID(vid)
   {
        router.vid = vid;
        router.ipType = ipType;
        SYS_ERR_CONTINUE(sal_mcast_router_get(&router));

        if(0 != router.vid)
            VLANMASK_SET_VID(vlanMask, vid);
        else
            continue;
   }

    if(isShowDynamic)
    {
        XMORE("\nDynamic Router Table\n");
        XMORE(" VID  |  Port   | Expiry Time(Sec) \n");
        XMORE("------+---------+------------------\n");

        FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
        {
            router.vid = vid;
            router.ipType = ipType;
            SYS_ERR_CONTINUE(sal_mcast_router_get(&router));

            if (IS_LOGIC_PORTMASK_CLEAR(router.router_pmsk))
                continue;

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, router.router_pmsk)
            {
                if(IS_LOGIC_PORTMASK_PORTCLEAR(router.router_static_pmsk, port))
                {
                    osal_memset(port_str, 0, sizeof(port_str));
                    LP2STR(port, port_str);
                    XMORE(" %4d |", router.vid);
                    XMORE(" %7s |", port_str);
                    XMORE(" %3d\n", router.portTimer[port]);
                    cnt++;
                }

            }
        }
        XMORE("\n\nTotal Entry %u\n\n", cnt);
    }

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    if(isShowStatic)
    {
        cnt = 0;
        XMORE("\nStatic Router Table\n");
        XMORE(" VID  | Port Mask\n");
        XMORE("------+------------------------\n");

        FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
        {
            router.vid = vid;
            router.ipType = ipType;
            SYS_ERR_CONTINUE(sal_mcast_router_get(&router));

            if (IS_LOGIC_PORTMASK_CLEAR(router.router_static_pmsk))
                continue;

            osal_memset(port_str, 0, sizeof(port_str));
            LPM2STR(&router.router_static_pmsk, port_str);

            XMORE(" %4d |", router.vid);
            XMORE(" %s  \n", port_str);
            cnt++;

        }
        XMORE("\n\nTotal Entry %u\n\n", cnt);
    }

    if(isShowForbbiden)
    {
        cnt = 0;
        XMORE("\nForbidden Router Table\n");
        XMORE(" VID  | Port Mask\n");
        XMORE("------+------------------------\n");

        FOR_EACH_VID_IN_VLANMASK(vid, vlanMask)
        {
            router.vid = vid;
            router.ipType = ipType;
            SYS_ERR_CONTINUE(sal_mcast_router_get(&router));

            if (IS_LOGIC_PORTMASK_CLEAR(router.router_forbid_pmsk))
                continue;

            osal_memset(port_str, 0, sizeof(port_str));
            LPM2STR(&router.router_forbid_pmsk, port_str);

            XMORE(" %4d |", router.vid);
            XMORE(" %s  \n", port_str);
            cnt++;

        }
        XMORE("\n\nTotal Entry %u\n\n", cnt);
    }
    #endif

    return SYS_ERR_OK;
}

int32 cmd_igmp_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char sip[CAPA_IPV4_STR_LEN];
    char dip[CAPA_IPV4_STR_LEN];
    char portInfo[CAPA_PORTMASK_STR_LEN];
    char export_str[CAPA_PORTMASK_STR_LEN];
    char lifeStr[8];
    uint16 cnt = 0;
    uint32 i = 0;
    uint32 dbSize = 0;
    sys_igmp_db_t db;
    mcast_group_type_t  form = IGMP_GROUP_ENTRY_ALL;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t action;

    #ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t mvrEn;
    sys_vid_t mvrVlanId = 0;
    #endif

const char *gActStr[] =
{
    "drop",
    "flood",
    "router",
};

    if(1 == argc)
    {
        switch(argv[0][0])
        {
            case 'd':
                form =  IGMP_GROUP_ENTRY_DYNAMIC;
                break;
            case 's':
                form =  IGMP_GROUP_ENTRY_STATIC;
                break;
             default:
                form =  IGMP_GROUP_ENTRY_ALL;
                break;
        }
    }
    else
    {
        form =  IGMP_GROUP_ENTRY_ALL;
    }

    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&action));

    #ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEn));
    if (ENABLED == mvrEn)
    {
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlanId));
    }
    #endif

    if(IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        XMORE(" VLAN | Group IP Address |  Type  | Life(Sec) | Port      \n");
        XMORE("------+------------------+--------+-----------+------------------\n");
    }
    else
    {
        XMORE(" VLAN | Group IP Address |  SIP Address  |  Type  | Life(Sec) | Port \n");
        XMORE("------+------------------+---------------+--------+-----------+------------------\n");
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db,0,sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        if((IGMP_GROUP_ENTRY_DYNAMIC == form) && (IGMP_GROUP_ENTRY_DYNAMIC != db.form))
            continue;

        if((IGMP_GROUP_ENTRY_STATIC == form) && (IGMP_GROUP_ENTRY_STATIC != db.form))
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (mvrVlanId == db.vid)
        {
            if (sal_igmp_mvr_group_ret(&db))
                continue;
        }
        #endif

        if (!IGMP_IN_MULTICAST(db.dip))
            continue;

        cnt ++;
        SYS_ERR_CHK(sys_util_ip2Str(db.sip, sip));
        SYS_ERR_CHK(sys_util_ip2Str(db.dip, dip));

        osal_memset(lifeStr,0,sizeof(lifeStr));
        if(0 == db.life)
            osal_sprintf(lifeStr, "--");
        else
            osal_sprintf(lifeStr, "%d", db.life);

        if(IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
        {
            osal_memset(portInfo,0,sizeof(portInfo));
            osal_memset(export_str,0,sizeof(export_str));

            if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
               LPM2STR(&db.portmask, portInfo);
               if ((0 == osal_strlen(portInfo)) &&  ( IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == action))
               {
                    osal_sprintf(portInfo,"%s", "router");
               }
            }
            else
               
               LPM2STR(&db.staticInMbr, portInfo);

            XMORE(" %4d | %16s |%8s| %4s      | %s\n", db.vid, dip,
                  (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                  lifeStr, portInfo);
        }
        else
        {
            if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            {
                LPM2STR(&db.portmask, portInfo);
                if (0 == osal_strlen(portInfo))
               {
                    osal_sprintf(portInfo,"%s", gActStr[action]);
               }
            }
            else
                LPM2STR(&db.staticInMbr, portInfo);

            XMORE(" %4d | %16s |%15s|%8s| %4s      | %s\n",db.vid, dip, sip,
                (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                lifeStr, portInfo);
        }
    }

    XMORE("\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}

int32 cmd_igmp_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mcast_vlan_entry_t  mcastVlan;
    sys_vlanmask_t  vlanmask;
    sys_vid_t vlanId;
    sys_enable_t igmpEnable, mcastFilterEnable = ENABLED;

    XMORE("\n");

    if (1 == argc)
    {
        SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    }
    else
    {
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vlanmask));
    }

    SYS_ERR_CHK(sal_igmp_enable_get(&igmpEnable));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        if (!IS_STATIC_VLAN_EXIST(vlanId))
            continue;

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan));

        XMORE("IGMP Snooping is globaly %s \n", igmpEnable ? "enabled" : "disabled");
        XMORE("IGMP Snooping VLAN %d admin : %s \n", mcastVlan.vid, mcastVlan.enable ? "enabled" : "disabled");
        XMORE("IGMP Snooping operation mode : %s \n", text_enable[mcastVlan.enable && igmpEnable && mcastFilterEnable].text);
        XMORE("IGMP Snooping robustness: admin %d oper %d\n",  mcastVlan.robustnessVar, mcastVlan.operRobNessVar);
        XMORE("IGMP Snooping query interval: admin %d sec oper %d sec\n",  mcastVlan.queryIntv, mcastVlan.operQueryIntv);
        XMORE("IGMP Snooping query max response : admin %d sec oper %d sec\n",  mcastVlan.responseTime, mcastVlan.operRespTime);
        XMORE("IGMP Snooping last member query counter: admin %d  oper %d \n",  mcastVlan.lastMmbrQueryCnt, mcastVlan.operLastMmbrQueryCnt);
        XMORE("IGMP Snooping last member query interval: admin %d sec  oper %d sec\n",  mcastVlan.grpSpecificQueryIntv, mcastVlan.operGsqueryIntv);
        XMORE("IGMP Snooping immediate leave: %s \n",  mcastVlan.fastleave_enable ? "enabled" : "disabled");
        XMORE("IGMP Snooping automatic learning of multicast router ports: %s \n\n",  mcastVlan.pimDvmrpLearn ? "enabled" : "disabled");
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

int32 cmd_igmp_forward_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mcast_vlan_entry_t  mcastVlan;
    sys_vlanmask_t  vlanmask;
    sys_vid_t vlanId;
    char port_str[CAPA_PORTMASK_STR_LEN];

    XMORE("\n");

    if (1 == argc)
    {
        SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    }
    else
    {
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vlanmask));
    }

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        CMD_ERR_CONTINUE_VLAN_NOTEXIST(vlanId);

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan));

        XMORE("IGMP Snooping VLAN           : %d  \n", mcastVlan.vid);
        osal_memset(port_str, 0, sizeof(port_str));
        LPM2STR(&mcastVlan.staticMbr, port_str);
        XMORE("IGMP Snooping static port    : %s \n",  IS_LOGIC_PORTMASK_CLEAR(mcastVlan.staticMbr) ? "None" : port_str);
        osal_memset(port_str, 0, sizeof(port_str));
        LPM2STR(&mcastVlan.forbidMbr, port_str);
        XMORE("IGMP Snooping forbidden port : %s \n\n",  IS_LOGIC_PORTMASK_CLEAR(mcastVlan.forbidMbr) ? "None" : port_str);

    }

    XMORE("\n");

    return SYS_ERR_OK;
}
#endif

int32 cmd_igmp_groupCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbSize = 0;

    SYS_ERR_CHK(sal_igmp_databaseSize_get(&dbSize));
    XMORE("Total ip igmp snooping group number: %u\n", dbSize);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

int32 cmd_igmp_groupMode_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i, portid;
    uint32 cnt = 0;
    uint32 dbSize = 0;

    sys_igmp_groupHead_db_t  grpHeadDb;
    uint8           dip[128], vid[16], life[16], filterMode[32];

    XMORE("   GIP Address    |  VLAN   | Port  Filter Mode | Life(Sec) |\n");
    XMORE("------------------+---------+-------------------+-----------\n");

    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize))

    for(i = 0; i < dbSize; i++)
    {
        osal_memset(&grpHeadDb,0,sizeof(sys_igmp_groupHead_db_t));
        grpHeadDb.indx = i;

        SYS_ERR_CHK(sal_igmp_groupHead_database_get(&grpHeadDb));
        if(grpHeadDb.dip != 0)
        {
            cnt++;

            if(cnt > 1)
                XMORE("------------------+---------+-------------------+-----------\n");

            sys_util_ip2Str(grpHeadDb.dip, dip);
            osal_sprintf(vid, "  %4d ", grpHeadDb.vid);

            XMORE("  %15s | %s |",  dip,  vid);

            FOR_EACH_LOGIC_PORT(portid)
            {
                osal_sprintf(filterMode, "%s : %s ", LPSTR(portid), (PORT_IS_FILTER_EXCLUDE(grpHeadDb.fmode,portid)) ? \
                    "Exclude" : "Include");
                osal_sprintf(life, "     %4d ",  (PORT_IS_FILTER_EXCLUDE(grpHeadDb.fmode,portid)) ? \
                    grpHeadDb.mbrTimer[portid] : 0);

                if(portid == LOGIC_PORT_START)
                {
                    XMORE("%11s  %s\n", filterMode, life);
                }
                else
                {
                    XMORE("%42s  %s \n", filterMode, life);
                }
            }
        }

    }

    XMORE("\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

int32 cmd_igmp_querier_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlanmask_t vmExist;
    sys_vid_t vid;
    char  sip[20];
    sys_igmp_querier_t querier;
    mcast_vlan_entry_t  mcastVlan;
    uint32 cnt = 0;
    sys_enable_t igmpEn;

    XMORE("\n\n  VID  |    State    |    Status   |");
#if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
    XMORE("  Version   |");
#endif
    XMORE("  Querier IP \n");

    XMORE("-------+-------------+-------------+");
#if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
    XMORE("------------+");
#endif
    XMORE("--------------------\n");

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmExist));
    SYS_ERR_CHK(sal_igmp_enable_get(&igmpEn));

    FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
    {
        osal_memset(&querier, 0, sizeof(sys_igmp_querier_t));
        osal_memset(&mcastVlan, 0, sizeof(mcast_vlan_entry_t));

        querier.vid = vid;
        querier.ipType = MULTICAST_TYPE_IPV4;
        XMORE("  %4d |", querier.vid);

        SYS_ERR_CHK(sal_igmp_querier_get(&querier));

        mcastVlan.vid = vid;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CHK(sal_mcast_vlan_get(&mcastVlan));

        if (ENABLED == querier.enabled)
            XMORE("   Enabled   |");
        else
            XMORE("  Disabled   |");

        if (ENABLED == querier.status)
            XMORE("   Querier   |");
        else
            XMORE(" Non-Querier |");

#if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
        
        if (ENABLED == querier.enabled)
            XMORE("   %s       |", (IGMP_QUERY_V3 == querier.version) ? "v3" : "v2");
        else
            XMORE("   No       |");
#endif

        if ((ENABLED == querier.enabled) && (ENABLED == mcastVlan.enable) && (ENABLED ==igmpEn))
        {
            sys_util_ip2Str(querier.ipAddr, sip);
            XMORE(" %10s\n", sip);
        }
        else
        {
            XMORE("    ------\n");
        }

        cnt++;
    }

    XMORE("\nTotal Entry %u\n", cnt);
    XMORE("\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cmd_igmp_maxGroup_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_logic_portmask_t logicPmsk;
    uint16 limitNum;
    char strPort[8];

    if (1 == argc)
        CMD_PORTLIST_GET(0, &logicPmsk);
    else
        LOGIC_PORTMASK_SET_ALL(logicPmsk);

    XMORE("Port ID  |  Max Group  \n");
    XMORE("---------+-------------- \n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPmsk)
    {
        SYS_ERR_CHK(LP2STR(port, strPort));
        SYS_ERR_CHK(sal_igmp_portGroup_limit_get(port,&limitNum));
        XMORE("%8s : %d \n", strPort, limitNum);
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_maxGroupAction_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_logic_portmask_t logicPmsk;
    igmp_maxGroupAct_t act;
    char strPort[8];

    if (1 == argc)
        CMD_PORTLIST_GET(0, &logicPmsk);
    else
        LOGIC_PORTMASK_SET_ALL(logicPmsk);

    XMORE("Port ID  |  Max-groups  Action \n");
    XMORE("---------+--------------------- \n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPmsk)
    {
        SYS_ERR_CHK(LP2STR(port, strPort));
        SYS_ERR_CHK(sal_igmp_portGroup_excessAct_get(port,&act));
        XMORE("%8s : %s \n", strPort, act == IGMP_ACT_REPLACE ? "replace" : "deny");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cmd_igmp_filter_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_logic_portmask_t logicPmsk;
    sys_igmp_profileId_t proIdx;
    char strPort[8];

    if (1 == argc)
        CMD_PORTLIST_GET(0, &logicPmsk);
    else
        LOGIC_PORTMASK_SET_ALL(logicPmsk);

    XMORE("Port ID  |  Profile ID  \n");
    XMORE("---------+-------------- \n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPmsk)
    {
        SYS_ERR_CHK(LP2STR(port, strPort));
        SYS_ERR_CHK(sal_igmp_port_filter_get(port,&proIdx));
        if ( INVALID_PROFILE_ID == proIdx)
            XMORE("%8s : %s \n", strPort, "None");
        else
            XMORE("%8s : %d \n", strPort,   proIdx);
    }
    return SYS_ERR_OK;
}

int32 cmd_igmp_profile_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 found =FALSE;
    sys_igmp_profileId_t sidx = 0, eidx = 0;
    sys_igmp_profileId_t profileId;
    sys_igmp_profile_t profileInfo;

    char lowIpStr[CAPA_IPV4_STR_LEN];
    char highIpStr[CAPA_IPV4_STR_LEN];

    profileId = 1;

    if (0 != argc)
    {
        profileId = SYS_STR2UINT(argv[0]);
        sidx = eidx = profileId;
    }
    else
    {
        if (IGMP_PROFILE_NODE == vty->node)
        {
            profileId = g_profileIdx;
            sidx = eidx = profileId;
        }
        else
        {
            eidx = BOND_MCAST_PROFILE_MAX;
         }
    }

    osal_memset(&profileInfo, 0, sizeof(sys_igmp_profile_t));

    if (sidx == eidx)
    {
        if(SYS_ERR_OK != sal_igmp_profile_get(profileId, &profileInfo))
            return SYS_ERR_IGMP_PROFILE_NOT_EXIST;

        found = TRUE;

        XMORE("IP igmp profile index: %d\n", profileId);
        XMORE("IP igmp profile action: %s\n", profileActStr[profileInfo.act]);

        SYS_ERR_CHK(sys_util_ip2Str(profileInfo.lowIp, lowIpStr));
        SYS_ERR_CHK(sys_util_ip2Str(profileInfo.highIp, highIpStr));

        XMORE("Range low ip: %s\n", lowIpStr);
        XMORE("Range high ip: %s\n", highIpStr);
        XMORE("\n");
    }
    else
    {
        for (profileId = BOND_MCAST_PROFILE_MIN; profileId <= BOND_MCAST_PROFILE_MAX;  profileId++)
        {
            SYS_ERR_CONTINUE(sal_igmp_profile_get(profileId, &profileInfo));
            found = TRUE;

            XMORE("IP igmp profile index: %d\n", profileId);
            XMORE("IP igmp profile action: %s\n", profileActStr[profileInfo.act]);

            SYS_ERR_CHK(sys_util_ip2Str(profileInfo.lowIp, lowIpStr));
            SYS_ERR_CHK(sys_util_ip2Str(profileInfo.highIp, highIpStr));

            XMORE("Range low ip: %s\n", lowIpStr);
            XMORE("Range high ip: %s\n", highIpStr);
            XMORE("\n");
        }
    }

    if (FALSE == found)
        return SYS_ERR_IGMP_PROFILE_EMPTY;

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_PROTO_MVR

int32 cmd_mvr_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mcast_group_type_t type = IGMP_GROUP_ENTRY_ALL;

    if (1 == argc)
    {
        switch(argv[0][0])
        {
            case 'd' :
                type = IGMP_GROUP_ENTRY_DYNAMIC;
                break;

            case 's':
                type = IGMP_GROUP_ENTRY_STATIC;
                break;

            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_mvr_groupByType_clear(type));

    return SYS_ERR_OK;

}

int32 cmd_igmp_mvr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_vid_t mvrVlanId;
    uint32 maxGroupNum = 0, curGroupNum = 0;
    uint32 startIp = 0, endIp = 0;
    uint32 responseTime;
    mvr_mode_t mode;
    char startIpStr[CAPA_IPV4_STR_LEN];
    char endIpStr[CAPA_IPV4_STR_LEN];

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    SYS_ERR_CHK(sal_igmp_mvr_mode_get(&mode));
    SYS_ERR_CHK(sal_igmp_mvr_querytime_get(&responseTime));
    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlanId));
    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&startIp, &endIp));
    SYS_ERR_CHK(sal_igmp_mvrGroupNum_get(&curGroupNum));

    maxGroupNum = CAPA_IGMP_MVR_GROUP_NUM;

    XMORE("MVR Running : %s\n", enable ? "Enabled" : "Disabled");
    XMORE("MVR Multicast VLAN : %d\n", mvrVlanId);
    if (0 == startIp)
    {
        XMORE("MVR Group Range :  None\n");
    }
    else
    {
        osal_memset(startIpStr,0,sizeof(startIpStr));
        osal_memset(endIpStr,0,sizeof(endIpStr));
        SYS_ERR_CHK(sys_util_ip2Str(startIp, startIpStr));
        SYS_ERR_CHK(sys_util_ip2Str(endIp, endIpStr));

        XMORE("MVR Group Range :  %s ~ %s\n", startIpStr, endIpStr);
    }
    XMORE("MVR Max Multicast Groups : %d\n", maxGroupNum);
    XMORE("MVR Current Multicast Groups : %d\n", curGroupNum);
    XMORE("MVR Global query response time : %d sec\n", responseTime);
    XMORE("MVR Mode : %s\n\n", mode == MVR_MODE_COMPATABLE ? "compatible" : "dynamic");

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_interface_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    char port_str[8];
    sys_enable_t   fastLeaveEnable;
    mvr_portType_t portType;
    sys_logic_portmask_t logicPmsk;

    if (1 == argc)
        CMD_PORTLIST_GET(0, &logicPmsk);
    else
        LOGIC_PORTMASK_SET_ALL(logicPmsk);

    XMORE("  Port  |  Type   | Immediate Leave \n");
    XMORE("--------+---------+-----------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPmsk)
    {
        SYS_ERR_CHK(sal_igmp_mvr_portType_get( port, &portType));
        SYS_ERR_CHK(sal_igmp_mvr_portImmediate_get( port, &fastLeaveEnable));

        SYS_ERR_CHK(LP2STR(port, port_str));
        XMORE("%4s    |%9s|%12s \n", port_str,
                                           gMvrPortType[portType],
                                           ENABLED == fastLeaveEnable ?  "Enabled": "Disabled");
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_mvr_member_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char sip[CAPA_IPV4_STR_LEN];
    char dip[CAPA_IPV4_STR_LEN];
    char portInfo[CAPA_PORTMASK_STR_LEN];
    char export_str[CAPA_PORTMASK_STR_LEN];
    char lifeStr[8];
    uint16 cnt = 0;
    uint32 i = 0;
    uint32 dbSize = 0;
    sys_igmp_db_t db;
    igmp_lookup_mode_t mode;
    sys_vid_t mvrVid = 1;
    sys_enable_t mvrEnable;
    uint32 groupStart , groupEnd;
    igmp_unknMcastAction_t action;

    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVid));
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEnable));
    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&action));

    if(IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        XMORE(" Gourp IP Address |  Type  | Life(Sec) | Port      \n");
        XMORE("------------------+--------+-----------+------------------\n");
    }
    else
    {
        XMORE(" Group IP Address |  SIP Address  |  Type  | Life(Sec) | Port \n");
        XMORE("------------------+---------------+--------+-----------+------------------\n");
    }

    if (ENABLED == mvrEnable)
    {
        SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));
        for (i = 0; i < dbSize; i++)
        {
            osal_memset(&db,0,sizeof(sys_igmp_db_t));
            db.indx = i;
            SYS_ERR_CHK(sal_mcast_database_get(&db));

            if (MULTICAST_TYPE_IPV4 != db.ipType)
                continue;

            if (mvrVid != db.vid)
                continue;

            if (db.dip < groupStart || db.dip > groupEnd)
                continue;

            cnt ++;
            SYS_ERR_CHK(sys_util_ip2Str(db.sip, sip));
            SYS_ERR_CHK(sys_util_ip2Str(db.dip, dip));

            osal_memset(lifeStr,0,sizeof(lifeStr));
            if(0 == db.life)
                osal_sprintf(lifeStr, "--");
            else
                osal_sprintf(lifeStr, "%d", db.life);

            if(IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
            {
                osal_memset(portInfo,0,sizeof(portInfo));
                osal_memset(export_str,0,sizeof(export_str));

                if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
                {
                    LPM2STR(&db.portmask, portInfo);
                    if ((0 == osal_strlen(portInfo)) &&  ( IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == action))
                    {
                        osal_sprintf(portInfo,"%s", "router");
                    }
                }
                else
                   
                   LPM2STR(&db.staticInMbr, portInfo);

                XMORE(" %16s |%8s| %4s      | %s\n",  dip,
                      (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                      lifeStr, portInfo);
            }
            else
            {
                if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
                    
                    LPM2STR(&db.portmask, portInfo);
                else
                    LPM2STR(&db.staticInMbr, portInfo);

                XMORE(" %16s |%15s|%8s| %4s      | %s\n",dip, sip,
                    (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                    lifeStr, portInfo);
            }
        }
    }
    XMORE("\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}
#endif 

int32 cmd_igmp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t igmpEnable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        igmpEnable = DISABLED;
    }
    else
        igmpEnable = ENABLED;

    SYS_ERR_CHK(sal_igmp_enable_set(igmpEnable));

    return SYS_ERR_OK;
}

int32 cmd_igmp_suppre_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_igmp_suppre_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_igmp_lookupMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mcast_info_t info;

    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (argv[0][0] == 'm')
    {
        SYS_ERR_CHK(sal_igmp_lookupMode_set(IGMP_DMAC_VID_MODE));
        if (IGMP_VERSION_V3_FULL == info.operVersion)
            SYS_ERR_CHK(sal_igmp_operVersion_set(IGMP_VERSION_V3_BASIC));
    }
    else if(argv[0][0] == 'd')
    {
        SYS_ERR_CHK(sal_igmp_lookupMode_set(IGMP_DIP_VID_MODE));
    }
    else
    {
        SYS_ERR_CHK(sal_igmp_lookupMode_set(IGMP_DIP_SIP_MODE));

        if (IGMP_VERSION_V3_BASIC == info.operVersion)
        {

            SYS_ERR_CHK(sal_igmp_operVersion_set(IGMP_VERSION_V2));
        }
    }

    return SYS_ERR_OK;

}

int32 cmd_igmp_unknMcastAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    igmp_unknMcastAction_t action = IGMP_UNKNOWN_MCAST_ACTION_FLOOD;

    if (CMD_IS_NO_FORM_EXIST())
        action = DFLT_IGMP_UNKNOWN_MCAST_ACTION;
    else if (argv[0][0] == 'd')
        action = IGMP_UNKNOWN_MCAST_ACTION_DROP;
    else if (argv[0][0] == 'f')
        action = IGMP_UNKNOWN_MCAST_ACTION_FLOOD;
    else if (argv[0][0] == 'r')
        action = IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT;

    SYS_ERR_CHK(sal_igmp_unknMcastAction_set(action));

    return SYS_ERR_OK;
}

int32 cmd_igmp_routerTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    return SYS_ERR_OK;
}

int32 cmd_igmp_fastleave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    sys_enable_t fastleaveEn;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        fastleaveEn = DISABLED;
    else
        fastleaveEn = ENABLED;

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (fastleaveEn == mcastVlan.fastleave_enable)
            continue;

        mcastVlan.fastleave_enable = fastleaveEn;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_pimDvmrpLearn_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    sys_enable_t routerLearn;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        routerLearn = DISABLED;
    else
        routerLearn = ENABLED;

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (routerLearn ==mcastVlan.pimDvmrpLearn)
            continue;
        mcastVlan.pimDvmrpLearn = routerLearn;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_robustnessVar_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    uint16 val = 0;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_IGMP_ROBUSTNESS_VAR;
    else
        val = (uint16)SYS_STR2UINT(argv[1]);

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (val == mcastVlan.robustnessVar)
            continue;
        mcastVlan.robustnessVar = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_responseTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    uint16 val = 0;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_IGMP_QUERY_RESPONSE_INTV;
    else
        val = (uint16)SYS_STR2UINT(argv[1]);

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (val == mcastVlan.responseTime)
            continue;

        mcastVlan.responseTime = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_queryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    uint16 val = 0;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_IGMP_QUERY_INTV;
    else
        val = (uint16)SYS_STR2UINT(argv[1]);

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (val == mcastVlan.queryIntv)
            continue;

        mcastVlan.queryIntv = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }
    return SYS_ERR_OK;
}

int32 cmd_igmp_lastMbrQueryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    uint16 val = 0;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_IGMP_GSQUERY_RESPONSE_INTV ;
    else
        val = (uint16)SYS_STR2UINT(argv[1]);

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (val == mcastVlan.grpSpecificQueryIntv)
            continue;

        mcastVlan.grpSpecificQueryIntv = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_lastMbrQueryCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    uint16 val = 0;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        val = DFLT_IGMP_ROBUSTNESS_VAR;
    else
        val = (uint16)SYS_STR2UINT(argv[1]);

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (val == mcastVlan.lastMmbrQueryCnt)
            continue;

        mcastVlan.lastMmbrQueryCnt = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask, vmExist;
    sys_enable_t enable = ENABLED;

    mcast_vlan_entry_t mcastVlan;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (enable == mcastVlan.enable)
            continue;

        mcastVlan.enable = enable;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

#ifdef  CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

int32 cmd_igmp_static_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask;
    sys_logic_portmask_t logicPmsk;
    uint8 isAdd = TRUE;
    mcast_vlan_entry_t mcastVlan;
    sys_vlanmask_t vmExist;

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;
    else
        isAdd = TRUE;

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);
    CMD_PORTLIST_GET(1, &logicPmsk);

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        #if 1
        if (!IS_VLAN_OPER_ENTRY_EXIST(vlanId))
        {
            SYS_PRINTF("Vlan %d is not exist!\n", vlanId);
            continue;
        }
        #endif
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (isAdd)
        {
            LOGIC_PORTMASK_OR(mcastVlan.staticMbr, mcastVlan.staticMbr, logicPmsk);
            LOGIC_PORTMASK_ANDNOT(mcastVlan.forbidMbr, mcastVlan.forbidMbr, logicPmsk);
        }
        else
        {
            LOGIC_PORTMASK_ANDNOT(mcastVlan.staticMbr, mcastVlan.staticMbr,logicPmsk) ;
        }

        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_forbidden_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask;
    sys_logic_portmask_t logicPmsk;
    uint8 isAdd = TRUE;
    mcast_vlan_entry_t mcastVlan;
    sys_vlanmask_t  vmExist;

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;
    else
        isAdd = TRUE;

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);
    CMD_PORTLIST_GET(1, &logicPmsk);

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        #if 1
        if (!IS_VLAN_OPER_ENTRY_EXIST(vlanId))
        {
            SYS_PRINTF("Vlan %d is not exist!\n", vlanId);
            continue;
        }
        #endif

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }

        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (isAdd)
        {
            LOGIC_PORTMASK_OR(mcastVlan.forbidMbr, mcastVlan.forbidMbr, logicPmsk);
            LOGIC_PORTMASK_ANDNOT(mcastVlan.staticMbr, mcastVlan.staticMbr, logicPmsk);
        }
        else
            LOGIC_PORTMASK_ANDNOT(mcastVlan.forbidMbr, mcastVlan.forbidMbr, logicPmsk);

        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

int32 cmd_igmp_querier_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_igmp_querier_t querier, qEntry;
    sys_vlanmask_t vlanmask;
    sys_vlanmask_t vmExist;
    sys_vid_t vid;
    sys_mcast_info_t info;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&querier,0,sizeof(sys_igmp_querier_t));

    if (CMD_IS_NO_FORM_EXIST())
    {
        querier.enabled = DISABLED;
    }
    else
    {
        querier.enabled = ENABLED;

        if (argc == 2)
        {
            if (argv[1][0] == '2' )
                querier.version = IGMP_QUERY_V2;
            else
                querier.version = IGMP_QUERY_V3;
        }
        else
        {
            querier.version = IGMP_QUERY_V2;
        }

        osal_memset(&info,0,sizeof(sys_mcast_info_t));
        SYS_ERR_CHK(sal_igmp_info_get(&info));

        if (info.operVersion < IGMP_VERSION_V3_BASIC && querier.version == IGMP_QUERY_V3)
        {
            XMORE("igmp snooping v2 can't support v3 querier!\n");
            return SYS_ERR_OK;
        }
    }

    if (CMD_IS_NO_FORM_EXIST() && (0 == argc))
    {
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmExist));

        FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
        {
            CMD_ERR_CONTINUE_VLAN_NOTEXIST(vid);

            osal_memset(&qEntry,0,sizeof(sys_igmp_querier_t));

            qEntry.vid = vid;
            qEntry.ipType = MULTICAST_TYPE_IPV4;
            CMD_ERR_CONTINUE_VLAN(sal_igmp_querier_get(&qEntry),qEntry.vid);

            if (ENABLED == qEntry.enabled)
            {
                VLANMASK_SET_VID(vlanmask, qEntry.vid);
            }
        }
    }
    else
    {
        SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    }

    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
        querier.vid = vid;
        CMD_ERR_CONTINUE_VLAN_NOTEXIST(querier.vid);
        CMD_ERR_CONTINUE_VLAN(sal_igmp_querier_set(&querier), querier.vid);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

int32 cmd_igmp_routerPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_logic_portmask_t logicPortmask;
    sys_logic_portmask_t trunkMbr,tmpMbr;
    sys_logic_portmask_t setPortmask;
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask;
    igmp_router_entry_t routerEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    VLANMASK_CLEAR_ALL(vlanmask);
    LOGIC_PORTMASK_CLEAR_ALL(logicPortmask);
    LOGIC_PORTMASK_CLEAR_ALL(setPortmask);
    LOGIC_PORTMASK_CLEAR_ALL(trunkMbr);
    LOGIC_PORTMASK_CLEAR_ALL(tmpMbr);
    osal_memset(&routerEntry, 0, sizeof(igmp_router_entry_t));

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    CMD_PORTLIST_GET(1, &logicPortmask);

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        routerEntry.vid = vlanId;
        routerEntry.ipType = ipType;
        SYS_ERR_CHK(sal_mcast_router_get(&routerEntry));

        if (CMD_IS_NO_FORM_EXIST())
        {
            LOGIC_PORTMASK_ANDNOT(setPortmask,routerEntry.router_static_pmsk,logicPortmask);
        }
        else
        {
            #if 1
            if(FALSE == sal_vlan_operEntryExist_ret(vlanId))
            {
                SYS_PRINTF("VLAN %d is not exist.\n", vlanId);
                continue;
            }
            #endif

            LOGIC_PORTMASK_OR(setPortmask,routerEntry.router_static_pmsk,logicPortmask);

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
            {
                if(IS_LOGIC_PORTMASK_PORTSET(routerEntry.router_forbid_pmsk, port))
                {
                    XMORE("Port %s is forbidden router port!\nif you want to set static router port, you can delete if from forbidden port list \n",
                                      LPSTR(port));
                    LOGIC_PORTMASK_CLEAR_PORT(setPortmask, port);
                    continue;
                }
            }
        }

        SYS_ERR_CHK(sal_mcast_router_static_port_set(vlanId, ipType, setPortmask));
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_routerPortForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_portmask_t setPortmask;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    igmp_router_entry_t routerEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    VLANMASK_CLEAR_ALL(vlanmask);
    LOGIC_PORTMASK_CLEAR_ALL(logicPortmask);
    LOGIC_PORTMASK_CLEAR_ALL(setPortmask);
    osal_memset(&routerEntry, 0, sizeof(igmp_router_entry_t));

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    CMD_PORTLIST_GET(1, &logicPortmask);

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        routerEntry.vid = vlanId;
        routerEntry.ipType = ipType;
        SYS_ERR_CHK(sal_mcast_router_get(&routerEntry));

        if (CMD_IS_NO_FORM_EXIST())
            LOGIC_PORTMASK_ANDNOT(setPortmask,routerEntry.router_forbid_pmsk,logicPortmask);
        else
        {
            #if 1
            if(FALSE == sal_vlan_operEntryExist_ret(vlanId))
            {
                SYS_PRINTF("VLAN %d is not exist.\n", vlanId);
                continue;
            }
            #endif

            LOGIC_PORTMASK_OR(setPortmask,routerEntry.router_forbid_pmsk,logicPortmask);
        }

        SYS_ERR_CHK(sal_mcast_router_forbid_port_set(vlanId, ipType, setPortmask));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_igmp_operVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 operVersion;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));

    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (argv[0][0] == '3')
    {
        SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));
        if (IGMP_DMAC_VID_MODE ==  mode || IGMP_DIP_VID_MODE ==  mode)
            operVersion = IGMP_VERSION_V3_BASIC;
        else
        {
            operVersion = IGMP_VERSION_V3_FULL;
            XMORE("The forward method is src-dst-ip ! can't support igmp version 3!\n");
            return SYS_ERR_OK;
        }
    }
    else
    {
        operVersion = IGMP_VERSION_V2;
    }

    if (operVersion == info.operVersion)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_igmp_operVersion_set(operVersion));

    return SYS_ERR_OK;

}

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

int32 cmd_igmpv3_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isDel = FALSE;
    uint8 isFound = FALSE;
    uint16 fmode = FILTER_MODE_INCLUDE;
    uint32 groupAddr =0;
    uint32 sourceAddr =0;
    uint32 dbSize;
    uint32 i;
    sys_vid_t  vid ;
    sys_logic_portmask_t logicPortmask;
    sys_logic_portmask_t setpmsk;
    sys_logic_portmask_t exMsk;
    sys_igmp_db_t db;
    sys_mcast_info_t info;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));
    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (IGMP_VERSION_V3_BASIC == info.operVersion)
    {
        XMORE("The igmpv3 snooping version is v3-basic ,This action support for v3-full !\n");

        return SYS_ERR_OK;
    }

    if (CMD_IS_NO_FORM_EXIST())
        isDel = TRUE;

    vid = SYS_STR2UINT(argv[2]);

    if (argc == 5)
    {
        switch(argv[4][0])
        {
            case 'i' :
                fmode = FILTER_MODE_INCLUDE;
                break;

            case 'e':
                fmode = FILTER_MODE_EXCLUDE;
                break;

            default:
                break;
        }
    }
    else
    {
        fmode = FILTER_MODE_INCLUDE;
    }

    SYS_ERR_CHK(sys_util_str2Ip(argv[0], &groupAddr));
    SYS_ERR_CHK(sys_util_str2Ip(argv[1], &sourceAddr));

    if (!IGMP_IN_MULTICAST(ntohl(groupAddr)))
    {
        return SYS_ERR_MCAST_ADDR,
    }

    if ((ntohl(groupAddr) >= ntohl(0xE0000000L)) && (ntohl(groupAddr) <= ntohl(0xE00000FFL)))
    {
        return SYS_ERR_IGMP_RSV_ADDR;
    }

    if ((0x0 == sourceAddr) || (0xFFFFFFFF == sourceAddr))
        return SYS_ERR_IPV4_ADDR;

    CMD_PORTLIST_GET(3, &logicPortmask);
    LOGIC_PORTMASK_CLEAR_ALL(setpmsk);
    osal_memcpy(&setpmsk, &logicPortmask,sizeof(sys_logic_portmask_t));
    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    if (TRUE == isDel)
    {
        for (i = 0; i < dbSize; i++)
        {
            osal_memset(&db, 0, sizeof(sys_igmp_db_t));
            db.indx = i;
            SYS_ERR_CHK(sal_mcast_database_get(&db));

            if ((db.vid == vid) && (db.dip == groupAddr) && (db.sip == db.sip))
            {
                isFound = TRUE;
                break;
            }
        }

        if (FALSE == isFound)
        {
            return SYS_ERR_OK;
        }
        else
        {
            if (FILTER_MODE_INCLUDE == fmode)
            {
                
                LOGIC_PORTMASK_ANDNOT(setpmsk, db.staticInMbr, logicPortmask);
            }
            else
            {
                
                LOGIC_PORTMASK_ANDNOT(exMsk, db.staticMbr, db.staticInMbr);
                LOGIC_PORTMASK_ANDNOT(setpmsk, exMsk, logicPortmask);
            }
        }
    }
    else
    {
        for (i = 0; i < dbSize; i++)
        {
            osal_memset(&db, 0, sizeof(sys_igmp_db_t));
            db.indx = i;
            SYS_ERR_CHK(sal_mcast_database_get(&db));

            if ((db.vid == vid) && (db.dip == groupAddr) && (db.sip == db.sip))
            {
                isFound = TRUE;
                break;
            }
        }

        if (TRUE == isFound)
        {
            if (FILTER_MODE_INCLUDE == fmode)
            {
                LOGIC_PORTMASK_OR(setpmsk, db.staticInMbr,setpmsk);
            }
            else
            {
                osal_memset(&exMsk,0,sizeof(sys_logic_portmask_t));
                LOGIC_PORTMASK_ANDNOT(exMsk, db.staticMbr, db.staticInMbr);
                LOGIC_PORTMASK_OR(setpmsk, logicPortmask, exMsk);
            }
        }
    }

    SYS_ERR_CHK(sal_igmpv3_static_group_set(vid, groupAddr,sourceAddr,&setpmsk,fmode));

    return SYS_ERR_OK;
}

int32 cmd_igmp_static_filter_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isFound = FALSE;
    uint32 groupAddr =0;
    uint32 sourceAddr =0;
    uint32 dbSize;
    uint32 i;
    sys_vid_t vid;
    sys_igmp_db_t   db;
    sys_mcast_info_t info;
    sys_vlanmask_t vlanmask;
    igmp_group_entry_t group;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));
    osal_memset(&group, 0, sizeof(igmp_group_entry_t));
    VLANMASK_CLEAR_ALL(vlanmask);

    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (IGMP_VERSION_V3_BASIC == info.operVersion)
    {
        XMORE("The igmpv3 snooping version is v3-basic ,This action support for v3-full !\n");
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sys_util_str2Ip(argv[0], &groupAddr));
    SYS_ERR_CHK(sys_util_str2Ip(argv[1], &sourceAddr));

    if (!IGMP_IN_MULTICAST(ntohl(groupAddr)))
    {
        return SYS_ERR_MCAST_ADDR;
    }

    if ((ntohl(groupAddr) >= ntohl(0xE0000000L)) && (ntohl(groupAddr) <= ntohl(0xE00000FFL)))
    {
        return SYS_ERR_IGMP_RSV_ADDR;
    }

    if((0x0 == sourceAddr) || (0xFFFFFFFF == sourceAddr))
        return SYS_ERR_IPV4_ADDR;

    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if ((db.dip == groupAddr) && (db.form == IGMP_GROUP_ENTRY_STATIC) &&(db.sip == 0))
        {
            isFound = TRUE;
            VLANMASK_SET_VID(vlanmask, db.vid);
        }
    }

    if (FALSE == isFound)
    {
        return SYS_ERR_OK;
    }

    group.dip = groupAddr;
    group.sip = sourceAddr;

    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
        group.vid = vid;
        SYS_ERR_CONTINUE(sal_igmp_static_filter_group_set(&group, FILTER_MODE_INCLUDE));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_igmp_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAdd = TRUE;
    uint32 groupAddr =0;
    sys_logic_portmask_t   logicPortmask;

    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    igmp_group_entry_t  groupEntry;

#ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t enable;
    sys_vid_t mvrVlan;
    uint32 groupStart, groupEnd;
#endif
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    CMD_PORTLIST_GET(2, &logicPortmask);

    if (IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
    {
        return SYS_ERR_INPUT;
    }

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;

    SYS_ERR_CHK(sys_util_str2Ip(argv[1], &groupAddr));

    if (!IGMP_IN_MULTICAST(ntohl(groupAddr)))
    {
        return SYS_ERR_MCAST_ADDR;
    }

    if ((ntohl(groupAddr) >= ntohl(0xE0000000L)) && (ntohl(groupAddr) <= ntohl(0xE00000FFL)))
    {
        return SYS_ERR_IGMP_RSV_ADDR;
    }
    #ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd));
    if (groupAddr >= groupStart  &&  groupAddr <= groupEnd)
        return SYS_ERR_MVR_IGMP_STATIC_GROUP_SET;
    #endif
    osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
    groupEntry.dip = groupAddr;
    groupEntry.ipType = MULTICAST_TYPE_IPV4;

    osal_memcpy(&groupEntry.mbr, &logicPortmask,sizeof(sys_logic_portmask_t));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
  #ifdef CONFIG_SYS_PROTO_MVR
        SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
        if ((ENABLED == enable) && (mvrVlan == vlanId))
        {
            return SYS_ERR_STATIC_GROUP_SET;
        }
#endif
        groupEntry.vid = vlanId;
        SYS_ERR_CHK(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, isAdd));
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cmd_igmp_portGroup_limit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint16 maxNum;
    sys_logic_port_t port;

    if (CMD_IS_NO_FORM_EXIST())
        maxNum = DFLT_MCAST_PORT_LIMIT;
    else
        maxNum = (uint16)SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_igmp_portGroup_limit_set(port, maxNum), port);
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_portGroup_excessAct_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    igmp_maxGroupAct_t act = IGMP_ACT_DENY;

    switch(argv[0][0])
    {
        case 'r' :
            act = IGMP_ACT_REPLACE;
            break;

        case 'd':
        default:
            act = IGMP_ACT_DENY;
            break;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_igmp_portGroup_excessAct_set(port, act), port);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cmd_igmp_profile_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    g_profileIdx = SYS_STR2UINT(argv[0]);

    vty->node = IGMP_PROFILE_NODE;

    return SYS_ERR_OK;
}

int32 cmd_igmp_profileEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_igmp_profile_t profileEntry;
    sys_igmp_profileId_t  proIdx;
    sys_logic_portmask_t  logicPmsk;
    sys_logic_port_t port;

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);

    osal_memset(&profileEntry, 0 , sizeof(sys_igmp_profile_t));
    profileEntry.profileId = g_profileIdx;
    profileEntry.valid = 1;
    profileEntry.ipType = MULTICAST_TYPE_IPV4;
    profileEntry.profileType = MCAST_PROFILE_TYPE_FILTER;

    SYS_ERR_CHK(sys_util_str2Ip(argv[0],  &profileEntry.lowIp));

    if (!IGMP_IN_MULTICAST(ntohl(profileEntry.lowIp)))
    {
        return SYS_ERR_INPUT;
    }

    if (3 == argc)
    {
        SYS_ERR_CHK(sys_util_str2Ip(argv[1],  &profileEntry.highIp));

         if (!IGMP_IN_MULTICAST(ntohl(profileEntry.highIp)))
        {
            return SYS_ERR_INPUT;
        }

        if (profileEntry.highIp < profileEntry.lowIp)
        {
            return SYS_ERR_RANGE;
        }

        switch (argv[2][0])
        {
            case 'p':
                profileEntry.act = MCAST_PROFILE_ACT_PERMIT;
                break;
            case 'd':
            default:
                profileEntry.act = MCAST_PROFILE_ACT_DENY;
                break;
        }
    }
    else if (2 == argc)
    {
        profileEntry.highIp = profileEntry.lowIp;
        switch (argv[1][0])
        {
            case 'p':
                profileEntry.act = MCAST_PROFILE_ACT_PERMIT;
                break;
            case 'd':
            default:
                profileEntry.act = MCAST_PROFILE_ACT_DENY;
                break;
        }
    }

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_igmp_port_filter_get(port, &proIdx));

        if (proIdx == g_profileIdx)
        {
            LOGIC_PORTMASK_SET_PORT(logicPmsk, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(logicPmsk))
    {
        return SYS_ERR_IGMP_PROFILE_USED;
    }

    SYS_ERR_CHK(sal_igmp_profile_set(g_profileIdx, &profileEntry));

    return SYS_ERR_OK;
}

int32 cmd_igmp_profile_remove(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_igmp_profileId_t profileIdx, tmpIdx;
    sys_igmp_profile_t profileEntry;
    sys_logic_port_t port;
    sys_logic_portmask_t logicPmsk;

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);

    profileIdx = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_igmp_profile_get(profileIdx, &profileEntry));

    osal_memset(&profileEntry, 0, sizeof(sys_igmp_profile_t));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_igmp_port_filter_get(port, &tmpIdx));
        if (tmpIdx == profileIdx)
        {
            LOGIC_PORTMASK_SET_PORT(logicPmsk, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(logicPmsk))
    {
        return SYS_ERR_IGMP_PROFILE_USED;
    }

    SYS_ERR_CHK(sal_igmp_profile_set(profileIdx, &profileEntry));

    return SYS_ERR_OK;
}

int32 cmd_igmp_filter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_igmp_profileId_t proIdx, tmpIdx;
    sys_igmp_profile_t profile;

    osal_memset(&profile, 0, sizeof(sys_igmp_profile_t));

    if (CMD_IS_NO_FORM_EXIST())
        proIdx = INVALID_PROFILE_ID;
    else
    {
        proIdx = (uint16)SYS_STR2UINT(argv[0]);
        SYS_ERR_CHK(sal_igmp_profile_get(proIdx, &profile));
        if (0 == profile.valid)
            return SYS_ERR_IGMP_PROFILE_NOT_EXIST;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_igmp_port_filter_get(port, &tmpIdx));
        if (tmpIdx == proIdx)
            continue;

        SYS_ERR_CHK(sal_igmp_port_filter_set(port, proIdx));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_igmp_static_group_set_by_mac(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAdd = TRUE;
    sys_mac_t           mac;
    sys_logic_portmask_t   logicPortmask;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    l2_mcast_group_t  groupEntry;

    SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));

    CMD_PORTLIST_GET(2, &logicPortmask);

    if (IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
    {
        return SYS_ERR_INPUT;
    }

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));

    if (!(0x1 & mac.octet[0]))
        return SYS_ERR_MCAST_ADDR;

    osal_memset(&groupEntry, 0, sizeof(l2_mcast_group_t));
    
    osal_memcpy(groupEntry.mac, mac.octet, sizeof(sys_mac_t));

    osal_memcpy(&groupEntry.staticMbr, &logicPortmask, sizeof(sys_logic_portmask_t));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        groupEntry.vlanId = vlanId;
        SYS_ERR_CHK(sal_l2_mcast_static_group_set(&groupEntry, isAdd));
    }
    
#if 0
	char buf[SYS_BUF128_LEN];
	SYS_MEM_CLEAR(buf);
	osal_sprintf(buf, "%s",cmd->string);
	sal_snmp_system_user_operation_info_Trap(getpid(),buf);
#endif

    return SYS_ERR_OK;
}

int32 cmd_igmp_static_l2_mcast_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAdd = TRUE;
    sys_mac_t           mac;
    sys_logic_portmask_t   logicPortmask;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    l2_mcast_group_t  groupEntry;

    SYS_ERR_CHK(STR2VMSK(argv[1], &vlanmask));

    LOGIC_PORTMASK_CLEAR_ALL(logicPortmask);

    isAdd = FALSE;

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));

    if (!(0x1 & mac.octet[0]))
        return SYS_ERR_MCAST_ADDR;

    osal_memset(&groupEntry, 0, sizeof(l2_mcast_group_t));
    
    osal_memcpy(groupEntry.mac, mac.octet, sizeof(sys_mac_t));

    osal_memcpy(&groupEntry.staticMbr, &logicPortmask, sizeof(sys_logic_portmask_t));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        groupEntry.vlanId = vlanId;
        SYS_ERR_CHK(sal_l2_mcast_static_group_set(&groupEntry, isAdd));
    }
    
#if 0
	char buf[SYS_BUF128_LEN];
	SYS_MEM_CLEAR(buf);
	osal_sprintf(buf, "%s",cmd->string);
	sal_snmp_system_user_operation_info_Trap(getpid(),buf);
#endif

    return SYS_ERR_OK;
}

int32 cmd_igmp_static_l2_mcast_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char * ObjectN2Print[][2] = 
        {
         {"MAC Address","MAC 地址"}, 
         {"VLAN ID", "VLAN ID"}, 
         {"Static port list", "静态端口列表"},         
         {"GMRP port list", "GMRP端口列表"},
        }; 
    char * promptPrint[][2] =
        {
         {"show multicast table information", "\r\n显示多播组信息"},
         {"\r\nspecifing multicast group does not exist !\r\n","\r\n指定的多播组MAC地址不存在 !\r\n"},
         {"\r\nTotal entries: %d .\n", "总计%d条记录.\n"},
         {"\r\nMAC address is not multicast!.\r\n", "\r\nFDB 表MAC 地址不是组播地址.\r\n"},
         {"\r\nStatic multicast MAC address only can configure as 01:00:5E:xx:xx:xx, and the IP address configures as 224.x.x.x .\r\n", "\r\n静态组播表项MAC地址只能配置为01:00:5E:xx:xx:xx格式，IP地址只能配置为224.x.x.x格式。\r\n"},
         {"Time interval of multicast proxy is", "组播代理发送时间间隔是"}
        };

    vector printor = NULL;
    uint32  ulLanguageMode = 0;
    int     i,  k;
    int     printCount = 0;
    char    cInputChar = 0;
		sys_vlanmask_t vmList;
		l2_mcast_group_t l2mcastEntry;
		int rc = 0;
		char macStr[SYS_BUF32_LEN];
		char	untagPortBuf[5][128];
		int 	untagLine;
		char strPmUntagged[CAPA_PORTMASK_STR_LEN] = {0};
		uint32 iMaxCount = 0;

		if (argc)
		{
			SYS_ERR_CHK(STR2VMSK(argv[0], &vmList));
		}

    add_value_to_new_line_printor(PRINT_STRING, promptPrint[0][ulLanguageMode], &printor);
   
    printCount = 0;
    add_value_to_new_line_printor_bylen(PRINT_STRING, ObjectN2Print[0][ulLanguageMode], &printor, 19);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[1][ulLanguageMode], &printor, 10);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[2][ulLanguageMode], &printor, 20);
#ifdef CONFIG_SYS_PROTO_GARP
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[3][ulLanguageMode], &printor, 20);
#endif
		rc = sal_l2_mcast_databaseSize_get(&iMaxCount);	
		if(rc != SYS_ERR_OK)
			goto end;
		
    for (i = 0; i < iMaxCount; i++)
    { 
    	SYS_MEM_CLEAR(l2mcastEntry);
    	SYS_MEM_CLEAR(untagPortBuf);
			rc = sal_l2_mcast_database_get(&l2mcastEntry, i);
			if(rc != SYS_ERR_OK)
			{
				continue;
			}
			sys_util_mac2Str(l2mcastEntry.mac, macStr);
			add_value_to_new_line_printor_bylen(PRINT_STRING, macStr, &printor, 19);
			add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &l2mcastEntry.vlanId, &printor, 10);

		  SYS_MEM_CLEAR(strPmUntagged);
		  LPM2STR(&l2mcastEntry.staticMbr, strPmUntagged);
		  SYS_MEM_CLEAR(untagPortBuf);
		  untagLine = 0;
			untagLine = transLongStrToStrArray(strPmUntagged, untagPortBuf, 26);

		  add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[0], &printor, 28);
		      
			for (k = 1; k < untagLine; k++)
			{
		    add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 19);
		    add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 10); 
		    add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[k], &printor, 28);
		  }

#ifdef CONFIG_SYS_PROTO_GARP
		  SYS_MEM_CLEAR(strPmUntagged);
		  LPM2STR(&l2mcastEntry.gmrpMbr, strPmUntagged);
		  SYS_MEM_CLEAR(untagPortBuf);
		  untagLine = 0;
			untagLine = transLongStrToStrArray(strPmUntagged, untagPortBuf, 26);

		  add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[0], &printor, 28);
		      
			for (k = 1; k < untagLine; k++)
			{
		    add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 19);
		    add_value_to_new_line_printor_bylen(PRINT_STRING, "", &printor, 10); 
		    add_value_to_last_line_printor_bylen(PRINT_STRING, untagPortBuf[k], &printor, 28);
		  }
#endif
		  printCount++;
	}
	
end:	
	if (printor != NULL)
	{
		cmdPrintOutResult(printor, &cInputChar);
		free_printor_vector(&printor);
	}
	XMORE(promptPrint[2][ulLanguageMode], printCount);

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_l2_mcast_static_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 i;
	int32 rc = 0;
	uint32 iMaxCount = 0;
	l2_mcast_group_t l2mcastEntry;
	char portmask_str[CAPA_PORTMASK_STR_LEN];
	char macStr[SYS_BUF32_LEN];

	rc = sal_l2_mcast_databaseSize_get(&iMaxCount); 
	if(rc != SYS_ERR_OK)
		return SYS_ERR_OK;
		
    for (i = 0; i < iMaxCount; i++)
    { 
    	SYS_MEM_CLEAR(l2mcastEntry);
    	SYS_MEM_CLEAR(portmask_str);
		rc = sal_l2_mcast_database_get(&l2mcastEntry, i);
		if(rc != SYS_ERR_OK)
		{
			continue;
		}
		LPM2STR(&l2mcastEntry.staticMbr, portmask_str);
		sys_util_mac2Str(l2mcastEntry.mac, macStr);
		osal_sprintf(cmd,"multicast address-table %s vlan %u interfaces %s\n", macStr, l2mcastEntry.vlanId, portmask_str);   
		CMD_WRITE_TO_FILE(fp, cmd);
	}
	
    return SYS_ERR_OK;
}

int32 _cmd_igmp_vlan_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char portmaskStr[CAPA_PORTMASK_STR_LEN];
    sys_vlanmask_t vmExist;
    sys_vlanmask_t enVlanMsk, fastLeaveVlanMsk, learnVlanMsk;
    sys_vid_t vid;
    mcast_vlan_entry_t mcastVlan;
    igmp_router_entry_t router;
    char strVlanList[CAPA_VLANLIST_STR_LEN];

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    sys_igmp_querier_t query;
    sys_vlanmask_t vlanmask;
#endif

    SYS_MEM_CLEAR(vmExist);
    SYS_MEM_CLEAR(enVlanMsk);
    SYS_MEM_CLEAR(fastLeaveVlanMsk);
    SYS_MEM_CLEAR(learnVlanMsk);

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV4, &vmExist));

    FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
    {
        osal_memset(&mcastVlan, 0, sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vid;
        mcastVlan.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CHK(sal_mcast_vlan_get(&mcastVlan));

        if (DFLT_IGMP_VLAN_EBL != mcastVlan.enable)
        {
            VLANMASK_SET_VID(enVlanMsk, vid);
        }

        if (DFLT_IGMP_IMMEDIATE_LEAVE  != mcastVlan.fastleave_enable)
        {
            VLANMASK_SET_VID(fastLeaveVlanMsk, vid);
        }

        if (DFLT_IGMP_ROUTER_PIMDVMRP_LEARN != mcastVlan.pimDvmrpLearn)
        {
            VLANMASK_SET_VID(learnVlanMsk, vid);
        }

        if (DFLT_IGMP_ROBUSTNESS_VAR != mcastVlan.robustnessVar)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip igmp snooping vlan %d robustness-variable %d\n", vid, mcastVlan.robustnessVar);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_IGMP_QUERY_RESPONSE_INTV != mcastVlan.responseTime)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip igmp snooping vlan %d response-time %d\n", vid, mcastVlan.responseTime);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_IGMP_QUERY_INTV != mcastVlan.queryIntv)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip igmp snooping vlan %d query-interval %d\n", vid, mcastVlan.queryIntv);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_IGMP_GSQUERY_RESPONSE_INTV  != mcastVlan.grpSpecificQueryIntv)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip igmp snooping vlan %d last-member-query-interval %d\n", vid, mcastVlan.grpSpecificQueryIntv);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_IGMP_ROBUSTNESS_VAR != mcastVlan.lastMmbrQueryCnt)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip igmp snooping vlan %d last-member-query-count %d\n", vid, mcastVlan.lastMmbrQueryCnt);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(mcastVlan.staticMbr))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(portmaskStr);

            LPM2STR(&mcastVlan.staticMbr, portmaskStr);

            osal_sprintf(cmd, "ip igmp snooping vlan %d static-port %s\n", vid, portmaskStr);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(mcastVlan.forbidMbr))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(portmaskStr);

            LPM2STR(&mcastVlan.forbidMbr, portmaskStr);

            osal_sprintf(cmd, "ip igmp snooping vlan %d forbidden-port %s\n", vid, portmaskStr);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    if (!IS_VLANMASK_CLEAR(enVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&enVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip igmp snooping vlan %s\n", (DFLT_IGMP_VLAN_EBL == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (!IS_VLANMASK_CLEAR(fastLeaveVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&fastLeaveVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip igmp snooping vlan %s immediate-leave\n", (DFLT_IGMP_IMMEDIATE_LEAVE == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (!IS_VLANMASK_CLEAR(learnVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&learnVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip igmp snooping vlan %s router learn pim-dvmrp\n", (DFLT_IGMP_ROUTER_PIMDVMRP_LEARN == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    
    SYS_MEM_CLEAR(vmExist);
    SYS_MEM_CLEAR(vlanmask);

    SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vmExist));
    SYS_ERR_CHK(sal_igmp_querier_vlanmask_get(&vlanmask));

    FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
    {
        if (!IS_STATIC_VLAN_EXIST(vid))
            continue;

        if (VLANMASK_IS_VIDSET(vlanmask, vid))
        {
            SYS_MEM_CLEAR(cmd);

            query.vid = vid;
            query.ipType = MULTICAST_TYPE_IPV4;
            CMD_ERR_CONTINUE_VLAN(sal_igmp_querier_get(&query), vid);

            if (IGMP_QUERY_V3 == query.version)
                osal_sprintf(cmd, "ip igmp snooping vlan %d querier version 3\n", vid);
            else
                osal_sprintf(cmd, "ip igmp snooping vlan %d querier version 2\n", vid);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }
#endif

    FOR_EACH_VID(vid)
    {
        router.vid = vid;
        router.ipType = MULTICAST_TYPE_IPV4;
        SYS_ERR_CHK(sal_mcast_router_get(&router));

        if (0 != router.vid)
        {
            
            if (!IS_LOGIC_PORTMASK_CLEAR(router.router_static_pmsk))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(portmaskStr);

                LPM2STR(&router.router_static_pmsk, portmaskStr);

                osal_sprintf(cmd, "ip igmp snooping vlan %d static-router-port %s\n", vid, portmaskStr);

                CMD_WRITE_TO_FILE(fp, cmd);
            }

            if (!IS_LOGIC_PORTMASK_CLEAR(router.router_forbid_pmsk))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(portmaskStr);

                LPM2STR(&router.router_forbid_pmsk, portmaskStr);

                osal_sprintf(cmd, "ip igmp snooping vlan %d forbidden-router-port %s\n", vid, portmaskStr);

                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_igmp_staticGroup_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char portmask_str[CAPA_PORTMASK_STR_LEN];
    char gipStr[CAPA_IPV4_STR_LEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    uint32 dbSize;
    uint32 i = 0;
    uint32 j = 0;
    uint32 m = 0;
    sys_logic_portmask_t exmbr;
    sys_vlanmask_t  vlanmask;
    sys_igmp_db_t db;
#ifdef CONFIG_SYS_PROTO_MVR
    sys_enable_t mvrEnable;
    uint32 groupStart, groupEnd;
    sys_vid_t mvrVlanId;
#endif
    int32 ret = SYS_ERR_OK;

typedef struct group_set_s
{
    uint32 dip;
    sys_vid_t vid;
    uint8 mac[CAPA_MAC_BYTE_NUM];
    sys_logic_portmask_t inMbr;
    sys_logic_portmask_t exMbr;
    uint8 used;
}group_set_t;

    group_set_t *pGroupSet = NULL;
    group_set_t tmpSet;

    osal_memset(cmd, 0, sizeof(cmd));
    osal_memset(portmask_str, 0, sizeof(portmask_str));
    osal_memset(gipStr, 0, sizeof(gipStr));
    osal_memset(strVlanList, 0, sizeof(strVlanList));

    SYS_ERR_HDL(sal_mcast_databaseSize_get(&dbSize), out, ret);
    if (0 == dbSize)
        return SYS_ERR_OK;

    pGroupSet = (group_set_t *)osal_alloc(sizeof(group_set_t) * dbSize);
    if (NULL == pGroupSet)
        return SYS_ERR_FAILED;

    osal_memset(pGroupSet, 0, sizeof(group_set_t) * dbSize);

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CONTINUE(sal_mcast_database_get(&db));
        if (MULTICAST_TYPE_IPV4 != db.ipType)
            continue;

        if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            continue;

        if (IS_LOGIC_PORTMASK_CLEAR(db.staticMbr))
            continue;

        LOGIC_PORTMASK_ANDNOT(exmbr, db.staticMbr, db.staticInMbr);
        pGroupSet[j].dip = db.dip;
        pGroupSet[j].vid = db.vid;
        osal_memcpy(&pGroupSet[j].inMbr, &db.staticInMbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(&pGroupSet[j].exMbr, &exmbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(pGroupSet[j].mac, db.mac, CAPA_MAC_BYTE_NUM);
        j++;
   }

#ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_HDL(sal_igmp_mvr_enable_get(&mvrEnable), out, ret);
    SYS_ERR_HDL(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd), out, ret);
    SYS_ERR_HDL(sal_igmp_mvr_vlanId_get(&mvrVlanId), out, ret);
#endif
   for (i = 0; i < j ; i++)
   {
        osal_memset(&vlanmask, 0, sizeof(sys_vlanmask_t));
        osal_memset(&tmpSet, 0, sizeof(group_set_t));

        for (m = i; m < j; m++)
        {
            if (0 == pGroupSet[m].used)
            {
                if (0 == tmpSet.vid)
                {
                    osal_memcpy(&tmpSet, &pGroupSet[m],sizeof(group_set_t));
                    VLANMASK_SET_VID(vlanmask, tmpSet.vid);
                    pGroupSet[m].used = 1;
                }
                else
                {
                    if (0 != tmpSet.dip)
                    {
                        if ((tmpSet.dip == pGroupSet[m].dip) && (IS_LOGIC_PORTMASK_EQUAL(tmpSet.inMbr, pGroupSet[m].inMbr)))
                        {
                            VLANMASK_SET_VID(vlanmask, pGroupSet[m].vid);
                            pGroupSet[m].used = 1;
                        }
                    }
                    else
                    {
                        if ( (0 == osal_memcmp(tmpSet.mac, pGroupSet[m].mac, CAPA_MAC_BYTE_NUM)) &&
                             (IS_LOGIC_PORTMASK_EQUAL(tmpSet.inMbr, pGroupSet[m].inMbr)))
                         {
                            VLANMASK_SET_VID(vlanmask, pGroupSet[m].vid);
                            pGroupSet[m].used = 1;
                         }
                    }
                }
            }
        }

        if (0 != tmpSet.vid)
        {

            SYS_ERR_CONTINUE(sys_util_ip2Str(tmpSet.dip, gipStr));
            osal_memset(portmask_str, 0, sizeof(portmask_str));
            LPM2STR(&tmpSet.inMbr, portmask_str);

            SYS_ERR_CONTINUE(VMSK2STR(&vlanmask, strVlanList));

            if (!IS_LOGIC_PORTMASK_CLEAR(tmpSet.inMbr))
            {
#ifdef CONFIG_SYS_PROTO_MVR
                if((ENABLED == mvrEnable) && (tmpSet.dip >= groupStart) && (tmpSet.dip <= groupEnd) && (tmpSet.vid == mvrVlanId))
                {
                    osal_sprintf(cmd,"mvr vlan %d group %s interfaces %s\n", tmpSet.vid, gipStr, portmask_str );
                }
                else
#endif
                {
                    if (IGMP_IN_MULTICAST(tmpSet.dip))
                        osal_sprintf(cmd,"ip igmp snooping vlan %s static-group %s interfaces %s\n",
                            strVlanList, gipStr,portmask_str );
                    else
                    {
                        SYS_ERR_CHK(sys_util_mac2Str(tmpSet.mac, macStr));
                        osal_sprintf(cmd,"ip igmp snooping vlan %s static-mac %s interfaces %s\n",
                            strVlanList, macStr, portmask_str );
                    }
                }

                CMD_WRITE_TO_FILE(fp, cmd);
            }
       }
   }

out:
    if (pGroupSet)
        osal_free(pGroupSet);

    return SYS_ERR_OK;
}

int32 cmd_igmp_group_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char gipStr[CAPA_IPV4_STR_LEN];
    char sipStr[CAPA_IPV4_STR_LEN];
    uint32 dbSize;
    uint32 i;
    sys_igmp_db_t db;
    sys_mcast_info_t info;

    SYS_MEM_CLEAR(info);

    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (IGMP_VERSION_V3_FULL == info.operVersion)
    {
        SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));

        for (i = 0; i < dbSize; i++)
        {
            SYS_MEM_CLEAR(db);
            SYS_MEM_CLEAR(gipStr);
            SYS_MEM_CLEAR(sipStr);

            db.indx = i;

            SYS_ERR_CONTINUE(sal_mcast_database_get(&db));

            if (IGMP_GROUP_ENTRY_DYNAMIC == db.form)
                continue;

            if (0 == db.sip)
                continue;

            SYS_ERR_CONTINUE(sys_util_ip2Str(db.dip, gipStr));
            SYS_ERR_CONTINUE(sys_util_ip2Str(db.sip, sipStr));

            osal_sprintf(cmd,"ip igmp snooping group %s filter %s\n", gipStr, sipStr);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    SYS_ERR_CHK(_cmd_igmp_staticGroup_save(fp));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 _cmd_igmp_profile_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_igmp_profileId_t proIdx ;
    sys_igmp_profile_t    profileEntry;
    char lowIpStr[CAPA_IPV4_STR_LEN];
    char highIpStr[CAPA_IPV4_STR_LEN];
    char strIpv6Low[CAPA_IPV6_STR_LEN];
    char strIpv6High[CAPA_IPV6_STR_LEN];

    for (proIdx = BOND_MCAST_PROFILE_MIN; proIdx <= BOND_MCAST_PROFILE_MAX; proIdx++)
    {
        osal_memset(&profileEntry, 0, sizeof(sys_igmp_profile_t));

        SYS_ERR_CONTINUE(sal_igmp_profile_get(proIdx, &profileEntry));

        osal_memset(cmd, 0, osal_strlen(cmd));
        
        osal_sprintf(cmd, "ip igmp profile %d\n", profileEntry.profileId);
        CMD_WRITE_TO_FILE(fp, cmd);

        osal_memset(cmd, 0, osal_strlen(cmd));

        if (MULTICAST_TYPE_IPV4 == profileEntry.ipType)
        {
            SYS_ERR_CHK(sys_util_ip2Str(profileEntry.lowIp, lowIpStr));
            SYS_ERR_CHK(sys_util_ip2Str(profileEntry.highIp, highIpStr));
            
            if (profileEntry.lowIp == profileEntry.highIp)
                osal_sprintf(cmd, "profile range ip %s action %s \n", lowIpStr, profileActStr[profileEntry.act]);
            else
                osal_sprintf(cmd, "profile range ip %s %s action %s \n", lowIpStr, highIpStr, profileActStr[profileEntry.act]);
        }
        else
        {
            inet_ntop(AF_INET6, profileEntry.ipv6Low, strIpv6Low, sizeof(strIpv6Low));
            inet_ntop(AF_INET6, profileEntry.ipv6High, strIpv6High, sizeof(strIpv6High));

            if (0 == osal_memcmp(profileEntry.ipv6Low, profileEntry.ipv6High, CAPA_IPV6_BYTE_NUM))
                osal_sprintf(cmd, "profile range ipv6 %s action %s \n", strIpv6Low, profileActStr[profileEntry.act]);
            else
                osal_sprintf(cmd, "profile range ipv6 %s %s action %s \n", strIpv6Low, strIpv6High, profileActStr[profileEntry.act]);

        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_igmp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    igmp_unknMcastAction_t val;
    sys_enable_t enable;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;
#ifdef CONFIG_SYS_PROTO_MVR
    mvr_mode_t mvrMode;
    sys_vid_t mvrVlan;
    uint32 groupStart, groupEnd;
    uint32 cnt = 1;
    char groupStr[CAPA_IPV4_STR_LEN];
    uint32 queryTime;
#endif

    SYS_ERR_CHK(sal_igmp_enable_get(&enable));

    if (DFLT_IGMP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%sip igmp snooping\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_suppre_enable_get(&enable));

    if (DFLT_IGMP_SUPPRE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%sip igmp snooping report-suppression\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_lookupMode_get(&mode));

    if (DFLT_IGMP_LOOKUP_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ip igmp snooping forward-method %s\n", gLookupMode[mode]);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&val));

    if (DFLT_IGMP_UNKNOWN_MCAST_ACTION != val)
    {
        SYS_MEM_CLEAR(cmd);

        if (val == IGMP_UNKNOWN_MCAST_ACTION_DROP)
            osal_sprintf(cmd, "ip igmp snooping unknown-multicast action drop\n");
        else if (val == IGMP_UNKNOWN_MCAST_ACTION_FLOOD)
            osal_sprintf(cmd, "ip igmp snooping unknown-multicast action flood\n");
        else if (val == IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT)
            osal_sprintf(cmd, "ip igmp snooping unknown-multicast action router-port\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(info);
    SYS_ERR_CHK(sal_igmp_info_get(&info));

    if (DFLT_IGMP_OPERATION_VER != info.operVersion)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ip igmp snooping version ");

        switch(info.operVersion)
        {
            case IGMP_VERSION_V2:
                osal_strcat(cmd, "2\n");
                break;

            case IGMP_VERSION_V3_BASIC:
                osal_strcat(cmd, "3\n");
                break;

            case IGMP_VERSION_V3_FULL:
                osal_strcat(cmd, "3\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(_cmd_igmp_vlan_save(fp));

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    
    SYS_ERR_CHK(_cmd_igmp_profile_save(fp));
#endif
#ifdef CONFIG_SYS_PROTO_MVR
    
    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));

    if (DFLT_IGMP_MVR_ENABLED != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%smvr\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

   SYS_ERR_CHK(sal_igmp_mvr_mode_get(&mvrMode));
   if (DFLT_IGMP_MVR_MODE != mvrMode)
   {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "mvr mode %s\n", (MVR_MODE_COMPATABLE == mvrMode) ? "compatible" : "dynamic");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
    if (DFLT_IGMP_MVR_VLAN != mvrVlan)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "mvr vlan %d\n", mvrVlan);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd));
    if ( 0 != groupStart)
    {
        SYS_MEM_CLEAR(cmd);

        cnt = groupEnd - groupStart + 1;
        SYS_ERR_CHK(sys_util_ip2Str(groupStart, groupStr));
        osal_sprintf(cmd, "mvr group %s %d\n", groupStr, cnt);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_mvr_querytime_get(&queryTime));
    if (DFLT_IGMP_MVR_QUERYTIME != queryTime)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "mvr query-time %d\n", queryTime);

        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}

int32 cmd_igmp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
#if defined CONFIG_SYS_PROTO_IGMP_FILTERING
    uint16 val = 0;
#endif
#ifdef CONFIG_SYS_PROTO_MVR
    mvr_portType_t portType;
    sys_enable_t   enable;
#endif

    SYS_MEM_CLEAR(cmd);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    igmp_maxGroupAct_t act;
    
    SYS_ERR_CHK(sal_igmp_portGroup_limit_get(port, &val));

    if (DFLT_MCAST_PORT_LIMIT != val)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ip igmp max-groups %d\n", val);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_portGroup_excessAct_get(port, &act));

    if (DFLT_IGMP_MAX_GROUP_ACT != act)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ip igmp max-groups action %s\n", (IGMP_ACT_REPLACE == act) ? "replace" : "deny");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#if defined CONFIG_SYS_PROTO_IGMP_FILTERING
    
    SYS_ERR_CHK(sal_igmp_port_filter_get(port, &val));

    if (INVALID_PROFILE_ID != val)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ip igmp filter %d\n", val);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));
    if (DFLT_IGMP_MVR_PORT_TYPE != portType)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "mvr type %s\n",  MVR_TYPE_RECEIVER == portType ? "receiver" : "source");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_mvr_portImmediate_get(port, &enable));
    if (DFLT_IGMP_MVR_IMMEDIATE_LEAVE != enable)
    {
        SYS_MEM_CLEAR(cmd);

        if (ENABLED == enable)
            osal_sprintf(cmd, "mvr immediate-leave\n");
        else
            osal_sprintf(cmd, "no mvr immediate-leave\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_MVR

int32 cmd_igmp_mvr_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_CLI_RUN())
    {
        char confirm[4];
        sys_enable_t enable, setEnable = ENABLED;

        if (CMD_IS_NO_FORM_EXIST())
            setEnable = DISABLED;

        SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
        if (setEnable == enable)
            return SYS_ERR_OK;

        if (CMD_IS_NO_FORM_EXIST())
            XMORE("The operation will make mvr setting is default. Continue? [yes/no]:");
        else
            XMORE("The operation will delete groups of VLAN ID is MVR VLAN include static groups. Continue? [yes/no]:");

        SYS_MEM_CLEAR(confirm);
        input_str(confirm, 3, 0);

        if (0 == osal_strlen(confirm) || 0 == osal_strncmp(confirm, "yes", osal_strlen(confirm)))
        {
            SYS_ERR_CHK(sal_igmp_mvr_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));
        }

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_igmp_mvr_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 startGroupAddr =0;
    uint32 endGroupAddr =0;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    if (DISABLED == enable)
        return SYS_ERR_MVR_ENABLE;

    SYS_ERR_CHK(sys_util_str2Ip(argv[0], &startGroupAddr));
    if ((ntohl(startGroupAddr) >= ntohl(0xE0000000L)) && (ntohl(startGroupAddr) <= ntohl(0xE00000FFL)))
    {
        return SYS_ERR_IGMP_RSV_ADDR;
    }

    if (CMD_IS_CLI_RUN())
    {
        char confirm[4];

        SYS_MEM_CLEAR(confirm);

        if (ENABLED == enable)
        {
            XMORE("The operation will delete the MVR VLAN groups include static MVR groups.Continue? [yes/no]:");

            SYS_MEM_CLEAR(confirm);
            input_str(confirm, 3, 0);
        }

        if (!(0 == osal_strlen(confirm) || 0 == osal_strncmp(confirm, "yes", osal_strlen(confirm))))
        {
            return SYS_ERR_OK;
        }
    }

    if (2 == argc)
    {
        endGroupAddr = startGroupAddr + SYS_STR2UINT(argv[1]) - 1;
    }
    else
    {
        endGroupAddr = startGroupAddr;
    }

    SYS_ERR_CHK(sal_igmp_mvr_groupRange_set(startGroupAddr, endGroupAddr));

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t vlanId;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    if (DISABLED == enable)
        return SYS_ERR_MVR_ENABLE;

    if (CMD_IS_CLI_RUN())
    {
        char confirm[4];

        SYS_MEM_CLEAR(confirm);

        if (ENABLED == enable)
        {
            XMORE("The operation will delete the old and new MVR VLAN groups include static MVR groups.Continue? [yes/no]:");
            input_str(confirm, 3, 0);
        }

        if (!(0 == osal_strlen(confirm) || 0 == osal_strncmp(confirm, "yes", osal_strlen(confirm))))
        {
            return SYS_ERR_OK;
        }
    }

    vlanId = (sys_vid_t)SYS_STR2UINT(argv[0]);
    SYS_ERR_CHK(sal_igmp_mvr_vlanId_set(vlanId));

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mvr_mode_t mode = MVR_MODE_COMPATABLE;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    if (DISABLED == enable)
        return SYS_ERR_MVR_ENABLE;

    if (argv[0][0] == 'd')
        mode = MVR_MODE_DYNAMIC;
    else if (argv[0][0] == 'f')
        mode = MVR_MODE_COMPATABLE;

    SYS_ERR_CHK(sal_igmp_mvr_mode_set(mode));

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_queryTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 querytime;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    if (DISABLED == enable)
        return SYS_ERR_MVR_ENABLE;

    if (CMD_IS_NO_FORM_EXIST())
        querytime = DFLT_IGMP_MVR_QUERYTIME;
    else
        querytime = SYS_STR2UINT(argv[0]) ;

    SYS_ERR_CHK(sal_igmp_mvr_querytime_set(querytime));

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_portType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mvr_portType_t type = MVR_TYPE_NONE;
    sys_logic_port_t port;

    sys_enable_t enable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&enable));
    if (DISABLED == enable)
        return SYS_ERR_MVR_ENABLE;

     if (CMD_IS_NO_FORM_EXIST())
        type = MVR_TYPE_NONE;
     else
     {
        if (argv[0][0] == 's')
            type = MVR_TYPE_SOURCE;
        else if (argv[0][0] == 'r')
            type = MVR_TYPE_RECEIVER;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_igmp_mvr_portType_set(port, type), port);
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_portFastLeave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t port;
    sys_enable_t mvrEnable;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEnable));
    if (DISABLED == mvrEnable)
        return SYS_ERR_MVR_ENABLE;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
     else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_igmp_mvr_portImmediate_set(port, enable), port);
    }

    return SYS_ERR_OK;
}

int32 cmd_igmp_mvr_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8  isAdd = TRUE;
    sys_enable_t mvrEnable;
    sys_vid_t  mvrVlan, vid;
    uint32 groupStart, groupEnd;
    uint32 groupAddr;
    igmp_group_entry_t  groupEntry;
    sys_logic_portmask_t logicPortmask;
    mvr_portType_t portType;
    sys_logic_port_t port;
    mvr_mode_t mvrMode;

    vid = (sys_vid_t)SYS_STR2UINT(argv[0]);
    SYS_ERR_CHK(sys_util_str2Ip(argv[1], &groupAddr));
    CMD_PORTLIST_GET(2, &logicPortmask);

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&mvrEnable));
    if (DISABLED == mvrEnable)
        return SYS_ERR_MVR_ENABLE;

    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
    if (vid !=  mvrVlan)
        return SYS_ERR_MVR_VLAN_STATIC_GROUP_SET;

    SYS_ERR_CHK(sal_igmp_mvr_groupRange_get(&groupStart, &groupEnd));
    if (groupAddr < groupStart || groupAddr > groupEnd)
        return SYS_ERR_MVR_STATIC_GROUP_CONFLICT;

    SYS_ERR_CHK(sal_igmp_mvr_mode_get(&mvrMode));
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &portType));
        if (MVR_TYPE_NONE == portType)
            return SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE;
        if (MVR_MODE_COMPATABLE == mvrMode)
        {
            if (MVR_TYPE_RECEIVER != portType)
                return SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE;
        }
    }

    osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
    groupEntry.dip = groupAddr;
    groupEntry.ipType = MULTICAST_TYPE_IPV4;
    groupEntry.vid = vid;
    osal_memcpy(&groupEntry.mbr, &logicPortmask,sizeof(sys_logic_portmask_t));

    SYS_ERR_CHK(sal_mcast_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, isAdd));

    return SYS_ERR_OK;
}

#endif

