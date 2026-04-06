
#ifndef __CMD_ACL_MGMT_H__
#define __CMD_ACL_MGMT_H__

#include <libsal/sal_acl_mgmt.h>

extern int32 cmd_acl_mgmtList_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmtClass_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_list_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_list_exit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_ace_ipSourceNocare_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_acl_mgmt_ace_ipSourceNocare_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_ace_ipCare_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_acl_mgmt_ace_ipCare_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_ace_ipv6Care_permit(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_acl_mgmt_ace_ipv6Care_deny(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

 extern int32 cmd_acl_mgmt_ace_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_class_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_acl_mgmt_class_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_list_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_class_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_mgmt_ace2cmd_convert( sys_acl_mgmt_ace_t  *pstacl_mgmt_ace, char *buf);

extern int32 cmd_acl_mgmt_save(FILE *fp);

#endif

