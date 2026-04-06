
#ifndef __SAL_L3_ROUTE_OSPF_H__
#define __SAL_L3_ROUTE_OSPF_H__

#include <common/sys_def.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>

#define FRR_OSPF_AREA_STUB          1
#define FRR_OSPF_AREA_NSSA          2
#define FRR_OSPF_NSSA_ROLE_NEVER     0
#define FRR_OSPF_NSSA_ROLE_CANDIDATE 1
#define FRR_OSPF_NSSA_ROLE_ALWAYS    2
#define FRR_OSPF_AUTH_NULL           0
#define FRR_OSPF_AUTH_SIMPLE         1
#define FRR_OSPF_AUTH_CRYPTOGRAPHIC  2
#define FRR_OSPF_PATH_INTER_AREA		2
#define FRR_OSPF_PATH_INTRA_AREA		1
#define FRR_OSPF_DESTINATION_NETWORK	2
#define FRR_OSPF_DESTINATION_DISCARD	3
#define FRR_ROUTER_LSA_BORDER	       0x01 
#define FRR_ROUTER_LSA_EXTERNAL	       0x02 
#define FRR_OSPF_PATH_TYPE1_EXTERNAL	3
#define FRR_OSPF_PATH_TYPE2_EXTERNAL	4

extern int32 sal_l3_routeV4_ospf_cloce(uint16 ProcessID);
extern int32 sal_l3_routeV4_ospf_open(uint16 ProcessID);
extern int32 sal_l3_routeV4_ospf_get(sys_enable_t *pBEnable);
extern int32 sal_l3_routeV4_ospf_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbrInfo);
extern int32 sal_l3_routeV4_ospf_nbma_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbmaNbrInfo);
extern int32 sal_l3_ospf_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp, sys_ipv4_t ulMask, uint32 areaId, uint16 ProcessID);
extern int32 sal_l3_ospf_router_id_set(sys_enable_t bNoFlag, sys_ipv4_t router_id, uint16 ProcessID);
extern int32 sal_l3_ospf_passive_interface_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr);
extern int32 sal_l3_ospf_passive_interface_default(sys_enable_t bNoFlag);
extern int32 sal_l3_ospf_area_set(sys_enable_t bNoFlag, uint32 areaId, uint16 ProcessID);
extern int32 sal_l3_ospf_area_range_set(sys_enable_t bNoFlag, uint32 areaId, char *prefix_str, uint32 advertise, uint32 cost, uint16 ProcessID);
extern int32 sal_l3_ospf_area_range_substitute_set(sys_enable_t bNoFlag, uint32 areaId, char *prefix_src, char *prefix_dst, uint16 ProcessID);
extern int32 sal_l3_ospf_area_vlink_set(sys_enable_t bNoFlag, struct ospf_vllink_config_data *vl_config, uint16 ProcessID);
extern int32 sal_l3_ospf_area_stub_set(sys_enable_t bNoFlag, uint32 areaId, int AreaSummaryStatus, uint16 ProcessID);
extern int32 sal_l3_ospf_area_nssa_set(sys_enable_t bNoFlag, uint32 areaId, char *role, int AreaSummaryStatus, uint16 ProcessID);
extern int32 sal_l3_ospf_area_default_cost_set(sys_enable_t bNoFlag, uint32 areaId, uint32 cost, uint16 ProcessID);
extern int32 sal_l3_ospf_area_export_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, uint16 ProcessID);
extern int32 sal_l3_ospf_area_import_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, uint16 ProcessID);
extern int32 sal_l3_ospf_area_filter_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, char *type_str, uint16 ProcessID);
extern int32 sal_l3_ospf_area_authentication_set(sys_enable_t bNoFlag, uint32 areaId, int auth_type, uint16 ProcessID);
extern int32 sal_l3_ospf_area_shortcut_set(sys_enable_t bNoFlag, uint32 areaId, char *type_str, uint16 ProcessID);
extern int32 sal_l3_ospf_abr_type_set(sys_enable_t bNoFlag, char *abr_type_str, uint16 ProcessID);
extern int32 sal_l3_ospf_log_adjacency_set(sys_enable_t bNoFlag, int detail, uint16 ProcessID);
extern int32 sal_l3_ospf_rfc1583compatibility_set(sys_enable_t bNoFlag, uint16 ProcessID);
extern int32 sal_l3_ospf_spf_timer_set(sys_enable_t bNoFlag, uint32 delay, uint32 hold, uint32 max, uint16 ProcessID);
extern int32 sal_l3_ospf_static_neighbor_set(sys_enable_t bNoFlag, uint32 nbrId, uint32 priority, uint32 interval, uint16 ProcessID);
extern int32 sal_l3_ospf_refresh_timer_set(sys_enable_t bNoFlag, uint32 time, uint16 ProcessID);
extern int32 sal_l3_ospf_auto_cost_set(sys_enable_t bNoFlag, uint32 refbw, uint16 ProcessID);
extern int32 sal_l3_ospf_redistribute_set(sys_enable_t bNoFlag, char *protoName, char *metric_str, char *type_str, char *rmapName, uint16 ProcessID);
extern int32 sal_l3_ospf_dft_info_set(sys_enable_t bNoFlag, sys_enable_t always, char *metric_str, char *type_str, char *rmapName, uint16 ProcessID);
extern int32 sal_l3_ospf_distri_list_out_set(sys_enable_t bNoFlag, char *alistName, char *protoName, uint16 ProcessID);
extern int32 sal_l3_ospf_default_metric_set(sys_enable_t bNoFlag, uint32 default_metric, uint16 ProcessID);
extern int32 sal_l3_ospf_distance_ospf_set(sys_enable_t bNoFlag, uint32 intra_dis, uint32 inter_dis, uint32 external_dis, uint16 ProcessID);
extern int32 sal_l3_ospf_distance_prefix_set(sys_enable_t bNoFlag, char *dis_str, char *prefix_str, char *alistName, uint16 ProcessID);
extern int32 sal_l3_ospf_distance_set(sys_enable_t bNoFlag, uint32 distance, uint16 ProcessID);
extern int32 sal_ip_ospf_authentication_mode_set(sys_enable_t bNoFlag, char *ifname, int auth_type, sys_ipv4_t addr);
extern int32 sal_ip_ospf_authentication_simple_set(sys_enable_t bNoFlag, char *ifname, char *passwdStr, sys_ipv4_t addr);
extern int32 sal_ip_ospf_authentication_md5_set(sys_enable_t bNoFlag, char *ifname, ulong_t keyId, char *keyStr, sys_ipv4_t addr);
extern int32 sal_ip_ospf_cost_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr);
extern int32 sal_ip_ospf_priority_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr);
extern int32 sal_ip_ospf_hello_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr);
extern int32 sal_ip_ospf_dead_set(sys_enable_t bNoFlag, char *ifname, char *interval_str, char *fast_hello_str, sys_ipv4_t addr);
extern int32 sal_ip_ospf_transmit_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr);
extern int32 sal_ip_ospf_retransmit_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr);
extern int32 sal_ip_ospf_network_set(sys_enable_t bNoFlag, char *ifname, int type);
extern int32 sal_ip_ospf_mtu_ignore_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr);

