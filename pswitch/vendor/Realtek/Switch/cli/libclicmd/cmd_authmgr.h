
#ifndef __CMD_AUTHMGR_H__
#define __CMD_AUTHMGR_H__

extern int32 cmd_authmgr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_session_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_session_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_typeEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_MACAUTH

extern int32 cmd_authmgr_radiusUserFmt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_macauthLocalInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

extern int32 cmd_authmgr_webauthLocalInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_authmgr_guestVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portGuestVlanEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portTypeEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portCtrl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portOrder_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portMethodOrder_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portReauthEnbl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portMaxHost_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portVlanAssignMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portReauthPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portInactTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portQuietPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_DOT1X

extern int32 cmd_authmgr_portSuppTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portSrvTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portEapTxPeriod_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_authmgr_portMaxEapReq_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH

extern int32 cmd_authmgr_portMaxLoginAttempt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_authmgr_save(FILE *fp);

extern int32 cmd_authmgr_port_save(FILE *fp, sys_logic_port_t port);

#endif 

