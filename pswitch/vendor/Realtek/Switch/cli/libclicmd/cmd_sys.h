
#ifndef __CMD_SYS_H__
#define __CMD_SYS_H__

#include <libclicmd/cmd_sys_auth.h>
#include <libclicmd/cmd_sys_line.h>

extern int32 cmd_sys_version_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_info_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ip_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipDhcp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_ADVANCE_DNS

extern int32 cmd_sys_advDns_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PING_TEST

extern int32 cmd_sys_ping_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_TRACE_ROUTE

extern int32 cmd_sys_traceroute_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_reboot_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ssl_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_CPU_UTILIZATION

extern int32 cmd_sys_cpuUtilz_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_cpuRate_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_MEM_UTILIZATION

extern int32 cmd_sys_memory_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_IPV6

int32 cmd_sys_ipv6_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_systemName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_systemLocation_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_systemContact_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_dhcp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipAddr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipAddr_sub_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipDfltGateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_ADVANCE_DNS

extern int32 cmd_sys_dnsLookup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_dnsDomainName_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_dnsNameServer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_dnsHostMapping_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_dhcp6c_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipv6Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipv6Autoconf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipv6Addr_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_ipv6Gateway_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_multiLang_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_multiLangFile_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_save(FILE *fp);
extern int32 cmd_sys_systemManufacturer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_systemSupportInfo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_systemTelephone_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif 

