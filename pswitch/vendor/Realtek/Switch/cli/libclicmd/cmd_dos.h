
#ifndef __CMD_DOS_H__
#define __CMD_DOS_H__

extern int32 cmd_dos_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_typeEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_maxPingLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_maxPingLen_set_default(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_minIPv6FragLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_smurfNetmaskLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_minTCPHdrLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_portAttackPreventEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_save(FILE *fp);

extern int32 cmd_dos_port_save(FILE *fp, sys_logic_port_t port);

#if 0

extern int32 cmd_dos_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_portEnableLength_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dos_port_save(FILE *fp, sys_logic_port_t port);
#endif

#endif

