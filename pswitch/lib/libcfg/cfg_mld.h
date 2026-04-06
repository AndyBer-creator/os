
#ifndef __CFG_MLD_H__
#define __CFG_MLD_H__

#include <libsal/sal_mld.h>

typedef struct cfg_mld_s
{
    sys_enable_t    enable;
    sys_enable_t    suppreEnable;
    igmp_unknMcastAction_t unknMcastAction;
    igmp_lookup_mode_t mode;

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    uint16          portLimit[SYS_LOGICPORT_NUM_MAX];
    uint16          portExAct[SYS_LOGICPORT_NUM_MAX]; 
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    sys_igmp_profileId_t  portProfileId[SYS_LOGICPORT_NUM_MAX]; 
    sys_igmp_profile_t  igmpProfile[CAPA_MCAST_PROFILE_NUM];
    #endif
} cfg_mld_t;

extern int32 cfg_mld_enable_get(sys_enable_t *pEnable);

extern int32 cfg_mld_enable_set(sys_enable_t enable);

extern int32 cfg_mld_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 cfg_mld_lookupMode_get(igmp_lookup_mode_t *mode);

extern int32 cfg_mld_unknMcastAction_get(igmp_unknMcastAction_t *pAction);

extern int32 cfg_mld_unknMcastAction_set(igmp_unknMcastAction_t action);

extern int32 cfg_mld_suppre_enable_set(sys_enable_t enable);

extern int32 cfg_mld_suppre_enable_get(sys_enable_t *pEnable);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cfg_mld_portGroup_limit_set(sys_logic_port_t port , uint16 maxnum);

extern int32 cfg_mld_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum);

extern int32 cfg_mld_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act);

extern int32 cfg_mld_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cfg_mld_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 cfg_mld_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 cfg_mld_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId);

extern int32 cfg_mld_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId);
#endif

#endif  

