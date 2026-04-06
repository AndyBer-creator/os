
#ifndef __CMD_DEBUG_H__
#define __CMD_DEBUG_H__

typedef struct cmd_dbg_desc_s
{
    const char       text[SYS_BUF64_LEN];
    const sys_text_t *extdesc;
} cmd_dbg_desc_t;

extern const cmd_inst_tbl_t cmdDebugTbl[];

extern int32 cmd_debug_init(void);

extern int32 cmd_debug_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_debug_booting_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_acl_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 cmd_auth_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_DAI

extern int32 cmd_dai_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

extern int32 cmd_ds_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_ISG

extern int32 cmd_isg_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_l2_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 cmd_l3_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_LLDP

extern int32 cmd_lldp_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PORT_SECURITY

extern int32 cmd_psecure_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI

extern int32 cmd_rmon_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_time_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_UDLD

extern int32 cmd_udld_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_vlan_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_debug_stp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_debug_lag_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_debug_mcast_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_debug_platform_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
extern int32 cmd_debug_erps_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
extern int32 cmd_dhcp_server_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

