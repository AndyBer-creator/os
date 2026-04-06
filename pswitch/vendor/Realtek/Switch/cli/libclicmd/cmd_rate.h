
#ifndef __SAL_CMD_RATE_H__
#define __SAL_CMD_RATE_H__

#include <libsal/sal_rate.h>

#ifdef CONFIG_SYS_STORM_CTRL

extern int32 cmd_rate_stromCtrl_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_portStromCtrl_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_STORM_CTRL

extern int32 cmd_rate_stormControlPortEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_stormControlTypeEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_stormControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_stormControlAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_stormCtrlIncludeIfg_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_stormCtrlRefreshMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif 

#ifdef CONFIG_SYS_BW_CTRL

extern int32 cmd_rate_igrbandwidthControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_igrbandwidthControlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_portIgrbandwidthControlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_egrBandwidthCtrlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_egrQueueControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_egrBandwidthCtrlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_egrSysQueueControlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_rate_bandwidthCtrlIncludeIfg_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32
cmd_rate_flowBurst_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

 extern int32 cmd_rate_limit_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif 

extern int32 cmd_rate_save(FILE *fp);

extern int32 cmd_rate_port_save(FILE *fp, sys_logic_port_t port);

#endif 
