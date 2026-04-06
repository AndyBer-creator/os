
#ifndef __CMD_ACL_H__
#define __CMD_ACL_H__

#include <libsal/sal_acl_usr.h>

extern sys_namestr_t g_aclname;
extern sys_acl_type_t g_acltype;

extern int32 cmd_acl_aceMac2cmd_convert(sys_ace_content_t *ace_content, char *buf);

extern int32 cmd_acl_aceIp2cmd_convert(sys_ace_content_t *ace_content, char *buf);

extern int32 cmd_acl_all_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_byName_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_byType_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_ace_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_utilization_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_exit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_aceMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_aceIP_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_ace_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_bind_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_unbind_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_save(FILE *fp);

extern int32 cmd_acl_port_save(FILE *fp, sys_logic_port_t port);
#endif

