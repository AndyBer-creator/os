
#ifndef __CMD_IGMP_H__
#define __CMD_IGMP_H__

extern int32 cmd_igmp_static_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_statistics_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_router_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

extern int32 cmd_igmp_forward_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_igmp_groupCnt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

extern int32 cmd_igmp_groupMode_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

extern int32 cmd_igmp_querier_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cmd_igmp_maxGroup_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_maxGroupAction_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cmd_igmp_filter_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_profile_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif

extern int32 cmd_igmp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_suppre_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_lookupMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_fastleave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_routerTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_robustnessVar_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_pimDvmrpLearn_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_responseTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_queryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_lastMbrQueryIntvl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_lastMbrQueryCount_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_querier_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

extern int32 cmd_igmp_routerPort_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_routerPortForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_igmp_operVersion_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

extern int32 cmd_igmpv3_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_static_filter_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_igmp_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_filter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_unknMcastAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD

extern int32 cmd_igmp_static_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_forbidden_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 cmd_igmp_portGroup_limit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_portGroup_excessAct_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 cmd_igmp_profile_enter_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_profileEntry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_profile_remove(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_igmp_save(FILE *fp);

extern int32 cmd_igmp_group_save(FILE *fp);

extern int32 cmd_igmp_port_save(FILE *fp, sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTO_MVR

extern int32 cmd_mvr_group_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_interface_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_mvr_member_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_vlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_mode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_queryTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_portType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_portFastLeave_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_mvr_static_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

extern int32 cmd_igmp_static_group_set_by_mac(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_static_l2_mcast_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_igmp_static_l2_mcast_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_l2_mcast_static_save(FILE *fp);
#endif

