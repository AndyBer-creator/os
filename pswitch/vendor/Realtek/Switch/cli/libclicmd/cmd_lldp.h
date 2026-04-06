
#ifndef __CMD_LLDP_H__
#define __CMD_LLDP_H__

extern int32 cmd_lldp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_local_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portLocal_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_remote_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portRemote_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_globalStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_statistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portStatistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_portTlvOverload_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_transmit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_receive_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_holdtime(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_tx_interval(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_reinit_delay(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_tx_delay(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_optional_tlv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_optional_tlv_pvid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_optional_tlv_vlan_name_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_lldpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lldp_save(FILE *fp);

extern int32 cmd_lldp_port_save(FILE *fp, sys_logic_port_t port);

#endif 

