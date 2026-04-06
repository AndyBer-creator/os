
#ifndef __SAL_L3_ROUTE_BGP_H__
#define __SAL_L3_ROUTE_BGP_H__

#include <common/sys_def.h>

extern int32 sal_l3_routeV4_bgp_cloce(uint32 as_number);
extern int32 sal_l3_routeV4_bgp_open(uint32 as_number);
extern int32 sal_l3_bgp_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp, sys_ipv4_t ulMask, uint32 areaId);
#if 0

extern int32 sal_rip_version_rip_set(int32 tmpVersion, int32 isSet);

extern int32 sal_rip_route_config(int32 isSet, char *ipAddrStr);

extern int32 sal_rip_distance_config(int32 isSet, int32 distanceVal);

extern int32 sal_rip_distance_source_config(int32 isSet, char *distanceVal, char *sourceAddr);

extern int32 sal_rip_distribute_list_config(int32 isSet, char *alistName, char *type, char *ifName);

extern int32 sal_rip_access_list_config(int32 isSet, int32 afi,char *alistName, char *typestr, char *prefix_str, int32 exact_match);

#endif
#endif 

