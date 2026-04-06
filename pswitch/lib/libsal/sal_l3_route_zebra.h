
#ifndef __SAL_L3_ROUTE_ZEBRA_H__
#define __SAL_L3_ROUTE_ZEBRA_H__

#include <common/sys_def.h>

extern int32 sal_l3_route_id_add(char *id_str);
extern int32 sal_l3_route_id_del(void);
extern int32 sal_l3_route_id_get(char *id_str);
extern int32 sal_l3_ip_route_add(sys_ipv4_t dIp, sys_ipv4_t Netmask, sys_ipv4_t Gateway, uint32 ulDistance, uint32 ulFlags);
extern int32 sal_l3_ip_route_delete(sys_ipv4_t dIp, sys_ipv4_t Netmask, sys_ipv4_t Gateway, uint32 ulDistance, uint32 ulFlags);
extern int32 sal_l3_ip_route_getnext(sys_l3_routeV4Info_t *pRouteEntry);
extern int32 sal_l3_ip_route_get(sys_l3_routeV4Info_t *pRouteEntry);
extern int32 sal_l3_ipv6_route_delete(sys_ipv6_t dIp, uint32 len, sys_ipv6_t Gateway, sys_l3_intfId_t intf);
extern int32 sal_l3_ipv6_route_add(sys_ipv6_t dIp, uint32 len, sys_ipv6_t Gateway, sys_l3_intfId_t intf);
extern int32 sal_l3_ipv6_route_getnext(sys_l3_routeV6Info_t *pRouteEntry);
extern int32 sal_l3_interface_ip_add(char *ifname, sys_ipv4_t dIp, sys_ipv4_t Netmask);
extern int32 sal_l3_interface_ip_del(char *ifname, sys_ipv4_t dIp, sys_ipv4_t Netmask);

#endif 

