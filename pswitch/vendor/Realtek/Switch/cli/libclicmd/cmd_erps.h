
#ifndef __CMD_ERPS_H__
#define __CMD_ERPS_H__

#define IS_BID_EQUAL(b1,b2) \
    (((b1)->priority == (b2)->priority) && \
     ((b1)->addr[0] == (b2)->addr[0]) && \
     ((b1)->addr[1] == (b2)->addr[1]) && \
     ((b1)->addr[2] == (b2)->addr[2]) && \
     ((b1)->addr[3] == (b2)->addr[3]) && \
     ((b1)->addr[4] == (b2)->addr[4]) && \
     ((b1)->addr[5] == (b2)->addr[5]))

int32 cmd_erps_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_inst_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ctrlVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_wtrTimer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_guardTimer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_workMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ringId_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ringLevel_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ringPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ring_protected_instance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_mel_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_ringState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_show_erps(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
int32 cmd_erps_save(FILE *fp);

#endif

