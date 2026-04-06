
#ifndef __CMD_RMON_H__
#define __CMD_RMON_H__

#include <libsal/sal_rmon.h>

extern int32 cmd_rmon_port_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_event_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_eventLog_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_alarm_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_history_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_historyStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_event_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_alarm_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_history_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rmon_save(FILE *fp);

#endif

