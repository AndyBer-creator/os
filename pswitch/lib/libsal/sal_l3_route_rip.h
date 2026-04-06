
#ifndef __SAL_L3_ROUTE_RIP_H__
#define __SAL_L3_ROUTE_RIP_H__

#include <common/sys_def.h>
#include <libipc/quaggaTask.h>

extern int32 sal_l3_routeV4_rip_cloce(void);
extern int32 sal_l3_routeV4_rip_open(void);
extern int32 sal_l3_rip_network_set(sys_enable_t bNoFlag, uint32 ulIp, uint32 ulMask, char *ifname);

extern int32 sal_rip_version_rip_set(int32 tmpVersion, int32 isSet);

extern int32 sal_rip_route_config(int32 isSet, char *ipAddrStr);

extern int32 sal_rip_distance_config(int32 isSet, int32 distanceVal);

extern int32 sal_rip_distance_source_config(int32 isSet, char *distanceVal, char *sourceAddr);

extern int32 sal_rip_distribute_list_config(int32 isSet, char *alistName, char *type, char *ifName);

extern int32 sal_rip_access_list_config(int32 isSet, int32 afi,char *alistName, char *typestr, char *prefix_str, int32 exact_match);

extern int32 sal_access_list_get_alist_info( DATA_OP_ENT op, int afi, ALIST_INFO_T *pAlistInfo);

extern int32 sal_access_list_get_filter_info(DATA_OP_ENT op, int afi, FILTER_INFO_T *pFilterInfo);

extern int32 sal_distribute_list_get_info(DATA_OP_ENT op, DISTRIBUTE_INFO_T *pDistributeInfo);

extern int32 sal_rip_debug(int bIsEnbale, char *pRipDebugStr);

extern int32 sal_rip_log_to_fileName(int bIsEnbale);

extern int32 sal_rip_log_to_stdout(int bIsEnbale);

extern int32 sal_rip_status_get(int32 *bEnable);

extern int32 sal_rip_version_get(int32 *sendVersion, int32 *recvVersion);

extern int32 sal_rip_enable_network_info_get(DATA_OP_ENT op, RIP_ENABLE_NW_INFO_T *pRipEnableNwInfo);

extern int32 sal_rip_ip_route_get(DATA_OP_ENT op, RIP_IP_ROUTE_STRC *pRipRoute);

int32 sal_rip_route_num_get(int32 *routeNum);

extern int32 sal_rip_information_info_get(RIP_INFO_T *ripInfo);

#endif 

