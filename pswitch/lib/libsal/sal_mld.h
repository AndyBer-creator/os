
#ifndef __SAL_MLD_H__
#define __SAL_MLD_H__

#include <igmp/proto_igmp.h>

extern int32 sal_mld_enable_get(sys_enable_t *pEnable);

extern int32 sal_mld_enable_set(sys_enable_t enable);

extern int32 sal_mld_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 sal_mld_lookupMode_get(igmp_lookup_mode_t *mode);

extern int32 sal_mld_unknMcastAction_get(igmp_unknMcastAction_t *pAction);

extern int32 sal_mld_unknMcastAction_set(igmp_unknMcastAction_t action);

extern int32 sal_mld_suppre_enable_set(sys_enable_t enable);

extern int32 sal_mld_suppre_enable_get(sys_enable_t *pEnable);

extern int32 sal_mld_operVersion_set(uint8 operVersion);

extern int32 sal_mld_info_get(sys_mcast_info_t *pInfo);

extern int32 sal_mld_lookupMode_get(igmp_lookup_mode_t *mode);

extern int32 sal_mld_static_group_set(igmp_group_entry_t *pGroup, uint8 fmode, uint8 isAdd);

extern int32 sal_mld_statistics_clear(void);

extern int32 sal_mld_groupByType_clear(mcast_group_type_t type);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 sal_mld_portGroup_limit_set(sys_logic_port_t port, uint16 maxnum);

extern int32 sal_mld_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum);

extern int32 sal_mld_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act);

extern int32 sal_mld_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 sal_mld_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_mld_profile_getNext(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_mld_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 sal_mld_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId);

extern int32 sal_mld_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId);
#endif

extern int32 sal_mld_databaseSize_get(uint32 *pSize);

#endif 

