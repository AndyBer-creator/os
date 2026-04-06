
#include <libsal/sal_mld.h>
#include <libsal/sal_igmp.h>
#include <libcmd/cmd.h>

sys_igmp_profileId_t g_mldProfileIdx;

int32 cmd_mld_static_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 groupAddr =0;
    sys_vlanmask_t  vlanmask;
    uint8 groupIpv6Addr[CAPA_IPV6_BYTE_NUM];
    igmp_group_entry_t groupEntry;
    sys_vid_t vid;

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    if (1 != inet_pton(AF_INET6, argv[1], groupIpv6Addr))
        return SYS_ERR_IPV6_ADDR;

    if (groupIpv6Addr[0] != 0xff)
    {
        return SYS_ERR_MCAST_ADDR;
    }

    groupAddr = IPV6_TO_IPV4(groupIpv6Addr);

    FOR_EACH_SYSTEM_VID_IN_VLANMASK(vid, vlanmask)
    {
        osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
        groupEntry.vid = vid;
        groupEntry.sip = 0;
        groupEntry.ipType = MULTICAST_TYPE_IPV6;
        groupEntry.dip = groupAddr;
        osal_memcpy(groupEntry.dipv6.addr, groupIpv6Addr,CAPA_IPV6_BYTE_NUM);

        SYS_ERR_CONTINUE(sal_mcast_static_group_clear(&groupEntry));
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mld_groupByType_clear(type));

    return SYS_ERR_OK;

}

int32 cmd_mld_statistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_mld_statistics_clear());

    return SYS_ERR_OK;
}

int32 cmd_mld_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mcast_info_t          info;
    igmp_lookup_mode_t mode;
    igmp_unknMcastAction_t unknMcastAction;
    sys_enable_t mldEnable;
    int32 i;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));
    SYS_ERR_CHK(sal_mld_enable_get(&mldEnable));
    SYS_ERR_CHK(sal_mld_info_get(&info));
    SYS_ERR_CHK(sal_mld_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&unknMcastAction));

    XMORE("\n\n\t\tMLD Snooping Status\n");
    XMORE("\t\t--------------------\n\n");

    XMORE("    Snooping                        : %sabled\n", mldEnable ? "En" : "Dis");
    XMORE("    Report Suppression              : %sabled\n", info.suppreEnable ? "En" : "Dis");
    XMORE("    Operation Version               : %s \n", (info.oper_version >= MLD_VERSION_V2_BASIC) ? "v2" : "v1");
    XMORE("    Forward Method                  : %s \n", gLookupMode[mode]);

    XMORE("    Unknown IPv6 Multicast Action   : %s\n", gUnknownMcastAct[unknMcastAction]);

    XMORE("\n\n\t\tPacket Statistics\n");
    for (i = 0; i < MCAST_STAT_END; i++)
    {
        XMORE ("    %-32s :  %d \n",  mcastStatCntName[i], info.totStat[i]);
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_router_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char port_str[CAPA_PORTMASK_STR_LEN];
    uint32 isShowStatic = FALSE;
    uint32 isShowDynamic = FALSE;
    uint32 isShowForbbiden = FALSE;
    uint32 cnt = 0;
    sys_vid_t vid;
    sys_vlanmask_t vlanMask;
    igmp_router_entry_t router;
    sys_logic_port_t port;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

    VLANMASK_CLEAR_ALL(vlanMask);

    if(1 == argc)
    {
        if( ('s' == argv[0][0]))
            isShowStatic = TRUE;

        if(('d' == argv[0][0]))
            isShowDynamic = TRUE;

        if(('f' == argv[0][0]))
            isShowForbbiden = TRUE;
    }
    else
    {
        isShowStatic = TRUE;
        isShowDynamic = TRUE;
        isShowForbbiden = TRUE;
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

int32 cmd_mld_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strIpv6AddrSip[CAPA_IPV6_STR_LEN];
    char strIpv6AddrDip[CAPA_IPV6_STR_LEN];
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

    SYS_ERR_CHK(sal_mld_lookupMode_get(&mode));
    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));
    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&action));

    if(IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE == mode)
    {
        XMORE(" VLAN |            Group IP Address            |  Type  | Life(Sec) | Port      \n");
        XMORE("------+----------------------------------------+--------+-----------+------------------\n");
    }
    else
    {
        XMORE(" VLAN |            Group IP Address            |              SIP Address               |  Type  | Life(Sec) | Port \n");
        XMORE("------+----------------------------------------+----------------------------------------+--------+-----------+------------------\n");
    }

    for (i = 0; i < dbSize; i++)
    {
        osal_memset(&db,0,sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CHK(sal_mcast_database_get(&db));

        if (MULTICAST_TYPE_IPV6 != db.ipType)
            continue;

        if((IGMP_GROUP_ENTRY_DYNAMIC == form) && (IGMP_GROUP_ENTRY_DYNAMIC != db.form))
            continue;

        if((IGMP_GROUP_ENTRY_STATIC == form) && (IGMP_GROUP_ENTRY_STATIC != db.form))
            continue;

        if (!MLD_IN_MULTICAST(db.dipv6))
            continue;

        cnt ++;
        
        inet_ntop(AF_INET6, db.dipv6, strIpv6AddrDip, sizeof(strIpv6AddrDip));

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

            XMORE(" %4d | %38s |%8s| %4s      | %s\n", db.vid, strIpv6AddrDip,
                  (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                  lifeStr, portInfo);
        }
        else
        {
            inet_ntop(AF_INET6, db.sipv6, strIpv6AddrSip, sizeof(strIpv6AddrSip));
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

            XMORE(" %4d | %38s | %38s |%8s| %4s      | %s\n",db.vid, strIpv6AddrDip, strIpv6AddrSip,
                (db.form == IGMP_GROUP_ENTRY_STATIC) ? "Static" : "Dynamic",
                lifeStr, portInfo);
        }
    }

    XMORE("\n\nTotal Number of Entry = %d\n\n", cnt);

    return SYS_ERR_OK;
}

