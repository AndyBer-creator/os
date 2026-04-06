
#ifndef __CMD_LOOPBACK_H__
#define __CMD_LOOPBACK_H__

#define IS_BID_EQUAL(b1,b2) \
    (((b1)->priority == (b2)->priority) && \
     ((b1)->addr[0] == (b2)->addr[0]) && \
     ((b1)->addr[1] == (b2)->addr[1]) && \
     ((b1)->addr[2] == (b2)->addr[2]) && \
     ((b1)->addr[3] == (b2)->addr[3]) && \
     ((b1)->addr[4] == (b2)->addr[4]) && \
     ((b1)->addr[5] == (b2)->addr[5]))

int32 cmd_loopback_proto_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_detectionTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_resumeTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_port_resume_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_port_resume_manual_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_loopback_port_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_loopback_save(FILE *fp);

#endif

