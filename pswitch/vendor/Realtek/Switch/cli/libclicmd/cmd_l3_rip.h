
#ifndef __CMD_L3_RIP_H__
#define __CMD_L3_RIP_H__

int32 cmd_l3_router_rip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_rip_network_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_version(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_route_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distance_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distance_source_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distribute_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_access_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_rip_status(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_access_list(void);
int32 cmd_show_distribute_list(void);
int32 cmd_rip_debug(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_log_to_fileName(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_log_to_stdout(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_rip_status(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_rip_route_information(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_router_rip_save(FILE *fp);
uint32 cmd_bool_interface_ip_in_rip_network(uint32 ipPrefix, uint32 ipPreLen);

#if 0

extern int32 cmd_l3_intfIPv4_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_routeV4_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_routeV6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_neigh_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_neigh_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_arpConf_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfExist_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_fwdIPv4Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intIPv4Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_staticRouteV4_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_fwdIPv6Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_icmpv6RateInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_icmpv6RateBurst_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_routev6Prefer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6Enbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6Mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6AutoconfEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6DadAttempt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6Dhcp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6DestUnreach_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS

extern int32 cmd_l3_intfIPv6DhcpStateless_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6DhcpRefreshTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_intfIPv6DhcpMinRefreshTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_l3_intfIPv6Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_staticRouteV6_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_staticRouteV6LinkLocal_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_staticRouteV6Reject_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_neigh_ageTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_l3_neigh_respTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_l3_neigh_retryTimes_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_l3_neigh_cacheSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_l3_neigh_renew_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_neighStatic_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l3_save(FILE *fp);

extern int32 cmd_l3_vlan_save(FILE *fp, sys_vid_t vid);

extern int32 cmd_l3_lo_save(FILE *fp, uint32 id);
#endif
#endif 

