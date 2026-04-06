
#ifndef __CMD_LLDP_MED_H__
#define __CMD_LLDP_MED_H__

extern int32 cmd_lldp_med_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portMed_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_fast_start_repeat_count_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_optional_tlv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_network_policy_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_network_policy_add_remove_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#if 0

extern int32 cmd_lldp_med_network_policy_voice_auto_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_lldp_med_location_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_med_save(FILE *fp);

extern int32 cmd_lldp_med_port_save(FILE *fp, sys_logic_port_t port);

#endif 

