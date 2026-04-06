
#ifndef __SAL_CMD_QOS_H__
#define __SAL_CMD_QOS_H__

#include <libsal/sal_qos.h>

extern int32 cmd_qos_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_queue_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_map_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_strictPriQueueNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_queueWeight_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_trust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_cosQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_dscpQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_ippQueue_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_portCos_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_portTrust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_queueCos_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_queueDscp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_queueIpp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_portRemark_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_qos_save(FILE *fp);

extern int32 cmd_qos_port_save(FILE *fp, sys_logic_port_t port);

#endif 
