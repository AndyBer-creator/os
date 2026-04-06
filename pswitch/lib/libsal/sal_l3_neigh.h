
#ifndef __SAL_L3_NEIGH_H__
#define __SAL_L3_NEIGH_H__

#include <rsd/rsd_l3.h>

extern int32 sal_l3_neigh_ageTime_set(uint32 ageTime);

extern int32 sal_l3_neigh_ageTime_get(uint32 *pAgeTime);

extern int32 sal_l3_neigh_respTime_set(uint32 respTime);
extern int32 sal_l3_neigh_respTime_get(uint32 *pRespTime);
extern int32 sal_l3_neigh_retryTimes_set(uint32 retryTimes);
extern int32 sal_l3_neigh_retryTimes_get(uint32 *pRetryTimes);
extern int32 sal_l3_neigh_cacheSize_set(uint32 cacheSize);
extern int32 sal_l3_neigh_cacheSize_get(uint32 *pCacheSize);
extern int32 sal_l3_neigh_renew_set(sys_enable_t enable);
extern int32 sal_l3_neigh_renew_get(sys_enable_t *pEnable);

extern int32 sal_l3_neigh_clear(sys_l3_neighType_t type, sys_l3_intfId_t intf);

extern int32 sal_l3_neighCnt_get(uint32 *pCnt);

extern int32 sal_l3_neigh_mac_validate(sys_mac_t *pMac);

extern int32 sal_l3_hrouteEntry_del(sys_ip_t ip, sys_mac_t mac);

extern int32 sal_l3_arpProxy_set(sys_logic_port_t lp, sys_enable_t enable);

extern int32 sal_l3_arpProxy_get(sys_logic_port_t lp, sys_enable_t *pEnable);

#endif 