int32 cmd_mld_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    mcast_vlan_entry_t  mcastVlan;
    sys_vlanmask_t  vlanmask;
    sys_vid_t vlanId;
    sys_enable_t mldEnable, mcastFilterEnable = ENABLED;

    XMORE("\n");

    if (1 == argc)
    {
        SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));
    }
    else
    {
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vlanmask));
    }

    SYS_ERR_CHK(sal_mld_enable_get(&mldEnable));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        if (!IS_STATIC_VLAN_EXIST(vlanId))
            continue;

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;
        SYS_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan));

        XMORE("MLD Snooping is globaly %s \n", mldEnable ? "enabled" : "disabled");
        XMORE("MLD Snooping VLAN %d admin : %s \n", mcastVlan.vid, mcastVlan.enable ? "enabled" : "disabled");
        XMORE("MLD Snooping oper mode : %s \n", text_enable[mcastVlan.enable && mldEnable && mcastFilterEnable].text);
        XMORE("MLD Snooping robustness: admin %d oper %d\n",  mcastVlan.robustnessVar,  mcastVlan.operRobNessVar);
        XMORE("MLD Snooping query interval: admin %d sec oper %d sec\n",  mcastVlan.queryIntv, mcastVlan.operQueryIntv);
        XMORE("MLD Snooping query max response : admin %d sec oper  %d sec\n",  mcastVlan.responseTime, mcastVlan.operRespTime);
        XMORE("MLD Snooping last member query counter: admin %d  oper %d \n",  mcastVlan.lastMmbrQueryCnt, mcastVlan.operLastMmbrQueryCnt);
        XMORE("MLD Snooping last member query interval: admin %d sec  oper %d sec\n",  mcastVlan.grpSpecificQueryIntv, mcastVlan.operGsqueryIntv);
        XMORE("MLD Snooping immediate leave: %s \n",  mcastVlan.fastleave_enable ? "enabled" : "disabled");
        XMORE("MLD Snooping automatic learning of multicast router ports: %s \n\n",  mcastVlan.pimDvmrpLearn ? "enabled" : "disabled");
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

int32 cmd_mld_forward_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;
        SYS_ERR_CONTINUE(sal_mcast_vlan_get(&mcastVlan));

        XMORE("MLD Snooping VLAN           : %d  \n", mcastVlan.vid);
        osal_memset(port_str, 0, sizeof(port_str));
        LPM2STR(&mcastVlan.staticMbr, port_str);
        XMORE("MLD Snooping static port    : %s \n",  IS_LOGIC_PORTMASK_CLEAR(mcastVlan.staticMbr) ? "None" : port_str);
        osal_memset(port_str, 0, sizeof(port_str));
        LPM2STR(&mcastVlan.forbidMbr, port_str);
        XMORE("MLD Snooping forbidden port : %s \n\n",  IS_LOGIC_PORTMASK_CLEAR(mcastVlan.forbidMbr) ? "None" : port_str);

    }

    XMORE("\n");

    return SYS_ERR_OK;
}
#endif

int32 cmd_mld_groupCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 dbSize = 0;

    SYS_ERR_CHK(sal_mld_databaseSize_get(&dbSize));
    XMORE("Total ipv6 mld snooping group number: %u\n", dbSize);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cmd_mld_maxGroup_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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
        SYS_ERR_CHK(sal_mld_portGroup_limit_get(port,&limitNum));
        XMORE("%8s : %d \n", strPort, limitNum);
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_maxGroupAction_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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
        SYS_ERR_CHK(sal_mld_portGroup_excessAct_get(port,&act));
        XMORE("%8s : %s \n", strPort, act == IGMP_ACT_REPLACE ? "replace" : "deny");
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cmd_mld_filter_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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
        SYS_ERR_CHK(sal_mld_port_filter_get(port,&proIdx));
        if ( INVALID_PROFILE_ID == proIdx)
            XMORE("%8s : %s \n", strPort, "None");
        else
            XMORE("%8s : %d \n", strPort,   proIdx);
    }
    return SYS_ERR_OK;
}

