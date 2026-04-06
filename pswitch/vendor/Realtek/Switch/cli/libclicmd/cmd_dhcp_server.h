
#ifndef __CMD_DHCP_SERVER_H__
#define __CMD_DHCP_SERVER_H__

extern int32 cmd_dhcp_server_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_server_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_server_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_server_intf_group_ip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_server_intf_group_ip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_device_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_info_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_format_verbose_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_port_strategy_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_port_circuit_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_relay_option82_port_remote_id_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_ip_pool_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_ip_pool_gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_ip_pool_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_ip_pool_gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_ip_pool_section_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_no_ip_pool_section(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_ip_pool_option43_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_relay_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_server_dns_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_server_dns_suffix_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_server_wins_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_server_lease_time_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_server_save(FILE *fp);

extern int32 cmd_dhcp_sever_l3intf_save(FILE *fp, sys_vid_t vid);

extern int32 cmd_dhcp_relay_port_save(FILE *fp, sys_logic_port_t port);
extern int32 cmd_dhcp_ip_pool_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_client_config_ip_bind(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_client_host_mac_ip_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_client_host_mac_table_all_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_client_unknown_client_assign(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_client_ip_bind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_ip_pool_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_dhcp_ip_pool_brief_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_show_dhcps_hash(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_sever_mgmt_intf_save(FILE *fp);
extern int32 cmd_no_ip_pool_gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_ip_pool_disable_ip(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

