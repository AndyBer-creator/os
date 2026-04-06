
#ifndef __CMD_SNMP_H__
#define __CMD_SNMP_H__

#include <libsal/sal_snmp.h>

extern int32 cmd_snmp_demo_restart(void);

extern int32 cmd_snmp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_MIB_TRAP

extern int32 cmd_snmp_trap_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_snmp_view_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_community_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_host_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_user_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_engineid_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_MIB_TRAP

extern int32 cmd_snmp_trapClass_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_snmp_host_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_host_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_SNMPV3

extern int32 cmd_snmp_view_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_view_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_group_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_access_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_access_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_accessv3_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_comView_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_comGroup_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_com2sec_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_authUser_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_privUser_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_user_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_engineid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_remoteEngineid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_hostv3_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_restart(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_config_reload_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#else

extern int32 cmd_snmp_community_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_snmp_community_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_snmp_save(FILE *fp);

#endif