int32 cmd_mld_profile_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 found =FALSE;
    sys_igmp_profileId_t sidx = 0, eidx = 0;

    sys_igmp_profileId_t profileId;
    sys_igmp_profile_t profileInfo;

    char strIpv6Low[CAPA_IPV6_STR_LEN];
    char strIpv6High[CAPA_IPV6_STR_LEN];

    profileId = 1;

    if (0 != argc)
    {
        profileId = SYS_STR2UINT(argv[0]);
        sidx = eidx = profileId;
    }
    else
    {
        if (MLD_PROFILE_NODE == vty->node)
        {
            profileId = g_mldProfileIdx;
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
        if(SYS_ERR_OK != sal_mld_profile_get(profileId, &profileInfo))
            return SYS_ERR_MLD_PROFILE_NOT_EXIST;

        found = TRUE;
        XMORE("IPv6 mld profile index: %d\n", profileId);
        XMORE("IPv6 mld profile action: %s\n", profileActStr[profileInfo.act]);

        inet_ntop(AF_INET6, profileInfo.ipv6Low, strIpv6Low, sizeof(strIpv6Low));
        inet_ntop(AF_INET6, profileInfo.ipv6High, strIpv6High, sizeof(strIpv6High));
        XMORE("Range low ip: %s\n", strIpv6Low);
        XMORE("Range high ip: %s\n", strIpv6High);
        XMORE("\n");
    }
    else
    {
        for (profileId = BOND_MCAST_PROFILE_MIN; profileId <= BOND_MCAST_PROFILE_MAX;  profileId++)
        {
            SYS_ERR_CONTINUE(sal_mld_profile_get(profileId, &profileInfo));
            found = TRUE;

            XMORE("IPv6 mld profile index: %d\n", profileId);
            XMORE("IPv6 mld profile action: %s\n", profileActStr[profileInfo.act]);

            inet_ntop(AF_INET6, profileInfo.ipv6Low, strIpv6Low, sizeof(strIpv6Low));
            inet_ntop(AF_INET6, profileInfo.ipv6High, strIpv6High, sizeof(strIpv6High));
            XMORE("Range low ip: %s\n", strIpv6Low);
            XMORE("Range high ip: %s\n", strIpv6High);
            XMORE("\n");
        }
    }

    if (FALSE == found)
        return SYS_ERR_MLD_PROFILE_EMPTY;

    return SYS_ERR_OK;
}
#endif

int32 cmd_mld_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_mld_enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_mld_unknMcastAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

int32 cmd_mld_lookupMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mcast_info_t info;

    SYS_ERR_CHK(sal_mld_info_get(&info));

    if (argv[0][0] == 'm')
    {
        SYS_ERR_CHK(sal_mld_lookupMode_set(IGMP_DMAC_VID_MODE));
        if (MLD_VERSION_V2_FULL == info.oper_version)
            SYS_ERR_CHK(sal_mld_operVersion_set(MLD_VERSION_V2_BASIC));
    }
    else if (argv[0][0] == 'd')
    {
        SYS_ERR_CHK(sal_mld_lookupMode_set(IGMP_DIP_VID_MODE));
    }
    else
    {
        SYS_ERR_CHK(sal_mld_lookupMode_set(IGMP_DIP_SIP_MODE));

        if (MLD_VERSION_V2_BASIC == info.oper_version)
        {
            SYS_ERR_CHK(sal_mld_operVersion_set(MLD_VERSION_V1));
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_suppre_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_mld_suppre_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_mld_operVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 operVersion;
    sys_mcast_info_t info;
    igmp_lookup_mode_t mode;

    osal_memset(&info, 0, sizeof(sys_mcast_info_t));
    SYS_ERR_CHK(sal_mld_info_get(&info));

    if (argv[0][0] == '2')
    {
        SYS_ERR_CHK(sal_mld_lookupMode_get(&mode));
        if (IGMP_DMAC_VID_MODE == mode || IGMP_DIP_VID_MODE ==  mode)
            operVersion = MLD_VERSION_V2_BASIC;
        else
        {
            operVersion = MLD_VERSION_V2_FULL;
            XMORE("The forward method is src-dst-ip ! can't support mld version 2!\n");
            return SYS_ERR_OK;
        }
    }
    else
    {
        operVersion = MLD_VERSION_V1;
    }

    if (operVersion == info.oper_version)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_mld_operVersion_set(operVersion));

    return SYS_ERR_OK;
}

int32 cmd_mld_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

