
#include <igmp/proto_igmp.h>
#include <ski/proto/igmp/ski_igmp.h>
#include <rsd/rsd_mcast.h>

int32 ski_igmp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_igmp_ioctl_t);
    ski_igmp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_IGMP_ENABLE_SET:
            SYS_ERR_HDL(mcast_igmp_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_IGMP_SUPPRE_ENABLE_SET:
            SYS_ERR_HDL(mcast_igmp_suppre_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_IGMP_LOOKUP_MODE_SET:
            SYS_ERR_HDL(mcast_igmp_lookupMode_set(pData->lookup_mode), _exit, ret);
            break;

        case SKI_IGMP_STATISTICS_RESET:
            SYS_ERR_HDL(mcast_igmp_statistics_clear(), _exit, ret);
            break;

        case SKI_IGMP_PORTSTAT_RESET:
            SYS_ERR_HDL(mcast_igmp_portStat_clear(pData->port), _exit, ret);
            break;

        case SKI_IGMP_VLAN_SET:
            SYS_ERR_HDL(mcast_vlan_set(&pData->vlan), _exit, ret);
            break;

        case SKI_IGMP_VLAN_DEL:
            SYS_ERR_HDL(mcast_vlan_del(pData->vlan.vid,pData->vlan.ipType), _exit, ret);
            break;

        case SKI_IGMP_VLAN_ADD:
            SYS_ERR_HDL(mcast_vlan_add(&pData->vlan), _exit, ret);
            break;

        case SKI_IGMP_TABLE_CLR:
            SYS_ERR_HDL(mcast_table_clear(), _exit, ret);
            break;

        case SKI_IGMP_UNKNOWN_MCAST_ACTION_SET:
            SYS_ERR_HDL(mcast_igmp_unknMcastAction_set(pData->unknMcastActn), _exit, ret);
            break;

        #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
        case SKI_IGMP_QUERIER_SET:
            SYS_ERR_HDL(mcast_igmp_querier_set(&pData->querier), _exit, ret);
            break;
        #endif

        case SKI_IGMP_OPER_VERSION_SET:
            SYS_ERR_HDL(mcast_igmp_operVersion_set(pData->operVersion), _exit, ret);
            break;

        #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
        case SKI_IGMP_PORTGROUP_LIMIT_SET:
            SYS_ERR_HDL(mcast_portGroup_limit_set(pData->limit.ipType, pData->limit.port, pData->limit.maxnum), _exit, ret);
            break;

        case SKI_IGMP_PORTGROUP_MAX_GROUP_ACT_SET:
            SYS_ERR_HDL(mcast_portGroup_excessAct_set(pData->maxgroup_act.ipType, pData->maxgroup_act.port, pData->maxgroup_act.act), _exit, ret);
            break;
        #endif

        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        case SKI_IGMP_STATIC_ROUTER_PORT_SET:
            SYS_ERR_HDL(mcast_router_static_port_set(pData->router.vid, pData->router.ipType,pData->router.router_pmsk), _exit, ret);
            break;

        case SKI_IGMP_FORBID_ROUTER_PORT_SET:
            SYS_ERR_HDL(mcast_router_forbid_port_set(pData->router.vid, pData->router.ipType, pData->router.router_pmsk), _exit, ret);
            break;
        #endif

        case SKI_IGMP_STATIC_GROUP_ENTRY_SET:
            SYS_ERR_HDL(mcast_basic_static_groupEntry_set(&pData->group.groupEntry, pData->group.fmode, pData->group.isAdd), _exit, ret);
            break;

        case SKI_IGMP_STATIC_GROUP_CLEAR:
            SYS_ERR_HDL(mcast_static_group_clear(&pData->group.groupEntry), _exit, ret);
            break;
            
        case SKI_IGMP_STATIC_GROUP_L2MCAST_ENTRY_ADD:
            SYS_ERR_HDL(mcast_static_l2_mcast_groupEntry_add(&pData->l2Mcast), _exit, ret);
            break;
            
        case SKI_IGMP_STATIC_GROUP_L2MCAST_ENTRY_DEL:
            SYS_ERR_HDL(mcast_static_l2_mcast_groupEntry_del(&pData->l2Mcast), _exit, ret);
            break;
            
        #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
        case SKI_IGMPV3_STATIC_GROUP_ENTRY_SET:
            SYS_ERR_HDL(mcast_igmpv3_static_groupEntry_set(pData->db.vid,pData->db.dip, pData->db.sip, &pData->db.portmask, pData->db.form), _exit, ret);
            break;

        case SKI_IGMPV3_STATIC_FILTER_GROUP_ENTRY_SET:
            SYS_ERR_HDL(mcast_igmpv3_static_filter_group_set(&pData->group.groupEntry, pData->group.fmode), _exit, ret);
            break;
        #endif

        #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
        case SKI_IGMP_PROFILE_SET:
            SYS_ERR_HDL(mcast_profile_set(pData->profile.ipType, pData->profile.profileId, &pData->profile), _exit, ret);
            break;

        case SKI_IGMP_FILTER_SET:
            SYS_ERR_HDL(mcast_port_filter_set(pData->filter.ipType, pData->filter.port, pData->filter.idx), _exit, ret);
            break;
        #endif

        case SKI_IGMP_GROUP_TYPE_CLEAR:
            SYS_ERR_HDL(mcast_igmp_group_del_byType(pData->type), _exit, ret);
            break;

#ifdef CONFIG_SYS_PROTO_MVR
        case SKI_IGMP_MVR_ENABLE_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_enable_set(pData->mvrConfig.mvr_enable), _exit, ret);
            break;

         case SKI_IGMP_MVR_MODE_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_mode_set(pData->mvrConfig.mode), _exit, ret);
            break;

        case SKI_IGMP_MVR_QUERYTIME_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_querytime_set(pData->mvrConfig.queryTime), _exit, ret);
            break;

        case SKI_IGMP_MVR_VLANID_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_vlanId_set(pData->mvrConfig.mvr_vid), _exit, ret);
            break;

        case SKI_IGMP_MVR_GROUPRANGE_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_groupRange_set(pData->mvrConfig.groupIp_start.ipv4, pData->mvrConfig.groupIp_end.ipv4), _exit, ret);
            break;

        case SKI_IGMP_MVR_PORTTYPE_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_portType_set(pData->mvrPortConfig.port, pData->mvrPortConfig.type), _exit, ret);
            break;

        case SKI_IGMP_MVR_PORTIMMEDIATE_SET:
            SYS_ERR_HDL(mcast_igmp_mvr_portImmediate_set(pData->mvrPortConfig.port, pData->mvrPortConfig.enable), _exit, ret);
            break;

        case SKI_IGMP_MVR_GROUP_TYPE_CLEAR:
            SYS_ERR_HDL(mcast_mvr_group_del_byType(pData->type), _exit, ret);
            break;
