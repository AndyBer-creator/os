
#ifndef __CMD_L3_BGP_H__
#define __CMD_L3_BGP_H__

int32 cmd_l3_router_bgp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_bgp_network_area_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#if 0
int32 cmd_rip_version(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_route_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distance_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distance_source_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_distribute_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_rip_access_list_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_rip_status(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#endif 