int32 cmd_mld_static_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        #if 0
        if (!IS_VLAN_OPER_ENTRY_EXIST(vlanId))
        {
            XMORE("Vlan %d not exist, can't configure static port!\n", vlanId);
            continue;
        }
        #endif

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

int32 cmd_mld_forbidden_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        #if 0
        if (!IS_VLAN_OPER_ENTRY_EXIST(vlanId))
        {
            XMORE("Vlan %d not exist, can't configure forbidden port!\n", vlanId);
            continue;
        }
        #endif

        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

int32 cmd_mld_routerPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t port;
    sys_logic_portmask_t logicPortmask;
    sys_logic_portmask_t trunkMbr,tmpMbr;
    sys_logic_portmask_t setPortmask;
    sys_vid_t  vlanId;
    sys_vlanmask_t vlanmask;
    igmp_router_entry_t routerEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

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
            #if 0
            if(FALSE == sal_vlan_operEntryExist_ret(vlanId))
                XMORE("[MLD WARNING]:VLAN %d is not exist. This entry may not take effect\n", vlanId);
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

int32 cmd_mld_routerPortForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    
    sys_logic_portmask_t logicPortmask;
    sys_logic_portmask_t setPortmask;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    igmp_router_entry_t routerEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

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
            #if 0
            if(FALSE == sal_vlan_operEntryExist_ret(vlanId))
                XMORE("[IGMP WARNING]:VLAN %d is not exist. This entry may not take effect\n", vlanId);
            #endif

            LOGIC_PORTMASK_OR(setPortmask,routerEntry.router_forbid_pmsk,logicPortmask);
        }

        SYS_ERR_CHK(sal_mcast_router_forbid_port_set(vlanId, ipType, setPortmask));
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_mld_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 isAdd = TRUE;
    uint8  groupIpv6Addr[CAPA_IPV6_BYTE_NUM];
    uint8  zeroIpv6Addr[CAPA_IPV6_BYTE_NUM];
    sys_logic_portmask_t   logicPortmask;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;
    igmp_group_entry_t  groupEntry;

    osal_memset(zeroIpv6Addr,0,sizeof(zeroIpv6Addr));
    osal_memset(groupIpv6Addr, 0, sizeof(groupIpv6Addr));
    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    if (1 != inet_pton(AF_INET6, argv[1], groupIpv6Addr))
        return SYS_ERR_IPV6_ADDR;

    CMD_PORTLIST_GET(2, &logicPortmask);

    if (IS_LOGIC_PORTMASK_CLEAR(logicPortmask))
    {
        return SYS_ERR_INPUT;
    }

    if (CMD_IS_NO_FORM_EXIST())
        isAdd = FALSE;

    if (groupIpv6Addr[0] != 0xff)
    {
        return SYS_ERR_MCAST_ADDR;
    }

    if ((groupIpv6Addr[1] == 0x0 || groupIpv6Addr[1] == 0x01) &&
          (osal_memcmp(&groupIpv6Addr[2], &zeroIpv6Addr[2] ,IPV6_ADDR_LEN-2) == 0)  )
    {
        return SYS_ERR_IGMP_RSV_ADDR;
    }

    osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
    groupEntry.ipType = MULTICAST_TYPE_IPV6;
    groupEntry.dip = IPV6_TO_IPV4(groupIpv6Addr);
    osal_memcpy(groupEntry.dipv6.addr, groupIpv6Addr, IPV6_ADDR_LEN);
    osal_memcpy(&groupEntry.mbr, &logicPortmask,sizeof(sys_logic_portmask_t));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        #if 0
        if (isAdd)
        {
            if (FALSE == sal_vlan_operEntryExist_ret(vlanId))
            {
                XMORE("[MLD WARNING]:VLAN %d is not exist. This entry may not take effect\n", vlanId);
            }
        }
        #endif

        groupEntry.vid = vlanId;
        SYS_ERR_CHK(sal_mld_static_group_set(&groupEntry, FILTER_MODE_INCLUDE, isAdd));
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_fastleave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

int32 cmd_mld_pimDvmrpLearn_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

        if (!VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }
        CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
        if (routerLearn == mcastVlan.pimDvmrpLearn)
            continue;

        mcastVlan.pimDvmrpLearn = routerLearn;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_robustnessVar_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

int32 cmd_mld_responseTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

        if (VLANMASK_IS_VIDSET(vmExist, vlanId))
        {
            CMD_ERR_CONTINUE_VLAN(sal_mcast_vlan_get(&mcastVlan),vlanId);
            if (val == mcastVlan.responseTime)
                continue;
        }
        else
        {
            SYS_ERR_CHK(sal_mcast_vlan_add(&mcastVlan));
        }
        mcastVlan.responseTime = val;
        SYS_ERR_CHK(sal_mcast_vlan_set(&mcastVlan));
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_queryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

int32 cmd_mld_lastMbrQueryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

