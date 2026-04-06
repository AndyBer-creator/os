
#ifndef __CFG_L3_H__
#define __CFG_L3_H__

 #include <libsal/sal_l3.h>

 typedef struct cfg_l3_s
{
    sys_enable_t           fwdIPv4;
    sys_enable_t           fwdIPv6;
    uint32                 ipv6HopLimit;
    uint32                 icmpv6RateIntvl;
    uint32                 icmpv6RateBurst;
    uint32                 routeV6Pref;

    sys_l3_intfAdmin_t     intfl[CAPA_L3_INTFLO_NUM_MAX];
    sys_l3_intfAdmin_t     intfv[SYS_VLAN_NUM_MAX];

    sys_l3_intfStsChg_t    intfvStsChg[CAPA_L3_INTFVLAN_NUM_MAX];

    sys_l3_staticRouteV4_t routeV4;
    sys_l3_staticRouteV6_t routeV6;

    uint32 arpAgeTime;
    uint32 arpRespTime;
    uint32 arpRetryTimes;
    uint32 arpCacheSize;
    sys_enable_t dynamicRenew;
    sys_enable_t arpProxy[SYS_LOGICPORT_NUM_MAX];
} cfg_l3_t;

extern int32 cfg_l3_init(void);

extern int32 cfg_l3_intf_init(sys_l3_intfId_t *pId);

extern int32 cfg_l3_fwdIPv4Enable_set(sys_enable_t enable);

extern int32 cfg_l3_fwdIPv4Enable_get(sys_enable_t *pEnable);

extern int32 cfg_l3_fwdIPv6Enable_set(sys_enable_t enable);

extern int32 cfg_l3_fwdIPv6Enable_get(sys_enable_t *pEnable);

extern int32 cfg_l3_hopLimitIPv6_set(uint32 num);

extern int32 cfg_l3_hopLimitIPv6_get(uint32 *pNum);

extern int32 cfg_l3_icmpv6RateInterval_set(uint32 time);

extern int32 cfg_l3_icmpv6RateInterval_get(uint32 *pNum);

extern int32 cfg_l3_icmpv6RateBurst_set(uint32 time);

extern int32 cfg_l3_icmpv6RateBurst_get(uint32 *pNum);

extern int32 cfg_l3_routev6Prefer_set(uint32 val);

extern int32 cfg_l3_routev6Prefer_get(uint32 *pVal);

extern int32 cfg_l3_intf_set(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin);

extern int32 cfg_l3_intf_get(sys_l3_intfId_t *pId, sys_l3_intfAdmin_t *pAdmin);

extern int32 cfg_l3_intfStatusChg_set(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg);

extern int32 cfg_l3_intfStatusChg_get(sys_l3_intfId_t *pId, sys_l3_intfStsChg_t *pStsChg);

extern int32 cfg_l3_intfEnable_set(sys_l3_intfId_t *pId, sys_enable_t enable);

extern int32 cfg_l3_intfEnable_get(sys_l3_intfId_t *pId, sys_enable_t *pEnable);

extern int32 cfg_l3_intfIPv4_set(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIp);

extern int32 cfg_l3_intfIPv4_get(sys_l3_intfId_t *pId, sys_l3_ipv4Admin_t *pIp);

extern int32 cfg_l3_intfIPv6_set(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIp);

extern int32 cfg_l3_intfIPv6_get(sys_l3_intfId_t *pId, sys_l3_ipv6Admin_t *pIp);

extern int32 cfg_l3_staticRouteV4_set(sys_l3_staticRouteV4_t *pRoute);

extern int32 cfg_l3_staticRouteV4_get(sys_l3_staticRouteV4_t *pRoute);

extern int32 cfg_l3_staticRouteV6_set(sys_l3_staticRouteV6_t *pRoute);

extern int32 cfg_l3_staticRouteV6_get(sys_l3_staticRouteV6_t *pRoute);

extern int32 cfg_l3_neigh_ageTime_set(uint32 ageTime);

extern int32 cfg_l3_neigh_ageTime_get(uint32 *pAgeTime);

extern int32 cfg_l3_neigh_respTime_set(uint32 respTime);
extern int32 cfg_l3_neigh_respTime_get(uint32 *pRespTime);
extern int32 cfg_l3_neigh_retryTimes_set(uint32 retryTimes);
extern int32 cfg_l3_neigh_retryTimes_get(uint32 *pRetryTimes);
extern int32 cfg_l3_neigh_cacheSize_set(uint32 cacheSize);
extern int32 cfg_l3_neigh_cacheSize_get(uint32 *pCacheSize);
extern int32 cfg_l3_neigh_renew_set(sys_enable_t enable);
extern int32 cfg_l3_neigh_renew_get(sys_enable_t *pEnable);

extern int32 cfg_l3_arpProxy_set(sys_logic_port_t lp, sys_enable_t enable);

extern int32 cfg_l3_arpProxy_get(sys_logic_port_t lp, sys_enable_t *pEnable);
#endif 

