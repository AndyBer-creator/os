
#ifndef __SYS_SWDEF_CLI_H__
#define __SYS_SWDEF_CLI_H__
#define CAPA_CLI_MAX_CMDSTR_LEN         512
#define CAPA_CLI_MAX_LINES_NUM          DEFDB_VAL(DEF_CLI_MAX_LINES_NUM)              
#define BOND_CLI_HISTORY_ENTRY_MIN      1
#define BOND_CLI_HISTORY_ENTRY_MAX      256
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
#define BOND_CLI_PASSRETRY_MIN          0
#define BOND_CLI_PASSRETRY_MAX          120
#define BOND_CLI_SILENTTIME_MIN         0
#define BOND_CLI_SILENTTIME_MAX         65535
#endif
#define DFLT_CLI_LOGIN_IDLE_TIMEOUT     DEFDB_VAL(DEF_CLI_LOGIN_IDLE_TIMEOUT) 
#define DFLT_CLI_RECOVERY_PASSWORD      DEFDB_STR(DEF_CLI_RECOVERY_PASSWORD)
#define DFLT_CLI_DEBUG_PASSWORD         DEFDB_STR(DEF_CLI_DEBUG_PASSWORD)     
#define DFLT_CLI_REMOTE_DEBUG           DEFDB_VAL(DEF_CLI_REMOTE_DEBUG)       
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
#define DFLT_CLI_PASSRETRY_CONSOLE      3                                     
#define DFLT_CLI_PASSRETRY_TELNET       3                                     
#define DFLT_CLI_PASSRETRY_SSH          3                                     
#define DFLT_CLI_SILENTTIME_CONSOLE     DEFDB_VAL(DEF_CLI_SILENTTIME_CONSOLE) 
#define DFLT_CLI_SILENTTIME_TELNET      DEFDB_VAL(DEF_CLI_SILENTTIME_TELNET)  
#define DFLT_CLI_SILENTTIME_SSH         DEFDB_VAL(DEF_CLI_SILENTTIME_SSH)     
#endif
#define DFLT_CLI_HISTORY_MAX_ENTRY      DEFDB_VAL(DEF_CLI_HISTORY_MAX_ENTRY)
#define DFLT_CLI_PROMPT_VIEW            "%s> "
#define DFLT_CLI_PROMPT_ENABLE          "%s# "
#define DFLT_CLI_PROMPT_CONFIG          "%s(config)# "
#define DFLT_CLI_PROMPT_IF              "%s(config-if)# "
#define DFLT_CLI_PROMPT_IF_RANGE        "%s(config-if-range)# "
#define DFLT_CLI_PROMPT_VTY             "%s(config-line)# "
#define DFLT_CLI_PROMPT_VLAN            "%s(config-vlan)# "
#define DFLT_CLI_PROMPT_DEBUG           "%s(debug)# "
#define DFLT_CLI_PROMPT_IF_EX           "%s(config-if-%s)# "
#define DFLT_CLI_PROMPT_IF_RANGE_EX     "%s(config-if-range-%s)# "

#ifdef CONFIG_SYS_USER_DEFINED_ACL
#define DFLT_CLI_PROMPT_MAC_ACL         "%s(config-mac-acl)# "
#define DFLT_CLI_PROMPT_IPV4_ACL        "%s(config-ip-acl)# "
#define DFLT_CLI_PROMPT_IPV6_ACL        "%s(config-ipv6-acl)# "
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
#define DFLT_CLI_PROMPT_IGMP_PROFILE    "%s(config-igmp-profile)# "
#endif

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define DFLT_CLI_PROMPT_MLD_PROFILE     "%s(config-mld-profile)# "
#endif

#ifdef CONFIG_SYS_MGMT_ACL
#define DFLT_CLI_PROMPT_MGMT_ACL        "%s(config-macl)# "
#endif

#ifdef CONFIG_SYS_PROTO_STP_MSTP
#define DFLT_CLI_PROMPT_MST             "%s(config-mst)# "
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
#define DFLT_CLI_PROMPT_ERPS_INST       "%s(config-erps-inst)# "
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
#define DFLT_CLI_PROMPT_DHCP_IPPOOL    "%s(config-ip-pool-%s)# "
#endif

#ifdef CONFIG_SYS_L3_ROUTE_RIP
#define DFLT_CLI_PROMPT_RIP            "%s(config-rip)# "
#endif

#ifdef CONFIG_SYS_L3_ROUTE_OSPF
#define DFLT_CLI_PROMPT_OSPF            "%s(config-ospf-%s)# "
#define DFLT_CLI_PROMPT_OSPF_AREA       "%s(config-ospf-%s-area-%s)# "
#endif

#ifdef CONFIG_SYS_APP_EASYCWMP
#define DFLT_CLI_PROMPT_CWMP            "%s(config-cwmp)# "
#endif

#endif 