int32 cmd_mld_lastMbrQueryCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6,&vmExist));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        osal_memset(&mcastVlan,0,sizeof(mcast_vlan_entry_t));
        mcastVlan.vid = vlanId;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;

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

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cmd_mld_portGroup_limit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint16 maxNum;
    sys_logic_port_t port;

    if (CMD_IS_NO_FORM_EXIST())
        maxNum = DFLT_MCAST_PORT_LIMIT;
    else
        maxNum = (uint16)SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_mld_portGroup_limit_set(port, maxNum), port);
    }

    return SYS_ERR_OK;
}

int32 cmd_mld_portGroup_excessAct_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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
        CMD_ERR_CONTINUE_PORT(sal_mld_portGroup_excessAct_set(port, act), port);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cmd_mld_profile_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    g_mldProfileIdx = SYS_STR2UINT(argv[0]);

    vty->node = MLD_PROFILE_NODE;

    return SYS_ERR_OK;
}

int32 cmd_mld_profileEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_igmp_profile_t profileEntry;
    sys_igmp_profileId_t  proIdx;
    sys_logic_portmask_t  logicPmsk;
    sys_logic_port_t port;

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);

    osal_memset(&profileEntry, 0 , sizeof(sys_igmp_profile_t));
    profileEntry.profileId = g_mldProfileIdx;
    profileEntry.valid = 1;
    profileEntry.ipType = MULTICAST_TYPE_IPV6;

    if (1 != inet_pton(AF_INET6, argv[0], profileEntry.ipv6Low))
        return SYS_ERR_IPV6_ADDR;

    if (3 == argc)
    {
         if (1 != inet_pton(AF_INET6, argv[1], profileEntry.ipv6High))
            return SYS_ERR_IPV6_ADDR;

        if (osal_memcmp(profileEntry.ipv6High, profileEntry.ipv6Low, CAPA_IPV6_BYTE_NUM) < 0)
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
        osal_memcpy(profileEntry.ipv6High, profileEntry.ipv6Low, CAPA_IPV6_BYTE_NUM);

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

    if (profileEntry.ipv6Low[0] != 0xff)
    {
        return SYS_ERR_MCAST_ADDR;
    }

    if (profileEntry.ipv6High[0] != 0xff)
    {
        return SYS_ERR_MCAST_ADDR;
    }

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_mld_port_filter_get(port, &proIdx));
        if (proIdx == g_mldProfileIdx)
        {
            LOGIC_PORTMASK_SET_PORT(logicPmsk, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(logicPmsk))
    {
        return SYS_ERR_IGMP_PROFILE_USED;
    }

    SYS_ERR_CHK(sal_mld_profile_set(g_mldProfileIdx, &profileEntry));

    return SYS_ERR_OK;
}

int32 cmd_mld_profile_remove(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_igmp_profileId_t profileIdx, tmpIdx;
    sys_igmp_profile_t profileEntry;
    sys_logic_port_t port;
    sys_logic_portmask_t logicPmsk;

    LOGIC_PORTMASK_CLEAR_ALL(logicPmsk);

    profileIdx = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_mld_profile_get(profileIdx, &profileEntry));

    osal_memset(&profileEntry, 0, sizeof(sys_igmp_profile_t));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_mld_port_filter_get(port, &tmpIdx));
        if (tmpIdx == profileIdx)
        {
            LOGIC_PORTMASK_SET_PORT(logicPmsk, port);
        }
    }

    if (!IS_LOGIC_PORTMASK_CLEAR(logicPmsk))
    {
        return SYS_ERR_MLD_PROFILE_USED;
    }

    SYS_ERR_CHK(sal_mld_profile_set(profileIdx, &profileEntry));

    return SYS_ERR_OK;
}

int32 cmd_mld_filter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
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

        SYS_ERR_CHK(sal_mld_profile_get(proIdx, &profile));
         if (0 == profile.valid)
            return SYS_ERR_MLD_PROFILE_NOT_EXIST;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_mld_port_filter_get(port, &tmpIdx));
        if (tmpIdx == proIdx)
            continue;

        SYS_ERR_CHK(sal_mld_port_filter_set(port, proIdx));
    }

    return SYS_ERR_OK;
}
#endif

