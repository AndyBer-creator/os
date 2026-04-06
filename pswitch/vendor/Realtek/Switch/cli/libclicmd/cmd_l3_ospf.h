
#ifndef __CMD_L3_OSPF_H__
#define __CMD_L3_OSPF_H__

#define OSPF_IFTYPE_NONE		0
#define OSPF_IFTYPE_POINTOPOINT		1
#define OSPF_IFTYPE_BROADCAST		2
#define OSPF_IFTYPE_NBMA		3
#define OSPF_IFTYPE_POINTOMULTIPOINT	4
#define OSPF_IFTYPE_VIRTUALLINK		5
#define OSPF_IFTYPE_LOOPBACK            6
#define OSPF_IFTYPE_MAX			7
#define RN                "\r\n"       
#define NSM_DependUpon          0
#define NSM_Deleted		1
#define NSM_Down		2
#define NSM_Attempt		3
#define NSM_Init		4
#define NSM_TwoWay		5
#define NSM_ExStart		6
#define NSM_Exchange		7
#define NSM_Loading		8
#define NSM_Full		9
#define OSPF_NSM_STATE_MAX     10

#define OSPF_UNKNOWN_LSA	      0
#define OSPF_ROUTER_LSA               1
#define OSPF_NETWORK_LSA              2
#define OSPF_SUMMARY_LSA              3
#define OSPF_ASBR_SUMMARY_LSA         4
#define OSPF_AS_EXTERNAL_LSA          5
#define OSPF_GROUP_MEMBER_LSA	      6  
#define OSPF_AS_NSSA_LSA	              7
#define OSPF_EXTERNAL_ATTRIBUTES_LSA  8  
#define OSPF_OPAQUE_LINK_LSA	      9
#define OSPF_OPAQUE_AREA_LSA	     10
#define OSPF_OPAQUE_AS_LSA	     11

#define OSPF_MIN_LSA		1  
#define OSPF_MAX_LSA           12

#define OSPF_AREA_RANGE_ADVERTISE	(1 << 0)
#define OSPF_AREA_RANGE_SUBSTITUTE	(1 << 1)

#define OSPF_AREA_RANGE_COST_UNSPEC	-1U

#define OSPF_AREA_DEFAULT       0
#define OSPF_AREA_STUB          1
#define OSPF_AREA_NSSA          2
#define OSPF_AREA_TYPE_MAX	3

#define OSPF_SHORTCUT_DEFAULT	0
#define OSPF_SHORTCUT_ENABLE	1
#define OSPF_SHORTCUT_DISABLE	2

int32 cmd_l3_router_ospf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_network_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_range_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_range_substitute_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_vlink_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_stub_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_nssa_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_default_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_export_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_import_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_filter_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_authentication_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_shortcut_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_router_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_abr_type_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_log_adjacency_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_rfc1583compatibility_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_spf_timer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_static_neighbor_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_refresh_timer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_auto_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_redistribute_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_dft_info_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_distri_list_out_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_default_metric_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_distance_ospf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_distance_prefix_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_distance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

int32 cmd_l3_ospf_passive_interface_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_authentication_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_authentication_simple_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_authentication_md5_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_cost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_priority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_hello_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_dead_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_transmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_retransmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_network_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ip_ospf_mtu_ignore_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

int32 cmd_ospf_neighbor_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_ospf_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_ospf(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_ospf_route_table(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ospf_virtual_link_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ospf_interface_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_ospf_database_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_router_ospf_save(FILE *fp);
int32 cmd_l3_ospf_interface_save(FILE *fp, sys_vid_t vid);

#endif 

