
#ifndef __RSD_L3_ROUTE_H__
#define __RSD_L3_ROUTE_H__

typedef struct rsd_l3_route_mapper_s
{
    int32 (*rsd_l3_hrouteEntry_chip_add)(sys_l3_hrouteEntry_t *l3hr);
    int32 (*rsd_l3_hrouteEntry_chip_del)(sys_l3_hrouteEntry_t *l3hr, sys_l3_hrouteEntry_t *next);
    int32 (*rsd_l3_hrouteEntry_chip_update)(sys_l3_hrouteEntry_t *l3hr);
    int32 (*rsd_l3_hrouteEntry_chip_enable)(sys_l3_hrouteEntry_t *l3hr, sys_enable_t enable);
    int32 (*rsd_l3_nRoute_chip_add)(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh, sys_l3_hrouteEntry_t *l3hr, sys_enable_t enable);
    int32 (*rsd_l3_nRoute_chip_del)(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh);
    int32 (*rsd_l3_nRoute_chip_enable)(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh, sys_enable_t enable);
    int32 (*rsd_l3_rejRoute_chip_add)(sys_l3_af_t af, sys_l3_ip_t *dst, sys_enable_t enable);
    int32 (*rsd_l3_rejRoute_chip_del)(sys_l3_af_t af, sys_l3_ip_t *dst);
    int32 (*rsd_l3_intfIpTrap_chip_add)(sys_l3_af_t af, sys_vid_t vid, sys_l3_ip_t *ip);
    int32 (*rsd_l3_intfIpTrap_chip_del)(sys_l3_af_t af, sys_vid_t vid, sys_l3_ip_t *ip);
    int32 (*rsd_l3_route_chip_init)(void);
} rsd_l3_route_mapper_t;

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
extern rsd_l3_route_mapper_t _l3_route_msw_mapper;
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
extern rsd_l3_route_mapper_t _l3_route_csw_mapper;
#endif

#define IS_IP_FWD(_af) (rsd_l3_fwdEnable_ret(_af))
#define IS_IPV4_FWD() (rsd_l3_fwdEnable_ret(SYS_L3_AF_IPV4))
#define IS_IPV6_FWD() (rsd_l3_fwdEnable_ret(SYS_L3_AF_IPV6))

extern int32 rsd_l3_route_init(void);

extern sys_enable_t rsd_l3_fwdEnable_ret(sys_l3_af_t af);

extern int32 rsd_l3_routeV4_add(sys_l3_routeV4_t *pRoute);

extern int32 rsd_l3_routeV4_del(sys_l3_routeV4_t *pRoute);

extern int32 rsd_l3_routeV6_add(sys_l3_routeV6_t *pRoute);

extern int32 rsd_l3_routeV6_del(sys_l3_routeV6_t *pRoute);

extern int32 rsd_l3_fwdIPv4Enable_set(sys_enable_t enable);

extern int32 rsd_l3_fwdIPv6Enable_set(sys_enable_t enable);

extern int32 rsd_l3_neigh_ageTime_set(uint32 ageTime);
extern int32 rsd_l3_neigh_respTime_set(uint32 respTime);
extern int32 rsd_l3_neigh_retryTimes_set(uint32 retryTimes);
extern int32 rsd_l3_neigh_renew_set(sys_enable_t enable);
extern int32 rsd_l3_neigh_cacheSize_set(uint32 size);

extern int32 rsd_l3_neigh_clear(sys_l3_neighType_t type, sys_l3_intfId_t inft);

extern int32 rsd_l3_neighDynamic_delByL2(sys_l2_entry_t *pL2);

extern int32 rsd_l3_neigh_getByIdx(uint32 idx, sys_l3_hrouteEntry_t *pL3hr);

extern int32 rsd_l3_neighCnt_get(uint32 *pCnt);

extern int32 rsd_l3_hrouteEntry_del(sys_ip_t ip, sys_mac_t mac);

extern int32 rsd_l3_neigh_v4_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_l3_neigh_v6_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_l3_neigh_v4_chk(sys_ipv4_t ip, sys_mac_t mac);

extern int32 rsd_l3_neigh_v6_chk(sys_ipv6_t *ipv6, sys_mac_t *mac, uint32 isRouter);

extern int32 rsd_l3_hrouteEntry_chip_add(sys_l3_hrouteEntry_t *l3hr);

extern int32 rsd_l3_hrouteEntry_chip_del(sys_l3_hrouteEntry_t *l3hr, sys_l3_hrouteEntry_t *next);

extern int32 rsd_l3_hrouteEntry_chip_enable(sys_l3_hrouteEntry_t *l3hr, sys_enable_t enable);

extern int32 rsd_l3_nRoute_chip_add(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh, sys_l3_hrouteEntry_t *l3hr, sys_enable_t enable);

extern int32 rsd_l3_nRoute_chip_del(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh);

extern int32 rsd_l3_nRoute_chip_enable(sys_l3_af_t af, sys_l3_ip_t *dst, sys_ip_t *nh, sys_enable_t enable);

extern int32 rsd_l3_intfIpTrap_chip_add(sys_l3_af_t af, sys_vid_t vid, sys_l3_ip_t *ip);

extern int32 rsd_l3_intfIpTrap_chip_del(sys_l3_af_t af, sys_vid_t vid, sys_l3_ip_t *ip);

extern int32 rsd_l3_route_chip_init(void);
#endif

