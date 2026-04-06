
#ifndef __CFG_IGMP_H__
#define __CFG_IGMP_H__

#include <libsal/sal_igmp.h>

typedef struct cfg_igmp_s
{
    sys_enable_t    enable;
    sys_enable_t    suppreEnable;
    sys_enable_t    debug;
    igmp_lookup_mode_t mode;

    #ifdef CONFIG_SYS_PROTO_MVR
    mvr_config_t  mvrConfig;
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    uint16          portLimit[SYS_LOGICPORT_NUM_MAX];
    uint16          portExAct[SYS_LOGICPORT_NUM_MAX]; 
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    sys_igmp_profileId_t  portFilterProfileId[SYS_LOGICPORT_NUM_MAX];
    sys_igmp_profileId_t  portPreviewProfileId[SYS_LOGICPORT_NUM_MAX];
    sys_igmp_profile_t  igmpProfile[CAPA_MCAST_PROFILE_NUM];
    #endif
    igmp_version_t         operVersion;
    igmp_unknMcastAction_t unknMcastAction;

    #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    sys_vlanmask_t  querier;   
    uint8                   querierVersion;
    #endif
} cfg_igmp_t;

extern int32 cfg_igmp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_igmp_enable_set(sys_enable_t enable);

extern int32 cfg_igmp_suppre_enable_get(sys_enable_t *pEnable);

extern int32 cfg_igmp_suppre_enable_set(sys_enable_t enable);

extern int32 cfg_igmp_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 cfg_igmp_lookupMode_get(igmp_lookup_mode_t *mode);

extern int32 cfg_igmp_unknMcastAction_get(igmp_unknMcastAction_t *pAction);

extern int32 cfg_igmp_unknMcastAction_set(igmp_unknMcastAction_t action);

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

extern int32 cfg_igmp_querier_set(sys_igmp_querier_t *pQuerier);

extern int32 cfg_igmp_querier_get(sys_vlanmask_t *pQuerier);
#endif

extern int32 cfg_igmp_operVersion_set(igmp_version_t operVersion);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cfg_igmp_portGroup_limit_set(sys_logic_port_t port , uint16 maxnum);

extern int32 cfg_igmp_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum);

extern int32 cfg_igmp_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act);

extern int32 cfg_igmp_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cfg_igmp_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 cfg_igmp_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);

extern int32 cfg_igmp_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId);

extern int32 cfg_igmp_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId);
#endif

#ifdef CONFIG_SYS_PROTO_MVR

extern int32 cfg_igmp_mvr_enable_get(sys_enable_t *pEnable);

extern int32 cfg_igmp_mvr_enable_set(sys_enable_t enable);

extern int32 cfg_igmp_mvr_mode_get(mvr_mode_t *pMode);

extern int32 cfg_igmp_mvr_mode_set(mvr_mode_t mode);

extern int32 cfg_igmp_mvr_querytime_get(uint32 *pValue);

extern int32 cfg_igmp_mvr_querytime_set(uint32 queryTime);

extern int32 cfg_igmp_mvr_vlanId_get(sys_vid_t  *pVid);

extern int32 cfg_igmp_mvr_vlanId_set(sys_vid_t vid);

extern int32 cfg_igmp_mvr_groupRange_get(uint32 *pStart, uint32 *pEnd);

extern int32 cfg_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp);

extern int32 cfg_igmp_mvr_portType_get(sys_logic_port_t port , mvr_portType_t *pType);

extern int32 cfg_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type);

extern int32 cfg_igmp_mvr_portImmediate_get(sys_logic_port_t port , sys_enable_t *pEnable);

extern int32 cfg_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable);
#endif 

#endif

