
#ifndef __CMD_ISG_H__
#define __CMD_ISG_H__

extern int32 cmd_isg_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_bind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

extern int32 cmd_isg_dhcpSnoopingbind_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_dhcpSnoopingDb_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_dhcpSnoopingDb_renew(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_dhcpSnoopingDb_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

extern int32 cmd_isg_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_portMaxEntryNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_bindTblEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_db_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_isg_save(FILE *fp);

extern int32 cmd_isg_port_save(FILE *fp, sys_logic_port_t port);

#endif

