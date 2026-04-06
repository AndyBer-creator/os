
#ifndef __CMD_STP_H__
#define __CMD_STP_H__

#define IS_BID_EQUAL(b1,b2) \
    (((b1)->priority == (b2)->priority) && \
     ((b1)->addr[0] == (b2)->addr[0]) && \
     ((b1)->addr[1] == (b2)->addr[1]) && \
     ((b1)->addr[2] == (b2)->addr[2]) && \
     ((b1)->addr[3] == (b2)->addr[3]) && \
     ((b1)->addr[4] == (b2)->addr[4]) && \
     ((b1)->addr[5] == (b2)->addr[5]))

extern int32 cmd_stp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 cmd_stp_mstConfig_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mst_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portMst_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_stp_portStatistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portStatistics_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_forwardBpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_forceVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_helloTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_maxHops_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_forwardDelay_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_maximumAge_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_txHoldCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_pathcostMethod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_priority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portAdminEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portForwardBpdu_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portPathCost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portEdgePort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portBpduFilter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portBpduGuard_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portPointToPointMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_portMcheck_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 cmd_stp_mst_config(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstConfigName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstConfigRevision_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstVlanInstance_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstPortPathCost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_stp_mstPortPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

extern int32 cmd_stp_save(FILE *fp);

extern int32 cmd_stp_port_save(FILE *fp, sys_logic_port_t port);

#endif