#endif
        
        case SKI_MCAST_ENTRYMAX_GET:
            pData->dbSize = CAPA_MCAST_GROUP_NUM;
            break;

       case SKI_IGMP_ENTRYMAX_GET:
            pData->dbSize = CAPA_IGMP_GROUP_NUM;
            break;
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
       case SKI_MLD_ENTRYMAX_GET:
            pData->dbSize = CAPA_MLD_GROUP_NUM;
            break;
#endif
        case SKI_IGMP_INFO_GET:
            SYS_ERR_HDL(mcast_igmp_info_get(&pData->info), _exit, ret);
            break;

        case SKI_IGMP_PORTSTAT_GET:
            SYS_ERR_HDL(mcast_igmp_portStat_get(pData->portStat.lport, &pData->portStat), _exit, ret);
            break;

        case SKI_IGMP_LOOKUP_MODE_GET:
            SYS_ERR_HDL(mcast_igmp_lookupMode_get(&pData->lookup_mode), _exit, ret);
            break;

        case SKI_MCAST_DBSIZE_GET:
            SYS_ERR_HDL(mcast_databaseSize_get(&pData->dbSize), _exit, ret);
            break;

        case SKI_IGMP_DBSIZE_GET:
            SYS_ERR_HDL(mcast_igmp_databaseSize_get(&pData->dbSize), _exit, ret);
            break;

        case SKI_IGMP_DIPONLY_DBSIZE_GET:
            SYS_ERR_HDL(mcast_dipOnly_databaseSize_get(&pData->dbSize), _exit, ret);
            break;

        case SKI_IGMP_DB_ENTRY_GET:
            SYS_ERR_HDL(mcast_database_get(&pData->db), _exit, ret);
            break;

        case SKI_IGMP_VLAN_GET:
            SYS_ERR_HDL(mcast_vlan_get(&pData->vlan), _exit, ret);
            break;

        case SKI_IGMP_VLAN_CONFIG_GET:
            SYS_ERR_HDL(mcast_vlanMskConfig_get(pData->configMsk.ipType, &pData->configMsk.confVlanMsk), _exit, ret);
            break;

        case SKI_IGMP_ROUTER_GET:
            SYS_ERR_HDL(mcast_router_get(&pData->router), _exit, ret);
            break;

        #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
        case SKI_IGMP_GRPHEAD_DB_ENTRY_GET:
            SYS_ERR_HDL(mcast_igmp_groupHead_database_get(&pData->grpHeadDb), _exit, ret);
            break;
        #endif

        #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
        case SKI_IGMP_QUERIER_GET:
            SYS_ERR_HDL(mcast_igmp_querier_get(&pData->querier), _exit, ret);
            break;
        #endif

        #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
        case SKI_IGMP_PROFILE_GET:
            SYS_ERR_HDL(mcast_profile_get(pData->profile.ipType, pData->profile.profileId, &pData->profile), _exit, ret);
            break;
        #endif

        #ifdef CONFIG_SYS_PROTO_MVR
        case SKI_IGMP_MVR_DBSIZE_GET:
            SYS_ERR_HDL(mcast_igmp_mvrGroupNum_get(&pData->dbSize), _exit, ret);
            break;
        #endif
        case SKI_IGMP_STATIC_GROUP_L2MCAST_DBSIZE_GET:
            SYS_ERR_HDL(mcast_static_l2_mcast_databaseSize_get(&pData->dbSize), _exit, ret);
            break;

        case SKI_IGMP_STATIC_GROUP_L2MCAST_DB_ENTRY_GET:
            SYS_ERR_HDL(mcast_static_l2_mcast_database_get(&pData->l2Mcast, pData->dbSize), _exit, ret);
            break;

        default:
            SYS_PRINTF("Unknown IGMP command option !!\n");
            ret = SYS_ERR_FAILED;
            goto _exit;
            break;
    }

    if(cmd > SKI_IGMP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_igmp_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_IGMP, ski_igmp_cmd_mux));

    return SYS_ERR_OK;
}

