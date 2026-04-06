
#ifndef __CMD_MIRROR_H__
#define __CMD_MIRROR_H__

#include <libsal/sal_mirror.h>

extern int32 cmd_mirror_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_session_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_group_sport_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_group_dport_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_group_remote_srcVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_group_remote_reflectPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mirror_save(FILE *fp);

#endif 

