
#ifndef __CMD_RADIUS_H__
#define __CMD_RADIUS_H__

extern int32 cmd_radius_dfltConfig_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_radius_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_radiusSrv_dftParam_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_radiusSrv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_radius_save(FILE *fp);

#endif 