static int32 _cmd_mld_vlan_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char portmaskStr[CAPA_PORTMASK_STR_LEN];
    sys_vlanmask_t vmExist, enVlanMsk, fastLeaveVlanMsk, learnVlanMsk;
    sys_vid_t vid;
    mcast_vlan_entry_t mcastVlan;
    igmp_router_entry_t router;
    char strVlanList[CAPA_VLANLIST_STR_LEN];

    SYS_MEM_CLEAR(enVlanMsk);
    SYS_MEM_CLEAR(fastLeaveVlanMsk);
    SYS_MEM_CLEAR(learnVlanMsk);

    SYS_ERR_CHK(sal_mcast_vlanMskConfig_get(MULTICAST_TYPE_IPV6, &vmExist));

    FOR_EACH_VID_IN_VLANMASK(vid, vmExist)
    {
        osal_memset(&mcastVlan, 0, sizeof(mcast_vlan_entry_t));

        mcastVlan.vid = vid;
        mcastVlan.ipType = MULTICAST_TYPE_IPV6;
        SYS_ERR_CHK(sal_mcast_vlan_get(&mcastVlan));

        if (DFLT_MLD_VLAN_EBL != mcastVlan.enable)
        {
             VLANMASK_SET_VID(enVlanMsk, vid);
        }

        if (DFLT_MLD_IMMEDIATE_LEAVE != mcastVlan.fastleave_enable)
        {
            VLANMASK_SET_VID(fastLeaveVlanMsk, vid);
        }

        if (DFLT_MLD_ROUTER_PIMDVMRP_LEARN != mcastVlan.pimDvmrpLearn)
        {
            VLANMASK_SET_VID(learnVlanMsk, vid);
        }

        if (DFLT_MLD_ROBUSTNESS_VAR != mcastVlan.robustnessVar)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d robustness-variable %d\n", vid, mcastVlan.robustnessVar);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_MLD_QUERY_RESPONSE_INTV != mcastVlan.responseTime)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d response-time %d\n", vid, mcastVlan.responseTime);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_MLD_QUERY_INTV != mcastVlan.queryIntv)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d query-interval %d\n", vid, mcastVlan.queryIntv);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_MLD_GS_QUERY_RESP_INTV != mcastVlan.grpSpecificQueryIntv)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d last-member-query-interval %d\n", vid, mcastVlan.grpSpecificQueryIntv);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (DFLT_MLD_ROBUSTNESS_VAR != mcastVlan.lastMmbrQueryCnt)
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d last-member-query-count %d\n", vid, mcastVlan.lastMmbrQueryCnt);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(mcastVlan.staticMbr))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(portmaskStr);

            LPM2STR(&mcastVlan.staticMbr, portmaskStr);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d static-port %s\n", vid, portmaskStr);

            CMD_WRITE_TO_FILE(fp, cmd);
        }

        if (!IS_LOGIC_PORTMASK_CLEAR(mcastVlan.forbidMbr))
        {
            SYS_MEM_CLEAR(cmd);
            SYS_MEM_CLEAR(portmaskStr);

            LPM2STR(&mcastVlan.forbidMbr, portmaskStr);

            osal_sprintf(cmd, "ipv6 mld snooping vlan %d forbidden-port %s\n", vid, portmaskStr);

            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    if (!IS_VLANMASK_CLEAR(enVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&enVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 mld snooping vlan %s\n", (DFLT_MLD_VLAN_EBL == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (!IS_VLANMASK_CLEAR(fastLeaveVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&fastLeaveVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 mld snooping vlan %s immediate-leave\n", (DFLT_MLD_IMMEDIATE_LEAVE == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (!IS_VLANMASK_CLEAR(learnVlanMsk))
    {
        SYS_MEM_CLEAR(strVlanList);
        SYS_ERR_CHK(VMSK2STR(&learnVlanMsk, strVlanList));
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sipv6 mld snooping vlan %s mrouter learn pim-dvmrp\n", (DFLT_MLD_ROUTER_PIMDVMRP_LEARN == DISABLED) ? "" : "no ", strVlanList);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    FOR_EACH_VID(vid)
    {
        router.vid = vid;
        router.ipType = MULTICAST_TYPE_IPV6;
        SYS_ERR_CHK(sal_mcast_router_get(&router));

        if (0 != router.vid)
        {
            
            if (!IS_LOGIC_PORTMASK_CLEAR(router.router_static_pmsk))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(portmaskStr);

                LPM2STR(&router.router_static_pmsk, portmaskStr);

                osal_sprintf(cmd, "ipv6 mld snooping vlan %d static-router-port %s\n", vid, portmaskStr);

                CMD_WRITE_TO_FILE(fp, cmd);
            }

            if (!IS_LOGIC_PORTMASK_CLEAR(router.router_forbid_pmsk))
            {
                SYS_MEM_CLEAR(cmd);
                SYS_MEM_CLEAR(portmaskStr);

                LPM2STR(&router.router_forbid_pmsk, portmaskStr);

                osal_sprintf(cmd, "ipv6 mld snooping vlan %d forbidden-router-port %s\n", vid, portmaskStr);

                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_mld_static_group_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char portmask_str[CAPA_PORTMASK_STR_LEN];
    char gipv6Str[CAPA_IPV6_STR_LEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    uint32 dbSize;
    uint32 i = 0;
    uint32 j = 0;
    uint32 m = 0;
    sys_logic_portmask_t exmbr;
    sys_vlanmask_t  vlanmask;
    sys_igmp_db_t db;
    sys_logic_port_t port;

typedef struct group_set_s
{
    sys_vid_t vid;
    uint8   mac[CAPA_MAC_BYTE_NUM];
    uint8   dipv6[IPV6_ADDR_LEN];
    sys_logic_portmask_t inMbr;
    sys_logic_portmask_t exMbr;
    uint8 used;
}group_set_t;

    group_set_t *pGroup = NULL;
    group_set_t *pGroupSet = NULL;
    group_set_t tmpSet;

    osal_memset(cmd, 0, sizeof(cmd));
    osal_memset(portmask_str, 0, sizeof(portmask_str));
    osal_memset(gipv6Str, 0, sizeof(gipv6Str));
    osal_memset(strVlanList, 0, sizeof(strVlanList));

    SYS_ERR_CHK(sal_mcast_databaseSize_get(&dbSize));
    if (0 == dbSize)
        return SYS_ERR_OK;

    pGroupSet = (group_set_t *)osal_alloc(sizeof(group_set_t) * dbSize);
    if (NULL == pGroupSet)
        return SYS_ERR_FAILED;

    osal_memset(pGroupSet, 0, sizeof(group_set_t) * dbSize);

    for (i = 0, j = 0; i < dbSize; i++)
    {
        osal_memset(&db, 0, sizeof(sys_igmp_db_t));
        db.indx = i;
        SYS_ERR_CONTINUE(sal_mcast_database_get(&db));
        if (MULTICAST_TYPE_IPV6 != db.ipType)
            continue;

        if(IGMP_GROUP_ENTRY_DYNAMIC == db.form)
            continue;

        if (IS_LOGIC_PORTMASK_CLEAR(db.staticMbr))
            continue;

        LOGIC_PORTMASK_ANDNOT(exmbr, db.staticMbr, db.staticInMbr);
        osal_memcpy(pGroupSet[j].dipv6, db.dipv6, IPV6_ADDR_LEN);
        pGroupSet[j].vid = db.vid;
        osal_memcpy(&pGroupSet[j].inMbr, &db.staticInMbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(&pGroupSet[j].exMbr, &exmbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(pGroupSet[j].mac, db.mac, CAPA_MAC_BYTE_NUM);
        j++;
   }

   for (i = 0; i < j ; i++)
   {
        osal_memset(&vlanmask, 0, sizeof(sys_vlanmask_t));
        osal_memset(&tmpSet, 0, sizeof(group_set_t));

        for (m = i; m < j; m++)
        {
            pGroup = &pGroupSet[m];

            if (0 == pGroup->used)
            {
                if (0 == tmpSet.vid)
                {
                    osal_memcpy(&tmpSet, pGroup, sizeof(group_set_t));
                    if (!IS_VID_INVALID(tmpSet.vid))
                        VLANMASK_SET_VID(vlanmask, tmpSet.vid);
                    pGroup->used = 1;
                }
                else
                {
                    if (( osal_memcmp(tmpSet.dipv6, pGroup->dipv6, IPV6_ADDR_LEN) == 0)
                          && (IS_LOGIC_PORTMASK_EQUAL(tmpSet.inMbr, pGroup->inMbr)))
                    {
                        if (!IS_VID_INVALID(pGroup->vid))
                            VLANMASK_SET_VID(vlanmask, pGroup->vid);
                        pGroup->used = 1;
                    }
                }
            }
        }

        if (0 != tmpSet.vid)
        {
            inet_ntop(AF_INET6, tmpSet.dipv6, gipv6Str, sizeof(gipv6Str));
            osal_memset(portmask_str, 0, sizeof(portmask_str));
            LPM2STR(&tmpSet.inMbr, portmask_str);

            SYS_ERR_CONTINUE(VMSK2STR(&vlanmask, strVlanList));

            if (!IS_LOGIC_PORTMASK_CLEAR(tmpSet.inMbr))
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, tmpSet.inMbr)
                {
                    LP2STR(port, portmask_str);
                    if (MLD_IN_MULTICAST(tmpSet.dipv6))
                    {
                        osal_sprintf(cmd,"ipv6 mld snooping vlan %s static %s interface %s\n",
                        strVlanList, gipv6Str,portmask_str );
                    }
                    else
                    {
                        SYS_ERR_CONTINUE(sys_util_mac2Str(tmpSet.mac, macStr));
                        osal_sprintf(cmd,"ipv6 mld snooping vlan %s static-mac %s interface %s\n",
                        strVlanList, macStr,portmask_str );
                    }

                    CMD_WRITE_TO_FILE(fp, cmd);
                }
                #if 0
                if (MLD_IN_MULTICAST(tmpSet.dipv6))
                {
                    osal_sprintf(cmd,"ipv6 mld snooping vlan %s static-group %s interfaces %s\n",
                        strVlanList, gipv6Str,portmask_str );
                }
                else
                {
                    SYS_ERR_CONTINUE(sys_util_mac2Str(tmpSet.mac, macStr));
                    osal_sprintf(cmd,"ipv6 mld snooping vlan %s static-mac %s interfaces %s\n",
                        strVlanList, macStr,portmask_str );
                }
               CMD_WRITE_TO_FILE(fp, cmd);
                #endif
            }
       }
#if 0
        
        osal_memset(portmask_str, 0, sizeof(portmask_str));
        LPM2STR(&db.staticInMbr, portmask_str);
        if(db.staticInMbr.bits[0] != 0)
        {

        }
#endif
   }

    if (pGroupSet)
        osal_free(pGroupSet);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
static int32 _cmd_mld_profile_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_igmp_profileId_t proIdx ;
    sys_igmp_profile_t    profileEntry;

    char strIpv6Low[CAPA_IPV6_STR_LEN];
    char strIpv6High[CAPA_IPV6_STR_LEN];

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(profileEntry);
    proIdx = 1;

    for (proIdx = BOND_MCAST_PROFILE_MIN; proIdx <= BOND_MCAST_PROFILE_MAX; proIdx++)
    {
        SYS_MEM_CLEAR(cmd);
        SYS_ERR_CONTINUE(sal_mld_profile_get(proIdx, &profileEntry));
        
        osal_sprintf(cmd, "ipv6 mld profile %d\n", profileEntry.profileId);
        CMD_WRITE_TO_FILE(fp, cmd);

        SYS_MEM_CLEAR(cmd);

        inet_ntop(AF_INET6, profileEntry.ipv6Low, strIpv6Low, sizeof(strIpv6Low));
        inet_ntop(AF_INET6, profileEntry.ipv6High, strIpv6High, sizeof(strIpv6High));

        if (0 == osal_memcmp(profileEntry.ipv6Low, profileEntry.ipv6High, CAPA_IPV6_BYTE_NUM))
            osal_sprintf(cmd, "profile range ipv6 %s action %s \n", strIpv6Low, profileActStr[profileEntry.act]);
        else
            osal_sprintf(cmd, "profile range ipv6 %s  %s action %s \n", strIpv6Low, strIpv6High, profileActStr[profileEntry.act]);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_mld_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    sys_mcast_info_t info;
    igmp_lookup_mode_t lookup_mode;
    igmp_unknMcastAction_t val;

    SYS_ERR_CHK(sal_mld_enable_get(&enable));

    if (DFLT_MLD_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%sipv6 mld snooping\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_mld_suppre_enable_get(&enable));

    if (DFLT_MLD_SUPPRE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%sipv6 mld snooping report-suppression\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_mld_lookupMode_get(&lookup_mode));

    if (DFLT_MLD_LOOKUP_MODE != lookup_mode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ipv6 mld snooping forward-method %s\n", gLookupMode[lookup_mode]);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&val));

    if (DFLT_IGMP_UNKNOWN_MCAST_ACTION != val)
    {
        SYS_MEM_CLEAR(cmd);

        if (val == IGMP_UNKNOWN_MCAST_ACTION_DROP)
            osal_sprintf(cmd, "ipv6 mld snooping unknown-multicast action drop\n");
        else if (val == IGMP_UNKNOWN_MCAST_ACTION_FLOOD)
            osal_sprintf(cmd, "ipv6 mld snooping unknown-multicast action flood\n");
        else if (val == IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT)
            osal_sprintf(cmd, "ipv6 mld snooping unknown-multicast action router-port\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(info);
    SYS_ERR_CHK(sal_mld_info_get(&info));

    if (DFLT_MLD_OPERATION_VER != info.oper_version)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ipv6 mld snooping version ");

        switch(info.oper_version)
        {
            case MLD_VERSION_V1:
                osal_strcat(cmd, "1\n");
                break;

            case MLD_VERSION_V2_BASIC:
            case MLD_VERSION_V2_FULL:
                osal_strcat(cmd, "2\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(_cmd_mld_vlan_save(fp));

    SYS_ERR_CHK(_cmd_mld_static_group_save(fp));

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    
    SYS_ERR_CHK(_cmd_mld_profile_save(fp));
#endif

    return SYS_ERR_OK;
}

int32 cmd_mld_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint16 val;

    SYS_MEM_CLEAR(cmd);
    val = 0;
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    igmp_maxGroupAct_t act;
    
    SYS_ERR_CHK(sal_mld_portGroup_limit_get(port, &val));

    if (DFLT_MCAST_PORT_LIMIT != val)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ipv6 mld max-groups %d\n", val);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_mld_portGroup_excessAct_get(port, &act));

    if (DFLT_IGMP_MAX_GROUP_ACT != act)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ipv6 mld max-groups action %s\n", (IGMP_ACT_REPLACE == act) ? "replace" : "deny");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif
#if defined CONFIG_SYS_PROTO_IGMP_FILTERING
    
    SYS_ERR_CHK(sal_mld_port_filter_get(port, &val));

    if (INVALID_PROFILE_ID != val)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "ipv6 mld filter %d\n", val);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif
    return SYS_ERR_OK;
}

