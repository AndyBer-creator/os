
#ifndef __SAL_IGMP_H__
#define __SAL_IGMP_H__

#include <igmp/proto_igmp.h>
#include <rsd/rsd_mcast.h>

extern const char *profileActStr[];
extern const char *mcastStatCntName[];
extern const char *gUnknownMcastAct[];
extern const char *gLookupMode[];

#ifdef CONFIG_SYS_PROTO_MVR
extern const char *gMvrPortType[];
#endif

typedef struct mcast_vlan_infoSave_s
{
    sys_vid_t vid;
    uint16 val;
}mcast_vlan_infoSave_t;

extern int32 sal_igmp_enable_get(sys_enable_t *pEnable);

extern int32 sal_igmp_suppre_enable_set(sys_enable_t enable);

extern int32 sal_igmp_suppre_enable_get(sys_enable_t *pEnable);

extern int32 sal_igmp_enable_set(sys_enable_t enable);

extern int32 sal_igmp_statistics_clear(void);

extern int32 sal_igmp_info_get(sys_mcast_info_t *pInfo);

extern int32 sal_igmp_portStat_get(sys_logic_port_t lport, sys_mcast_portStat_t *portStat);

extern int32 sal_igmp_portStat_clear(sys_logic_port_t lport);

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

extern int32 sal_igmp_querier_set(sys_igmp_querier_t* pQuerier);

extern int32 sal_igmp_querier_get(sys_igmp_querier_t *pQuerier);

extern int32 sal_igmp_querier_vlanmask_get(sys_vlanmask_t *pQuerier);
#endif

extern int32 sal_igmp_operVersion_set(uint8 operVersion);

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

extern int32 sal_igmp_groupHead_database_get(sys_igmp_groupHead_db_t *pGrpHeadDb);

extern int32 sal_igmp_static_filter_group_set(igmp_group_entry_t *pGroup, uint8 fmode);

extern int32 sal_igmpv3_static_group_set(sys_vid_t vid, uint32 groupAddr,
         uint32 sourceAddr, sys_logic_portmask_t *pSpmsk,  uint8 fmode);
#endif

extern int32 sal_igmp_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 sal_igmp_lookupMode_get(igmp_lookup_mode_t *mode);

extern int32 sal_mcast_table_clear(void);

extern int32 sal_igmp_groupByType_clear(mcast_group_type_t type);

extern int32 sal_igmp_databaseSize_get(uint32 *pSize);

extern int32 sal_mcast_vlan_get(mcast_vlan_entry_t *pMcastVlan);

extern int32 sal_mcast_vlan_set(mcast_vlan_entry_t* pMcastVlan);

extern int32 sal_mcast_vlan_add(mcast_vlan_entry_t* pMcastVlan);

extern int32 sal_mcast_vlanMskConfig_get(multicast_ipType_t ipType, sys_vlanmask_t *pBitmap);

extern int32 sal_mcast_router_get(igmp_router_entry_t *pRouter);

extern int32 sal_mcast_database_get(sys_igmp_db_t *pDb);

extern int32 sal_mcast_databaseSize_get(uint32 *pSize);

extern int32 sal_mcast_dipOnly_databaseSize_get(uint32 *pSize);

extern int32 sal_igmp_unknMcastAction_get(igmp_unknMcastAction_t *pAction);

extern int32 sal_igmp_unknMcastAction_set(igmp_unknMcastAction_t action);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 sal_igmp_portGroup_limit_set(sys_logic_port_t port, uint16 maxnum);

extern int32 sal_igmp_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum);

extern int32 sal_igmp_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act);

extern int32 sal_igmp_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

extern int32 sal_mcast_router_forbid_port_set(sys_vid_t vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);

 extern int32 sal_mcast_router_static_port_set(sys_vid_t vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);
#endif

extern int32 sal_mcast_static_group_clear(igmp_group_entry_t *pGroup);

extern int32 sal_mcast_static_group_set(igmp_group_entry_t *pGroup, uint8 fmode, uint8 isAdd);

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 sal_igmp_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_igmp_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_igmp_profile_getNext(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_igmp_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId);

extern int32 sal_igmp_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId);
#endif

#ifdef CONFIG_SYS_PROTO_MVR

extern int32 sal_igmp_mvr_enable_get(sys_enable_t *pEnable);

extern int32 sal_igmp_mvr_enable_set(sys_enable_t enable);

extern int32 sal_igmp_mvr_mode_get(mvr_mode_t *pMode);

extern int32 sal_igmp_mvr_mode_set(mvr_mode_t mode);

extern int32 sal_igmp_mvr_querytime_get(uint32 *pValue);

extern int32 sal_igmp_mvr_querytime_set(uint32 queryTime);

extern int32 sal_igmp_mvr_vlanId_get(sys_vid_t  *pVid);

extern int32 sal_igmp_mvr_vlanId_set(sys_vid_t vid);

extern int32 sal_igmp_mvr_groupRange_get(uint32 *pStart, uint32 *pEnd);

extern int32 sal_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp);

extern int32 sal_igmp_mvr_portType_get(sys_logic_port_t port , mvr_portType_t *pType);

extern int32 sal_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type);

extern int32 sal_igmp_mvr_portImmediate_get(sys_logic_port_t port , sys_enable_t *pEnable);

extern int32 sal_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable);

extern int32 sal_igmp_mvrGroupNum_get(uint32 *pSize);

extern uint8 sal_igmp_mvr_group_ret(sys_igmp_db_t *pGrpDb);

extern int32 sal_mvr_groupByType_clear(mcast_group_type_t type);

#endif
extern int32 sal_l2_mcast_static_group_set(l2_mcast_group_t *pGroup, uint8 isAdd);

extern int32 sal_l2_mcast_static_group_get(l2_mcast_group_t *pGroup, uint32 codeOp);

extern int32 sal_l2_mcast_databaseSize_get(uint32 *pSize);

extern int32 sal_l2_mcast_database_get(l2_mcast_group_t *pDb, uint32 index);
#endif 