extern int32 sal_ospf_Info_get(OSPF_INFO_T *pOspf);
extern int32 sal_ospf_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo);
extern int32 sal_ospf_process_status_get(DATA_OP_ENT op, uint16* pProcess);
extern int32 sal_ospf_area_network_get(DATA_OP_ENT op, OSPF_AREA_NETWORK_T* network);
extern int32 sal_ospf_area_range_get(DATA_OP_ENT op, OSPF_AREA_RANGE_T* range);
extern int32 sal_ospf_route_info_get(DATA_OP_ENT op, OSPF_ROUTE_INFO_T* pRouteInfo);
extern int32 sal_ospf_route_border_info_get(DATA_OP_ENT op, OSPF_RTR_ROUTE_INFO_T* pRouteInfo);
extern int32 sal_l3_ospf_virtual_link_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pVirIntfInfo);
extern int32 sal_l3_ospf_virtual_link_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pVirIntfInfo);
extern int32 sal_l3_ospf_intf_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pIntfInfo);
extern int32 sal_l3_ospf_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo);
extern int32 sal_l3_ospf_lsa_info_get(DATA_OP_ENT op, OSPF_LSA_INFO_T *pLsaInfo);
extern int32 sal_ospf_redist_info_get(DATA_OP_ENT op, OSPF_REDIST_INFO_T *pRedistInfo);
extern int32 sal_ospf_passive_interface_get(DATA_OP_ENT op, OSPF_PASSIVE_INTF_INFO_T *pIntfInfo);
extern int32 sal_ospf_redist_nfo_get(OSPF_REDIST_INFO_T *pRedistInfo);
extern int32 sal_ospf_distance_get(DATA_OP_ENT op, OSPF_DISTANCE_INFO_T *pDistInfo);

#endif 

