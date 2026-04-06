
#ifndef __CMD_MLD_H__
#define __CMD_MLD_H__

extern int32 cmd_mld_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_static_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_statistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_router_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_mac_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

extern int32 cmd_mld_forward_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_mld_groupCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cmd_mld_maxGroup_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_maxGroupAction_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cmd_mld_filter_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_profile_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_mld_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_unknMcastAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_lookupMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_suppre_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_operVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

extern int32 cmd_mld_static_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_forbidden_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

extern int32 cmd_mld_routerPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_routerPortForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_mld_fastleave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_pimDvmrpLearn_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_robustnessVar_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_responseTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_queryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_lastMbrQueryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_lastMbrQueryCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cmd_mld_portGroup_limit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_portGroup_excessAct_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cmd_mld_profile_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_profile_remove(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_filter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mld_profileEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_mld_save(FILE *fp);

extern int32 cmd_mld_port_save(FILE *fp, sys_logic_port_t port);

#endif 

