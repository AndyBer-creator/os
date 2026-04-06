
#ifndef __CMD_PSECURE_H__
#define __CMD_PSECURE_H__

extern int32 cmd_portsec_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_addr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_addr_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portLimitNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portViolation_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portSticky_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portStickyMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_portStaticMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_psecure_save(FILE *fp);

extern int32 cmd_psecure_port_save(FILE *fp, sys_logic_port_t port);

#endif 

