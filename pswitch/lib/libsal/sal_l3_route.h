
#ifndef __SAL_L3_ROUTE_H__
#define __SAL_L3_ROUTE_H__

#include <rsd/rsd_l3.h>

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 sal_l3_routeV4_add(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_routeV4_del(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_routeV6_add(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_routeV6_del(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_routeV4Info_get(sys_l3_routeV4InfoDb_t *pInfo);

extern uint32 sal_l3_routeV4TakeEffect_ret(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_routeV6Info_get(sys_l3_routeV6InfoDb_t *pInfo);

extern uint32 sal_l3_routeV6TakeEffect_ret(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_staticRouteV4_add(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_staticRouteV4_del(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_staticRouteV4_get(sys_l3_staticRouteV4_t *pRoute);

extern int32 sal_l3_staticRouteV4Engine_add(sys_l3_routeV4_t *pRoute);

extern int32 sal_l3_staticRouteV6_add(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_staticRouteV6_del(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_staticRouteV6_get(sys_l3_staticRouteV6_t *pRoute);

extern int32 sal_l3_staticRouteV6Engine_add(sys_l3_routeV6_t *pRoute);

extern int32 sal_l3_routev6Prefer_set(uint32 val);

extern int32 sal_l3_routev6Prefer_get(uint32 *pVal);
#endif

extern int32 sal_l3_routeV4DefalutGw_get(sys_l3_routeV4_t *pGw);

#ifdef CONFIG_SYS_IPV6

extern int32 sal_l3_routeV6DefalutGw_get(sys_l3_routeV6_t *pGw);
#endif

#endif 

