
#ifndef __RSD_MCAST_H__
#define __RSD_MCAST_H__

#include <common/sys_def.h>
#include <igmp/proto_igmp.h>
#include <common/sys_l2.h>

typedef rtk_l2_lookupMissType_t rsd_l2_lookupMissType_t;

typedef struct rsd_mcast_mapper_s
{
    int32 (*rsd_igmp_init)(void);
    int32 (*rsd_igmp_enable_set)(sys_enable_t enable, igmp_lookup_mode_t mode);
    int32 (*rsd_igmp_enable_get)(sys_enable_t *pEnable);
    int32 (*rsd_igmp_lookupMode_set)(igmp_lookup_mode_t mode);
    int32 (*rsd_igmp_unKnown_routerPortFlood_set)(sys_enable_t enable);

    int32 (*rsd_mld_init)(void);
    int32 (*rsd_mld_enable_set)(sys_enable_t enable);
    int32 (*rsd_mld_enable_get)(sys_enable_t *pEnable);
    int32 (*rsd_mld_lookupMode_set)(igmp_lookup_mode_t mode);
    int32 (*rsd_mld_unKnown_routerPortFlood_set)(sys_enable_t enable);

    int32 (*rsd_mcast_unKnown_drop_set)(multicast_ipType_t ipType, sys_enable_t enable);
    int32 (*rsd_mcast_lookupMode_get)(multicast_ipType_t ipType, igmp_lookup_mode_t *pMode);
    int32 (*rsd_mcast_ipMcastPkt2CpuEnable_set)(multicast_ipType_t ipType, sys_enable_t enable, igmp_unknMcastAction_t action);
    int32 (*rsd_mcast_enable_set)(sys_enable_t enable);
    int32 (*rsd_mcast_routerPacket_flood)(sys_nic_pkt_t *pPacket, sys_vid_t vid, sys_logic_portmask_t blockPm, sys_pri_t priority);
    int32 (*rsd_mcast_mcastLeakyEnable_set)(sys_enable_t enable);
    int32 (*rsd_mcast_l2_addressRsv_set)(sys_enable_t enable);
    int32 (*rsd_mcast_igmp_unKnownMcast_action_set)(igmp_unknMcastAction_t newAct, igmp_unknMcastAction_t oldAct, sys_enable_t igmpEnable, sys_enable_t mldEnable, uint8 isGroupFull);
    int32 (*rsd_mcast_fwdIndexFreeCount_get)(uint32 *pFreeCount);
    int32 (*rsd_mcast_lookupMissAction_set)(rsd_l2_lookupMissType_t type, sys_action_t action);
    int32 (*rsd_mcast_lookupMissAction_get)(rsd_l2_lookupMissType_t type, sys_action_t *pAction);
} rsd_mcast_mapper_t;

extern int32 rsd_mcast_init(void);

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
extern rsd_mcast_mapper_t _mcast_msw_mapper;
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
extern rsd_mcast_mapper_t _mcast_csw_mapper;
#endif

extern int32 rsd_igmp_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 rsd_mcast_lookupMode_get(multicast_ipType_t ipType, igmp_lookup_mode_t *pMode);

extern int32 rsd_igmp_enable_set(sys_enable_t enable, igmp_lookup_mode_t mode);

extern int32 rsd_igmp_enable_get(sys_enable_t *pEnable);

extern int32 rsd_mcast_ipMcastPkt2CpuEnable_set(multicast_ipType_t ipType, sys_enable_t enable, igmp_unknMcastAction_t action);

extern int32 rsd_igmp_unKnown_routerPortFlood_set(sys_enable_t enable);

extern int32 rsd_mld_enable_set(sys_enable_t enable);

extern int32 rsd_mld_enable_get(sys_enable_t *pEnable);

extern int32 rsd_mld_unKnown_routerPortFlood_set(sys_enable_t enable);

extern int32 rsd_mcast_unKnown_drop_set(multicast_ipType_t ipType, sys_enable_t enable);

extern int32 rsd_mcast_enable_set(sys_enable_t enable);

extern int32 rsd_mcast_routerPacket_flood(sys_nic_pkt_t *pPacket, sys_vid_t vid, sys_logic_portmask_t blockPm, sys_pri_t priority);

extern int32 rsd_mld_lookupMode_set(igmp_lookup_mode_t mode);

extern int32 rsd_mcast_mcastLeakyEnable_set(sys_enable_t enable);

extern int32 rsd_mcast_l2_addressRsv_set(sys_enable_t enable);

extern int32 rsd_mcast_igmp_unKnownMcast_action_set(igmp_unknMcastAction_t newAct, igmp_unknMcastAction_t oldAct, sys_enable_t igmpEnable, sys_enable_t mldEnable, uint8 isGroupFull);

extern int32 rsd_mcast_fwdIndexFreeCount_get(uint32 *pFreeCount);

extern int32 rsd_mcast_lookupMissAction_set(rsd_l2_lookupMissType_t type, sys_action_t action);

extern int32 rsd_mcast_lookupMissAction_get(rsd_l2_lookupMissType_t type, sys_action_t *pAction);

#endif 

