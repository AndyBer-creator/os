
#ifndef __CMD_L3_VRRP_H__
#define __CMD_L3_VRRP_H__

int32 cmd_vrrp_vip_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_vrrp_priority_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_vrrp_preempt_mode_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_vrrp_advertise_timer_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_vrrp_track_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_vrrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_l3_vrrp_interface_save(FILE *fp, sys_vid_t vid);

#endif 

