
#ifndef __CMD_DAI_H__
#define __CMD_DAI_H__

extern int32 cmd_dai_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_portStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_vlan_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_port_trust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_port_smacChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_port_dmacChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_port_ipChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dai_save(FILE *fp);

extern int32 cmd_dai_port_save(FILE *fp, sys_logic_port_t port);

#endif

