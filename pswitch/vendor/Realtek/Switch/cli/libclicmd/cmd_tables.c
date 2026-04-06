
#include <libcmd/cmd.h>
#include <libclicmd/cmd_desc.h>

const cat_tbl_t catNameTbl[] =
{

    {CAT_ID_SYSTEM,             "SYSTEM"},
    {CAT_ID_TIME,               "TIME"},
    {CAT_ID_LINE,               "LINE"},
    {CAT_ID_PORT,               "PORT"},
#ifdef CONFIG_SYS_PORT_SECURITY
    {CAT_ID_PORT_SECURITY,      "PORT_SECURITY"},
#endif
    {CAT_ID_SWITCH,             "Switch"},
#ifdef CONFIG_SYS_EEE
    {CAT_ID_EEE,                "EEE"},
#endif
    {CAT_ID_VLAN,               "VLAN"},
#ifdef CONFIG_SYS_VOICEVLAN
    {CAT_ID_VLAN_VOICE,         "Voice VLAN"},
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {CAT_ID_VLAN_PROTO,         "Protocol VLAN"},
#endif
#ifdef CONFIG_SYS_MACVLAN
    {CAT_ID_VLAN_MAC,           "MAC VLAN"},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {CAT_ID_VLAN_SURVEILLANCE,  "Surveillance VLAN"},
#endif
#ifdef CONFIG_SYS_MIRROR
    {CAT_ID_MIRROR,             "MIRROR"},
#endif
    {CAT_ID_L2,                 "L2"},
#ifdef CONFIG_SYS_L3_ROUTE
    {CAT_ID_L3,                 "L3"},
#endif
    {CAT_ID_TRUNK,              "TRUNK"},
#ifdef CONFIG_SYS_PROTO_LACP
    {CAT_ID_LACP,               "LACP"},
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {CAT_ID_ACL,                "ACL"},
#endif
    {CAT_ID_QOS,                "QOS"},
    {CAT_ID_RATE,               "RATE"},
#ifdef CONFIG_SYS_UI_SNMP
    {CAT_ID_SNMP,               "SNMP"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {CAT_ID_IGMP,               "IGMP"},
#endif

#if defined(CONFIG_SYS_PROTO_IGMP_SNOOPING) && defined(CONFIG_SYS_PROTO_MLD_SNOOPING)
    {CAT_ID_MLD,                "MLD"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {CAT_ID_IGMP_PROFILE,       "IGMP PROFILE"},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {CAT_ID_STP,                "STP"},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {CAT_ID_LLDP,               "LLDP"},
#endif

#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_LLDP_MED)
    {CAT_ID_LLDP_MED,           "LLDP MED"},
#endif

#ifdef CONFIG_SYS_SYSLOG
    {CAT_ID_SYSLOG,             "SYSLOG"},
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    {CAT_ID_DOS,                "DoS"},
#endif
#ifdef CONFIG_SYS_AAA
    {CAT_ID_AAA,                "AAA"},
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    {CAT_ID_CUSTOM,             "Cutsom"},
#endif

#ifdef CONFIG_SYS_PROTO_ISG
    {CAT_ID_ISG,                "ISG"},
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {CAT_ID_DHCP_SNOOPING,      "DHCP_SNOOPING"},
#endif

#ifdef CONFIG_SYS_PROTO_DAI
    {CAT_ID_DAI,                "DAI"},
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {CAT_ID_GVRP,               "GVRP"},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {CAT_ID_GMRP,               "GMRP"},
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    {CAT_ID_RADIUS,             "RADIUS"},
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    {CAT_ID_TACPLUS,            "TACACS+"},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CAT_ID_RMON,               "RMON"},
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    {CAT_ID_MGMTACL,            "MGMT ACL"},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {CAT_ID_UDLD,               "UDLD"},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
    {CAT_ID_ERPS,				"ERPS"},
#endif
#ifdef CONFIG_SYS_POE
	{CAT_ID_POE,				"POE"},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	{CAT_ID_DHCP_SERVER,		"DHCP_SERVER"},
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{CAT_ID_EASYCWMP,		    "easyCWMP"},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	{CAT_ID_LOOPBACCK,				"LOOPBACK"},
#endif

    {CAT_ID_END,                "All Categories"},

    {-1, ""}
};

const cat_tbl_t catSaveNameTbl[] =
{

    {CAT_ID_SYSTEM,             "system"},
    {CAT_ID_TIME,               "time"},
    {CAT_ID_LINE,               "line"},
    {CAT_ID_PORT,               "port"},
#ifdef CONFIG_SYS_PORT_SECURITY
    {CAT_ID_PORT_SECURITY,      "port-security"},
#endif
    {CAT_ID_SWITCH,             "switch"},
#ifdef CONFIG_SYS_EEE
    {CAT_ID_EEE,                "eee"},
#endif
    {CAT_ID_VLAN,               "vlan"},
#ifdef CONFIG_SYS_VOICEVLAN
    {CAT_ID_VLAN_VOICE,         "voice-vlan"},
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {CAT_ID_VLAN_PROTO,         "protocol-vlan"},
#endif
#ifdef CONFIG_SYS_MACVLAN
    {CAT_ID_VLAN_MAC,           "mac-vlan"},
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {CAT_ID_VLAN_SURVEILLANCE,  "surveillance-vlan"},
#endif
#ifdef CONFIG_SYS_MIRROR
    {CAT_ID_MIRROR,             "mirror"},
#endif
    {CAT_ID_L2,                 "l2"},
#ifdef CONFIG_SYS_L3_ROUTE
    {CAT_ID_L3,                 "l3"},
#endif
    {CAT_ID_TRUNK,              "trunk"},
#ifdef CONFIG_SYS_PROTO_LACP
    {CAT_ID_LACP,               "lacp"},
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {CAT_ID_ACL,                "acl"},
#endif
    {CAT_ID_QOS,                "qos"},
    {CAT_ID_RATE,               "rate"},
#ifdef CONFIG_SYS_UI_SNMP
    {CAT_ID_SNMP,               "snmp"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    {CAT_ID_IGMP,               "igmp"},
#endif

#if defined(CONFIG_SYS_PROTO_IGMP_SNOOPING) && defined(CONFIG_SYS_PROTO_MLD_SNOOPING)
    {CAT_ID_MLD,                "mld"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {CAT_ID_IGMP_PROFILE,       "igmp-profile"},
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {CAT_ID_STP,                "stp"},
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {CAT_ID_LLDP,               "lldp"},
#endif

#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_LLDP_MED)
    {CAT_ID_LLDP_MED,           "lldp-med"},
#endif

#ifdef CONFIG_SYS_SYSLOG
    {CAT_ID_SYSLOG,             "syslog"},
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    {CAT_ID_DOS,                "dos"},
#endif
#ifdef CONFIG_SYS_AAA
    {CAT_ID_AAA,                "aaa"},
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
    {CAT_ID_CUSTOM,             "custom"},
#endif

#ifdef CONFIG_SYS_PROTO_ISG
    {CAT_ID_ISG,                "isg"},
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {CAT_ID_DHCP_SNOOPING,      "dhcp-snooping"},
#endif

#ifdef CONFIG_SYS_PROTO_DAI
    {CAT_ID_DAI,                "dai"},
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {CAT_ID_GVRP,               "gvrp"},
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    {CAT_ID_GMRP,               "gmrp"},
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    {CAT_ID_RADIUS,             "radius"},
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    {CAT_ID_TACPLUS,            "tacplus"},
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CAT_ID_RMON,               "rmon"},
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    {CAT_ID_MGMTACL,            "mgmt-acl"},
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
    {CAT_ID_UDLD,               "udld"},
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
    {CAT_ID_ERPS,				"erps"},
#endif
#ifdef CONFIG_SYS_POE
	{CAT_ID_POE,				"POE"},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	{CAT_ID_DHCP_SERVER,		"dhcp-server"},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	{CAT_ID_L3_ROUTE_RIP,		"rip"},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	{CAT_ID_L3_ROUTE_OSPF,		"ospf"},
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{CAT_ID_EASYCWMP,		    "cwmp"},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	{CAT_ID_LOOPBACCK,			"loopback"},
#endif
    {CAT_ID_END,                "all categories"},

    {-1, ""}
};

const cmd_inst_tbl_t cmdCommonTbl[] =
{
#ifdef CONFIG_SYS_AAA
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_authenList_show,
         "show aaa authentication (login|enable) lists",
        SHOW_DESC
        AAA_DESC
        AAA_AUTHEN_DESC
        AAA_LOGIN_DESC AAA_ENABLE_DESC
        AAA_AUTHLIST_DESC
    },
  #ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineLists_show,
         "show line lists",
        SHOW_DESC
        LINE_DESC
        AAA_AUTHLIST_DESC
    },
  #endif
#endif

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_all_show,
         "show acl",
        SHOW_DESC
        "Display not empty access control lists (ACLs) configured on the switch\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_byName_show,
         "show (mac|ip|ipv6) acl NAME",
        SHOW_DESC
        "MAC configuration\n" "Global IP configuration commands\n" "IPV6 configuration\n"
        "Display not empty access control lists (ACLs) configured on the switch\n"
        "Name of the ACL\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_byType_show,
         "show (mac|ip|ipv6) acl",
        SHOW_DESC
        "MAC configuration\n" "Global IP configuration commands\n" "IPV6 configuration\n"
        "Display not empty access control lists (ACLs) configured on the switch\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_ace_show,
         "show acl",
        SHOW_DESC
        "Display all access control entries(ACEs) configured on this ACL\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_utilization_show,
         "show acl utilization",
        SHOW_DESC
        "Display all access control entries(ACEs) configured on this ACL\n"
        "Utilization"
    },
#endif 

#ifdef CONFIG_SYS_MGMT_ACL
    
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_list_exit,
         "exit",
        "Exit from current context\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmtList_show,
         "show management access-list [NAME]",
        SHOW_DESC
        MGMT_ACL_IPMGMT_SHOW_DESC
        MGMT_ACL_LIST_SHOW_DESC
        MGMT_ACL_LIST_NAME_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmtClass_show,
         "show management access-class",
        SHOW_DESC
        MGMT_ACL_IPMGMT_SHOW_DESC
        MGMT_ACL_CLASS_SHOW_DESC
    },
#endif 

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_show,
         "show authentication",
        SHOW_STR
        AUTH_SHOW_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_show,
         "show authentication interfaces IF_NMLPORTS",
        SHOW_STR
        AUTH_SHOW_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_show,
         "show authentication sessions [detail]",
        SHOW_STR
        AUTH_SHOW_DESC
        AUTH_SESS_DESC
        "Display session detail information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_show,
         "show authentication sessions interfaces IF_NMLPORTS",
        SHOW_STR
        AUTH_SHOW_DESC
        AUTH_SESS_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_show,
         "show authentication sessions session-id WORD",
        SHOW_STR
        AUTH_SHOW_DESC
        AUTH_SESS_DESC
        "Use session id to find specific session\n"
        "Session ID string (Not case sensitive)\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_show,
         "show authentication sessions type (dot1x|mac|web)",
        SHOW_STR
        AUTH_SHOW_DESC
        AUTH_SESS_DESC
        "Use authentication type to find sessions\n"
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_clear,
         "clear authentication sessions",
        CLEAR_STR
        AUTH_CLEAR_DESC
        AUTH_SESS_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_clear,
         "clear authentication sessions interfaces IF_NMLPORTS",
        CLEAR_STR
        AUTH_CLEAR_DESC
        AUTH_SESS_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_clear,
         "clear authentication sessions mac A:B:C:D:E:F",
        CLEAR_STR
        AUTH_CLEAR_DESC
        AUTH_SESS_DESC
        "Use MAC address to find specific session\n"
        "MAC Address of host\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_clear,
         "clear authentication sessions session-id WORD",
        CLEAR_STR
        AUTH_CLEAR_DESC
        AUTH_SESS_DESC
        "Use session id to find specific session\n"
        "Session ID string (Not case sensitive)\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_session_clear,
         "clear authentication sessions type (dot1x|mac|web)",
        CLEAR_STR
        AUTH_CLEAR_DESC
        AUTH_SESS_DESC
        "Use authentication type to find sessions\n"
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_WEB_DESC
    },
#endif

#ifdef CONFIG_SYS_CABLE_DIAG
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_cablediag_show,
         "show cable-diag interfaces IF_NMLPORTS",
        SHOW_DESC
        CABLEDIAG_COPPER_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
  #ifdef CONFIG_SYS_OPTICALMOD_STATUS
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_cablediag_fiber_show,
         "show fiber-transceiver interfaces IF_NMLPORTS",
        SHOW_DESC
        CABLEDIAG_OPTICAL_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_cablediag_fiber_module_show,
         "show fiber-transceiver interfaces IF_NMLPORTS details",
        SHOW_DESC
        CABLEDIAG_OPTICAL_DESC
        IFS_DESC
        ARG_IF_NMLPORT
        "SFP details"
    },
  #endif
#endif 

    {
         NODE_CONFIG_ALLSUB,
         ATTR_PRIV_15,
         NULL,
         cmd_do_exec,
         "do SEQUENCE",
        "To run exec commands in current mode\n"
        "Exec Command\n"
    },

#ifdef CONFIG_SYS_PROTO_DAI
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_clear,
         "clear ip arp inspection interfaces IF_PORTS statistics",
        CLEAR_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        IFS_DESC
        ARG_IF_PORT
        "Arp inspection statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_show,
         "show ip arp inspection",
        SHOW_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_port_show,
         "show ip arp inspection interfaces IF_PORTS",
        SHOW_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_portStatistic_show,
         "show ip arp inspection interfaces IF_PORTS statistics",
        SHOW_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        IFS_DESC
        ARG_IF_PORT
        "Dynamic ARP Inspection Mib counter\n"
    },
#endif 

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_clear,
         "clear ip dhcp snooping interfaces IF_PORTS statistics",
        CLEAR_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        IFS_DESC
        ARG_IF_PORT
        "DHCP Snooping statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_show,
         "show ip dhcp snooping",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_port_show,
         "show ip dhcp snooping interfaces IF_PORTS",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_portStatistic_show,
         "show ip dhcp snooping interfaces IF_PORTS statistics",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        IFS_DESC
        ARG_IF_PORT
        "DHCP Snooping Mib statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_remoteId_show,
         "show ip dhcp snooping option remote-id",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
        "String of remote ID"
    },
#endif 
#ifdef CONFIG_SYS_DHCP_SERVER
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_show,
         "show dhcp-server",
        SHOW_DESC
        DHCP_DESC
        DHCP_SERVER_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_client_host_mac_table_all_show,
         "show dhcp-client",
        SHOW_DESC
        "configure the static ip of host\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_client_ip_bind_show,
         "show dhcp-client bind",
        SHOW_DESC
        "configure the static ip of host\n"
        "configure dhcp-client bind\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_ip_pool_show,
         "show ip pool [WORD<1-32>]",
        SHOW_DESC
        IP_DESC
		"IP pool information\n"
		"IP pool name\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_ip_pool_brief_show,
         "show ip pool-brief",
        SHOW_DESC
        IP_DESC
		"The brief infomation of ip pool\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_show_dhcps_hash,
         "show dhcp-server clients",
        SHOW_DESC
        "DHCP client\n"
		"The DHCP server clients information\n"
    },
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_show,
         "show dos",
        SHOW_STR
        DOS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_port_show,
         "show dos interfaces IF_PORTS",
        SHOW_STR
        DOS_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#endif 

#ifdef CONFIG_SYS_PROTO_GVRP
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_show,
         "show gvrp",
        SHOW_STR
        GVRP_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_statistic_show,
         "show gvrp statistics [interfaces IF_PORTS]",
        SHOW_STR
        GVRP_DESC
        "Gvrp Statistics \n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_errStatistic_show,
         "show gvrp error-statistics [interfaces IF_PORTS]",
        SHOW_STR
        GVRP_DESC
        "Gvrp Error Statistics \n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_config_show,
         "show gvrp configuration [interfaces IF_PORTS]",
        SHOW_STR
        GVRP_DESC
        "GVRP configuration\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_statistic_clear,
         "clear gvrp statistics [interfaces IF_PORTS]",
        CLEAR_STR
        GVRP_DESC
        "GVRP Statistics info\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_errStatistic_clear,
         "clear gvrp error-statistics [interfaces IF_PORTS]",
        CLEAR_STR
        GVRP_DESC
         "GVRP Error Statistics info\n"
        IFS_DESC
        ARG_IF_PORT
    },
#endif 

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_group_clear,
         "clear ip igmp snooping groups [(dynamic |static)]",
        CLEAR_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "IPv4 multicast groups\n"
        "dynamic groups\n"
        "static groups\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_statistics_clear,
         "clear ip igmp snooping statistics",
        CLEAR_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Clear IGMP snooping statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_show,
         "show ip igmp snooping",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_router_show,
        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
         "show ip igmp snooping router [(dynamic|forbidden|static)]",
        #else
         "show ip igmp snooping router",
        #endif
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "ipv4 multicast routers\n"
        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        "dynamic routers\n"
        "forbidded routers\n"
        "static routers\n"
        #endif
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_group_show,
         "show ip igmp snooping groups [(dynamic | static)]",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "ipv4 multicast groups\n"
        "dynamic groups\n"
        "static groups\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_vlan_show,
         "show ip igmp snooping vlan [VLAN-LIST]",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_forward_show,
         "show ip igmp snooping forward-all [vlan VLAN-LIST]",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "IPv4 forward all \n"
        VLAN_DESC
        ARG_VLAN_DESC
    },
    #endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_groupCnt_show,
         "show ip igmp snooping groups counters",
        SHOW_DESC
        IP_DESC
        IGMP_STR
        IGMP_SNOOPING_DESC
        "ipv4 multicast groups\n"
        "ipv4 group total entries\n"
    },
  #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_groupMode_show,
         "show ip igmp snooping groups mode",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "IGMP groups\n"
        "IGMP groups filter mode\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_querier_show,
         "show ip igmp snooping querier",
        SHOW_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Querier information\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
   {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_maxGroup_show,
         "show ip igmp max-group [interfaces IF_PORTS]",
        SHOW_STR
        IP_STR
        IGMP_STR
        "IGMP port group limit num\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_maxGroupAction_show,
         "show ip igmp max-group action [interfaces IF_PORTS]",
        SHOW_STR
        IP_STR
        IGMP_STR
        "IGMP Interface max groups\n"
        "IGMP excess max groups action\n"
        IFS_DESC
        ARG_IF_PORT
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_filter_show,
         "show ip igmp filter [interfaces IF_PORTS]",
        SHOW_DESC
        IP_STR
        IGMP_STR
        "IGMP port filter\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_profile_show,
         "show ip igmp profile [<"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">]",
        SHOW_DESC
        IP_STR
        IGMP_STR
        "IGMP profile configuration\n"
        "IGMP profile index\n"
    },
  #endif

  #ifdef CONFIG_SYS_PROTO_MVR
  {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_mvr_group_clear,
         "clear mvr members [(dynamic |static)]",
        CLEAR_DESC
        "MVR group\n"
        "MVR all groups\n"
        "MVR dynamic groups\n"
        "MVR static groups\n"
    },
   {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_show,
         "show mvr",
        SHOW_DESC
        "Show MVR global configuration information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mvr_member_show,
         "show mvr members",
        SHOW_DESC
        "Show MVR group\n"
        "MVR groups information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_interface_show,
         "show mvr interface [IF_PORTS]",
        SHOW_DESC
        "Show MVR interface configuration\n"
        "MVR interface configure information\n"
        ARG_IF_PORT
    },
  #endif 
#endif 

#ifdef CONFIG_SYS_PROTO_ISG
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_port_show,
         "show ip source interfaces IF_PORTS",
        SHOW_DESC
        IP_DESC
        ISG_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_bind_show,
         "show ip source binding [(dynamic|static)]",
        SHOW_DESC
        IP_DESC
        ISG_DESC
        ISG_BINDING_DESC
        "Dynamic binding entry\nStatic binding entry\n"
    },
  #ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_dhcpSnoopingbind_show,
         "show ip dhcp snooping binding",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "binding entry\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_dhcpSnoopingDb_show,
         "show ip dhcp snooping database",
        SHOW_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_dhcpSnoopingDb_renew,
         "renew ip dhcp snooping database",
        "Renew functions\n"
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_dhcpSnoopingDb_clear,
         "clear ip dhcp snooping database statistics",
        CLEAR_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Statistics\n"
    },
  #endif
#endif 

    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_macTable_clear,
         "clear mac address-table dynamic [interfaces IF_PORTS]",
        CLEAR_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        DYNAMIC_MAC_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_macTableVlan_clear,
         "clear mac address-table dynamic vlan <1-4094>",
        CLEAR_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        DYNAMIC_MAC_DESC
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAging_show,
         "show mac address-table aging-time",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "aging time of the address table\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAddress_show,
         "show mac address-table A:B:C:D:E:F [vlan <1-4094>]",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAddressTable_show,
         "show mac address-table [vlan <1-4094>] [interfaces IF_PORTS]",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAddressTable_show,
         "show mac address-table static [vlan <1-4094>] [interfaces IF_PORTS]",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "Static entries\n"
        VLAN_DESC
        ARG_VLAN_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAddressTable_show,
         "show mac address-table dynamic [vlan <1-4094>] [interfaces IF_PORTS]",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "Dynamic entries\n"
        VLAN_DESC
        ARG_VLAN_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l2_macAddressTableCnt_show,
         "show mac address-table counters",
        SHOW_DESC
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "total entries\n"
    },

#ifdef CONFIG_SYS_L3_ROUTE
    
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_intfIPv4_show,
         "show ip interface [IF_L3ALL]",
        SHOW_DESC
        IP_STR
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_routeV4_show,
         "show ip route [(connected|detail|static)]",
        SHOW_DESC
        IP_STR
        "Display the IP routing table\n"
        "Connected\nDetail\nStatic routes\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_intfIPv6_show,
         "show ipv6 interface [IF_L3ALL]",
        SHOW_DESC
        IPV6_STR
        IFS_DESC
        ARG_IF_PORT
    },
	{
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_DHCPv6_client_show,
         "show dhcpv6-client interface [vlan VLAN-LIST]",
        SHOW_DESC
        "dhcpv6 client information\n"
        IFS_DESC
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_routeV6_show,
         "show ipv6 route [(connected|detail|static)]",
        SHOW_DESC
        IPV6_STR
        "Display the IP routing table\n"
        "Connected\nDetail\nStatic routes\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_clear,
         "clear arp-cache",
        CLEAR_DESC
        "Clear dynamic entries in the ARP cache\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_clear,
         "clear ipv6 neighbors",
        CLEAR_DESC
        "Configure IPv6\n"
        "Clear all dynamic IPv6 entries in the ND cache\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_neigh_show,
         "show arp [ip-address A.B.C.D] [mac-address A:B:C:D:E:F] [vlan <1-4094>]",
        SHOW_DESC
        "Display the entries in the ARP table\n"
        "Filter ARP show by IP\n"
        "Filter ARP show by IP\n"
        "Filter ARP show by MAC\n"
        "Filter ARP show by MAC\n"
        "Filter ARP show by VLAN\n"
        "Filter ARP show by VLAN\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_l3_neigh_show,
         "show ipv6 neighbors [ipv6-address X:X::X:X] [mac-address A:B:C:D:E:F] [vlan <1-4094>]",
        SHOW_DESC
        "Internet protocol v6 information\n"
        "Show IPv6 neighbor cache entries\n"
        "Filter neighbor show by IP\n"
        "Filter neighbor show by IP\n"
        "Filter neighbor show by MAC\n"
        "Filter neighbor show by MAC\n"
        "Filter neighbor show by VLAN\n"
        "Filter neighbor show by VLAN\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_arpConf_show,
         "show arp configuration",
        SHOW_DESC
        IP_STR
        IFS_DESC
        ARG_IF_PORT
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_ZEBRA
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_router_id_show,
         "show router-id",
        SHOW_DESC
        "IP address to use for router-id"
    },
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_show,
         "show lldp",
        SHOW_DESC
        "LLDP information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_port_show,
         "show lldp interfaces IF_NMLPORTS",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
    },
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_show,
         "show lldp med",
        SHOW_DESC
        "LLDP information\n"
        "LLDP MED information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portMed_show,
         "show lldp interfaces IF_NMLPORTS med",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
        "LLDP MED information\n"
    },
  #endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_local_show,
         "show lldp local-device",
        SHOW_DESC
        "LLDP information\n"
        LLDP_L_DEVICE_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portLocal_show,
         "show lldp interfaces IF_NMLPORTS local-device",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
        LLDP_L_DEVICE_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_remote_show,
         "show lldp neighbor",
        SHOW_DESC
        "LLDP information\n"
        "display information about neighboring devices |discovered using Link Layer Discovery Protocol\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portRemote_show,
         "show lldp interfaces IF_NMLPORTS neighbor",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
        "display information about neighboring devices |discovered using Link Layer Discovery Protocol\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_globalStatistics_clear,
         "clear lldp global statistics",
        CLEAR_DESC
        "Reset lldp information\n"
        "Clear LLDP statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portStatistics_clear,
         "clear lldp interfaces IF_PORTS statistics",
        CLEAR_DESC
        ARG_IF_PORT
        "Reset lldp information\n"
        "Clear LLDP statistics for specified ports\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_statistics_show,
         "show lldp statistics",
        SHOW_DESC
        "LLDP information\n"
        LLDP_STATISTICS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portStatistics_show,
         "show lldp interfaces IF_NMLPORTS statistics",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
        LLDP_STATISTICS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_portTlvOverload_show,
         "show lldp interfaces IF_NMLPORTS tlvs-overloading",
        SHOW_DESC
        "LLDP information\n"
        "LLDP interface status and configuration\n"
        ARG_IF_NMLPORT
        "Display LLDP TLVs overloading\n"
    },
#endif 
#ifdef CONFIG_SYS_MIRROR
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mirror_show,
         "show mirror",
        SHOW_STR
        MIRROR_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_session_show,
         "show mirror session <$DEF1-$DEF2>",
        SHOW_STR
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC
    },
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_group_clear,
         "clear ipv6 mld snooping groups [(dynamic|static)]",
        CLEAR_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "IPv6 multicast groups\n"
        "dynamic groups\nstatic groups\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_statistic_clear,
         "clear ipv6 mld snooping statistics",
        CLEAR_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "Clear MLD snooping statistics\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_show,
         "show ipv6 mld snooping",
        SHOW_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_router_show,
        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
         "show ipv6 mld snooping router [(dynamic|forbidden|static)]",
        #else
         "show ipv6 mld snooping router",
        #endif
        SHOW_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "mld routers\n"
        #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
        "dynamic routers\nforbidded routers\nstatic routers\n"
        #endif
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_group_show,
         "show ipv6 mld snooping groups [(dynamic|static)]",
        SHOW_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "MLD groups\n"
        "dynamic groups\nstatic groups\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_vlan_show,
         "show ipv6 mld snooping vlan [VLAN-LIST]",
        SHOW_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_forward_show,
         "show ipv6 mld snooping forward-all [vlan VLAN-LIST]",
        SHOW_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "IPv6 forward all \n"
        VLAN_DESC
        ARG_VLAN_DESC
    },
    #endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_groupCnt_show,
         "show ipv6 mld snooping groups counters",
        SHOW_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "ipv6 multicast groups\n"
        "Ipv6 group total entries\n"
    },
  #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
   {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_maxGroup_show,
         "show ipv6 mld max-group [interfaces IF_PORTS]",
        SHOW_STR
        IPV6_STR
        MLD_STR
        "MLD port group limit num\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_maxGroupAction_show,
         "show ipv6 mld max-group action [interfaces IF_PORTS]",
        SHOW_STR
        IPV6_STR
        MLD_STR
        "MLD Interface max groups\n"
        "MLD excess max groups action\n"
        IFS_DESC
        ARG_IF_PORT
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_filter_show,
         "show ipv6 mld filter [interfaces IF_PORTS]",
        SHOW_DESC
        IPV6_STR
        MLD_STR
        "IPv6 interface filter\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_profile_show,
         "show ipv6 mld profile [<"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">]",
        SHOW_DESC
        IPV6_STR
        MLD_STR
        "MLD profile configuration\n"
        "MLD profile index\n"
    },
  #endif 
#endif 

    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_port_show,
         "show interfaces IF_PORTS",
        SHOW_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_port_status_show,
         "show interfaces IF_PORTS status",
        SHOW_DESC
        IFS_DESC
        ARG_IF_PORT
        "Port status information\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_port_status_brief,
         "show interfaces brief [connected]",
        SHOW_DESC
        IFS_DESC
        "Port information brief\n"
        "connected port"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_port_counter_clear,
         "clear interfaces IF_PORTS counters",
        CLEAR_DESC
        IFS_DESC
        ARG_IF_PORT
        "Port counter information\n"
    },
#ifdef CONFIG_SYS_PORT_ISOLATION
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_port_protected_show,
         "show interfaces IF_PORTS protected",
        SHOW_DESC
        IFS_DESC
        ARG_IF_PORT
        PORT_PROTECTED_DESC
    },
#endif
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_port_errdisable_show,
         "show errdisable recovery",
        SHOW_DESC
        PORT_ERRDISABLE_DESC
        PORT_ERRDISABLE_RECV_DESC
    },
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
     {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_portsec_show,
         "show port-security",
        SHOW_DESC
        PSECURE_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_port_show,
         "show port-security interfaces IF_NMLPORTS",
        SHOW_DESC
        PSECURE_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_addr_show,
         "show port-security address",
        SHOW_DESC
        PSECURE_DESC
        "All port security related MAC addresses\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_addr_clear,
         "clear port-security (all|configured|dynamic|sticky)",
        CLEAR_DESC
        PSECURE_DESC
        PSECURE_CLEAR_TYPE
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_addr_clear,
         "clear port-security (all|configured|dynamic|sticky) address A:B:C:D:E:F [vlan <1-4094>]",
        CLEAR_DESC
        PSECURE_DESC
        PSECURE_CLEAR_TYPE
        "Clear a secure address\n"
        ARG_MACADDR_DESC
        "VLAN\n"
        "VLAN ID\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_addr_clear,
         "clear port-security (all|configured|dynamic|sticky) interface IF_NMLPORT",
        CLEAR_DESC
        PSECURE_DESC
        PSECURE_CLEAR_TYPE
        "Clear all secure addresses on an interface\n"
        ARG_IF_NMLPORT
    },
#endif

    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_show,
         "show qos",
        SHOW_DESC
        QOS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_queue_show,
         "show qos queueing",
        SHOW_DESC
        QOS_DESC
        QOS_QUEUEING_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_port_show,
         "show qos interfaces IF_PORTS",
        SHOW_DESC
        QOS_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_map_show,
         "show qos map [(cos-queue|dscp-queue|precedence-queue|queue-cos|queue-dscp|queue-precedence)]",
        SHOW_DESC
        QOS_DESC
        QOS_MAP_DESC
        "CoS to Queue mapping\nDSCP to Queue mapping\nIP Precedence to Queue mapping\nQueue to CoS mapping\nQueue to DSCP mapping\nQueue to IP Precedence mapping\n"
    },

#ifdef CONFIG_SYS_LIB_RADIUS
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_radius_dfltConfig_show,
         "show radius default-config",
        SHOW_DESC
        RADIUS_SRV_DESC
        RADIUS_SRV_DFTPARAM_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_radius_server_show,
         "show radius",
        SHOW_DESC
        RADIUS_SRV_DESC
    },
#endif 

#ifdef CONFIG_SYS_BW_CTRL
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_limit_show,
         "show rate limit",
        SHOW_DESC
        RATE_LIMIT_INFORMATION
        RATE_LIMIT_INFORMATION
    },

#endif

#ifdef CONFIG_SYS_STORM_CTRL
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stromCtrl_show,
         "show storm-control",
        SHOW_STR
        STORM_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_portStromCtrl_show,
         "show storm-control interfaces IF_NMLPORTS",
        SHOW_STR
        STORM_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
#endif 

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_port_clear,
         "clear rmon interfaces IF_PORTS statistics",
        CLEAR_DESC
        RMON_STR
        IFS_DESC
        ARG_IF_PORT
        RMON_STATISTICS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_port_show,
         "show rmon interfaces IF_PORTS statistics",
        SHOW_DESC
        RMON_STR
        IFS_DESC
        ARG_IF_PORT
        RMON_STATISTICS_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_event_show,
         "show rmon event (<1-65535>|all)",
        SHOW_DESC
        RMON_STR
        RMON_EVENT_DESC
        "index of event\nall events\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_eventLog_show,
         "show rmon event <1-65535> log",
        SHOW_DESC
        RMON_STR
        RMON_EVENT_DESC
        "index of event\n"
        "log information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_alarm_show,
         "show rmon alarm (<1-65535>|all)",
        SHOW_DESC
        RMON_STR
        RMON_ALARM_DESC
        "index of event\n"
        "all alarm\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_history_show,
         "show rmon history (<1-65535>|all)",
        SHOW_DESC
        RMON_STR
        RMON_HISTORY_DESC
        "index of history\n"
        "all history\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_historyStatistic_show,
         "show rmon history <1-65535> statistic",
        SHOW_DESC
        RMON_STR
        RMON_HISTORY_DESC
        "index of history\n"
        "statistic table\n"
    },
#endif

#ifdef CONFIG_SYS_UI_SNMP
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_show,
         "show snmp",
        SHOW_STR
        SNMP_STR
    },
    #ifdef CONFIG_SYS_MIB_TRAP
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_trap_show,
         "show snmp trap",
        SHOW_DESC
        SNMP_STR
        "Display snmp class of trap enable or disable\n"
    },
    #endif
    #ifdef CONFIG_SYS_SNMPV3
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_community_show,
         "show snmp community",
        SHOW_STR
        SNMP_STR
        "Display SNMP community entries\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_host_show,
         "show snmp host",
        SHOW_STR
        SNMP_STR
        "Display all trap host entry\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_view_show,
         "show snmp view",
        SHOW_STR
        SNMP_STR
        "Display SNMP view entries\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_group_show,
         "show snmp group",
        SHOW_STR
        SNMP_STR
        "Display SNMP access group entries\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_user_show,
         "show snmp user",
        SHOW_STR
        SNMPV3_STR
        "Show snmp user\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_engineid_show,
         "show snmp engineid",
        SHOW_STR
        SNMP_STR
        "Display snmp engine id\n"
    },
    #endif
#endif 

#ifdef CONFIG_SYS_APP_SSH
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_ssh_set,
         "[no] ip ssh (v1|v2|all)",
        NO_DESC
        IP_STR
        SSH_STR
        "SSH v1 host keys\nSSH v2 host keys\nBoth SSH v1 and v2 host keys\n"
    },
#endif 

#ifdef CONFIG_SYS_PROTO_STP
    
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portStatistics_clear,
         "clear spanning-tree interfaces IF_PORTS statistics",
        CLEAR_STR
        "spanning-tree count info\n"
        IFS_DESC
        ARG_IF_PORT
        "statistics for specified ports\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_show,
         "show spanning-tree",
        SHOW_DESC
        "Displays spanning-tree information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_port_show,
         "show spanning-tree interfaces IF_PORTS",
        SHOW_DESC
        STP_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstConfig_show,
         "show spanning-tree mst configuration",
        SHOW_DESC
        STP_DESC
        "Multiple spanning trees\n"
        "MST current region configuration\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mst_show,
         "show spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)">",
        SHOW_DESC
        STP_DESC
        "Display spanning-tree parameters of instances\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portMst_show,
         "show spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> interfaces IF_PORTS",
        SHOW_DESC
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        IFS_DESC
        ARG_IF_PORT
    },
#endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portStatistics_show,
         "show spanning-tree interfaces IF_PORTS statistics",
        SHOW_STR
        STP_DESC
        IFS_DESC
        ARG_IF_PORT
        "statistics for specified ports\n"
    },
#endif 
#ifdef CONFIG_SYS_PROTO_ERPS
		{
			 NODE_PRIVMODE,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_show_erps,
			 "show erps instance (all | <0-"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)">)",
			SHOW_DESC
			"Erps\n"
			"Erps instance\n"
			"Erps all\n"
			"Erps instace range\n"
		},
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK
		{
			 NODE_PRIVMODE,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_show_loopback_port_config,
			 "show interfaces IF_PORTS loopback",
			SHOW_DESC
			IFS_DESC
       		ARG_IF_PORT
			LOOPBACK_DESC
		},
#endif

#if 0 
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_erps_statistics_show,
         "show erps statistics",
        SHOW_DESC
        ERPS_DESC
        "statistics for erps\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_erps_show,
         "show erps",
        SHOW_DESC
        "Displays erps information\n"
    },
#endif 
#ifdef CONFIG_SYS_SYSLOG
    
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_log_target_show,
         "show logging (buffered|file)",
        SHOW_STR
        LOG_DESC
        "Buffered logging\n"
        "File logging\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_log_target_show,
         "show logging",
        SHOW_STR
        LOG_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_log_clear,
         "clear logging (buffered|file)",
        CLEAR_STR
        LOG_DESC
        "Buffered logging\n"
        "File logging\n"
    },
#endif 

    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_version_show,
         "show version",
        SHOW_DESC
        "System hardware and software status\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_info_show,
         "show info",
        SHOW_DESC
        "Basic information\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemName_set,
         "hostname WORD<1-32>",
        "Set system's network name\n"
        "This system's network name\n"
    },
#ifdef CONFIG_SYS_ADVANCE_DNS
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_advDns_show,
         "show hosts",
        SHOW_DESC
        "To display the default domain name, a list of name |server hosts, the static of host names and addresses\n"
    },
#endif
#ifndef CONFIG_SYS_L3_ARCHITECTURE
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_ip_show,
         "show ip",
        SHOW_DESC
        "IP Address, Subnet Mask, Default Gateway\n"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_ipDhcp_show,
         "show ip dhcp",
        SHOW_DESC
        IP_STR
        "DHCP information\n"
    },
#endif
#if defined(CONFIG_SYS_IPV6) && !defined(CONFIG_SYS_L3_ARCHITECTURE)
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_ipv6_show,
         "show ipv6",
        SHOW_DESC
        IPV6_STR
    },
#endif
#ifdef CONFIG_SYS_PING_TEST
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_ping_exec,
         "ping (A.B.C.D|X:X::X:X|HOSTNAME) [count <1-999999999>]",
        "Send ICMP ECHO_REQUEST to network hosts\n"
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
        "The number of repetitions\n"
        "1-999999999\n"
    },
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
    {
         NODE_VIEW | NODE_PRIVMODE,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_traceroute_exec,
         "traceroute HOSTNAME [max_hop <2-255>]",
        "Trace route to network hosts\n"
        "The IP address or hostname address to trace\n"
        "The number of maximun hop. Default : 30\n"
        "2-255\n"
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         cmd_ext_nosort_install,
         cmd_time_set,
         "clock set HH:MM:SS (jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) <1-31> <2000-2035>",
        "Manage the system clock\n"
        "Manually set the system clock\n"
        "Current time in hours (24 Hour format), minutes, and seconds.\n"
        TIME_MONTHS
        "Current day in the month."
        "Current year"
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_time_show,
         "show clock [detail]",
        SHOW_STR
        "Display the time and date from the system clock\n"
        "Show timezone and summertime configuration\n"
    },
#ifdef CONFIG_SYS_APP_MSNTP
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_time_sntp_show,
         "show sntp",
        SHOW_STR
        "Simple Network Time Protocol (SNTP) information\n"
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_reboot_exec,
         "reboot",
        "Halt and perform a cold restart\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ssl_exec,
         "ssl",
        "Setup SSL host keys\n"
    },
#ifdef CONFIG_SYS_CPU_UTILIZATION
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_cpuUtilz_show,
         "show cpu utilization",
        SHOW_DESC
        CPUINFO_DESC
        CPUINFO_UTILIZATION_DESC
    },
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_cpuRate_show,
         "show cpu input rate",
        SHOW_DESC
        CPUINFO_DESC
        CPUINFO_INPUT_DESC
        CPUINFO_INPUT_RATE_DESC
    },
#endif
#ifdef CONFIG_SYS_MEM_UTILIZATION
    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_sys_memory_show,
         "show memory statistics",
        SHOW_DESC
        MEMINFO_DESC
        MEMINFO_STATISTICS_DESC
    },
#endif

    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_username_show,
         "show username",
        SHOW_DESC
        AAA_LOCAL_USER_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_0,
         NULL,
         cmd_sys_privilege_show,
         "show privilege",
        SHOW_DESC
        AAA_LOCAL_PRIV_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_username_access_method_show,
         "show username access-method",
        SHOW_DESC
        AAA_LOCAL_USER_DESC
        "user access method"
    },

    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_delete,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        "delete (backup-config|startup-config|flash://)",
#else
        "delete (startup-config|flash://)",
#endif
        "Delete a file from the flash file system\n"
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_STARTUPCFG_DESC
        "Delete a file from the flash file system\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dfltConfig_restore,
         "restore-defaults [interfaces IF_PORTS]",
        "Restore to default\n"
        IFS_DESC
        ARG_IF_PORT
    },
#ifdef CONFIG_SYS_DUAL_IMAGE
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_image_boot,
         "boot system (image0|image1)",
        BOOT_DESC
        IMAGE_DESC
        "Runtime image 0\n"
        "Runtime image 1\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_image_delete,
         "delete system (image0|image1)",
        DELETE_DESC
        IMAGE_DESC
        "Runtime image 0\n"
        "Runtime image 1\n"
    },
#endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_flash_show,
         "show flash",
        SHOW_DESC
        FLASH_DESC
    },
#ifdef CONFIG_SYS_DUAL_IMAGE
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_bootvar_show,
         "show bootvar",
        SHOW_DESC
        BOOTVAR_DESC
    },
#endif
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_show,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
         "show (backup-config|startup-config|running-config)",
#else
         "show (startup-config|running-config)",
#endif
        SHOW_DESC
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_STARTUPCFG_DESC
        FILE_MGMT_RUNNINGCFG_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_intf_config_print,
         "show running-config interfaces IF_ALLS",
        SHOW_DESC
        FILE_MGMT_RUNNINGCFG_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_show_features_module,
         "show running-config (system"
			        #ifdef CONFIG_SYS_L3_ROUTE_RIP
			        "|rip"
			        #endif
			        #ifdef CONFIG_SYS_L3_ROUTE_OSPF
			        "|ospf"
			        #endif
			        "|time"
			        "|vlan"
			        #ifdef CONFIG_SYS_DHCP_SERVER
					"|dhcp-server"
					#endif
			        ")",
        SHOW_DESC
        FILE_MGMT_RUNNINGCFG_DESC
        "system config info\n"
        #ifdef CONFIG_SYS_L3_ROUTE_RIP
        "rip config info\n"
        #endif
        #ifdef CONFIG_SYS_L3_ROUTE_OSPF
        "ospf config info\n"
        #endif
        "time config info\n"
        "static vlan config info\n"
        #ifdef CONFIG_SYS_DHCP_SERVER
        "dhcp server config info\n"
        #endif
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_save,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
         "copy running-config (startup-config|backup-config)",
#else
         "copy running-config (startup-config|)",
#endif
        COPY_DESC
        FILE_MGMT_RUNNINGCFG_DESC
        FILE_MGMT_STARTUPCFG_DESC
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_save,
         "save",
        "Save running configuration to flash"
    },
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_save,
         "copy startup-config backup-config",
        COPY_DESC
        FILE_MGMT_STARTUPCFG_DESC
        FILE_MGMT_BACKUPCFG_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config_save,
         "copy backup-config startup-config",
        COPY_DESC
        FILE_MGMT_BACKUPCFG_DESC
        FILE_MGMT_STARTUPCFG_DESC
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_file_copy,
        "copy (flash://|tftp://) (flash://|tftp://)",
        COPY_DESC
        "Copy from flash: file system\n"
        "Copy from tftp: file system\n"
        "Copy to flash: file system\n"
        "Copy to tftp: file system\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_banner_file_cfg,
         "banner login",
        "Switch banner info\n"
        "Banner content config\n"
    },
#ifdef CONFIG_SYS_HW_USB
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config2usb_copy,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
		/* cmd	*/ "copy (backup-config|running-config|startup-config) usb://",
#else
		/* cmd	*/ "copy (running-config|startup-config) usb://",
#endif
		COPY_DESC
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
		FILE_MGMT_BACKUPCFG_DESC
#endif
		FILE_MGMT_RUNNINGCFG_DESC
		FILE_MGMT_STARTUPCFG_DESC
		"Copy to USB device : file system\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_usb2config_copy,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        "copy usb:// (backup-config|running-config|startup-config)",
#else
        "copy usb:// (running-config|startup-config)",
#endif
        COPY_DESC
        "Copy from USB device : file system\n"
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_RUNNINGCFG_DESC
        FILE_MGMT_STARTUPCFG_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_usb_enable,
         "usb install",
        "USB device\n"
        "Install\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_usb_disable,
         "usb remove",
        "USB device\n"
        "Remove\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_usbDev_show,
         "show usb",
        SHOW_DESC
        "USB device info\n"
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config2Tftp_copy,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        "copy (backup-config|running-config|startup-config) tftp://",
#else
        "copy (running-config|startup-config) tftp://",
#endif
        COPY_DESC
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_RUNNINGCFG_DESC
        FILE_MGMT_STARTUPCFG_DESC
        "Copy to tftp: file system\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_tftp2Config_copy,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        "copy tftp:// (backup-config|running-config|startup-config)",
#else
        "copy tftp:// (running-config|startup-config)",
#endif
        COPY_DESC
        "Copy from tftp: file system\n"
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_RUNNINGCFG_DESC
        FILE_MGMT_STARTUPCFG_DESC
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_config2Running_copy,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
         "copy (backup-config|startup-config) running-config",
#else
         "copy (startup-config|) running-config",
#endif
        COPY_DESC
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
        FILE_MGMT_BACKUPCFG_DESC
#endif
        FILE_MGMT_STARTUPCFG_DESC
        "Update (merge with) current system configuration\n"
    },

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_line_show,
         "show line [(console|telnet|ssh)]",
        SHOW_DESC
        LINE_DESC
        ACCESS_CONSOLE_DESC ACCESS_TELNET_DESC ACCESS_SSH_DESC
    },
#endif 
#ifdef CONFIG_SYS_APP_TELNETD
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_telnet_clear,
         "clear line telnet",
        CLEAR_STR
        LINE_DESC
        TELNET_STR
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_linetelnet_show,
         "show ip telnet",
        SHOW_DESC
        IP_DESC
        TELNET_STR
    },
#endif
#ifdef CONFIG_USER_SSH_SSHD
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ssh_clear,
         "clear line ssh",
        CLEAR_STR
        LINE_DESC
        SSH_STR
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_linessh_show,
         "show ip ssh",
        SHOW_DESC
        IP_DESC
        SSH_STR
    },
#endif
    {
         NODE_CONFIG_SHOW,
         CMD_PRIV_15,
         NULL,
         cmd_sys_lineUsers_show,
         "show users",
        SHOW_DESC
        "Display information about users\n"
    },
    {
         NODE_VIEW | NODE_PRIVMODE,
         CMD_PRIV_1,
         cmd_ext_dynDef_install,
         cmd_sys_cliTermLength_set,
         "terminal length <0-$DEF>",
        "Terminal configuration\n"
        "Terminal length\n"
        "Length value. 0 means no limit\n"
    },
#ifdef CONFIG_SYS_UI_WEB
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_lineHttp_show,
  #ifdef CONFIG_USER_BOA_WITH_SSL
         "show ip (http|https)",
  #else
         "show ip http",
  #endif
        SHOW_DESC
        IP_DESC
        HTTP_DESC
  #ifdef CONFIG_USER_BOA_WITH_SSL
        HTTPS_DESC
  #endif
    },
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS
    
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_tacplus_dfltParam_show,
         "show tacacs default-config",
        SHOW_DESC
        AAA_TACPLUS_SRV_DESC
        AAA_TACPLUS_DFTPARAM_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_tacplus_server_show,
         "show tacacs",
        SHOW_DESC
        AAA_TACPLUS_SRV_DESC
    },
#endif

#ifdef CONFIG_SYS_TECH_SUPPORT
        
        {
             NODE_CONFIG_SHOW,
             ATTR_PRIV_15,
             NULL,
             cmd_tech_show,
             "show tech-support [(system|register"
            "|time"
            "|board|users|ip|dns"
#ifdef CONFIG_SYS_L3_ROUTE
            "|static-route"
#else
            "|dhcp"
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
            
"|jumbo"
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
            "|udld"
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
            "|igmp-snooping"
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            "|mld-snooping"
#endif
#ifdef CONFIG_SYS_AAA
            "|aaa"
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
            "|authmgr"
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
            "|lldp"
#endif
#ifdef CONFIG_SYS_PROTO_MVR
            "|mvr"
#endif
            "|interfaces|interfaces-status|mac-address-table|mac-address-counters|acl"
#ifdef CONFIG_SYS_USER_DEFINED_ACL
            "|user-defined-acl"
#endif
            "|vlan"
#ifdef CONFIG_SYS_PROTOCOL_VLAN
            "|protocol-vlan"
#endif
#ifdef CONFIG_SYS_VOICEVLAN
            "|voice-vlan"
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
            "|surveillance-vlan"
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
            "|port-isolation"
#endif
            "|lag"
#ifdef CONFIG_SYS_PORT_SECURITY
            "|port-security"
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
            "|dos"
#endif
#ifdef CONFIG_SYS_STORM_CTRL
            "|storm-control"
#endif
            "|qos"
            "|rate-limit"
#ifdef CONFIG_SYS_CABLE_DIAG
            "|cable-diag"
#endif
#ifdef CONFIG_SYS_MIRROR
            "|mirror"
#endif
#ifdef CONFIG_SYS_FIBER
            "|optical-module"
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
            "|rmon"
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
            "|dhcp-snooping"
#endif
#ifdef CONFIG_SYS_PROTO_DAI
            "|dai"
#endif
#ifdef CONFIG_SYS_PROTO_ISG
            "|isg"
#endif
#ifdef CONFIG_SYS_UI_SNMP
            "|snmp"
#endif
#ifdef CONFIG_SYS_APP_SSH
            "|ssh"
#endif
#ifdef CONFIG_SYS_PROTO_STP
            "|spanning-tree"
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
            "|erps"
#endif
#if 0 
            "|poe"
#endif
#ifdef CONFIG_SYS_SYSLOG
            "|logging"
#endif
            "|memory|filesystem|mtd"
            "|config-analysis|mib-counters|chip|cpu)]",
            SHOW_DESC
            TECH_INFO_DESC
            "System information\n"
            "Register & Table information\n"
            "Time\n"
            "Board configuration\n"
            "User status\n"
            "IP configuration\n"
            "DNS configuration\n"
#ifdef CONFIG_SYS_L3_ROUTE
            "Static routing\n"
#else
            "DHCP configuration\n"
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
            
			"Jumbo frame\n"
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
            "UDLD\n"
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
            "IGMP snooping\n"
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            "MLD snooping\n"
#endif
#ifdef CONFIG_SYS_AAA
            "AAA\n"
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
            "Authentication manager\n"
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
            "LLDP\n"
#endif
#ifdef CONFIG_SYS_PROTO_MVR
            "MVR\n"
#endif
            "Interfaces\n"
            "Interfaces status\n"
            "MAC address table\n"
            "MAC address counters\n"
            "ACL\n"
#ifdef CONFIG_SYS_USER_DEFINED_ACL
            "User-defined ACL\n"
#endif
            "VLAN\n"
#ifdef CONFIG_SYS_PROTOCOL_VLAN
            "Protocol VLAN\n"
#endif
#ifdef CONFIG_SYS_VOICEVLAN
            "Voice VLAN\n"
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
            "Suerveillance VLAN\n"
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
            "Port isolation\n"
#endif
            "LAG summary\n"
#ifdef CONFIG_SYS_PORT_SECURITY
            "Port security\n"
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
            "Dos protection\n"
#endif
#ifdef CONFIG_SYS_STORM_CTRL
            "Storm control\n"
#endif
            "QoS\n"
            "Rate limit\n"
#ifdef CONFIG_SYS_CABLE_DIAG
            "Cable diagnostic\n"
#endif
#ifdef CONFIG_SYS_MIRROR
            "Mirror\n"
#endif
#ifdef CONFIG_SYS_FIBER
            "Optical module\n"
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
            "RMON\n"
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
            "DHCP Snooping\n"
#endif
#ifdef CONFIG_SYS_PROTO_DAI
            "DAI\n"
#endif
#ifdef CONFIG_SYS_PROTO_ISG
            "IP Source Guard\n"
#endif
#ifdef CONFIG_SYS_UI_SNMP
            "SNMP\n"
#endif
#ifdef CONFIG_SYS_APP_SSH
            "SSH\n"
#endif
#ifdef CONFIG_SYS_PROTO_STP
            "spanning-tree\n"
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
            "erps\n"
#endif
#ifdef CONFIG_SYS_SYSLOG
            "logging\n"
#endif
            "Process memory\n"
            "File system\n"
            "MTD information\n"
            "Config analysis\n"
            "MIB counters\n"
            "Chip information\n"
            "CPU process usage\n"
        },
        {
             NODE_PRIVMODE,
             ATTR_PRIV_15,
             NULL,
             cmd_tech_Support2Tftp_copy,
             "copy tech-support tftp://",
            COPY_DESC
            TECH_FILE_DESC
            "Copy to tftp: file system\n"
        },
#endif

    {
         NODE_VIEW | NODE_CONFIG_SHOW,
         ATTR_PRIV_1,
         NULL,
         cmd_trunk_show,
         "show lag",
        SHOW_STR
        LAG_DESC
    },
#ifdef CONFIG_SYS_PROTO_LACP
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_internal_show,
         "show lacp [<1-$DEF>] internal",
        SHOW_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Internal information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_internal_detail_show,
         "show lacp [<1-$DEF>] internal detail",
        SHOW_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Internal information\n"
        "Detailed internal information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_neighbor_show,
         "show lacp [<1-$DEF>] neighbor",
        SHOW_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Neighbor information\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_neighbor_detail_show,
         "show lacp [<1-$DEF>] neighbor detail",
        SHOW_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Neighbor information\n"
        "Detailed neighbor information"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_sysid_show,
         "show lacp sys-id",
        SHOW_STR
        LACP_DESC
        "LACP System ID\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_counters_show,
         "show lacp [<1-$DEF>] counters",
        SHOW_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Traffic information\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_counters_clear,
         "clear lacp [<1-$DEF>] counters",
        CLEAR_STR
        LACP_DESC
        ARG_PORT_CHANNEL_NUM
        "Traffic information\n"
    },
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
   
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_show,
         "show udld",
        SHOW_DESC
        UDLD_INFO_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_port_show,
         "show udld interfaces IF_NMLPORTS",
        SHOW_DESC
        UDLD_INFO_DESC
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
     NODE_PRIVMODE,
     ATTR_PRIV_15,
     NULL,
     cmd_udld_reset,
     "udld reset",
    UDLD_GLOBAL_DESC
    UDLD_RESET_DESC
},
#endif

    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_port_show,
         "show interfaces switchport IF_PORTS",
        SHOW_DESC
        IFS_DESC
        SWITCHPORT_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMembership_show,
         "show vlan VLAN-LIST interfaces IF_PORTS membership",
        SHOW_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        IFS_DESC
        ARG_IF_PORT
        "Membership\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_show,
         "show vlan [(VLAN-LIST|dynamic|static)]",
        SHOW_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Display dynamic entries\n"
        "Display static entries\n"
    },
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_proto_show,
         "show vlan protocol-vlan [group <1-8>]",
        SHOW_DESC
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        "Protocol group\n"
        "Group index\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portProto_show,
         "show vlan protocol-vlan interfaces IF_PORTS",
        SHOW_DESC
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_vlan_ip_subnet_show,
		 "show vlan ip-subnet-vlan [group <1-"SYS_STR_CONST(CAPA_VLAN_IP_SUBNET_GROUP_NUM)">]",
		SHOW_DESC
		VLAN_DESC
		VLAN_IP_SUBNET_BASED_DESC
		"Protocol group\n"
		"Group index\n"
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_vlan_port_ip_subnet_show,
		 "show vlan ip-subnet-vlan interfaces IF_PORTS",
		SHOW_DESC
		VLAN_DESC
		VLAN_IP_SUBNET_BASED_DESC
		IFS_DESC
		ARG_IF_PORT
	},
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voice_show,
         "show voice-vlan",
        SHOW_DESC
        VLAN_VOICEVLAN_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceType_show,
         "show voice-vlan interfaces IF_PORTS",
        SHOW_DESC
        VLAN_VOICEVLAN_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#endif
#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_MANAGEMENT_VLAN)
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mgmt_show,
         "show management-vlan",
        SHOW_DESC
        VLAN_MGMTVLAN_DESC
    },
#endif
#ifdef CONFIG_SYS_MACVLAN
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_group_show,
         "show vlan mac-vlan groups",
        SHOW_DESC
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        "MAC group\n"
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_port_show,
         "show vlan mac-vlan [interfaces IF_PORTS]",
        SHOW_DESC
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_show,
         "show surveillance-vlan",
        SHOW_DESC
        VLAN_SURVEILLANCE_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_port_show,
         "show surveillance-vlan interfaces IF_PORTS",
        SHOW_DESC
        VLAN_SURVEILLANCE_DESC
        IFS_DESC
        ARG_IF_PORT
    },
#endif

#ifdef CONFIG_SYS_PLAT_DBGMOD
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_debug_show,
         "show debugging",
        SHOW_DESC
        "debugging information"
    },
#endif
#ifdef CONFIG_SYS_POE
	   
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 cmd_ext_deprecated_install,
		 cmd_poe_show,
		 "show poe",
		SHOW_DESC
		POE_DESC
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 cmd_ext_deprecated_install,
		 cmd_poe_port_show,
		 "show poe interfaces IF_NMLPORTS",
		SHOW_DESC
		POE_DESC
		IFS_DESC
		ARG_IF_NMLPORT
	},
#endif
#ifdef CONFIG_SYS_EEE
    {
	     NODE_CONFIG_SHOW,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_eee_port_config_show,
	     "show eee",
	    NO_STR
	    EEE_DESC
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
    {
	     NODE_CONFIG_SHOW,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_show_rip_status,
	     "show rip",
	    SHOW_DESC
	    "Rip\n"
	    "Rip information\n"
    },
    {
	     NODE_CONFIG_SHOW,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_show_rip_route_information,
	     "show ip route rip",
	    SHOW_DESC
        IP_STR
        "Display the IP routing table\n"
	    "Rip route information\n"
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
    {
	     NODE_CONFIG_SHOW,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_show_ospf,
	     "show ospf",
	    SHOW_DESC
	    "ospf\n"
	    "ospf information\n"
	    "Process ID\n"
    },
    {
	     NODE_CONFIG_SHOW,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_show_ospf_route_table,
	     "show ip route ospf",
	    SHOW_DESC
	    IP_STR
	    "routing table\n"
	    "OSPF routing table\n"
    },
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ospf_neighbor_show,
		 "show ospf neighbor",
		SHOW_DESC
		"OSPF information\n"
		"OSPF neighbor information\n"
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ospf_virtual_link_show,
		 "show ospf virtual-link",
		SHOW_DESC
		"OSPF information\n"
		"OSPF virtual link information\n"
	},

	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ospf_interface_show,
		 "show ospf interface [vlan-interface <1-4094>]",
		SHOW_DESC
		"OSPF information\n"
		"OSPF virtual link information\n"
		"Vlan interface\n"
		"Vlan interface number\n"
	},

	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ospf_database_show,
		 "show ospf database [(asbr-summary|external|network|router|summary|nssa-external)]",
		SHOW_DESC
		"OSPF information\n"
		"Database summary\n"
		"ASBR summary link states\n"
		"External link states\n"
		"Network link states\n"
		"Router link states\n"
		"Network summary link states\n"
		"NSSA external link state\n"
	},
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_device_show,
		 "show cwmp device",
		SHOW_DESC
		"CWMP protocol\n"
		"CPE device info\n"
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_acs_show,
		 "show cwmp acs",
		SHOW_DESC
		"CWMP protocol\n"
		"acs device info\n"
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_show,
		 "show cwmp local",
		SHOW_DESC
		"CWMP protocol\n"
		"local config info\n"
	},
#endif
#ifdef CONFIG_SYS_L3_VRRP
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_vrrp_show,
		 "show vrrp [ vlan-interface <1-4094>]",
		SHOW_DESC
		"VRRP information\n"
		"Vlan interface\n"
		"Vlan interface number\n"
	},
	{
		 NODE_CONFIG_SHOW,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_vrrp_show,
		 "show vrrp vlan-interface <1-4094> vrid <1-5>",
		SHOW_DESC
		"VRRP information\n"
		"Vlan interface\n"
		"Vlan interface number\n"
		"virtual router id\n"
		"vrid (1-5)\n"
	},
#endif

    {0, 0, NULL, NULL, NULL, NULL}
};

const cmd_inst_tbl_t cmdSetTbl[] =
{
#ifdef CONFIG_SYS_AAA
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_authenLoginList_set,
         "aaa authentication login (default|LISTNAME) (enable|local|none|radius|tacacs+) [(enable|local|none|radius|tacacs+)] [(enable|local|none|radius|tacacs+)] [(enable|local|none|radius|tacacs+)]",
        AAA_DESC
        AAA_AUTHEN_DESC
        AAA_LOGIN_DESC
        ARG_AUTHLIST_DFLT ARG_AUTHLIST_NAME
        ARG_AUTH_LOGIN_METHOD
        ARG_AUTH_LOGIN_METHOD
        ARG_AUTH_LOGIN_METHOD
        ARG_AUTH_LOGIN_METHOD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_authenLoginList_set,
         "no aaa authentication login LISTNAME",
        NO_DESC
        AAA_DESC
        AAA_AUTHEN_DESC
        AAA_LOGIN_DESC
        ARG_AUTHLIST_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_authenEnableList_set,
         "aaa authentication enable (default|LISTNAME) (enable|none|radius|tacacs+) [(enable|none|radius|tacacs+)] [(enable|none|radius|tacacs+)]",
        AAA_DESC
        AAA_AUTHEN_DESC
        AAA_ENABLE_DESC
        ARG_AUTHLIST_DFLT ARG_AUTHLIST_NAME
        ARG_AUTH_ENABLE_METHOD
        ARG_AUTH_ENABLE_METHOD
        ARG_AUTH_ENABLE_METHOD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_authenEnableList_set,
         "no aaa authentication enable LISTNAME",
        NO_DESC
        AAA_DESC
        AAA_AUTHEN_DESC
        AAA_ENABLE_DESC
        ARG_AUTHLIST_NAME
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineLoginAuthenList_set,
         "login authentication LISTNAME",
        AAA_LOGIN_DESC
        AAA_AUTHEN_DESC
        ARG_AUTHLIST_NAME
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineLoginAuthenList_set,
         "no login authentication",
        NO_DESC
        AAA_LOGIN_DESC
        AAA_AUTHEN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineLoginAuthenList_set,
         "ip (http|https) login authentication LISTNAME",
        IP_DESC
        HTTP_DESC HTTPS_DESC
        AAA_LOGIN_DESC
        AAA_AUTHEN_DESC
        ARG_AUTHLIST_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineLoginAuthenList_set,
         "no ip (http|https) login authentication",
        NO_DESC
        IP_DESC
        HTTP_DESC HTTPS_DESC
        AAA_LOGIN_DESC
        AAA_AUTHEN_DESC
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineEnableAuthenList_set,
         "enable authentication LISTNAME",
        AAA_ENABLE_DESC
        AAA_AUTHEN_DESC
        ARG_AUTHLIST_NAME
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_aaa_lineEnableAuthenList_set,
         "no enable authentication",
        NO_DESC
        AAA_ENABLE_DESC
        AAA_AUTHEN_DESC
    },
#endif 

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_enter_set,
         "[no] mac acl NAME",
        NO_DESC
        "MAC configuration\n"
        "This command enters the extended MAC ACL configuration in order to create layer 2 extended ACL.\n"
        "Specify the ACL name.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_enter_set,
         "[no] ip acl NAME",
        NO_DESC
        "Global IP configuration commands\n"
        "This command creates an ACL, which perform classification on layer 3 fields and enters ip-access configuration mode.\n"
        "Specify the ACL name.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_enter_set,
         "[no] ipv6 acl NAME",
        NO_DESC
        "IPV6 configuration\n"
        "This command creates an ACL, which perform classification on layer 3 fields and enters to ipv6-access configuration mode.\n"
        "Specify the ACL name.\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_bind_set,
         "(mac|ip|ipv6) acl NAME",
        "MAC configuration\n" "Global IP configuration commands\n" "IPV6 configuration\n"
        "Specify access control for packets\n"
        "ACL name\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_unbind_set,
         "no (mac|ip|ipv6) acl",
        NO_DESC
        "MAC configuration\n" "Global IP configuration commands\n" "IPV6 configuration\n"
        "Specify access control for packets\n"
    },
    {
         NODE_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_ace_del,
         "no sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">",
        NO_DESC
        "Sequence of access control entry\n"
        "Specify sequence of access control entry to delete\n"
    },
    {
         NODE_ACL_MAC,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceMac_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (A:B:C:D:E:F/A:B:C:D:E:F|any) (A:B:C:D:E:F/A:B:C:D:E:F|any) [vlan <1-4094>] [cos <0-7> <0-7>] [ethtype <0x0600-0xFFFF>]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_MAC_SRC
        ARG_ACL_MAC_DST
        ACL_MAC_VLAN_DESC
        ACL_MAC_VLAN_DESC
        ACL_MAC_COS_DESC
        ACL_MAC_COS_DESC
        ARG_ACL_MAC_COSMASK
        ACL_MAC_ETHER_DESC
        ACL_MAC_ETHER_DESC
    },
    {
         NODE_ACL_MAC,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceMac_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (A:B:C:D:E:F/A:B:C:D:E:F|any) (A:B:C:D:E:F/A:B:C:D:E:F|any) [vlan <1-4094>] [cos <0-7> <0-7>] [ethtype <0x0600-0xFFFF>] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENY_DESC
        ARG_ACL_MAC_SRC
        ARG_ACL_MAC_DST
        ACL_MAC_VLAN_DESC
        ACL_MAC_VLAN_DESC
        ACL_MAC_COS_DESC
        ACL_MAC_COS_DESC
        ARG_ACL_MAC_COSMASK
        ACL_MAC_ETHER_DESC
        ACL_MAC_ETHER_DESC
        "The Ethernet interface would be disabled if the condition is matched\n"
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (<0-255>|ipinip|egp|igp|hmp|rdp|ipv6|ipv6:rout|ipv6:frag|rsvp|ipv6:icmp|ospf|pim|l2tp|ip) (A.B.C.D/A.B.C.D|any) (A.B.C.D/A.B.C.D|any) [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_PARAM
        ARG_ACL_IP_SRC
        ARG_ACL_IP_DST
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (icmp|) (A.B.C.D/A.B.C.D|any) (A.B.C.D/A.B.C.D|any) (<0-255>|echo-reply|destination-unreachable|source-quench|echo-request|router-advertisement|router-solicitation|time-exceeded|timestamp|timestamp-reply|traceroute|any) (<0-255>|any) [(dscp|precedence) VALUE] [time-range NAME]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_ICMP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_DST
        ARG_ACL_IP_ICMP_PARAM
        ARG_ACL_IP_ICMP_CODE
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (tcp|) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) [match-all TCP_FLAG] [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_TCP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_TCP_PARAM
        ARG_ACL_IP_DST
        ARG_ACL_IP_TCP_PARAM
        "Set TCP flags\n"
        "List of TCP flags that should occur. If a flag should be set it is prefixed by \"+\".If a flag should be unset it is prefixed by \"-\". Available options are +urg, +ack, +psh, +rst, +syn, +fin, -urg, -ack, -psh, -rst, -syn and -fin.To define more than 1 flag - enter additional flags one after another without a space (example +syn-ack).\n"
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (udp|) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|talk|rip|PORT_RANGE|any) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|PORT_RANGE|any) [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_UDP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DST
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (<0-255>|ipinip|egp|igp|hmp|rdp|ipv6|ipv6:rout|ipv6:frag|rsvp|ipv6:icmp|ospf|pim|l2tp|ip) (A.B.C.D/A.B.C.D|any) (A.B.C.D/A.B.C.D|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIP_DESC
        ARG_ACL_IP_PARAM
        ARG_ACL_IP_SRC
        ARG_ACL_IP_DST
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (icmp|) (A.B.C.D/A.B.C.D|any) (A.B.C.D/A.B.C.D|any) (<0-255>|echo-reply|destination-unreachable|source-quench|echo-request|router-advertisement|router-solicitation|time-exceeded|timestamp|timestamp-reply|traceroute|any) (<0-255>|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIP_DESC
        ARG_ACL_IP_ICMP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_DST
        ARG_ACL_IP_ICMP_PARAM
        ARG_ACL_IP_ICMP_CODE
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (tcp|) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) [match-all TCP_FLAG] [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIP_DESC
        ARG_ACL_IP_TCP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_TCP_PARAM
        ARG_ACL_IP_DST
        ARG_ACL_IP_TCP_PARAM
        "Set TCP flags\n"
        "List of TCP flags that should occur. If a flag should be set it is prefixed by \"+\".If a flag should be unset it is prefixed by \"-\". Available options are +urg, +ack, +psh, +rst, +syn, +fin, -urg, -ack, -psh, -rst, -syn and -fin.To define more than 1 flag - enter additional flags one after another without a space (example +syn-ack).\n"
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV4,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (udp|) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|talk|rip|PORT_RANGE|any) (A.B.C.D/A.B.C.D|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|PORT_RANGE|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIP_DESC
        ARG_ACL_IP_UDP
        ARG_ACL_IP_SRC
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DST
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (<0-255>|ipv6) (X:X::X:X/<0-128>|any) (X:X::X:X/<0-128>|any) [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IPV6
        ARG_ACL_IPV6_SRC
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (icmp|) (X:X::X:X/<0-128>|any) (X:X::X:X/<0-128>|any) (<0-255>|destination-unreachable|packet-too-big|time-exceeded|parameter-problem|echo-request|echo-reply|router-solicitation|router-advertisement|nd-ns|nd-na|any) (<0-255>|any) [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_ICMP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IPV6_DST
        ARG_ACL_IPV6_ICMP_PARAM
        ARG_ACL_IP_ICMP_CODE
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (tcp|) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) [match-all TCP_FLAG] [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_TCP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IP_TCP_PARAM
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_TCP_PARAM
        "Set TCP flags\n"
        "List of TCP flags that should occur. If a flag should be set it is prefixed by \"+\".If a flag should be unset it is prefixed by \"-\". Available options are +urg, +ack, +psh, +rst, +syn, +fin, -urg, -ack, -psh, -rst, -syn and -fin.To define more than 1 flag - enter additional flags one after another without a space (example +syn-ack).\n"
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] permit (udp|) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|talk|rip|PORT_RANGE|any) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|PORT_RANGE|any) [(dscp|precedence) VALUE]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_PERMIT_DESC
        ARG_ACL_IP_UDP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (<0-255>|ipv6) (X:X::X:X/<0-128>|any) (X:X::X:X/<0-128>|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIPV6_DESC
        ARG_ACL_IPV6
        ARG_ACL_IPV6_SRC
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (icmp|) (X:X::X:X/<0-128>|any) (X:X::X:X/<0-128>|any) (<0-255>|destination-unreachable|packet-too-big|time-exceeded|parameter-problem|echo-request|echo-reply|router-solicitation|router-advertisement|nd-ns|nd-na|any) (<0-255>|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIPV6_DESC
        ARG_ACL_IP_ICMP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IPV6_DST
        ARG_ACL_IPV6_ICMP_PARAM
        ARG_ACL_IP_ICMP_CODE
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (tcp|) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|daytime|ftp-data|ftp|telnet|smtp|time|hostname|whois|tacacs-ds|domain|www|pop2|pop3|syslog|talk|klogin|kshell|sunrpc|drip|PORT_RANGE|any) [match-all TCP_FLAG] [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIPV6_DESC
        ARG_ACL_IP_TCP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IP_TCP_PARAM
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_TCP_PARAM
        "Set TCP flags\n"
        "List of TCP flags that should occur. If a flag should be set it is prefixed by \"+\".If a flag should be unset it is prefixed by \"-\". Available options are +urg, +ack, +psh, +rst, +syn, +fin, -urg, -ack, -psh, -rst, -syn and -fin.To define more than 1 flag - enter additional flags one after another without a space (example +syn-ack).\n"
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
    {
         NODE_ACL_IPV6,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_aceIP_set,
         "[sequence <"SYS_STR_CONST(BOND_ACL_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_ACL_USER_PRI_MAX)">] deny (udp|) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|talk|rip|PORT_RANGE|any) (X:X::X:X/<0-128>|any) (<0-65535>|echo|discard|time|nameserver|tacacs-ds|domain|bootps|bootpc|tftp|sunrpc|ntp|netbios-ns|snmp|snmptrap|who|syslog|PORT_RANGE|any) [(dscp|precedence) VALUE] [shutdown]",
        ACL_SEQ_DESC
        ACL_SEQ_DESC
        ACL_DENYIPV6_DESC
        ARG_ACL_IP_UDP
        ARG_ACL_IPV6_SRC
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IPV6_DST
        ARG_ACL_IP_UDP_PARAM
        ARG_ACL_IP_DSCP_FILTER
        ARG_ACL_IP_DSCP_FILTER_VAL
        ACL_SHUTDOWN
    },
#endif 

#ifdef CONFIG_SYS_MGMT_ACL
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_list_set,
         "[no] management access-list NAME",
        NO_DESC
        MGMT_ACL_IPMGMT_CFG_DESC
        MGMT_ACL_LIST_CFG_DESC
        MGMT_ACL_LIST_NAME_DESC
    },
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipSourceNocare_permit,
         "[sequence <1-65535>] permit interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_PERMIT_CFG_DESC
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipCare_permit,
         "[sequence <1-65535>] permit ip A.B.C.D/A.B.C.D interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_PERMIT_CFG_DESC
        "Global IP configuration commands\n"
        "Specify IP source address and mask. For example, to set 176.212.xx.xx use addr 176.212.0.0 with mask 255.255.0.0\n"
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
  #ifdef CONFIG_SYS_IPV6
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipv6Care_permit,
         "[sequence <1-65535>] permit ipv6  X:X::X:X/<0-128> interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_PERMIT_CFG_DESC
        "IPV6 configuration\n"
        "Specify IPv6 source address and prefix length.\n"
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
  #endif
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipSourceNocare_deny,
         "[sequence <1-65535>] deny interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_DENY_CFG_DESC
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipCare_deny,
         "[sequence <1-65535>] deny ip A.B.C.D/A.B.C.D interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_DENY_CFG_DESC
        "Global IP configuration commands\n"
        "Specify IP source address and mask. For example, to set 176.212.xx.xx use addr 176.212.0.0 with mask 255.255.0.0\n"
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
  #ifdef CONFIG_SYS_IPV6
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_ipv6Care_deny,
         "[sequence <1-65535>] deny ipv6 X:X::X:X/<0-128> interfaces IF_PORTS service (all|http|https|snmp|ssh|telnet)",
        MGMT_ACL_ACE_SEQ_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
        MGMT_ACL_ACE_DENY_CFG_DESC
        "IPV6 configuration\n"
        "Specify IPv6 source address and prefix length.\n"
        IFS_DESC
        ARG_IF_PORT
        MGMT_ACL_ACE_SERVICE_DESC
        MGMT_ACL_ACE_SERVICE_DETAIL_DESC
    },
  #endif
    {
         NODE_MGMT_ACL,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_ace_del,
         "no sequence <1-65535>",
        NO_DESC
        MGMT_ACL_ACE_PRIDEL_DESC
        MGMT_ACL_ACE_SEQ_VALUE_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_class_set,
         "management access-class NAME",
        MGMT_ACL_IPMGMT_CFG_DESC
        MGMT_ACL_CLASS_CFG_DESC
        MGMT_ACL_CLASS_DETAIL_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_mgmt_class_del,
         "no management access-class",
        NO_DESC
        MGMT_ACL_IPMGMT_CFG_DESC
        MGMT_ACL_CLASS_CFG_DESC
    },
#endif 

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_typeEnbl_set,
         "[no] authentication (dot1x|mac|web)",
        NO_DESC
        AUTH_GLB_DESC
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_guestVlan_set,
         "authentication guest-vlan <1-4094>",
        AUTH_GLB_DESC
        GUEST_VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_guestVlan_set,
         "no authentication guest-vlan",
        NO_STR
        AUTH_GLB_DESC
        GUEST_VLAN_DESC
    },
  #ifdef CONFIG_SYS_PROTO_MACAUTH
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_radiusUserFmt_set,
         "authentication mac radius mac-delimiter (colon|dot|hyphen|none) [gap (2|4|6)]",
        AUTH_GLB_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_MAC_RADIUS
        AUTH_MAC_DELIMITER
        ARG_AUTH_MAC_DELIMITER
        AUTH_MAC_DELIMITER_GAP
        ARG_AUTH_GAP
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_radiusUserFmt_set,
         "authentication mac radius mac-case (lower|upper)",
        AUTH_GLB_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_MAC_RADIUS
        AUTH_MAC_CASE
        ARG_AUTH_CASE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_macauthLocalInfo_set,
         "authentication mac local A:B:C:D:E:F control auth [vlan <1-4094>] [reauth-period <"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MAX)">] [inactive-timeout <"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MAX)">]",
        AUTH_GLB_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_MAC_LOCAL
        "MAC address of entry to be added\n"
        "Set local entry control mode\n"
        "Host will be set to Authorized\n"
        "Local entry assigned vlan\n"
        ARG_VLAN_DESC
        AUTH_TIME_REAUTH_DESC
        AUTH_TIME_REAUTH_DESC
        AUTH_TIME_INACT_DESC
        AUTH_TIME_INACT_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_macauthLocalInfo_set,
         "authentication mac local A:B:C:D:E:F control unauth",
        AUTH_GLB_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_MAC_LOCAL
        "MAC address of entry to be added\n"
        "Set local entry control mode\n"
        "Host will be set to UnAuthorized\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_macauthLocalInfo_set,
         "no authentication mac local A:B:C:D:E:F",
        NO_DESC
        AUTH_GLB_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_MAC_LOCAL
        "MAC address of entry to be deleted\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_webauthLocalInfo_set,
         "authentication web local username WORD<1-32> password [encrypted] PASSWORD [vlan <1-4094>] [reauth-period <"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MAX)">] [inactive-timeout <"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MAX)">]",
        AUTH_GLB_DESC
        AUTH_TYPE_WEB_DESC
        AUTH_WEB_LOCAL
        "User name to be added\n"
        "Specify user name to be added\n"
        "Password of the user\n"
        "Encrypted password\n"
        "Specify password of the user\n"
        "Local entry assigned vlan\n"
        ARG_VLAN_DESC
        AUTH_TIME_REAUTH_DESC
        AUTH_TIME_REAUTH_DESC
        AUTH_TIME_INACT_DESC
        AUTH_TIME_INACT_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_webauthLocalInfo_set,
         "no authentication web local username WORD<1-32>",
        NO_DESC
        AUTH_GLB_DESC
        AUTH_TYPE_WEB_DESC
        AUTH_WEB_LOCAL
        "Local user name\n"
        "Specify user name to be deleted\n"
    },
  #endif
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portTypeEnbl_set,
         "[no] authentication (dot1x|mac|web)",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portGuestVlanEnbl_set,
         "[no] authentication guest-vlan",
        NO_STR
        AUTH_INTF_DESC
        GUEST_VLAN_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portCtrl_set,
         "authentication port-control (auto|force-auth|force-unauth)",
        AUTH_INTF_DESC
        AUTH_PORTCTRL_DESC
        ARG_AUTH_PORTCTRL
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portCtrl_set,
         "no authentication port-control",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_PORTCTRL_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMode_set,
         "authentication host-mode (multi-auth|multi-host|single-host)",
        AUTH_INTF_DESC
        AUTH_HOSTMODE_DESC
        "Multiple Authentication Mode\n"
        "Multiple Host Mode\n"
        "Single Host Mode\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMode_set,
         "no authentication host-mode",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_HOSTMODE_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portOrder_set,
         "authentication order dot1x [mac] [web]",
        AUTH_INTF_DESC
        AUTH_ORDER_DESC
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portOrder_set,
         "authentication order mac [dot1x] [web]",
        AUTH_INTF_DESC
        AUTH_ORDER_DESC
        AUTH_TYPE_MAC_DESC
        AUTH_TYPE_DOT1X_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portOrder_set,
         "authentication order web",
        AUTH_INTF_DESC
        AUTH_ORDER_DESC
        AUTH_TYPE_WEB_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portOrder_set,
         "no authentication order",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_ORDER_DESC
    },
  #if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMethodOrder_set,
         "authentication method radius [local]",
        AUTH_INTF_DESC
        AUTH_METHOD_DESC
        ARG_AUTH_METHOD_RADIUS
        ARG_AUTH_METHOD_LOCAL
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMethodOrder_set,
         "authentication method local [radius]",
        AUTH_INTF_DESC
        AUTH_METHOD_DESC
        ARG_AUTH_METHOD_LOCAL
        ARG_AUTH_METHOD_RADIUS
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMethodOrder_set,
         "no authentication method",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_METHOD_DESC
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMaxLoginAttempt_set,
         "authentication web max-login-attempts (infinite|<"SYS_STR_CONST(BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MAX)">)",
        AUTH_INTF_DESC
        AUTH_TYPE_WEB_DESC
        AUTH_MAXLOGIN_DESC
        "No limit to login attempt number\n"
        AUTH_MAXLOGIN_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMaxLoginAttempt_set,
         "no authentication web max-login-attempts",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_TYPE_WEB_DESC
        AUTH_MAXLOGIN_DESC
    },
  #endif
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portMaxHost_set,
         "authentication max-hosts <$DEF1-$DEF2>",
        AUTH_INTF_DESC
        AUTH_MAXHOST_DESC
        AUTH_MAXHOST_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMaxHost_set,
         "no authentication max-hosts",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_MAXHOST_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portReauthEnbl_set,
         "[no] authentication reauth",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_REAUTH_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portVlanAssignMode_set,
         "authentication radius-attributes vlan (reject|static)",
        AUTH_INTF_DESC
        AUTH_RADATTR_DESC
        AUTH_RADATTR_VLAN_DESC
        "If the Radius server authorized the supplicant, but did not provide a supplicant VLAN, the supplicant is rejected. If the parameter is omitted the option is applied by default.\n"
        "If the Radius server authorized the supplicant, but did not provide a supplicant VLAN, the supplicant is accepted.\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portVlanAssignMode_set,
         "no authentication radius-attributes vlan",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_RADATTR_DESC
        AUTH_RADATTR_VLAN_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portReauthPeriod_set,
         "authentication timer reauth <"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_REAUTH_PERIOD_MAX)">",
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_REAUTH_DESC
        AUTH_TIME_REAUTH_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portReauthPeriod_set,
         "no authentication timer reauth",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_REAUTH_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portInactTimeout_set,
         "authentication timer inactive <"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_INACT_TIMEOUT_MAX)">",
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_INACT_DESC
        AUTH_TIME_INACT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portInactTimeout_set,
         "no authentication timer inactive",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_INACT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portQuietPeriod_set,
         "authentication timer quiet <"SYS_STR_CONST(BOND_AUTHMGR_QUIET_PERIOD_MIN)"-"SYS_STR_CONST(BOND_AUTHMGR_QUIET_PERIOD_MAX)">",
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_QUIET_DESC
        AUTH_TIME_QUIET_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portQuietPeriod_set,
         "no authentication timer quiet",
        NO_DESC
        AUTH_INTF_DESC
        AUTH_TIMER_DESC
        AUTH_TIME_QUIET_DESC
    },
  #ifdef CONFIG_SYS_PROTO_DOT1X
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_typeEnbl_set,
         "[no] dot1x",
        NO_DESC
        DOT1X_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_guestVlan_set,
         "dot1x guest-vlan <1-4094>",
        DOT1X_DESC
        GUEST_VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_guestVlan_set,
         "no dot1x guest-vlan",
        NO_STR
        DOT1X_DESC
        GUEST_VLAN_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portGuestVlanEnbl_set,
         "[no] dot1x guest-vlan",
        NO_STR
        DOT1X_DESC
        GUEST_VLAN_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_authmgr_portCtrl_set,
         "dot1x (auto|force-auth|force-unauth)",
        DOT1X_DESC
        ARG_AUTH_PORTCTRL
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portCtrl_set,
         "dot1x port-control (auto|force-auth|force-unauth)",
        DOT1X_DESC
        AUTH_PORTCTRL_DESC
        ARG_AUTH_PORTCTRL
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portCtrl_set,
         "no dot1x port-control",
        NO_STR
        DOT1X_DESC
        AUTH_PORTCTRL_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portReauthEnbl_set,
         "[no] dot1x reauth",
        NO_STR
        DOT1X_DESC
        AUTH_REAUTH_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portReauthPeriod_set,
         "dot1x timeout reauth-period <$DEF1-$DEF2>",
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Number of seconds between re-authentication attempts\n"
        "Re-authentication period (default: $DEF3 seconds)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portReauthPeriod_set,
         "no dot1x timeout reauth-period",
        NO_STR
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Number of seconds between re-authentication attempts\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portQuietPeriod_set,
         "dot1x timeout quiet-period <$DEF1-$DEF2>",
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Quiet period\n"
        "Quiet period (default: $DEF3 seconds)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portQuietPeriod_set,
         "no dot1x timeout quiet-period",
        NO_STR
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Quiet period\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portSuppTimeout_set,
         "dot1x timeout supp-timeout <$DEF1-$DEF2>",
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Supplicant timeout period\n"
        "Supplicant timeout period (default: $DEF3 seconds)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portSuppTimeout_set,
         "no dot1x timeout supp-timeout",
        NO_STR
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Supplicant timeout period\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portSrvTimeout_set,
         "dot1x timeout server-timeout <$DEF1-$DEF2>",
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Set the number of seconds to wait for a retransmission of packets to the authentication server\n"
        "Supplicant timeout period (default: $DEF3 seconds)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portSrvTimeout_set,
         "no dot1x timeout server-timeout",
        NO_STR
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Set the number of seconds to wait for a retransmission of packets to the authentication server\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portEapTxPeriod_set,
         "dot1x timeout tx-period <$DEF1-$DEF2>",
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Set the number of seconds to wait for a response to an EAP-request/identity before resending the request.\n"
        "Supplicant timeout period (default: $DEF3 seconds)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portEapTxPeriod_set,
         "no dot1x timeout tx-period",
        NO_STR
        DOT1X_DESC
        DOT1X_TIMEOUT_STR
        "Set the number of seconds to wait for a response to an EAP-request/identity before resending the request.\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_authmgr_portMaxEapReq_set,
         "dot1x max-req <$DEF1-$DEF2>",
        DOT1X_DESC
        "Maximum request retries\n"
        "Maximum request retries (default: $DEF3 times)\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_authmgr_portMaxEapReq_set,
         "no dot1x max-req",
        NO_STR
        DOT1X_DESC
        "Maximum request retries\n"
    },
  #endif
#endif

    {
         NODE_CONFIG_ALLSUB,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_intf_set,
         "interface IF_ALL",
        IF_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_ALLSUB,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_intf_set,
         "interface range IF_ALLS",
        IF_DESC
        IF_RANGE_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG_ALLSUB,
         ATTR_PRIV_15,
         NULL,
         cmd_exit_set,
         "exit",
        "Exit from current mode\n"
    },

#ifdef CONFIG_SYS_PROTO_DAI
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_enable_set,
         "[no] ip arp inspection",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_vlan_enable_set,
         "[no] ip arp inspection vlan VLAN-LIST",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_port_trust_set,
         "[no] ip arp inspection trust",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        "Switch does not check ARP packets that it receives on the trust interface\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_port_smacChk_set,
         "[no] ip arp inspection validate src-mac",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        "Validate ARP packets\n"
        "Compare the source MAC address in the Ethernet header against the sender MAC address in the ARP body\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_port_dmacChk_set,
         "[no] ip arp inspection validate dst-mac",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        "Validate ARP packets\n"
        "Compare the destination MAC address in the Ethernet header against the target MAC address in the ARP body\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_port_ipChk_set,
         "[no] ip arp inspection validate ip [allow-zeros]",
        NO_DESC
        IP_DESC
        ARP_DESC
        DAI_DESC
        "Validate ARP packets\n"
        "Compare the ARP body for invalid and unexpected IP addresses. Addresses include 0.0.0.0, 255.255.255.255, and all IP multicast addresses. \n"
        "Sender address of 0.0.0.0 (ARP probes) are not denied.\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_rateLimit_set,
         "ip arp inspection rate-limit <"SYS_STR_CONST(BOND_DAI_RATELIMIT_MIN)"-"SYS_STR_CONST(BOND_DAI_RATELIMIT_MAX)">",
        IP_DESC
        ARP_DESC
        DAI_DESC
        DAI_LIMIT_RATE
        "Value of rate pps\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_rateLimit_set,
         "no ip arp inspection rate-limit",
        NO_STR
        IP_DESC
        ARP_DESC
        DAI_DESC
        DAI_LIMIT_RATE
    },
#endif 

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_enable_set,
         "[no] ip dhcp snooping",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_vlan_enable_set,
         "[no] ip dhcp snooping vlan VLAN-LIST",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_portTrust_set,
         "[no] ip dhcp snooping trust",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP Snooping trust port configuration\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_check_set,
         "[no] ip dhcp snooping verify mac-address",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "verify on an untrusted port\n"
        "the source MAC address in a DHCP packet matches the client hardware address. \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_remoteID_set,
         "ip dhcp snooping option remote-id STRING",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
        "Remote ID\n"
        "ID string length (1~63)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_remoteID_set,
         "no ip dhcp snooping option remote-id",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
        "Restore Remote ID to default\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_enable_set,
         "[no] ip dhcp snooping option",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_action_set,
         "ip dhcp snooping option action (drop|keep|replace)",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
        "Action for packets with option82 that are received on untrusted ports\n"
        "Drop packets with option82\n" "Keep original option82\n" "Replace option82 content by switch setting\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_action_set,
         "no ip dhcp snooping option action",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_OPTION82_DESC
        "Restore action to default for packets with option82 that are received on untrusted ports\n"
    },
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_circuitID_set,
         "ip dhcp snooping [vlan <1-4094>] option circuit-id STRING",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        DHCP_OPTION82_DESC
        "Circuit ID\n"
        "ID string (1~63)\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_option82_circuitID_set,
         "no ip dhcp snooping [vlan <1-4094>] option circuit-id",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        DHCP_OPTION82_DESC
        "Circuit ID\n"
    },
#endif
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_dhcp_snooping_rateLimit_set,
         "ip dhcp snooping limit rate <1-300>",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_LIMIT
        DHCP_LIMIT_RATE
        "Value 1-300 pps\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_rateLimit_set,
         "ip dhcp snooping rate-limit <"SYS_STR_CONST(BOND_DHCP_SNOOPING_RATELIMIT_MIN)"-"SYS_STR_CONST(BOND_DHCP_SNOOPING_RATELIMIT_MAX)">",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_LIMIT_RATE
        "Value of rate pps\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_snooping_rateLimit_set,
         "no ip dhcp snooping rate-limit",
        NO_STR
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        DHCP_LIMIT_RATE
    },
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_enable_set,
         "[no] dhcp-server",
        NO_DESC
        DHCPSERVER_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_group_set,
         "dhcp-server group <1-256> ip A.B.C.D",
        DHCPSERVER_DESC
        DHCPSERVERGROUP_DESC
        "dhcp server group number , from 1 to 256\n"
        "configure ipaddress \n"
        "dhcp server group's ip address with A.B.C.D"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_group_set,
         "no dhcp-server group <1-256> ip",
        NO_DESC
		DHCPSERVER_DESC
        DHCPSERVERGROUP_DESC
		"dhcp server group number , from 1 to 256\n"
		"configure ipaddress \n"
    },
    {
#ifdef CONFIG_SYS_L3_ARCHITECTURE
         NODE_IF_VLAN,
#else
		 NODE_IF_VLAN | NODE_CONFIG,
#endif
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_intf_group_ip_set,
         "[no] dhcp-server group <1-256>",
        NO_DESC
		DHCPSERVER_DESC
		DHCPSERVERGROUP_DESC
		"dhcp server group number , from 1 to 256\n"
    },
	
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_enable_set,
		 "[no] ip dhcp option82",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_device_enable_set,
		 "[no] ip dhcp option82 device-id",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Carry device id in the circuit id of sub option 1\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_format_set,
		 "ip dhcp option82 format (normal|verbose)",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Format of option82\n"
		"Normal format of option82\n"
		"Verbose format of option82\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_format_set,
		 "no ip dhcp option82 format",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Format of option82\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_info_format_set,
		 "ip dhcp option82 information format (hex|ascii)",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Information format for verbose\n"
		"Format of option82\n"
		"Use hex format\n"
		"Use ascii code format\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_info_format_set,
		 "no ip dhcp option82 information format",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Information format for verbose\n"
		"Format of option82\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_format_verbose_set,
		 "ip dhcp option82 format verbose node-identifier (mac | hostname |  WORD<1-50>)",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Format of option82\n"
		"Verbose format of option82\n"
		"Node identifier\n"
		"Take mac address as node identifier\n"
		"Take hostname as node identifier\n"
		"User-defined node identifier (1--50 chars)\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_format_verbose_set,
		 "no ip dhcp option82 format verbose node-identifier",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Format of option82\n"
		"Verbose format of option82\n"
		"Node identifier\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_enable_set,
		 "[no] ip dhcp option82",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_strategy_set,
		 "ip dhcp option82 strategy (drop | keep | replace)",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Strategy of option82\n"
		"Drop packet when it has option82\n"
		"Keep packet's option82 when it has option82\n"
		"Replace packet's option82 when it has option82\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_strategy_set,
		 "ip dhcp option82 strategy apend (hostname|hostname-ip)",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Strategy of option82\n"
		"Append suboption9 to packet's option82 when it has option82\n"
		"Append suboption9 to packet's option82 with hostname\n"
		"Append suboption9 to packet's option82 with hostname and ip\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_strategy_set,
		 "no ip dhcp option82 strategy",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Strategy of option82\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_circuit_id_set,
		 "ip dhcp option82 circuit-id string WORD<1-64>",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Circuit id\n"
		"String\n"
		"String id\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_circuit_id_set,
		 "no ip dhcp option82 circuit-id string",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Circuit id\n"
		"String\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_remote_id_set,
		 "ip dhcp option82 remote-id string (WORD<1-64>|hostname) ",
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Remote id\n"
		"String\n"
		"User-defined remote id (1--64 chars)\n"
		"Take hostname as remote id\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_option82_port_remote_id_set,
		 "no ip dhcp option82 remote-id string",
		NO_DESC
		IP_DESC
		DHCPSERVER_DESC
		DHCP_OPTION82_DESC
		"Remote id\n"
		"String\n"
	},
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_ip_pool_set,
         "[no] ip pool WORD<1-32>",
        NO_DESC
		IP_DESC
        "ip pool configuration\n"
        "create pool\n"
    },
    {
	     NODE_IP_POOL,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_ip_pool_gateway_set,
	     "gateway A.B.C.D/M",
		"The gateway of the ip pool\n"
	    L3_IP_DST_PRFIX_DESC
	    L3_IP_DST_PRFIX_LEN_DESC
	},
	{
	     NODE_IP_POOL,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_no_ip_pool_gateway_set,
	     "no gateway",
	    NO_DESC
		"The gateway of the ip pool\n"
	},
    {
	     NODE_IP_POOL,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_ip_pool_section_set,
	     "section <1-8> A.B.C.D A.B.C.D",
		"The section of the ip pool\n"
		"The section id of the ip pool\n"
	    "start ip address of a section\n"
	    "end ip address of a section\n"
	},
    {
	     NODE_IP_POOL,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_no_ip_pool_section,
	     "no section <1-8>",
		NO_DESC
		"The section of the ip pool\n"
		"The section id of the ip pool\n"
	},
	{
	     NODE_IP_POOL,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_ip_pool_disable_ip,
	     "[no] ip disable A.B.C.D",
		NO_DESC
		"disable/enable specified ip address\n"
		"disable ipaddress\n"
		"ipv4 address\n"
	},
	{
		 NODE_IF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_relay_port_enable_set,
		 "[no] dhcp-relay",
		NO_DESC
		DHCPRELAY_DESC
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_client_config_ip_bind,
		 "[no] dhcp-client bind",
		NO_DESC
		"configure the static ip of host\n"
		"configure dhcp-client bind "
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_client_host_mac_ip_set,
		 "dhcp-client  A:B:C:D:E:F A.B.C.D <1-4094> NAME",
		"configure the static ip of host\n"
		"MAC address xx:xx:xx:xx:xx:xx\n"
		ARG_IPADDR_DESC
		"VLAN ID identification\n"
		"User name(1--32 chars)\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_client_host_mac_ip_set,
		 "no dhcp-client A:B:C:D:E:F <1-4094>",
		NO_DESC
		"configure the static ip of host\n"
		"MAC address xx:xx:xx:xx:xx:xx\n"
		"VLAN ID identification\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_client_unknown_client_assign,
		 "[no] dhcp-client unknown-client assign",
		NO_DESC
		"configure the static ip of host\n"
		"configure unbound client\n"
		"allow assignment of ip address\n"
	},

	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_dns_set,
		 "dns (primary-ip|second-ip) A.B.C.D",
		"The dns server ip of an ip pool configure\n"
		"The primary dns ip\n"
		"The second dns ip\n"
		"The third dns ip\n"
		"The fourth dns ip\n"
		ARG_IPADDR_DESC
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_dns_set,
		 "no dns (primary-ip|second-ip)",
		NO_DESC
		"The dns server ip of an ip pool configure\n"
		"The primary dns ip\n"
		"The second dns ip\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_dns_suffix_set,
		 "dns suffix WORD<1-32>",
		"IP pool dns suffix configure\n"
		"dns suffix\n"
        "IP pool dns suffix name\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_dns_suffix_set,
		 "no dns suffix",
		NO_DESC
		"IP pool dns suffix configure\n"
		"dns suffix\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_wins_set,
		 "wins (primary-ip|second-ip) A.B.C.D",
		"The wins server ip configure for an ip pool\n"
		"The primary wins ip\n"
		"The second wins ip\n"
		ARG_IPADDR_DESC
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_wins_set,
		 "no wins (primary-ip|second-ip)",
		NO_DESC
		"The wins server ip configure for an ip pool\n"
		"The primary wins ip\n"
		"The second wins ip\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_lease_time_set,
		 "lease DD:HH:MM",
		"The lease time configure for an ip pool\n"
		"days:hours:minutes, max lease time 999:00:00; eg 1:02:33\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_dhcp_server_lease_time_set,
		 "no lease",
		NO_DESC
		"The lease time configure for an ip pool\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_pool_option43_set,
		 "option 43 (ascii|hex) WORD<1-64> ",
		"Configure dhcp option value of ip pool\n"
		"DHCP-Option 43 configuration\n"
		"Ascii format\n"
		"Hex format\n"
		"Value of option\n"
	},
	{
		 NODE_IP_POOL,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_pool_option43_set,
		 "no option 43",
		NO_DESC
		"Configure dhcp option value of ip pool\n"
		"DHCP-Option 43 configuration\n"
	},
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_typeEnable_set,
         "[no] dos (tcp-frag-off-min-check|synrst-deny|synfin-deny|xma-deny|nullscan-deny|syn-sportl1024-deny|tcphdr-min-check|smurf-deny|icmpv6-ping-max-check|icmpv4-ping-max-check|icmp-frag-pkts-deny|ipv6-min-frag-size-check|pod-deny|tcpblat-deny|udpblat-deny|land-deny|daeqsa-deny)",
        NO_DESC
        DOS_DESC
        ARG_DOS_TYPE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_maxPingLen_set,
         "dos icmp-ping-max-length <"SYS_STR_CONST(BOND_DOS_PINGMAXLENGTH_MIN)"-"SYS_STR_CONST(BOND_DOS_PINGMAXLENGTH_MAX)">",
        DOS_DESC
        "ICMPv4/ICMPv6 ping maximum packet size\n"
        "Maximum length ICMP packet allowed before dropping\n"
    },
    {
	     NODE_CONFIG,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_dos_maxPingLen_set_default,
	     "no dos icmp-ping-max-length",
	    NO_DESC
	    DOS_DESC
	    "ICMPv4/ICMPv6 ping maximum packet size\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_minIPv6FragLen_set,
         "dos ipv6-min-frag-size-length <"SYS_STR_CONST(BOND_DOS_IPV6MINFRAGLENGTH_MIN)"-"SYS_STR_CONST(BOND_DOS_IPV6MINFRAGLENGTH_MAX)">",
        DOS_DESC
        "Check minimum size of IPv6 fragments size\n"
        "Minimum size of IPv6 fragments\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_smurfNetmaskLen_set,
         "dos smurf-netmask <"SYS_STR_CONST(BOND_DOS_SMURFLENGTH_MIN)"-"SYS_STR_CONST(BOND_DOS_SMURFLENGTH_MAX)">",
        DOS_DESC
        "Smurf Attacks size\n"
        "Netmask Length\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_minTCPHdrLen_set,
         "dos tcphdr-min-length <"SYS_STR_CONST(BOND_DOS_TCPHDRMINCHKLENGTH_MIN)"-"SYS_STR_CONST(BOND_DOS_TCPHDRMINCHKLENGTH_MAX)">",
        DOS_DESC
        "Check minimum TCP header size\n"
        "TCP header length\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_dos_portAttackPreventEnable_set,
         "[no] dos",
        NO_DESC
        DOS_DESC
    },
#endif 

#ifdef CONFIG_SYS_EEE
    
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_eee_portEnable_set,
         "[no] eee",
        NO_STR
        EEE_DESC
    },
#endif 

#ifdef CONFIG_SYS_PROTO_GVRP
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_enable_set,
         "[no] gvrp",
        NO_DESC
        GVRP_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_port_enable_set,
         "[no] gvrp",
        NO_DESC
        GVRP_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_port_regmode_set,
         "gvrp registration-mode (normal|fixed|forbidden)",
        GVRP_DESC
        "GVRP registration mode setting\n"
        "Normal mode\n" "Fixed mode\n" "Forbidden mode\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_port_forbidVlan_set,
         "[no] gvrp vlan-creation-forbid",
        NO_DESC
        GVRP_DESC
        "GVRP vlan creation forbid setting\n"
    },
    {
        
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_gvrp_timer_set,
         "gvrp timer (join|leave|leaveall) <5-32765>",
        GVRP_DESC
        "Sets the time parameter\n"
        "join time\n" "leave time\n" "leaveall time\n"
        "timeout interval (centisecond)\n"
    },
#endif 

#ifdef CONFIG_SYS_PROTO_GARP
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_gmrp_enable_set,
         "[no] gmrp",
        NO_DESC
        GMRP_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_gmrp_port_enable_set,
         "[no] gmrp",
        NO_DESC
        GMRP_DESC
    },
#endif 

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_enable_set,
         "[no] ip igmp snooping",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_suppre_enable_set,
         "[no] ip igmp snooping report-suppression",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "IGMP v1/v2 report suppression\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_fastleave_set,
         "[no] ip igmp snooping vlan VLAN-LIST immediate-leave",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        IGMP_IMMEDLEAVE_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_igmp_fastleave_set,
         "[no] ip igmp snooping vlan VLAN-LIST fastleave",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Fast leave function \n"
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_routerPort_set,
         "[no] ip igmp snooping vlan VLAN-LIST static-router-port IF_PORTS",
        NO_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Static mrouter port configuration\n"
        PORT_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_routerPortForbid_set,
         "[no] ip igmp snooping vlan VLAN-LIST forbidden-router-port IF_PORTS",
        NO_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Forbidden mrouter port configuration\n"
        PORT_DESC
        ARG_IF_PORT
    },
    #endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_group_set,
         "[no] ip igmp snooping vlan VLAN-LIST static-group A.B.C.D interfaces IF_PORTS",
        NO_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Static group configuration\n"
        "IPV4 multicast address\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_group_clear,
  #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
         "no ip igmp snooping vlan VLAN-LIST group A.B.C.D [filter A.B.C.D]",
  #else
         "no ip igmp snooping vlan VLAN-LIST group A.B.C.D",
  #endif
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Group address\n"
        "Class-D IP address\n"
  #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
        "Source address\n"
  #endif
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_unknMcastAction_set,
         "ip igmp snooping unknown-multicast action (drop|flood|router-port)",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Unknown multicast\n"
        "Action on receiving unknown multicast packets\n"
        "Drop the packets\nFlood the packets\nForward to router ports\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_unknMcastAction_set,
         "no ip igmp snooping  unknown-multicast action",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Unknown multicast\n"
        "Default action(flood) on receiving unknown multicast packets\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_pimDvmrpLearn_set,
         "[no] ip igmp snooping vlan VLAN-LIST router learn pim-dvmrp",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "IGMP snooping router\n"
        "IGMP snooping router port learn\n"
        "IGMP snooping router port learn by pim, dvmrp and IGMP messages\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_robustnessVar_set,
         "ip igmp snooping vlan VLAN-LIST robustness-variable <"SYS_STR_CONST(BOND_MCAST_ROBUST_MIN)"-"SYS_STR_CONST(BOND_MCAST_ROBUST_MAX)">",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Robustness Variable\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_ROBUST_MIN)"-"SYS_STR_CONST(BOND_MCAST_ROBUST_MAX)". Default is "SYS_STR_CONST(DFLT_IGMP_ROBUSTNESS_VAR)".\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_robustnessVar_set,
         "no ip igmp snooping vlan VLAN-LIST robustness-variable",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Robustness Variable is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_responseTime_set,
         "ip igmp snooping vlan VLAN-LIST response-time <"SYS_STR_CONST(BOND_MCAST_RESPONSE_MIN)"-"SYS_STR_CONST(BOND_MCAST_RESPONSE_MAX)">",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Response Time\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_RESPONSE_MIN)"-"SYS_STR_CONST(BOND_MCAST_RESPONSE_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_QUERY_RESPONSE_INTV)" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_responseTime_set,
         "no ip igmp snooping vlan VLAN-LIST response-time",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Response time is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_queryIntvl_set,
         "ip igmp snooping vlan VLAN-LIST query-interval <"SYS_STR_CONST(BOND_MCAST_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_QRYINTV_MAX)">",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Query Interval\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_QRYINTV_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_QUERY_INTV)" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_queryIntvl_set,
         "no ip igmp snooping vlan VLAN-LIST query-interval",
        NO_DESC
        IP_STR
        IGMP_DESC
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Query Interval is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_lastMbrQueryIntvl_set,
         "ip igmp snooping vlan VLAN-LIST  last-member-query-interval <"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MAX)">",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Interval\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_GSQUERY_RESPONSE_INTV )" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_lastMbrQueryIntvl_set,
         "no ip igmp snooping vlan VLAN-LIST last-member-query-interval",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Interval is default.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_lastMbrQueryCount_set,
         "ip igmp snooping vlan VLAN-LIST last-member-query-count <"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MAX)">",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Count\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_ROBUSTNESS_VAR)".\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_lastMbrQueryCount_set,
         "no ip igmp snooping vlan VLAN-LIST  last-member-query-count",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Count is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_vlan_set,
         "[no] ip igmp snooping vlan VLAN-LIST",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_port_set,
         "[no] ip igmp snooping vlan VLAN-LIST static-port IF_PORTS",
        NO_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "IPv4 static port configuration\n"
        PORT_DESC
        ARG_PORTLIST_ALL_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_forbidden_port_set,
         "[no] ip igmp snooping vlan VLAN-LIST forbidden-port IF_PORTS",
        NO_STR
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "IPv4 forbidden port configuration\n"
        PORT_DESC
        ARG_PORTLIST_ALL_DESC
    },
    #endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_lookupMode_set,
         "ip igmp snooping forward-method (mac|dip)",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Forward method\n"
        "MAC method\n"
        "DIP method\n"
        "Source and destination IP\n"
    },
  #if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_operVersion_set,
         "ip igmp snooping version (2|3)",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "IGMP Snooping Operation Version\n"
        "IGMP Operation Version is v2\nIGMP Operation Version is v3\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_filter_group_set,
         "ip igmp snooping group A.B.C.D filter A.B.C.D",
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        "Filter the Group"
        "Group ip address\n"
        "Filter the source\n"
        "Filter source ip address\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_querier_set,
      #if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
         "ip igmp snooping vlan VLAN-LIST querier [version (2|3)]",
      #else
         "ip igmp snooping vlan VLAN-LIST querier",
      #endif
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        IGMP_QRY_STR
      #if defined(CONFIG_SYS_PROTO_IGMPV3_BASIC) || defined(CONFIG_SYS_PROTO_IGMPV3_FULL)
        "Querier Version configuration\n"
        "Version 2\nVersion 3\n"
      #endif
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_querier_set,
         "no ip igmp snooping [vlan VLAN-LIST] querier",
        NO_DESC
        IP_STR
        IGMP_STR
        IGMP_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        IGMP_QRY_STR
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
      {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_igmp_portGroup_limit_set,
         "ip igmp max-groups <0-$DEF>",
        IP_STR
        IGMP_STR
        "Max group number\n"
        "IGMP snooping max group number 0~$DEF"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_portGroup_limit_set,
         "no ip igmp max-groups",
        NO_STR
        IP_STR
        IGMP_STR
        "Default max groups number \n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_portGroup_excessAct_set,
         "ip igmp max-groups action (deny|replace)",
        IP_STR
        IGMP_STR
        "IGMP max groups\n"
        "Excess max group action \n"
        "IGMP max-group action deny\nIGMP max-group action replace\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_profile_enter_set,
         "ip igmp profile <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        IP_STR
        IGMP_STR
        "IGMP profile\n"
        "Profile index\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_profile_remove,
         "no ip igmp profile <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        NO_DESC
        IP_STR
        IGMP_STR
        "IGMP profile\n"
        "Profile index\n"
    },
    {
         NODE_IGMP_PROFILE,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_profileEntry_set,
         "profile range ip A.B.C.D [A.B.C.D] action (permit|deny)",
        "IGMP profile\n"
        "IGMP group ip range\n"
        "Group ip configuration\n"
        "IPv4 multicast address start\n"
        "IPv4 multicast address end\n"
        "Profile rule action\n"
        "Action permit\n"
        "Action deny\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_filter_set,
         "ip igmp filter <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        IP_STR
        IGMP_STR
        "IPv4 filter\n"
        "IPv4 filter profile index\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_filter_set,
         "no ip igmp filter",
        NO_DESC
        IP_STR
        IGMP_STR
        "IPv4 filter\n"
    },
  #endif 

  #ifdef CONFIG_SYS_PROTO_MVR
   {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_enable_set,
         "[no] mvr",
        NO_DESC
        "MVR global enable\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_igmp_mvr_group_set,
         "mvr group A.B.C.D [<1-$DEF>]",
        MVR_STR
        "MVR group address configuration\n"
        "IPV4 multicast address\n"
        "Contiguous series of ipv4 multicast address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_vlan_set,
         "mvr vlan <1-4094>",
        MVR_STR
        "MVR VLAN ID configuration\n"
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_mode_set,
         "mvr mode (dynamic|compatible)",
        MVR_STR
        "MVR mode configuration\n"
        "Dynamic mode \n"
        "Compatible mode \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_queryTime_set,
         "mvr query-time <"SYS_STR_CONST(BOND_IGMP_MVR_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_IGMP_MVR_QRYINTV_MAX)">",
        MVR_STR
        "MVR Query Response Time\n"
        "Valid range is "SYS_STR_CONST(BOND_IGMP_MVR_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_IGMP_MVR_QRYINTV_MAX)" second. Default is "SYS_STR_CONST(DFLT_IGMP_MVR_QUERYTIME)".\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_queryTime_set,
         "no mvr query-time",
        NO_DESC
        MVR_STR
        "Query Response Time is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_static_group_set,
         "[no] mvr vlan <1-4094> group A.B.C.D interfaces IF_PORTS",
        NO_STR
        MVR_STR
        VLAN_DESC
        ARG_VLAN_DESC
        "Static group configuration\n"
        "IPV4 multicast address\n"
        IFS_DESC
        ARG_PORTLIST_ALL_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_portType_set,
         "mvr type (source|receiver)",
        MVR_STR
        "MVR port type configuration\n"
        "MVR port type is source\n"
        "MVR port type is receiver\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_portType_set,
         "no mvr type",
        NO_DESC
        MVR_STR
        "MVR port type configuration is none\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_mvr_portFastLeave_set,
         "[no] mvr immediate",
        NO_DESC
        MVR_STR
        "MVR port immediate configuration\n"
    },
  #endif 

#endif 
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_group_set_by_mac,
         "[no] multicast address-table A:B:C:D:E:F vlan VLAN-LIST interfaces IF_PORTS",
        NO_STR
		"multicast group\n"
		MAC_ADDRESS_TABLE_DESC
		"MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLANLIST_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_l2_mcast_group_del,
         "no multicast address-table A:B:C:D:E:F vlan VLAN-LIST",
        NO_STR
		"multicast group\n"
		MAC_ADDRESS_TABLE_DESC
		"MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    {
         NODE_CONFIG_SHOW,
         ATTR_PRIV_15,
         NULL,
         cmd_igmp_static_l2_mcast_group_show,
         "show multicast address-table",
        SHOW_STR
		"multicast group\n"
		MAC_ADDRESS_TABLE_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },

#ifdef CONFIG_SYS_PROTO_ISG
    
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_port_enable_set,
         "[no] ip source verify",
        NO_DESC
        IP_DESC
        "source ip mode\n"
        "Verify packet\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_port_enable_set,
         "ip source verify mac-and-ip",
        IP_DESC
        ISG_DESC
        "Verify packet\n"
        "Source mac and ip mode\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_portMaxEntryNum_set,
         "ip source binding max-entry (<"SYS_STR_CONST(BOND_IPSG_PORT_ENTRY_MIN)"-"SYS_STR_CONST(BOND_IPSG_PORT_ENTRY_MAX)">|no-limit)",
        IP_DESC
        ISG_DESC
        ISG_BINDING_DESC
        "Port max dynamic binding entry number configuration\n"
        "Max entry number range, "SYS_STR_CONST(BOND_IPSG_PORT_ENTRY_MIN)"-"SYS_STR_CONST(BOND_IPSG_PORT_ENTRY_MAX)"\nNo limited entry number\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_bindTblEntry_set,
         "[no] ip source binding A:B:C:D:E:F vlan <1-4094> A.B.C.D interface IF_PORT",
        NO_DESC
        IP_DESC
        ISG_DESC
        ISG_BINDING_DESC
        "MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLAN_DESC
        "IP address\n"
        IF_DESC
        ARG_IF_PORT
    },
  #ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_db_set,
         "ip dhcp snooping database flash",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Write database to flash\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_db_set,
         "ip dhcp snooping database tftp (A.B.C.D|HOSTNAME) NAME",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Write database to remote tftp server\n"
        "IP Address of remote tftp server\nHostname of remote tftp server\n"
        "File name of backup file\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_db_set,
         "ip dhcp snooping database timeout <"SYS_STR_CONST(BOND_IPSG_DB_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_IPSG_DB_TIMEOUT_MAX)">",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Configure abort timeout interval\n"
        SYS_STR_CONST(BOND_IPSG_DB_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_IPSG_DB_TIMEOUT_MAX)" seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_db_set,
         "ip dhcp snooping database write-delay <"SYS_STR_CONST(BOND_IPSG_DB_WRITE_DELAY_MIN)"-"SYS_STR_CONST(BOND_IPSG_DB_WRITE_DELAY_MAX)">",
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Configure delay timer for writes to URL\n"
        SYS_STR_CONST(BOND_IPSG_DB_WRITE_DELAY_MIN)"-"SYS_STR_CONST(BOND_IPSG_DB_WRITE_DELAY_MAX)" seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_db_set,
         "no ip dhcp snooping database [(timeout|write-delay)]",
        NO_DESC
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
        "DHCP snooping database agent\n"
        "Configure abort timeout interval\nConfigure delay timer for writes to URL\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_ISG_NETMASK
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_bindTblEntry_set,
         "[no] ip source binding vlan <1-4094> A.B.C.D A.B.C.D interface IF_PORT",
        NO_DESC
        IP_DESC
        ISG_DESC
        ISG_BINDING_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        "IP address\n"
        "Netmask of IP address\n"
        IF_DESC
        ARG_IF_PORT
    },
  #endif
#endif 

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_l2_agingtime_set,
         "mac address-table aging-time <$DEF1-$DEF2>",
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "aging time of the address table\n"
        "Aging-time range in seconds indicating how long an entry remain in mac address table\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_agingtime_never_set,
         "mac address-table aging-time 0",
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "aging time of the address table\n"
        "Aging time of 0 means no aging\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_staticMacEntry_set,
         "mac address-table static A:B:C:D:E:F vlan <1-4094> interfaces IF_PORTS",
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "Static MAC address\n"
        "MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLAN_DESC
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_staticMacEntry_del,
         "no mac address-table static A:B:C:D:E:F vlan <1-4094>",
        NO_STR
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "Static MAC address\n"
        "MAC address xx:xx:xx:xx:xx:xx\n"
        VLAN_DESC
        ARG_VLAN_DESC
    },
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_filterMacEntry_set,
         "mac address-table static A:B:C:D:E:F vlan <1-4094> drop",
        MAC_DESC
        MAC_ADDRESS_TABLE_DESC
        "Static configuration\n"
        "Unicast Source or Destination MAC address\n"
        VLAN_DESC
        ARG_VLAN_DESC
        "Drop packets with the specified source or destination unicast mac address\n"
    },
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfEnbl_set,
         "[no] shutdown",
        NO_DESC
        PORT_SHUTDOWN_DESC
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfExist_set,
         "[no] on",
        NO_DESC
        PORT_SHUTDOWN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_fwdIPv4Enable_set,
         "[no] ip unicast-routing",
        NO_DESC
        IP_STR
        "Enable forwarding of IPv4 unicast datagrarm\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_set,
         "ip address A.B.C.D A.B.C.D",
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
        "Netmask\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_sub_set,
         "ip address A.B.C.D A.B.C.D sub",
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
        "Netmask\n"
        "Indicate a subordinate address\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_set,
         "ip address A.B.C.D/M",
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_sub_set,
         "ip address A.B.C.D/M sub",
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
        "Indicate a subordinate address\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_set,
         "no ip address [A.B.C.D]",
        NO_DESC
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intIPv4Addr_sub_set,
         "no ip address A.B.C.D sub",
        NO_DESC
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
        "Indicate a subordinate address\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intIPv4Addr_set,
         "[no] ip dhcp",
        NO_DESC
        IP_STR
        "DHCP client\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV4_set,
         "ip route A.B.C.D/M A.B.C.D [metric <1-255>] [description WORD<0-31>]",
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        "Route Description\n"
        "Description String\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV4_set,
         "no ip route A.B.C.D/M [A.B.C.D]",
        NO_DESC
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV4_set,
         "ip route A.B.C.D A.B.C.D A.B.C.D [metric <1-255>] [description WORD<0-31>]",
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_DESC
        L3_IP_DST_PRFIX_MASK_DESC
        L3_STATIC_ROUTE_GW_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        "Route Description\n"
        "Description String\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV4_set,
         "no ip route A.B.C.D A.B.C.D [A.B.C.D]",
        NO_DESC
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_DESC
        L3_IP_DST_PRFIX_MASK_DESC
        L3_STATIC_ROUTE_GW_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_fwdIPv6Enable_set,
         "[no] ipv6 unicast-routing",
        NO_DESC
        IPV6_STR
        "Enable forwarding of IPv6 unicast datagrarm\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_icmpv6RateInterval_set,
         "ipv6 icmp-rate-limit interval <0-2147483647>",
        IPV6_STR
        "ICMPv6 reate limit\n"
        "Interval\n"
        "Interval value (msecs)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_icmpv6RateInterval_set,
         "no ipv6 icmp-rate-limit interval",
        NO_DESC
        IPV6_STR
        "ICMPv6 reate limit\n"
        "Interval\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_icmpv6RateBurst_set,
         "ipv6 icmp-rate-limit burst <1-200>",
        IPV6_STR
        "ICMPv6 reate limit\n"
        "Burst\n"
        "Burst value (secs)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_icmpv6RateBurst_set,
         "no ipv6 icmp-rate-limit burst",
        NO_DESC
        IPV6_STR
        "ICMPv6 reate limit\n"
        "Burst\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_routev6Prefer_set,
         "ipv6 route preference <1-255>",
        IPV6_STR
        ROUTE_STR
        "Preference\n"
        "Preference value\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_routev6Prefer_set,
         "no ipv6 route preference",
        NO_DESC
        IPV6_STR
        ROUTE_STR
        "Preference\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6Enbl_set,
         "[no] ipv6 enable",
        NO_DESC
        IPV6_STR
        "Enable IPv6 on interface\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6Mode_set,
         "[no] ipv6 mode",
        NO_DESC
        IPV6_STR
        "Enable IPv6 mode on interface\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6AutoconfEnbl_set,
         "[no] ipv6 address autoconfig",
        NO_DESC
        IPV6_STR
        L3_INTF_IPV6_ADDR_DESC
        "Enable automatic configuration of IPv6 addresses using |stateless autoconfiguration on an interface\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intfIPv6Addr_set,
         "ipv6 address X:X::X:X link-local",
        IPV6_STR
        L3_INTF_IPV6_ADDR_DESC
        "IPv6 link-local address\n"
        "Use link-local address\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intfIPv6Addr_set,
         "ipv6 address X:X::X:X/<0-128> [eui-64]",
        IPV6_STR
        L3_INTF_IPV6_ADDR_DESC
        ARG_IPV6ADDRPFXLEN_DESC
        "Specifies to build an interface ID in the low order 64|bits of the IPv6address based on the interface MAC|address.\n"
    },
    {
         NODE_IF_VLAN | NODE_IF_LO,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_l3_intfIPv6Addr_set,
         "no ipv6 address [X:X::X:X/<0-128>]",
        NO_DESC
        IPV6_STR
        L3_INTF_IPV6_ADDR_DESC
        ARG_IPV6ADDRPFXLEN_DESC
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6Dhcp_set,
         "[no] ipv6 dhcp client",
        NO_DESC
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
    },
#ifdef CONFIG_SYS_APP_DHCP6C_STATELESS
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DhcpStateless_set,
         "[no] ipv6 dhcp client stateless",
        NO_DESC
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
        "Acquire DHCPv6 stateless configuration from DHCP server\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DhcpRefreshTime_set,
         "ipv6 dhcp client information refresh <86400-4294967294>",
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
        L3_INTF_DHCPV6_INFO_DESC
        L3_INTF_DHCPV6_REFRESH_DESC
        "Specify timeout in seconds\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DhcpRefreshTime_set,
         "no ipv6 dhcp client information refresh",
        NO_DESC
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
        L3_INTF_DHCPV6_INFO_DESC
        L3_INTF_DHCPV6_REFRESH_DESC
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DhcpMinRefreshTime_set,
         "ipv6 dhcp client information refresh minimum <600-4294967294>",
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
        L3_INTF_DHCPV6_INFO_DESC
        L3_INTF_DHCPV6_REFRESH_DESC
        L3_INTF_DHCPV6_REFRESH_MIN_DESC
        "Specify timeout in seconds\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DhcpMinRefreshTime_set,
         "no ipv6 dhcp client information refresh minimum",
        NO_DESC
        IPV6_STR
        L3_INTF_DHCPV6_DESC
        L3_INTF_DHCPV6_CLIENT_DESC
        L3_INTF_DHCPV6_INFO_DESC
        L3_INTF_DHCPV6_REFRESH_DESC
        L3_INTF_DHCPV6_REFRESH_MIN_DESC
    },
#endif
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DadAttempt_set,
         "ipv6 nd dad attempts <0-600>",
        IPV6_STR
        L3_INTF_ND_DESC
        L3_INTF_DAD_DESC
        L3_INTF_DAD_ATTEMPT_DESC
        "Number of attempts\n"
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DadAttempt_set,
         "no ipv6 nd dad attempts",
        NO_DESC
        IPV6_STR
        L3_INTF_ND_DESC
        L3_INTF_DAD_DESC
        L3_INTF_DAD_ATTEMPT_DESC
    },
    {
         NODE_IF_VLAN,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_intfIPv6DestUnreach_set,
         "[no] ipv6 destination-unreachable",
        NO_DESC
        IPV6_STR
        "ICMPv6 Destination Unreachable message send\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV6_set,
         "ipv6 route X:X::X:X/<0-128> X:X::X:X [metric <1-255>]",
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        L3_STATIC_ROUTE_METRIC_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV6Reject_set,
         "ipv6 route X:X::X:X/<0-128> [metric <1-255>] reject",
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        L3_STATIC_ROUTE_METRIC_DESC
        "Reject\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV6_set,
         "no ipv6 route X:X::X:X/<0-128> [X:X::X:X]",
        NO_DESC
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV6LinkLocal_set,
         "ipv6 route X:X::X:X/<0-128> X:X::X:X link-local interface vlan <1-4094> [metric <1-255>]",
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
        "Link Local\n"
        "interface\n"
        "VLAN\n"
        "VLAN ID\n"
        L3_STATIC_ROUTE_METRIC_DESC
        L3_STATIC_ROUTE_METRIC_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_staticRouteV6LinkLocal_set,
         "no ipv6 route X:X::X:X/<0-128> X:X::X:X link-local interface vlan <1-4094>",
        NO_DESC
        IP_STR
        L3_STATIC_ROUTE_DESC
        L3_IP_DST_PRFIX_LEN_DESC
        L3_STATIC_ROUTE_GW_DESC
        "Link Local\n"
        "interface\n"
        "VLAN\n"
        "VLAN ID\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_ageTime_set,
         "arp timeout <"SYS_STR_CONST(BOND_L3_NEIGH_AGE_TIME_MIN)"-"SYS_STR_CONST(BOND_L3_NEIGH_AGE_TIME_MAX)">",
        "Global ARP table configuration commands\n"
        "Set ARP cache timeout\n"
        "Seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_ageTime_set,
         "no arp timeout",
        NO_DESC
        "Global ARP table configuration commands\n"
        "Set ARP cache timeout\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_respTime_set,
         "arp response <"SYS_STR_CONST(BOND_L3_NEIGH_RESP_TIME_MIN)"-"SYS_STR_CONST(BOND_L3_NEIGH_RESP_TIME_MAX)">",
        "Global ARP table configuration commands\n"
        "Set ARP response time\n"
        "Seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_respTime_set,
         "no arp response",
        NO_DESC
        "Global ARP table configuration commands\n"
        "Set ARP response time\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_retryTimes_set,
         "arp retry <"SYS_STR_CONST(BOND_L3_NEIGH_RETRY_TIMES_MIN)"-"SYS_STR_CONST(BOND_L3_NEIGH_RETRY_TIMES_MAX)">",
        "Global ARP table configuration commands\n"
        "Set ARP retry times\n"
        "Seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_retryTimes_set,
         "no arp retry",
        NO_DESC
        "Global ARP table configuration commands\n"
        "Set ARP retry times\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_cacheSize_set,
         "arp cache <79-"SYS_STR_CONST(CAPA_L3_HOST_ROUTE_ENTRY_NUM)">",
        "Global ARP table configuration commands\n"
        "Set ARP cache size\n"
        "Seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_cacheSize_set,
         "no arp cache",
        NO_DESC
        "Global ARP table configuration commands\n"
        "Set ARP cache size\n"
    },
#if 0
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_renew_set,
         "arp renew",
        "Global ARP table configuration commands\n"
        "Set ARP renew\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neigh_renew_set,
         "no arp renew",
        NO_DESC
        "Global ARP table configuration commands\n"
        "Set ARP renew\n"
    },
#endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neighStatic_set,
         "arp A.B.C.D A:B:C:D:E:F [vlan <1-4094>]",
        "Global ARP table configuration commands\n"
        "IP address of ARP entry\n"
        "MAC address of ARP entry\n"
        "IEEE 802.1Q Vlans\n"
        "Vlan interface number\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neighStatic_set,
         "no arp A.B.C.D [vlan <1-4094>]",
        NO_DESC
        "Global ARP table configuration commands\n"
        "IP address of ARP entry\n"
        "IEEE 802.1Q Vlans\n"
        "Vlan interface number\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neighStatic_set,
         "ipv6 neighbor X:X::X:X vlan <1-4094> A:B:C:D:E:F",
        "IPV6 configuration\n"
        "Neighbor\n"
        "Neighbor ipv6 address\n"
        "IEEE 802.1Q Vlans\n"
        "Vlan interface number\n"
        "MAC address of ARP entry\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_neighStatic_set,
         "no ipv6 neighbor [X:X::X:X] [vlan <1-4094>]",
        NO_DESC
        "IPV6 configuration\n"
        "Neighbor\n"
        "Neighbor ipv6 address\n"
        "IEEE 802.1Q Vlans\n"
        "Vlan interface number\n"
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_router_rip_set,
         "[no] rip",
        NO_DESC
        "enable rip\n"
    },
    {
         NODE_RIP,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_rip_network_set,
         "[no] network A.B.C.D/M",
        NO_DESC
		"Enable routing on an IP network\n"
		"IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
    },
	{
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_version,
	     "version (1|2)",
	    "Rip version\n"
	    "Version 1\n"
	    "Version 2\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_version,
	     "no version",
	    NO_DESC
	    "Rip version\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_route_config,
	     "route A.B.C.D/M",
	    "Rip route\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_route_config,
	     "no route A.B.C.D/M",
	    NO_DESC
	    "Rip route\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distance_config,
	     "distance <1-255>",
	    "Rip distance\n"
	    "Distance <1-255>\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distance_config,
	     "no distance",
	    NO_DESC
	    "Rip distance\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distance_source_config,
	     "distance <1-255> A.B.C.D/M",
	    "Rip distance\n"
	    "Distance <1-255>\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distance_source_config,
	     "no distance <1-255> A.B.C.D/M",
	    NO_DESC
	    "Rip distance\n"
	    "Distance <1-255>\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distribute_list_config,
	     "distribute-list WORD<1-32> (in|out) interface vlan <1-4094>",
	    "Rip distribute list name\n"
	    "WORD<1-32>\n"
	    "Type\n"
	    "Type\n"
	    "Interface\n"
	    "IEEE 802.1Q Vlans\n"
	    "Vlan interface number\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_distribute_list_config,
	     "no distribute-list WORD<1-32> (in|out) interface vlan <1-4094>",
	    NO_DESC
	    "Rip distribute list name\n"
	    "WORD<1-32>\n"
	    "Type\n"
	    "Type\n"
	    "Interface\n"
	    "IEEE 802.1Q Vlans\n"
	    "Vlan interface number\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_access_list_config,
	     "access-list WORD<1-32> (deny|permit) A.B.C.D/M [exact-match]",
	    "Rip access list\n"
	    "Rip access list name\n"
	    "Action type\n"
	    "Action type\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
	    "Exact match\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_access_list_config,
	     "no access-list WORD<1-32> (deny|permit) A.B.C.D/M [exact-match]",
	    NO_DESC
	    "Rip access list\n"
	    "Rip access list name\n"
	    "Action type\n"
	    "Action type\n"
	    "IP prefix <network>/<length>, e.g., 10.0.0.0/8\n"
	    "Exact match\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_access_list_config,
	     "access-list WORD<1-32> (deny|permit) any",
	    "Rip access list\n"
	    "Rip access list name\n"
	    "Action type\n"
	    "Action type\n"
	    "Any 0.0.0.0/0\n"
    },
    {
	     NODE_RIP,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_access_list_config,
	     "no access-list WORD<1-32> (deny|permit) any",
	    NO_DESC
	    "Rip access list\n"
	    "Rip access list name\n"
	    "Action type\n"
	    "Action type\n"
	    "Any 0.0.0.0/0\n"
    },
    {
	     NODE_CONFIG,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_log_to_fileName,
	     "log (rip|ospf) file",
	    "Logging control\n"
	    "Mode type\n"
	    "Mode type\n"
	    "Logging to file\n"
    },
    {
	     NODE_CONFIG,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_log_to_fileName,
	     "no log (rip|ospf) file",
	    "Logging control\n"
	    "Mode type\n"
	    "Mode type\n"
	    "Logging to file\n"
    },
    {
	     NODE_CONFIG,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_log_to_stdout,
	     "log (rip|ospf) console",
	    "Logging control\n"
	    "Mode type\n"
	    "Mode type\n"
	    "Logging to console\n"
    },
    {
	     NODE_CONFIG,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_rip_log_to_stdout,
	     "no log (rip|ospf) console",
	    NO_DESC
	    "Logging control\n"
	    "Mode type\n"
	    "Mode type\n"
	    "Logging to console\n"
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_router_ospf_set,
		 "[no] ospf [1]",
		NO_DESC
		"Set the OSPF area ID\n"
		"Process ID\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_set,
		 "[no] area (A.B.C.D|<0-4294967295>)",
		NO_DESC
		"Set the OSPF area ID\n"
		"OSPF area ID in IP address format\n"
		"OSPF area ID as a decimal value\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_network_area_set,
		 "[no] network A.B.C.D/M",
		NO_DESC
		"Enable routing on an IP network\n"
		"OSPF network prefix\n"
		"Set the OSPF area ID\n"
	},
	#if 0
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_set,
		 "range A.B.C.D/M [(advertise | not-advertise)] [cost <0-16777215>]",
		"Summarize routes matching address/mask (border routers only)\n"
	    "Area range prefix\n"
	    "Advertise this range (default)\n"
		"DoNotAdvertise this range\n"
		"User specified metric for this range\n"
		"Advertised metric for this range\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_set,
		 "no range A.B.C.D/M",
		NO_DESC
		"Summarize routes matching address/mask (border routers only)\n"
		"Area range prefix\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_substitute_set,
		 "range A.B.C.D/M substitute A.B.C.D/M",
		"Summarize routes matching address/mask (border routers only)\n"
		"Area range prefix\n"
		"Announce area range as another prefix\n"
		"Network prefix to be announced instead of range\n"

	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_substitute_set,
		 "no range A.B.C.D/M substitute A.B.C.D/M",
		NO_DESC
		"Summarize routes matching address/mask (border routers only)\n"
		"Area range prefix\n"
		"Announce area range as another prefix\n"
        "Network prefix to be announced instead of range\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_vlink_set,
		 "virtual-link A.B.C.D [hello-interval <1-65535>] [retrans-interval <1-65535>] [transmit-delay <1-65535>] [dead-interval <1-65535>]",
		"Configure a virtual link\n"
		"Router ID of the remote ABR\n"
		"Time between HELLO packets\n"
		"Seconds\n"
		"Time between retransmitting lost link state advertisements\n"
		"Seconds\n"
		"Link state transmit delay\n"
		"Seconds\n"
		"Interval after which a neighbor is declared dead\n"
		"Seconds\n"
	},	
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_vlink_set,
		 "no virtual-link A.B.C.D",
		NO_DESC
		"Configure a virtual link\n"
		"Router ID of the remote ABR\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_vlink_set,
		 "virtual-link A.B.C.D authentication-key WORD",
		"Configure a virtual link\n"
		"Router ID of the remote ABR\n"
		"Enable authentication on this virtual link\n"
		"dummy string \n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_vlink_set,
		 "virtual-link A.B.C.D message-digest-key <1-255> md5 WORD",
		"Configure a virtual link\n"
		"Router ID of the remote ABR\n"
		"message-digest-key id of interface \n"
		"Key ID\n"
		"Use MD5 algorithm\n"
		"The OSPF password (key)\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_substitute_set,
		 "[no] range A.B.C.D/M substitute A.B.C.D/M",
		"Summarize routes matching address/mask (border routers only)\n"
		"Area range prefix\n"
		"Announce area range as another prefix\n"
		"Network prefix to be announced instead of range\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_range_substitute_set,
		 "no range A.B.C.D/M substitute A.B.C.D/M",
		NO_DESC
		"Summarize routes matching address/mask (border routers only)\n"
		"Area range prefix\n"
		"Announce area range as another prefix\n"
		"Network prefix to be announced instead of range\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_stub_set,
		 "[no] stub [no-summary]",
		NO_DESC
		"Configure OSPF area as stub\n"
		"Do not inject inter-area routes into area\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_nssa_set,
		 "nssa [(translate-candidate|translate-never|translate-always)] [no-summary]",
		"Configure OSPF area as nssa\n"
        "Configure NSSA-ABR for translate election (default)\n"
        "Configure NSSA-ABR to never translate\n"
        "Configure NSSA-ABR to always translate\n"
        "Do not inject inter-area routes into nssa\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_nssa_set,
		 "no nssa [no-summary]",
		NO_DESC
		"Configure OSPF area as nssa\n"
		"Configure NSSA-ABR for translate election (default)\n"
		"Configure NSSA-ABR to never translate\n"
		"Configure NSSA-ABR to always translate\n"
		"Do not inject inter-area routes into nssa\n"
	},
	#endif
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_default_cost_set,
		 "default-cost <0-16777215>",
		"Set the summary-default cost of a NSSA or stub area\n"
		"Stub's advertised default summary cost\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_default_cost_set,
		 "no default-cost",
		NO_DESC
		"Set the summary-default cost of a NSSA or stub area\n"
	},
	#if 0
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_export_list_set,
		 "[no] export-list WORD<1-32>",
		NO_DESC
		"Unset the filter for networks announced to other areas\n"
		"Name of the access-list\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_import_list_set,
		 "[no] import-list WORD<1-32>",
		NO_DESC
		"Set the filter for networks from other areas announced to the specified one\n"
		"Name of the access-list\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_filter_list_set,
		 "[no] filter-list prefix WORD<1-32> (in|out)",
		NO_DESC
		"Filter networks between OSPF areas\n"
        "Filter prefixes between OSPF areas\n"
        "Name of an IP prefix-list\n"
        "Filter networks sent to this area\n"
        "Filter networks sent from this area\n"
	},
	#endif
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_authentication_set,
		 "authentication (simple | message-digest)",
		"Enable authentication\n"
		"Simple authentication mode\n"
		"Use HMAC-MD5 algorithm\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_authentication_set,
		 "no authentication",
		NO_DESC
		"Enable authentication\n"
	},
	#if 0
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_shortcut_set,
		 "shortcut (default|enable|disable)",
		"Configure the area's shortcutting mode\n"
        "Set default shortcutting behavior\n"
        "Enable shortcutting through the area\n"
        "Disable shortcutting through the area\n"
	},
	{
		 NODE_AREA,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_area_shortcut_set,
		 "no shortcut (enable|disable)",
		NO_DESC
		"Deconfigure the area's shortcutting mode\n"
        "Deconfigure enabled shortcutting through the area\n"
        "Deconfigure disabled shortcutting through the area\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_abr_type_set,
		  "ospf abr-type (cisco|ibm|shortcut|standard)",
       "OSPF specific commands\n"
       "Set OSPF ABR type\n"
       "Alternative ABR, cisco implementation\n"
       "Alternative ABR, IBM implementation\n"
       "Shortcut ABR\n"
       "Standard behavior (RFC2328)\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_abr_type_set,
		 "no ospf abr-type",
       NO_DESC
       "OSPF specific commands\n"
       "Set OSPF ABR type\n"

	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_log_adjacency_set,
		  "log-adjacency-changes [detail]",
		"Log changes in adjacency state\n"
		"Log all state changes\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_log_adjacency_set,
		 "no log-adjacency-changes",
       NO_DESC
	   "Log changes in adjacency state\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_rfc1583compatibility_set,
		  "[no] compatible rfc1583",
		NO_DESC
       "OSPF compatibility list\n"
       "compatible with RFC 1583\n"
	},
	#endif
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_spf_timer_set,
		 "timers throttle spf <0-600000> <0-600000> <0-600000>",
       "Adjust routing timers\n"
       "Throttling adaptive timer\n"
       "OSPF SPF timers\n"
       "Delay (msec) from first change received till SPF calculation\n"
       "Initial hold time (msec) between consecutive SPF calculations\n"
       "Maximum hold time (msec)\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_spf_timer_set,
		 "no timers throttle spf",
		NO_DESC
       "Adjust routing timers\n"
       "Throttling adaptive timer\n"
       "OSPF SPF timers\n"
	},
	#if 0
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_static_neighbor_set,
		 "neighbor A.B.C.D [priority <0-255>] [poll-interval <1-65535>]",
        "Specify neighbor router\n"
        "Neighbor IP address\n"
        "Neighbor Priority\n"
        "Priority\n"
        "Dead Neighbor Polling interval\n"
        "Seconds\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_static_neighbor_set,
		 "no neighbor A.B.C.D",
		NO_DESC
        "Specify neighbor router\n"
        "Neighbor IP address\n"
	},
	#endif
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_refresh_timer_set,
		 "refresh timer <10-1800>",
       "Adjust refresh parameters\n"
       "Set refresh timer\n"
       "Timer value in seconds\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_refresh_timer_set,
		 "no refresh timer",
		NO_DESC
       "Adjust refresh parameters\n"
       "Unset refresh timer\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_auto_cost_set,
		 "auto-cost reference-bandwidth <1-4294967>",
       "Calculate OSPF interface cost according to bandwidth\n"
       "Use reference bandwidth method to assign OSPF cost\n"
       "The reference bandwidth in terms of Mbits per second\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_auto_cost_set,
		 "no auto-cost reference-bandwidth",
		NO_DESC
       "Calculate OSPF interface cost according to bandwidth\n"
       "Use reference bandwidth method to assign OSPF cost\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_router_id_set,
		 "router-id A.B.C.D",
        "router-id for the OSPF process\n"
        "OSPF router-id in IP address format\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_router_id_set,
		 "no router-id",
		NO_DESC
       "router-id for the OSPF process\n"
	},
	#if 0
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_redistribute_set,
		 "redistribute (connected|static|rip|bgp) [metric <0-16777214>] [metric-type (1|2)] [route-map WORD<1-32>]",
		"Redistribute information from another routing protocol\n"
		"Connected routes (directly attached subnet or host)\n"
		"Statically configured routes\n"
		"Routing Information Protocol (RIP)\n"
		"Border Gateway Protocol (BGP)\n"
		"Metric for redistributed routes\n"
		"OSPF default metric\n"
		"OSPF exterior metric type for redistributed routes\n"
		"Set OSPF External Type 1 metrics\n"
		"Set OSPF External Type 2 metrics\n"
		"Route map reference\n"
		"Pointer to route-map entries\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_redistribute_set,
		 "no redistribute (connected|static|rip|bgp) [metric] [metric-type] [route-map]",
		NO_DESC
		"Redistribute information from another routing protocol\n"
		"Connected routes (directly attached subnet or host)\n"
		"Statically configured routes\n"
		"Routing Information Protocol (RIP)\n"
		"Border Gateway Protocol (BGP)\n"
		"Metric for redistributed routes\n"
		"OSPF exterior metric type for redistributed routes\n"
		"Route map reference\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_dft_info_set,
		 "default-information originate [always] [metric <0-16777214>] [metric-type (1|2)] [route-map WORD<1-32>]",
		"Control distribution of default information\n"
		"Distribute a default route\n"
		"Always advertise default route\n"
		"OSPF default metric\n"
		"OSPF metric\n"
		"OSPF metric type for default routes\n"
		"Set OSPF External Type 1 metrics\n"
		"Set OSPF External Type 2 metrics\n"
		"Route map reference\n"
		"Pointer to route-map entries\n"

	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_dft_info_set,
		 "no default-information originate [always] [metric] [metric-type] [route-map]",
		NO_DESC
		"Control distribution of default information\n"
		"Distribute a default route\n"
		"Always advertise default route\n"
		"OSPF default metric\n"
		"OSPF metric type for default routes\n"
		"Route map reference\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distri_list_out_set,
		 "[no] distribute-list WORD<1-32> out (connected|static|rip|bgp)",
		NO_DESC
		"Filter networks in routing updates\n"
		"Access-list name\n"
		"Filter outgoing routing updates\n"
		"Connected routes (directly attached subnet or host)\n"
		"Statically configured routes\n"
		"Routing Information Protocol (RIP)\n"
		"Border Gateway Protocol (BGP)\n"
	},
	#endif
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_default_metric_set,
		 "default-metric <0-16777214>",
		"Set metric of redistributed routes\n"
		"Default metric\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_default_metric_set,
		 "no default-metric",
		NO_DESC
		"Set metric of redistributed routes\n"
	},
	#if 0
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distance_ospf_set,
		 "distance ospf [intra-area <1-255>] [inter-area <1-255>] [external <1-255>]",
		"Define an administrative distance\n"
        "OSPF Administrative distance\n"
        "Intra-area routes\n"
        "Distance for intra-area routes\n"
        "Inter-area routes\n"
        "Distance for inter-area routes\n"
        "External routes\n"
        "Distance for external routes\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distance_ospf_set,
		 "no distance ospf [intra-area] [inter-area] [external]",
		NO_DESC
		"Define an administrative distance\n"
        "OSPF Administrative distance\n"
        "Intra-area routes\n"
        "Inter-area routes\n"
        "External routes\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distance_prefix_set,
		 "[no] distance <1-255> A.B.C.D/M [WORD<1-32>]",
		NO_DESC
		"Administrative distance\n"
        "Distance value\n"
        "IP source prefix\n"
        "Access list name\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distance_set,
		 "distance <1-255>",
		"Define an administrative distance\n"
        "OSPF Administrative distance\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_distance_set,
		 "no distance",
		NO_DESC
		"Define an administrative distance\n"
	},
	#endif
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_passive_interface_set,
		 "[no] passive-interface vlan-interface <1-4094> [A.B.C.D]",
		NO_DESC
		"Suppress routing updates on an interface\n"
		"Vlan interface\n"
		"Vlan interface number\n"
		"Interface's address\n"
	},
	{
		 NODE_OSPF,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_l3_ospf_passive_interface_set,
		 "[no] passive-interface default",
		NO_DESC
		"Suppress routing updates on an interface\n"
		"Suppress routing updates on interfaces by default\n"
	},
	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_mode_set,
		 "ospf authentication (null | simple | message-digest)",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Enable authentication on this interface\n"
		"Simple authentication mode\n"
		"Use HMAC-MD5 algorithm\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_mode_set,
		 "no ospf authentication",// [A.B.C.D]",
		NO_DESC
		"IP Information\n"
		"OSPF interface commands\n"
		"Enable authentication on this interface\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_simple_set,
		 "ospf authentication-key WORD<1-8>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Simple authentication password (key)\n"
		"The OSPF password (key)\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_simple_set,
		 "no ospf authentication-key",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Simple authentication password (key)\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_md5_set,
		 "ospf message-digest-key <1-255> md5 WORD<1-16>",// [A.B.C.D]",
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n"
       "Use MD5 algorithm\n"
       "The OSPF password (key)\n"
       
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_authentication_md5_set,
		 "no ospf message-digest-key <1-255>",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Message digest authentication password (key)\n"
		"Key ID\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_cost_set,
		 "ospf cost <1-65535>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Interface cost\n"
		"Cost\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_cost_set,
		 "no ospf cost",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Interface cost\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_priority_set,
		 "ospf priority <0-255>",// [A.B.C.D]",
       "OSPF interface commands\n"
       "Router priority\n"
       "Priority\n"
       
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_priority_set,
		 "no ospf priority",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Router priority\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_hello_set,
		 "ospf hello-interval <1-65535>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Time between HELLO packets\n"
		"Seconds\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_hello_set,
		 "no ospf hello-interval",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Time between HELLO packets\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_dead_set,
		 "ospf dead-interval <1-65535>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Interval after which a neighbor is declared dead\n"
		"Seconds\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_dead_set,
		 "ospf dead-interval minimal hello-multiplier <1-10>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Interval after which a neighbor is declared dead\n"
		"Minimal 1s dead-interval with fast sub-second hellos\n"
		"Hello multiplier factor\n"
		"Number of Hellos to send each second\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_dead_set,
		 "no ospf dead-interval",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Interval after which a neighbor is declared dead\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_transmit_set,
		 "ospf transmit-delay <1-65535>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Link state transmit delay\n"
		"Seconds\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_transmit_set,
		 "no ospf transmit-delay",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Link state transmit delay\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_retransmit_set,
		 "ospf retransmit-interval <3-65535>",// [A.B.C.D]",
		"OSPF interface commands\n"
		"Time between retransmitting lost link state advertisements\n"
		"Seconds\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_retransmit_set,
		 "no ospf retransmit-interval",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Time between retransmitting lost link state advertisements\n"
		
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_network_set,
		 "ospf network (broadcast|non-broadcast|point-to-multipoint|point-to-point)",
		"OSPF interface commands\n"
		"Network type\n"
		"Specify OSPF broadcast multi-access network\n"
		"Specify OSPF NBMA network\n"
		"Specify OSPF point-to-multipoint network\n"
		"Specify OSPF point-to-point network\n"
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_network_set,
		 "no ospf network",
		NO_DESC
		"OSPF interface commands\n"
		"Network type\n"
	},

	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_ip_ospf_mtu_ignore_set,
		 "[no] ospf mtu-ignore",// [A.B.C.D]",
		NO_DESC
		"OSPF interface commands\n"
		"Disable mtu mismatch detection\n"
		
	},

#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
		{
			 NODE_CONFIG,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_l3_router_bgp_set,
			 "[no] bgp <1-4294967295>",
			NO_DESC
			"enable bgp\n"
			"AS number\n"
		},
		{
			 NODE_BGP,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_l3_bgp_network_area_set,
			 "[no] network A.B.C.D/M area (A.B.C.D|<0-4294967295>)",
			NO_DESC
			"Enable routing on an IP network\n"
			"OSPF network prefix\n"
			"Set the OSPF area ID\n"
			"OSPF area ID in IP address format\n"
			"OSPF area ID as a decimal value\n"
		},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_ZEBRA
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_router_id_config,
         "[no] router-id A.B.C.D",
        NO_DESC
        "Manually set the router-id\n"
        "IP address to use for router-id\n"
    },
#endif
#ifdef CONFIG_SYS_L3_VRRP
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_vip_config,
	     "[no] vrrp vrid <1-5> virtual-ip A.B.C.D",
	    NO_DESC
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "virtual ip address\n"
	    "IP Address format is A.B.C.D where (A/B/C/D = 0 ~ 255)\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_priority_config,
	     "vrrp vrid <1-5> priority <1-254>",
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "vrrp priority\n"
	    "priority (1-254)\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_priority_config,
	     "no vrrp vrid <1-5> priority",
	    NO_DESC
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "vrrp priority\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_preempt_mode_config,
	     "vrrp vrid <1-5> preempt-mode [timer delay <0-255>]",
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "preempt delay\n"
	    "delay (0-255)\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_preempt_mode_config,
	     "no vrrp vrid <1-5> preempt-mode",
	    NO_DESC
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "preempt delay\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_advertise_timer_config,
	     "vrrp vrid <1-5> timer advertise <1-255>",
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
	    "advertise timer\n"
	    "timer (1-255)\n"
    },
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_advertise_timer_config,
	     "no vrrp vrid <1-5> timer advertise",
	    NO_DESC
	    "config vrrp\n"
	    "virtual router id\n"
	    "vrid (1-5)\n"
		"advertise timer\n"
    },
	{
		 NODE_IF_VLAN,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_vrrp_track_config,
		 "vrrp vrid <1-5> track vlan-interface <1-4094> [reduced <1-254>]",
		"config vrrp\n"
		"virtual router id\n"
		"vrid (1-5)\n"
		"config vrrp track interface\n"
		"vlan interface\n"
		"vlan interface number\n"
		"reduced priority\n"
		"priority\n"
	},
    {
	     NODE_IF_VLAN,
	     ATTR_PRIV_15,
	     NULL,
	     cmd_vrrp_track_config,
	     "no vrrp vrid <1-5> track  vlan-interface <1-4094>",
	    NO_DESC
		"config vrrp\n"
		"virtual router id\n"
		"vrid (1-5)\n"
		"config vrrp track interface\n"
		"vlan interface\n"
		"vlan interface number\n"
    },
#endif 
#ifdef CONFIG_SYS_PROTO_LLDP
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_enable_set,
         "[no] lldp",
        NO_DESC
        LLDP_GLOBAL_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_transmit_set,
         "[no] lldp tx",
        NO_DESC
        LLDP_ITF_DESC
        LLDP_TRANSMIT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_receive_set,
         "[no] lldp rx",
        NO_DESC
        LLDP_ITF_DESC
        LLDP_RECEIVE_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_holdtime,
         "lldp holdtime-multiplier <"SYS_STR_CONST(BOND_LLDP_HOLDTIME_MIN)"-"SYS_STR_CONST(BOND_LLDP_HOLDTIME_MAX)">",
        LLDP_GLOBAL_DESC
        LLDP_HOLDTIME_DESC
        LLDP_HOLDTIME_VAL_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_holdtime,
         "no lldp holdtime-multiplier",
        NO_DESC
        LLDP_GLOBAL_DESC
        LLDP_HOLDTIME_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_tx_interval,
         "lldp tx-interval <"SYS_STR_CONST(BOND_LLDP_TX_INTERVAL_MIN)"-"SYS_STR_CONST(BOND_LLDP_TX_INTERVAL_MAX)">",
        LLDP_GLOBAL_DESC
        LLDP_TXINTERVAL_DESC
        LLDP_TXINTERVAL_VAL_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_tx_interval,
         "no lldp tx-interval",
        NO_DESC
        LLDP_GLOBAL_DESC
        LLDP_TXINTERVAL_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_reinit_delay,
         "lldp reinit-delay <"SYS_STR_CONST(BOND_LLDP_REINIT_DELAY_MIN)"-"SYS_STR_CONST(BOND_LLDP_REINIT_DELAY_MAX)">",
        LLDP_GLOBAL_DESC
        LLDP_REINIT_STR
        LLDP_REINIT_VAL_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_reinit_delay,
         "no lldp reinit-delay",
        NO_DESC
        LLDP_GLOBAL_DESC
        LLDP_REINIT_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_tx_delay,
         "lldp tx-delay <"SYS_STR_CONST(BOND_LLDP_TX_DELAY_MIN)"-"SYS_STR_CONST(BOND_LLDP_TX_DELAY_MAX)">",
        LLDP_GLOBAL_DESC
        "Delay between successive LLDP frame transmission\n"
        "LLDP Tx-delay time in seconds\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_tx_delay,
         "no lldp tx-delay",
        NO_DESC
        LLDP_GLOBAL_DESC
        "Delay between successive LLDP frame transmission\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_optional_tlv_pvid_set,
         "lldp tlv-select pvid (enable|disable)",
        LLDP_DESC
        "Selection of LLDP TLVs to send\n"
        LLDP_TLV_8021_PVID_DESC
        "Enable Tx optional-TLV 802.1 PVID\nDisable Tx optional-TLV 802.1 PVID\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_optional_tlv_pvid_set,
         "no lldp tlv-select pvid",
        NO_DESC
        LLDP_DESC
        "Selection of LLDP TLVs to send\n"
        LLDP_TLV_8021_PVID_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_optional_tlv_vlan_name_set,
         "lldp tlv-select vlan-name (add|remove) VLAN-LIST",
        LLDP_DESC
        "Selection of LLDP TLVs to send\n"
        LLDP_TLV_8021_VLAN_DESC
        "Add configuration to a port\nRemove configuration from a port\n"
        "VLAN List (e.g. 3,6-8): The range of VLAN ID is 0 to 4095\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_optional_tlv_set,
         "lldp tlv-select TLV [TLV] [TLV] [TLV] [TLV] [TLV] [TLV] [TLV]",
        LLDP_ITF_DESC
        "Selection of LLDP TLVs to send\n"
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
        LLDP_TLV_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_optional_tlv_set,
         "no lldp tlv-select",
        NO_STR
        LLDP_ITF_DESC
        "Remove all selected LLDP optional TLVs\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_lldpdu_set,
         "lldp lldpdu (filtering|bridging|flooding)",
        LLDP_GLOBAL_DESC
        "Configure the action on LLDPDU upon disabled LLDP\n"
        "Drop LLDP PDU\nBridging LLDP PDU to VLAN member ports\nFlooding LLDP PDU to all ports (VLAN unawared)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_lldpdu_set,
         "no lldp lldpdu",
        NO_DESC
        LLDP_GLOBAL_DESC
        "Configure the action on LLDPDU upon disabled LLDP\n"
    },
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_enable_set,
         "[no] lldp med",
        NO_DESC
        LLDP_ITF_DESC
        LLDP_MED_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_optional_tlv_set,
         "lldp med tlv-select MEDTLV [MEDTLV] [MEDTLV] [MEDTLV]",
        LLDP_ITF_DESC
        LLDP_MED_DESC
        "Configure LLDP MED optional TLVs selection\n"
        LLDP_MED_TLV_DESC
        LLDP_MED_TLV_DESC
        LLDP_MED_TLV_DESC
        LLDP_MED_TLV_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_optional_tlv_set,
         "no lldp med tlv-select",
        NO_STR
        LLDP_ITF_DESC
        LLDP_MED_DESC
        "Remove all selected LLDP MED optional TLVs\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_fast_start_repeat_count_set,
         "lldp med fast-start-repeat-count <"SYS_STR_CONST(BOND_LLDP_MED_FAST_REPEAT_COUNT_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_FAST_REPEAT_COUNT_MAX)">",
        LLDP_GLOBAL_DESC
        LLDP_MED_DESC
        "Configure LLDP MED fast start repeat count\n"
        "Fast start repeat count, range is 1-10\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_fast_start_repeat_count_set,
         "no lldp med fast-start-repeat-count",
        NO_DESC
        LLDP_GLOBAL_DESC
        LLDP_MED_DESC
        "Configure LLDP MED fast start repeat count\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_network_policy_set,
         "lldp med network-policy <"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_MAX)"> app (voice|voice-signaling|guest-voice|guest-voice-signaling|softphone-voice|video-conferencing|streaming-video|video-signaling) vlan <"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_VID_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_VID_MAX)"> vlan-type (tag|untag) priority <"SYS_STR_CONST(BOND_LLDP_MED_USER_PRI_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_USER_PRI_MAX)"> dscp <"SYS_STR_CONST(BOND_LLDP_MED_DSCP_PRI_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_DSCP_PRI_MAX)">",
        LLDP_GLOBAL_DESC
        LLDP_MED_DESC
        LLDP_MED_NETWORK_POLICY_DESC
        "Network policy index\n"
        "Configure network policy application type\n"
        "Voice\nVoice-signaling\nGuest-voice\nGuest-voice-signaling\nSoftphone-voice\nVideo-conferencing\nStreaming-video\nVideo-signaling\n"
        "Vlan configuration\n"
        "Vlan ID\n"
        "Vlan type configuration\n"
        "Tagged\nUntagged\n"
        "Configure CoS priority\n"
        "CoS priority\n"
        "Configure DSCP priority\n"
        "DSCP priority\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_network_policy_set,
         "no lldp med network-policy <"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_MIN)"-"SYS_STR_CONST(BOND_LLDP_MED_NETWORK_POLICY_MAX)">",
        NO_DESC
        LLDP_GLOBAL_DESC
        LLDP_MED_DESC
        LLDP_MED_NETWORK_POLICY_DESC
        "Network policy index\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_network_policy_add_remove_set,
         "lldp med network-policy (add|remove) IDX_LIST",
        LLDP_ITF_DESC
        LLDP_MED_DESC
        LLDP_MED_NETWORK_POLICY_DESC
        "Add network policy to port binding\nRemove network policy to port binding\n"
        "Network policy index list, index range is 1-32, ex: 1,30-32\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_location_set,
         "lldp med location (coordinate|civic-address|ecs-elin) ADDR",
        LLDP_ITF_DESC
        LLDP_MED_DESC
        "Configure LLDP MED location information for an interface\n"
        "The location is specified as coordinates. Range: "SYS_STR_CONST(CAPA_LLDP_MED_COORDINATE_LEN)" |hexadecimal bytes exactly.\n"
        "The location is specified as civic address. Range: "SYS_STR_CONST(BOND_LLDP_MED_CIVIC_LEN_MIN )" |to "SYS_STR_CONST(BOND_LLDP_MED_CIVIC_LEN_MAX )" hexadecimal bytes.\n"
        "The location is specified as ECS ELIN. Range: "SYS_STR_CONST(BOND_LLDP_MED_ELIN_LEN_MIN)" to "SYS_STR_CONST(BOND_LLDP_MED_ELIN_LEN_MAX)" |hexadecimal bytes.\n"
        "The data format specifies the location as hexadecimal data:|Each byte in hexadecimal character strings is two hexadecimal digits.\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_med_location_set,
         "no lldp med location (coordinate|civic-address|ecs-elin)",
        NO_DESC
        LLDP_ITF_DESC
        LLDP_MED_DESC
        "Configure LLDP MED location information for an interface\n"
        "The location is specified as coordinates. Range: "SYS_STR_CONST(CAPA_LLDP_MED_COORDINATE_LEN)" |hexadecimal bytes exactly.\n"
        "The location is specified as civic address. Range: "SYS_STR_CONST(BOND_LLDP_MED_CIVIC_LEN_MIN )" |to "SYS_STR_CONST(BOND_LLDP_MED_CIVIC_LEN_MAX )" hexadecimal bytes.\n"
        "The location is specified as ECS ELIN. Range: "SYS_STR_CONST(BOND_LLDP_MED_ELIN_LEN_MIN)" to "SYS_STR_CONST(BOND_LLDP_MED_ELIN_LEN_MAX)" |hexadecimal bytes.\n"
    },
  #endif
#endif 

#ifdef CONFIG_SYS_MIRROR
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_group_sport_set,
         "mirror session <$DEF1-$DEF2> source interfaces IF_PORTS (both|rx|tx)",
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC
        "Mirror Source configuration\n"
        IFS_DESC
        ARG_IF_PORT
        "Both\nRX only\nTX only\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_group_dport_set,
         "mirror session <$DEF1-$DEF2> destination interface IF_NMLPORT [allow-ingress]",
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC
        "Mirror destination configuration\n"
        IFS_DESC
        ARG_IF_NMLPORT
        "ingress configuration\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_group_del,
         "no mirror session (<$DEF1-$DEF2>|all)",
        NO_STR
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC "all Session ID\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_group_dport_set,
         "no mirror session <$DEF1-$DEF2> destination interface IF_NMLPORT",
        NO_STR
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC
        "Mirror destination configuration\n"
        IFS_DESC
        ARG_IF_NMLPORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mirror_group_sport_set,
         "no mirror session <$DEF1-$DEF2> source interfaces IF_PORTS (both|rx|tx)",
        NO_STR
        MIRROR_DESC
        SESSION_DESC
        ARG_SESSION_DESC
        "Mirror Source configuration\n"
        IFS_DESC
        ARG_IF_PORT
        "Both\nRX only\nTX only\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_enable_set,
         "[no] ipv6 mld snooping",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_suppre_enable_set,
         "[no] ipv6 mld snooping report-suppression",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "MLD v1 report suppression\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_unknMcastAction_set,
         "ipv6 mld snooping unknown-multicast action (drop|flood|router-port)",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "Unknown multicast\n"
        "Action on receiving unknown multicast packets\n"
        "Drop the packets\nFlood the packets\nForward to router ports\n"

    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_unknMcastAction_set,
          "no ipv6 mld snooping  unknown-multicast action",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "Unknown multicast\n"
        "Default action(flood) on receiving unknown multicast packets\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_lookupMode_set,
         "ipv6 mld snooping forward-method (mac|dip)",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "Forward method\n"
        "MAC method\n"
        "DIP method\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_operVersion_set,
         "ipv6 mld snooping version (1|2)",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        "MLD Snooping Operation Version\n"
        "MLD Operation Version is v1\nMLD Operation Version is v2\n"
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_routerPort_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST static-router-port IF_PORTS",
        NO_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Static mrouter port configuration\n"
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_routerPortForbid_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST forbidden-router-port IF_PORTS",
        NO_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Forbidden mrouter port configuration\n"
        ARG_IF_PORT
    },
    #endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_static_group_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST static-group X:X::X:X interfaces IF_PORTS",
        NO_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        "Static group configuration \n"
        "IPv6 multicast address\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_static_group_clear,
         "no ipv6 mld snooping vlan VLAN-LIST group X:X::X:X",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLAN_DESC
        "Group address\n"
        "Multicast IPv6 address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_fastleave_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST immediate-leave",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        MLD_IMMEDLEAVE_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_mld_fastleave_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST fastleave",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Fast leave function \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_pimDvmrpLearn_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST router learn pim-dvmrp",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "MLD snooping router\n"
        "MLD snooping router port learn\n"
        "MLD snooping router port learn by pim,dvmrp and MLD messages\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_robustnessVar_set,
         "ipv6 mld snooping vlan VLAN-LIST robustness-variable <"SYS_STR_CONST(BOND_MCAST_ROBUST_MIN)"-"SYS_STR_CONST(BOND_MCAST_ROBUST_MAX)">",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Robustness Variable\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_ROBUST_MIN)"-"SYS_STR_CONST(BOND_MCAST_ROBUST_MAX)". Default is "SYS_STR_CONST(DFLT_MLD_ROBUSTNESS_VAR)".\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_robustnessVar_set,
         "no ipv6 mld snooping vlan VLAN-LIST robustness-variable",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Robustness Variable is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_responseTime_set,
         "ipv6 mld snooping vlan VLAN-LIST response-time <"SYS_STR_CONST(BOND_MCAST_RESPONSE_MIN)"-"SYS_STR_CONST(BOND_MCAST_RESPONSE_MAX)">",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Response time\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_RESPONSE_MIN)"-"SYS_STR_CONST(BOND_MCAST_RESPONSE_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_MLD_QUERY_RESPONSE_INTV)" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_responseTime_set,
         "no ipv6 mld snooping vlan VLAN-LIST response-time",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Response time is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_queryIntvl_set,
         "ipv6 mld snooping vlan VLAN-LIST query-interval <"SYS_STR_CONST(BOND_MCAST_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_QRYINTV_MAX)">",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Query Interval\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_QRYINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_QRYINTV_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_MLD_QUERY_INTV)" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_queryIntvl_set,
         "no ipv6 mld snooping vlan VLAN-LIST query-interval",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Query Interval is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_lastMbrQueryIntvl_set,
         "ipv6 mld snooping vlan VLAN-LIST  last-member-query-interval <"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MAX)">",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Interval\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRINTV_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_GSQUERY_RESPONSE_INTV )" Sec.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_lastMbrQueryIntvl_set,
         "no ipv6 mld snooping vlan VLAN-LIST last-member-query-interval",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Interval is default.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_lastMbrQueryCount_set,
         "ipv6 mld snooping vlan VLAN-LIST  last-member-query-count <"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MAX)">",
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Count\n"
        "Valid range is "SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MIN)"-"SYS_STR_CONST(BOND_MCAST_LASTMBRCNT_MAX)" Sec. Default is "SYS_STR_CONST(DFLT_IGMP_ROBUSTNESS_VAR)".\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_lastMbrQueryCount_set,
         "no ipv6 mld snooping vlan VLAN-LIST  last-member-query-count",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "Last Member Query Count  is default\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_vlan_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST",
        NO_DESC
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_static_port_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST static-port IF_PORTS",
        NO_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "IPv6 static port configuration\n"
        PORT_DESC
        ARG_PORTLIST_ALL_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_forbidden_port_set,
         "[no] ipv6 mld snooping vlan VLAN-LIST forbidden-port IF_PORTS",
        NO_STR
        IPV6_STR
        MLD_STR
        MLD_SNOOPING_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
        "IPv6 forbidden port configuration\n"
        PORT_DESC
        ARG_PORTLIST_ALL_DESC
    },
    #endif
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_mld_portGroup_limit_set,
         "ipv6 mld max-groups <0-$DEF>",
        IPV6_STR
        MLD_STR
        "Max group number \n"
        "MLD snooping max group number 0~$DEF\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_portGroup_limit_set,
         "no ipv6 mld max-groups",
        NO_STR
        IPV6_STR
        MLD_STR
        "Default max groups number \n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_portGroup_excessAct_set,
         "ipv6 mld max-groups action (deny|replace)",
        IPV6_STR
        MLD_STR
        "MLD max groups\n"
        "Excess max group action \n"
        "MLD max-group action deny\n"
        "MLD max-group action replace\n"
    },
  #endif
  #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_profile_enter_set,
         "ipv6 mld profile <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        IPV6_STR
        MLD_STR
        "MLD profile\n"
        "Profile index\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_profile_remove,
         "no ipv6 mld profile <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        NO_DESC
        IPV6_STR
        MLD_STR
        "MLD profile\n"
        "Profile index\n"
    },
    {
         NODE_MLD_PROFILE,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_profileEntry_set,
         "profile range ipv6 X:X::X:X [X:X::X:X] action (permit|deny)",
        "MLD profile\n"
        "ipv6 multicast address range\n"
        IPV6_STR
        "IPv6 multicast address start\n"
        "IPv6 multicast address end\n"
        "Profile rule action\n"
        "Action permit\nAction deny\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_filter_set,
         "ipv6 mld filter <"SYS_STR_CONST(BOND_MCAST_PROFILE_MIN)"-"SYS_STR_CONST(BOND_MCAST_PROFILE_MAX)">",
        IPV6_STR
        MLD_STR
        "IPv6 filter\n"
        "IPv6 filter profile index\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_mld_filter_set,
         "no ipv6 mld filter",
        NO_DESC
        IPV6_STR
        MLD_STR
        "Multicast IPv6 filter\n"
    },
  #endif
#endif 

    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_enable_set,
         "[no] shutdown",
        NO_DESC
        PORT_SHUTDOWN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_media_type_set,
         "no media-type",
        NO_DESC
        PORT_MEDIA_TYPE_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_media_type_set,
         "media-type (auto-select|rj45|sfp)",
        PORT_MEDIA_TYPE_DESC
        ARG_MEDIA_TYPE_AUTO
        ARG_MEDIA_TYPE_RJ45
        ARG_MEDIA_TYPE_SFP
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_no_nego_set,
         "[no] speed nonegotiate",
        NO_DESC
        PORT_SPEED_DESC
        ARG_PORT_SPEED_NO_NEGO
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_set,
         "speed (10|100|1000)",
        PORT_SPEED_DESC
        ARG_PORT_SPEED_10_100
        ARG_PORT_SPEED_1000
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_set,
         "speed (1000|10000)",
        PORT_SPEED_DESC
        ARG_PORT_SPEED_1000
        ARG_PORT_SPEED_10000
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_set,
         "speed auto",
        PORT_SPEED_DESC
        PORT_SPEED_AUTO_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_set,
         "speed auto (10|100|1000)",
        PORT_SPEED_DESC
        PORT_SPEED_AUTO_DESC
        ARG_PORT_AUTO_SPEED_10_100
        ARG_PORT_AUTO_SPEED_1000
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_speed_set,
         "speed dac (50|100|300|500)",
        PORT_SPEED_DESC
        "Direct Attach Cable\n"
        "50 CM\n"
        "100 CM\n"
        "300 CM\n"
        "500 CM\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_port_duplex_set,
         "duplex (auto|full|half)",
        PORT_DUPLEX_DESC
        ARG_PORT_DUPLEX
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_backPressure_set,
         "[no] back-pressure",
        NO_DESC
        "Enable back-pressure\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_flowCtrl_set,
         "flowcontrol (auto|off|on)",
        PORT_FLOWCTL_DESC
        ARG_PORT_FLOWCTL
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_flowCtrl_set,
         "no flowcontrol",
        NO_STR
        PORT_FLOWCTL_DESC
    },
#ifdef CONFIG_SYS_PORT_DESCRIPTION
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_descp_set,
         "description WORD<1-" SYS_STR_CONST(CAPA_PORTDESC_STR_LEN) ">",
        IF_DESCP_DESC
        ARG_IF_DESCP
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_descp_set,
         "no description",
        NO_STR
        IF_DESCP_DESC
    },
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_port_protected_set,
         "[no] protected",
        NO_STR
        PORT_PROTECTED_DESC
    },
#endif
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    #ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        {
             NODE_CONFIG,
             ATTR_PRIV_15,
             NULL,
             cmd_port_errDisableRecovery_set,
             "[no] errdisable recovery cause (all|acl|arp-inspection|broadcast-flood|bpduguard|dhcp-rate-limit|psecure-violation|unicast-flood|unknown-multicast-flood|selfloop)",
            NO_DESC
            PORT_ERRDISABLE_DESC
            PORT_ERRDISABLE_RECV_DESC
            PORT_ERRDISABLE_CAUSE_DESC
            ARG_PORT_ERRDISABLE_CAUSE
        },
    #else
        {
             NODE_CONFIG,
             ATTR_PRIV_15,
             NULL,
             cmd_port_errDisableRecovery_set,
             "[no] errdisable recovery cause (all|acl|arp-inspection|broadcast-flood|bpduguard|dhcp-rate-limit|psecure-violation|unicast-flood|unknown-multicast-flood)",
            NO_DESC
            PORT_ERRDISABLE_DESC
            PORT_ERRDISABLE_RECV_DESC
            PORT_ERRDISABLE_CAUSE_DESC
            ARG_PORT_ERRDISABLE_CAUSE
        },
    #endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_port_errDisableInterval_set,
         "errdisable recovery interval <"SYS_STR_CONST(BOND_PORT_ERRDISABLE_RECOVERY_INTERVAL_MIN)"-"SYS_STR_CONST(BOND_PORT_ERRDISABLE_RECOVERY_INTERVAL_MAX)">",
        PORT_ERRDISABLE_DESC
        PORT_ERRDISABLE_RECV_DESC
        PORT_ERRDISABLE_INTV_DESC
        "Interval with the number of seconds\n"
    },
  #ifdef CONFIG_SYS_PROTO_UDLD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_port_errDisableRecoveryUDLD_set,
         "[no] errdisable recovery cause udld",
        NO_DESC
        PORT_ERRDISABLE_DESC
        PORT_ERRDISABLE_RECV_DESC
        PORT_ERRDISABLE_CAUSE_DESC
        "Enable timer to recover from udld causes\n"
    },
  #endif
#endif 
#ifdef CONFIG_SYS_PORT_SECURITY
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_enable_set,
         "[no] port-security",
        NO_STR
        PSECURE_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_rateLimit_set,
         "port-security rate-limit <"SYS_STR_CONST(BOND_PSECURE_RATELIMIT_MIN)"-"SYS_STR_CONST(BOND_PSECURE_RATELIMIT_MAX)">",
        PSECURE_DESC
        PSECURE_RATELIMIT_DESC
        "Rate in packet per second (pps)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_rateLimit_set,
         "no port-security rate-limit",
        NO_STR
        PSECURE_DESC
        PSECURE_RATELIMIT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portEnable_set,
         "[no] port-security",
        NO_STR
        PSECURE_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_psecure_portLimitNum_set,
         "port-security address-limit <$DEF1-$DEF2>",
        PSECURE_DESC
        PSECURE_ADDRLIMIT_DESC
        "Number of limitation\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portLimitNum_set,
         "no port-security address-limit",
        NO_STR
        PSECURE_DESC
        PSECURE_ADDRLIMIT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portViolation_set,
         "port-security violation (protect|restrict|shutdown)",
        PSECURE_DESC
        PSECURE_VIOLATION_DESC
        "Drops packets with unknown source addresses\n"
        "Drops packets with unknown source addresses and causes the |SecurityViolation counter to increment\n"
        "Puts the interface into the error-disabled state immediately\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portViolation_set,
         "no port-security violation",
        NO_STR
        PSECURE_DESC
        PSECURE_VIOLATION_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portSticky_set,
         "[no] port-security mac-address sticky",
        NO_STR
        PSECURE_DESC
        PSECURE_MACADDR_DESC
        "Dynamic mac address will be converted to static secure mac |address\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_psecure_portStickyMac_set,
         "port-security mac-address sticky A:B:C:D:E:F vlan <1-4094>",
        PSECURE_DESC
        PSECURE_MACADDR_DESC
        "Dynamic mac address will be converted to static secure mac |address\n"
        "MAC Address\n"
        "VLAN\n"
        "VLAN ID\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_portStaticMac_set,
         "[no] port-security mac-address A:B:C:D:E:F [vlan <1-4094>]",
        NO_STR
        PSECURE_DESC
        PSECURE_MACADDR_DESC
        "MAC Address\n"
        "VLAN\n"
        "VLAN ID\n"
    },
#endif 

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_mode_set,
         "[no] qos",
        NO_DESC
        QOS_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_strictPriQueueNum_set,
         "qos queue strict-priority-num <0-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)">",
        QOS_DESC
        QOS_QUEUE_DESC
        QOS_SPQ_NUMQ_DESC
        ARG_QOS_SPQ_NUM
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_queueWeight_set,
         "qos queue weight SEQUENCE",
        QOS_DESC
        QOS_QUEUE_DESC
        QOS_BW_DESC
        ARG_BW_SEQ
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_cosQueue_set,
         "qos map cos-queue SEQUENCE to <1-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)">",
        QOS_DESC
        QOS_MAP_DESC
        QOS_COS_MAP_DESC
        ARG_COS_MAP_SEQ
        QOS_MAP_TO_DESC
        ARG_COS_MAP_QUEUE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_dscpQueue_set,
         "qos map dscp-queue SEQUENCE to <1-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)">",
        QOS_DESC
        QOS_MAP_DESC
        QOS_MAP_DSCP_Q_DESC
        ARG_MAP_DSCP_Q_SEQ
        QOS_MAP_TO_DESC
        ARG_MAP_DSCP_Q_QUE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_ippQueue_set,
         "qos map precedence-queue SEQUENCE to <1-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)">",
        QOS_DESC
        QOS_MAP_DESC
        QOS_MAP_IPP_Q_DESC
        ARG_MAP_IPP_Q_SEQ
        QOS_MAP_TO_DESC
        ARG_MAP_IPP_Q_QUE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_trust_set,
         "qos trust (cos|cos-dscp|dscp|precedence)",
        QOS_DESC
        QOS_TRUST_DESC
        ARG_QOS_TRUST_MODE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_trust_set,
         "no qos trust",
        NO_DESC
        QOS_DESC
        QOS_TRUST_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_portCos_set,
         "qos cos <0-7>",
        QOS_DESC
        QOS_PORTCOS_DESC
        ARG_QOS_PORTCOS
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_portTrust_set,
         "[no] qos trust",
        NO_DESC
        QOS_DESC
        QOS_PORT_TRUST_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_queueCos_set,
         "qos map queue-cos SEQUENCE to <0-7>",
        QOS_DESC
        QOS_MAP_DESC
        QOS_MAP_Q_COS_MAP_DESC
        ARG_MAP_Q_COS_MAP_SEQ
        QOS_MAP_TO_DESC
        ARG_MAP_Q_COS_COS
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_queueDscp_set,
         "qos map queue-dscp SEQUENCE to <0-63>",
        QOS_DESC
        QOS_MAP_DESC
        QOS_MAP_Q_DSCP_MAP_DESC
        ARG_MAP_Q_DSCP_MAP_SEQ
        QOS_MAP_TO_DESC
        ARG_MAP_Q_DSCP_DSCP
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_queueIpp_set,
         "qos map queue-precedence SEQUENCE to <0-7>",
        QOS_DESC
        QOS_MAP_DESC
        QOS_MAP_Q_IPP_MAP_DESC
        ARG_MAP_Q_IPP_MAP_SEQ
        QOS_MAP_TO_DESC
        ARG_MAP_Q_IPP_COS
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_qos_portRemark_set,
         "[no] qos remark (cos|dscp|precedence)",
        NO_DESC
        QOS_DESC
        QOS_PORT_REMARK_DESC
        QOS_PORT_REMARK_TYPE
    },

#ifdef CONFIG_SYS_LIB_RADIUS
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_radiusSrv_dftParam_set,
         "radius default-config [key RADIUSKEY] [retransmit <"SYS_STR_CONST(BOND_RADIUS_RETRY_MIN)"-"SYS_STR_CONST(BOND_RADIUS_RETRY_MAX)">] [timeout <"SYS_STR_CONST(BOND_RADIUS_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_RADIUS_TIMEOUT_MAX)">]",
        RADIUS_SRV_DESC
        RADIUS_SRV_DFTPARAM_DESC
        RADIUS_KEY_DESC
        ARG_RADIUS_KEY
        RADIUS_RETRANS_DESC
        ARG_RADIUS_RETRANS
        RADIUS_TIMEOUT_DESC
        ARG_RADIUS_TIMEOUT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_radiusSrv_set,
         "radius host (A.B.C.D|X:X::X:X|HOSTNAME) [auth-port <0-65535>] [key RADIUSKEY] [priority <"SYS_STR_CONST(BOND_RADIUS_PRI_MIN)"-"SYS_STR_CONST(BOND_RADIUS_PRI_MAX)">] [retransmit <"SYS_STR_CONST(BOND_RADIUS_RETRY_MIN)"-"SYS_STR_CONST(BOND_RADIUS_RETRY_MAX)">] [timeout <"SYS_STR_CONST(BOND_RADIUS_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_RADIUS_TIMEOUT_MAX)">] [type (login|802.1x|all)]",
        RADIUS_SRV_DESC
        RADIUS_HOST_DESC
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
        RADIUS_AUTHPORT_DESC ARG_TCPUDP_PORT_DESC
        RADIUS_KEY_DESC ARG_RADIUS_KEY
        SERVER_PRIOR_DESC ARG_SERVER_PRIOR
        RADIUS_RETRANS_DESC ARG_RADIUS_RETRANS
        RADIUS_TIMEOUT_DESC ARG_RADIUS_TIMEOUT
        RADIUS_USGTYPE_DESC ARG_RADIUS_USGTYPE
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_radiusSrv_set,
         "no radius [host (A.B.C.D|X:X::X:X|HOSTNAME)]",
        NO_STR
        RADIUS_SRV_DESC
        RADIUS_HOST_DESC
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
    },
#endif 

#ifdef CONFIG_SYS_STORM_CTRL
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormCtrlIncludeIfg_set,
         "storm-control ifg (include|exclude)",
        STORM_DESC
        "Interframe configuration\n"
        "Include preamble and IFG\nExclude preamble and IFG\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormCtrlRefreshMode_set,
         "storm-control unit (bps|pps)",
        STORM_DESC
        "Unit configuration\n"
        "Bits per second\nPackets per second\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlPortEnable_set,
         "[no] storm-control",
        NO_DESC
        STORM_DESC
        "Enable storm control\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlTypeEnable_set,
         "[no] storm-control (broadcast|unknown-unicast|unknown-multicast)",
        NO_DESC
        STORM_DESC
        BCAST_STORM_DESC UUCAST_STORM_DESC UMCAST_STORM_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlRate_set,
         "storm-control (broadcast|unknown-unicast|unknown-multicast) level <"SYS_STR_CONST(BOND_RATE_STORM_PPS_MIN)"-"SYS_STR_CONST(BOND_RATE_STORM_BPS_MAX)">",
        STORM_DESC
        BCAST_STORM_DESC
        UUCAST_STORM_DESC
        UMCAST_STORM_DESC
        STORM_RATE_DESC
        "Rate value(bps:"SYS_STR_CONST(BOND_RATE_STORM_BPS_MIN)"-"SYS_STR_CONST(BOND_RATE_STORM_BPS_MAX)",pps:"SYS_STR_CONST(BOND_RATE_STORM_PPS_MIN)"-"SYS_STR_CONST(BOND_RATE_STORM_PPS_MAX)")\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlRate_set,
         "no storm-control (broadcast|unknown-unicast|unknown-multicast) level",
        NO_DESC
        STORM_DESC
        BCAST_STORM_DESC
        UUCAST_STORM_DESC
        UMCAST_STORM_DESC
        STORM_RATE_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlAction_set,
         "storm-control action (drop|shutdown)",
        STORM_DESC
        STORM_ACTION_DESC
        STORM_ACTION_DROP
        STORM_ACTION_SHUTDOWN
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_stormControlAction_set,
         "no storm-control action",
        NO_DESC
        STORM_DESC
        STORM_ACTION_DESC
    },
#endif 
#ifdef CONFIG_SYS_BW_CTRL
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_igrbandwidthControlRate_set,
         "rate-limit ingress <"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MIN)"-"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MAX)">",
        RATE_LIMIT_DESC
        RL_INPUT_DESC
        "The average traffic rate in Kbps, must be a multiple of 16\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_igrbandwidthControlRate_set,
         "no rate-limit ingress",
        NO_STR
        RATE_LIMIT_DESC
        RL_INPUT_DESC
    },

    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_egrBandwidthCtrlRate_set,
         "rate-limit egress <"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MIN)"-"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MAX)">",
        RATE_LIMIT_DESC
        RL_OUTPUT_DESC
        "The average traffic rate in Kbps, must be a multiple of 16\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_egrBandwidthCtrlRate_set,
         "no rate-limit egress",
        NO_STR
        RATE_LIMIT_DESC
        RL_INPUT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_egrQueueControlRate_set,
         "rate-limit egress queue <1-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)"> <"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MIN)"-"SYS_STR_CONST(BOND_RATE_BANDWIDTH_MAX)">",
        RATE_LIMIT_DESC
        RL_INPUT_DESC
        "queue configuration\n"
        "queue id\n"
        "The average traffic rate in Kbps, must be a multiple of 16\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_rate_egrQueueControlRate_set,
         "no rate-limit egress queue <1-"SYS_STR_CONST(CAPA_QOS_QUEUE_NUM)">",
        NO_STR
        RATE_LIMIT_DESC
        RL_INPUT_DESC
        "queue id\n"
    },

#endif 

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_event_set,
         "rmon event <1-65535> [log] [trap COMMUNITY] [description DESCRIPTION] [owner NAME]",
        RMON_STR
        RMON_EVENT_DESC
        "index of event\n"
        "enable log for event\n"
        "enable trap for event\n"
        "community name of trap (0~31 charactors)\n"
        "description of event (0~127 charactors)\n"
        "description of event (0~127 charactors)\n"
        "owner name of event (0~31 charactors)\n"
        "owner name of event (0~31 charactors)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_event_set,
         "no rmon event <1-65535>",
        NO_DESC
        RMON_STR
        RMON_EVENT_DESC
        "index of event\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_alarm_set,
         "rmon alarm <1-65535> interface IF_PORT (drop-events|octets|pkts|broadcast-pkts|multicast-pkts|crc-align-errors|undersize-pkts|oversize-pkts|fragments|jabbers|collisions|pkts64octets|pkts65to127octets|pkts128to255octets|pkts256to511octets|pkts512to1023octets|pkts1024to1518octets) <1-2147483647> (absolute|delta) rising <0-2147483647> <0-65535> falling <0-2147483647> <0-65535> startup (rising|rising-falling|falling) [owner NAME]",
        RMON_STR
        RMON_ALARM_DESC
        "index of alarm\n"
        IF_DESC
        ARG_IF_PORT
        "DropEvents\nOctets\nPkts\nBroadcastPkts\nMulticastPkts\nCRCAlignErrors\nUnderSizePkts\nOverSizePkts\nFragments\nJabbers\nCollision\nPkts64Octets\nPkts65to127Octets\nPkts128to255Octets\nPkts256to511Octets\nPkts512to1023Octets\nPkts1024to1518Octets\n"
        "sample interval\n"
        "compare each sample directly\ncompare delta between samples\n"
        "rising\n"
        "threshold of rising\n"
        "event index of rising\n"
        "falling\n"
        "threshold of falling\n"
        "event index of falling\n"
        "Specifies the alarm that may be sent when this entry becomes valid\n"
        "Specifies that if the first sample is greater than or equal to rising-threshold, a single rising alarm is generated\nSpecifies that if the first sample is greater than, less than or equal to threshold, a single rising alarm is generated.\nSpecifies that if the first sample is less than or equal to falling-threshold, a single falling alarm is generated\n"
        "owner\n"
        "name (0~31 chars)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_alarm_set,
         "no rmon alarm <1-65535>",
        NO_DESC
        RMON_STR
        RMON_ALARM_DESC
        "index of alarm\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_history_set,
         "rmon history <1-65535> interface IF_PORT [buckets <1-50>] [interval <1-3600>] [owner NAME]",
        RMON_STR
        RMON_HISTORY_DESC
        "index of history\n"
        IF_DESC
        ARG_IF_PORT
        "the maximum number of buckets. default is 50.\n"
        "the maximum number of buckets value\n"
        "the number of seconds for each sample. default is 1800\n"
        "the number of seconds value.\n"
        "owner name of event (0~31 charactors)\n"
        "owner name of event (0~31 charactors)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_history_set,
         "no rmon history <1-65535>",
        NO_DESC
        RMON_STR
        RMON_HISTORY_DESC
        "index of history\n"
    },
#endif 

#ifdef CONFIG_SYS_UI_SNMP
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_enable_set,
         "[no] snmp",
        NO_DESC
        SNMP_STR
    },
    #ifdef CONFIG_SYS_MIB_TRAP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_trapClass_set,
         "[no] snmp trap (auth|linkUpDown|warm-start|cold-start)",
        NO_STR
        SNMP_STR
        "Snmp class trap setting\n"
        "Set snmp authentication failure trap\nSet snmp link up and down trap\nSet snmp bootup warm start-up trap\nSet snmp bootup cold start-up trap\n"
    },
    #endif
    #ifdef CONFIG_SYS_SNMPV3
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_comView_add,
         "snmp community NAME [view NAME] (ro|rw)",
        SNMP_STR
        "Set community or security name string\n"
        SNMP_COMMUNITY_NAME
        "Community assign the access view\n"
        SNMP_VIEW_NAME
        "Set community access read_only\n set community access read_write\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_com2sec_del,
         "no snmp community NAME",
        NO_STR
        SNMP_STR
        "Delete community and security group string\n"
        SNMP_COMMUNITY_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_host_add,
         "snmp host (A.B.C.D|X:X::X:X|HOSTNAME) [(traps|informs)] [version (1|2c)] NAME [udp-port <"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)">] [timeout <"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MAX)">] [retries <"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MAX)">]",
        SNMP_STR
        "Trap or inform host\n"
        ARG_IPADDR_DESC ARG_IPV6ADDR_DESC ARG_HOSTNAME_DESC
        "Notification type is Traps\nNotification type is informs\n"
        "Version of trap or inform\n"
        "Notify version v1 \nNotify version v2c\n"
        SNMP_COMMUNITY_NAME
        "Udp port number\n"
        "Port range "SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)"\n"
        "V2c inform timeout\n"
        "Time range "SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MAX)"\n"
        "V2c inform retries\n"
        "Retries range "SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MAX)"\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_host_del,
         "no snmp host (A.B.C.D|X:X::X:X|HOSTNAME) [(traps|informs)] [version (1|2c|3)]",
        NO_STR
        SNMP_STR
        "Remove trap host entry\n"
        ARG_IPADDR_DESC ARG_IPV6ADDR_DESC ARG_HOSTNAME_DESC
        "Notification type is Traps\nNotification type is informs\n"
        "Trap or inform version\n"
        "Trap version v1 \nTrap or inform version v2c\nTrap or inform version 3\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_view_add,
         "snmp view NAME subtree OID oid-mask (all|MASK) viewtype (included|excluded)",
        SNMP_STR
        "Set view string\n"
        SNMP_VIEW_NAME
        "View subtree \n"
        SNMP_VIEW_TREE_OID
        "Subtree oid mask \n"
        "Oid all mask\nOid mask length less than subtree OID length(eg:1110)\n"
        "Subtree view type\n"
        "Oid viewtype include\n Oid viewtype exclude\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_view_del,
         "no snmp view NAME subtree (all|OID)",
        NO_STR
        SNMP_STR
        "Delete view string\n"
        SNMP_VIEW_NAME
        "The view's subtree\n"
        "view's sub-tree all\n" SNMP_VIEW_TREE_OID
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_access_add,
         "snmp group NAME version (1|2c|3) (noauth|auth|priv) read-view NAME [write-view NAME] [notify-view NAME]",
        SNMP_STR
        "Set access group string\n"
        SNMP_GROUP_NAME
        "Version for group security mode\n"
        "security mode v1 \n  security mode v2c \n security mode v3 \n"
        "security level noauth \n security level auth \n security level priv \n"
        "Read view name \n"
        SNMP_VIEW_NAME
        "Write view name \n"
        SNMP_VIEW_NAME
        "Notify view name\n"
        SNMP_VIEW_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_access_del,
         "no snmp group NAME security-mode version (1|2c)",
        NO_STR
        SNMP_STR
        "Delete group string\n"
        SNMP_GROUP_NAME
        "Delete group security mode \n"
        "Version of group security mode\n"
        "Security mode v1 \n  Security mode v2c \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_accessv3_del,
         "no snmp group NAME security-mode version 3 [security-level (noauth|auth|priv)]",
        NO_STR
        SNMP_STR
        "Delete group string\n"
        SNMP_GROUP_NAME
        "Delete group security mode \n"
        "Version of group security mode\n"
        "Security mode v3 \n"
        "Security level\n"
        "security level noauth \n security level auth \n security level priv \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_comGroup_add,
         "snmp community NAME group NAME",
        SNMP_STR
        "Set community or security name string\n"
        SNMP_COMMUNITY_NAME
        "Community assign the access group\n"
        SNMP_GROUP_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_hostv3_add,
         "snmp host (A.B.C.D|X:X::X:X|HOSTNAME) [(traps|informs)] version 3 [(auth|noauth|priv)] NAME [udp-port <"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)">] [timeout <"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MAX)">] [retries <"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MAX)">]",
        SNMP_STR
        "Trap or inform host\n"
        ARG_IPADDR_DESC ARG_IPV6ADDR_DESC ARG_HOSTNAME_DESC
        "Notification type is Traps\nNotification type is informs\n"
        "Notify version\n"
        "Version 3\n"
        "Security level auth\nSecurity level noauth\nSecurity level priv\n"
        SNMP_SECURITY_NAME
        "Udp port number\n"
        "Port range "SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)"\n"
        "V3 inform timeout\n"
        "Time range Time range "SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_TIMEOUT_MAX)"\n"
        "V3 inform retries\n"
        "Retries range "SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_RETRY_MAX)"\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_authUser_add,
         "snmp user NAME NAME [auth (md5|sha) AUTHPASSWD]",
        SNMPV3_STR
        "Set user Settings\n"
        SNMP_USER_NAME
        SNMP_GROUP_NAME
        "Authentication prototol\n"
        "Use md5 protocol\nUse sha protocol\n"
        SNMP_AUTH_PASSWORD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_privUser_add,
         "snmp user NAME NAME auth (md5|sha) AUTHPASSWD priv PRIVPASSWD",
        SNMPV3_STR
        "Set user Settings\n"
        SNMP_USER_NAME
        SNMP_GROUP_NAME
        "Authentication prototol\n"
        "Use md5 protocol\nUse sha protocol\n"
        SNMP_AUTH_PASSWORD
        "Use encryption protocol\n"
        SNMP_PRIV_PASSWORD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_user_del,
         "no snmp user NAME",
        NO_STR
        SNMPV3_STR
        "Delete User\n"
        "Delete User name\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_engineid_set,
         "snmp engineid (default|ENGINEID)",
        SNMP_STR
        "SNMP engine id setting\n"
        "Set snmp engine id default \n Set snmp engineid engine id("SYS_STR_CONST(BOND_SNMP_ENGINEID_LEN_MIN)"-"SYS_STR_CONST(BOND_SNMP_ENGINEID_LEN_MAX)" hex, the hex num must be divided by 2) \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_remoteEngineid_set,
         "snmp engineid remote (A.B.C.D|X:X::X:X|HOSTNAME) ENGINEID",
        SNMP_STR
        "SNMP engine id setting\n"
        "SNMP remote engine id setting\n"
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
        "SNMP remote engineid engine id("SYS_STR_CONST(BOND_SNMP_ENGINEID_LEN_MIN)"-"SYS_STR_CONST(BOND_SNMP_ENGINEID_LEN_MAX)" hex, the hex num must be divided by 2) \n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_remoteEngineid_set,
         "no snmp engineid remote (A.B.C.D|X:X::X:X|HOSTNAME)",
        NO_STR
        SNMP_STR
        "SNMP engine id setting\n"
        "Remove remote engine id\n"
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
    },
    #else
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_community_add,
         "snmp community NAME (ro|rw)",
        SNMP_STR
        "Set community or security name string\n"
        SNMP_COMMUNITY_NAME
        "Set community access read_only\n set community access read_write\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_community_del,
         "no snmp community NAME",
        NO_STR
        SNMP_STR
        "Delete community and security group string\n"
        SNMP_COMMUNITY_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_host_add,
          "snmp host (A.B.C.D|X:X::X:X|HOSTNAME) [(traps|informs)] [version (1|2c)] NAME",
        SNMP_STR
        "Trap or inform host\n"
        ARG_IPADDR_DESC ARG_IPV6ADDR_DESC ARG_HOSTNAME_DESC
        "Notification type is Traps\nNotification type is informs\n"
        "Version of trap or inform\n"
        "Notify version v1 \nNotify version v2c\n"
        SNMP_COMMUNITY_NAME
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_snmp_host_del,
         "no snmp host (A.B.C.D|X:X::X:X|HOSTNAME) [(traps|informs)] [version (1|2c)]",
        NO_STR
        SNMP_STR
        "Remove trap host entry\n"
        ARG_IPADDR_DESC ARG_IPV6ADDR_DESC ARG_HOSTNAME_DESC
        "Notification type is Traps\nNotification type is informs\n"
        "Trap or inform version\n"
        "Trap version v1 \nTrap or inform version v2c\n"
    },
    #endif
#endif 

#ifdef CONFIG_SYS_PROTO_STP
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_enable_set,
         "[no] spanning-tree",
        NO_STR
        STP_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forwardBpdu_set,
         "spanning-tree bpdu (filtering|flooding)",
        STP_DESC
        "action for bpdu packet\n"
        "bpdu packets are filtered when stp is disabled on ports\nbpdu packets are flooded to all ports with stp disabled and flooding mode\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forwardBpdu_set,
         "no spanning-tree bpdu",
        NO_DESC
        STP_DESC
        "Restore to default bpdu action. Default action is flooding\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forceVersion_set,
         "spanning-tree mode (stp|rstp)",
        STP_DESC
        "Spanning tree protocol type\n"
        "Configure IEEE 802.1D Spanning Tree Protocol\nConfigure IEEE 802.1W Rapid Spanning Tree Protocol\n"
    },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forceVersion_set,
         "spanning-tree mode (mstp|)",
        STP_DESC
        "Spanning tree protocol type\n"
        "Configure IEEE 802.1S Multiple Spanning Tree Protocol\n"
    },
#endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forceVersion_set,
         "no spanning-tree force-version",
        NO_STR
        STP_DESC
        "Sets the force-protocol-version parameter\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_priority_set,
         "spanning-tree priority <0-"SYS_STR_CONST(BOND_STP_PRI_MAX)">",
        STP_DESC
        "Sets the priority for specified instance\n"
        "Priority (0~"SYS_STR_CONST(BOND_STP_PRI_MAX)")\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_priority_set,
         "no spanning-tree priority",
        NO_STR
        STP_DESC
        "Sets the priority for specified instance\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_helloTime_set,
         "spanning-tree hello-time <"SYS_STR_CONST(BOND_STP_HELLO_TIME_MIN)"-"SYS_STR_CONST(BOND_STP_HELLO_TIME_MAX)">",
        STP_DESC
        "Sets the hello-time parameter\n"
        "specifies hello time of Spanning-tree\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_helloTime_set,
         "no spanning-tree hello-time",
        NO_STR
        STP_DESC
        "Sets the hello-time parameter\n"
    },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_maxHops_set,
         "spanning-tree max-hops <"SYS_STR_CONST(BOND_STP_MAX_HOPS_MIN)"-"SYS_STR_CONST(BOND_STP_MAX_HOPS_MAX)">",
        STP_DESC
        "Sets the max-hops parameter\n"
        "hop number\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_maxHops_set,
         "no spanning-tree max-hops",
        NO_STR
        STP_DESC
        "Sets the max-hops parameter\n"
    },
#endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forwardDelay_set,
         "spanning-tree forward-delay <"SYS_STR_CONST(BOND_STP_FORWARD_DELAY_MIN)"-"SYS_STR_CONST(BOND_STP_FORWARD_DELAY_MAX)">",
        STP_DESC
        "Sets the forward-delay parameter\n"
        "Forward-delay interval\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_forwardDelay_set,
         "no spanning-tree forward-delay",
        NO_STR
        STP_DESC
        "Sets the forward-delay parameter\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_maximumAge_set,
         "spanning-tree maximum-age <"SYS_STR_CONST(BOND_STP_MAX_AGE_MIN)"-"SYS_STR_CONST(BOND_STP_MAX_AGE_MAX)">",
        STP_DESC
        "Changes the interval between messages the spanning tree receives from the root switch\n"
        "Interval the switch waits between receiving BPDUs from the root switch\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_maximumAge_set,
         "no spanning-tree maximum-age",
        NO_STR
        STP_DESC
        "Sets the maximum-age parameter\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_txHoldCount_set,
         "spanning-tree tx-hold-count <"SYS_STR_CONST(BOND_STP_TX_HOLD_COUNT_MIN)"-"SYS_STR_CONST(BOND_STP_TX_HOLD_COUNT_MAX)">",
        STP_DESC
        "Set spanning-tree tx hold count, in seconds\n"
        "Specifies the tx hold count\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_txHoldCount_set,
         "no spanning-tree tx-hold-count",
        NO_STR
        STP_DESC
        "Sets the tx-hold-count parameter\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_pathcostMethod_set,
         "spanning-tree pathcost method (long|short)",
        STP_DESC
        "Spanning tree path-cost method\n"
        "Spanning tree path-cost method\n"
        "Specifies that the default port path costs are within the range: 1-"SYS_STR_CONST(BOND_STP_PATHCOST_LONG_MAX)".\nSpecifies that the default port path costs are within the range: 1-"SYS_STR_CONST(BOND_STP_PATHCOST_SHORT_MAX)".\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portAdminEnable_set,
         "[no] spanning-tree",
        NO_STR
        STP_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPriority_set,
         "spanning-tree port-priority <0-"SYS_STR_CONST(BOND_STP_PORT_PRI_MAX)">",
        STP_DESC
        "Sets the priority for specified instance\n"
        "Priority (0~"SYS_STR_CONST(BOND_STP_PORT_PRI_MAX)")\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPriority_set,
         "no spanning-tree port-priority",
        NO_STR
        STP_DESC
        "Sets the priority for specified instance\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPathCost_set,
         "spanning-tree cost <0-"SYS_STR_CONST(BOND_STP_PATHCOST_LONG_MAX)">",
        STP_DESC
        "Change an interface's spanning tree path cost\n"
        "The value of external path cost (0 = Auto)\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPathCost_set,
         "no spanning-tree cost",
        NO_STR
        STP_DESC
        "the external path-cost parameter\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portEdgePort_set,
         "[no] spanning-tree (edge|autoedge)",
        NO_DESC
        STP_DESC
        "Sets the edge-port for specified port\n"
		"Sets the auto-edge-port for specified port\n"
    },
  #ifdef CONFIG_SYS_STP_BPDU_FILTER
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portBpduFilter_set,
         "[no] spanning-tree bpdu-filter",
        NO_DESC
        STP_DESC
        "Sets the BPDU-Filter for specified port\n"
    },
  #endif
  #ifdef CONFIG_SYS_STP_BPDU_GUARD
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portBpduGuard_set,
         "[no] spanning-tree bpdu-guard",
        NO_DESC
        STP_DESC
        "Sets the BPDU-Guard for specified port\n"
    },
  #endif
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPointToPointMac_set,
         "spanning-tree link-type (point-to-point|shared)",
        STP_DESC
        "Specify a link type for spanning tree protocol use\n"
        "Consider the interface as point-to-point\nConsider the interface as shared\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portPointToPointMac_set,
         "no spanning-tree link-type",
        NO_STR
        STP_DESC
        "Specify a link type for spanning tree protocol use\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_portMcheck_set,
         "spanning-tree mcheck",
        STP_DESC
        "Set the mcheck for specified port to migrate\n"
    },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mst_config,
         "spanning-tree mst configuration",
        STP_DESC
        "Multiple spanning tree configuration\n"
        "Enter MST configuration submode\n"
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstConfigName_set,
         "name NAME",
        "Set configuration name\n"
        "Configuration Bridge name (Max."SYS_STR_CONST(CAPA_STP_MST_CFG_NAME_LEN)" charactor)\n"
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstConfigName_set,
         "no name",
        NO_STR
        "Sets the bridge name\n"
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstConfigRevision_set,
         "revision <0-"SYS_STR_CONST(BOND_STP_MST_CFG_REV_MAX)">",
        "Sets the revision level\n"
        "Revision level\n"
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstConfigRevision_set,
         "no revision",
        NO_STR
        "Sets the revision level\n"
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstVlanInstance_set,
         "[no] instance <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> vlan [VLAN-LIST]",
        NO_STR
        "Sets spanning-tree parameters of instances\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Add the MSTI-to-VLAN mapping\n"
        ARG_VLANLIST_DESC
    },
    {
         NODE_MST_CFG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstVlanInstance_set,
         "no instance <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)">",
        NO_STR
        "Remove instance\n"
        "Instance ID (0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPriority_set,
         "spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> priority <0-"SYS_STR_CONST(BOND_STP_PRI_MAX)">",
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Sets the priority for specified instance\n"
        "Priority (0~"SYS_STR_CONST(BOND_STP_PRI_MAX)")\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPriority_set,
         "no spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> priority",
        NO_STR
        STP_DESC
        "Sete spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Sets the priority for specified instance\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPortPathCost_set,
         "spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> cost <0-"SYS_STR_CONST(BOND_STP_PATHCOST_LONG_MAX)">",
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Sets the internal path cost for specified instance\n"
        "The value of internal path cost (0 = Auto)\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPortPathCost_set,
         "no spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> cost",
        NO_STR
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~15)\n"
        "the internal path cost for specified instance\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPortPriority_set,
         "spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> port-priority <0-"SYS_STR_CONST(BOND_STP_PORT_PRI_MAX)">",
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Sets the priority for specified instance\n"
        "Priority (0~"SYS_STR_CONST(BOND_STP_PORT_PRI_MAX)")\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_stp_mstPortPriority_set,
         "no spanning-tree mst <0-"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)"> port-priority",
        NO_STR
        STP_DESC
        "Sets spanning-tree parameters of instance\n"
        "Instance ID (0~"SYS_STR_CONST(BOND_STP_MST_INSTANCE_MAX)")\n"
        "Sets the priority for specified instance\n"
    },
#endif
#endif 

#ifdef CONFIG_SYS_JUMBO_FRAME
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_switch_maxFrameSize_set,
         "jumbo-frame [<" SYS_STR_CONST(BOND_PORT_JUMBO_FRAME_MIN) "-" SYS_STR_CONST(BOND_PORT_JUMBO_FRAME_MAX) ">]",
        JUMBO_DESC
        "Maximum frame size\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_switch_maxFrameSize_set,
         "no jumbo-frame",
        NO_DESC
        JUMBO_DESC
    },
#endif

#ifdef CONFIG_SYS_SYSLOG
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_enable_set,
         "[no] logging",
        NO_DESC
        LOG_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_ratelimitEnable_set,
         "[no] logging aggregation",
        NO_DESC
        LOG_DESC
        LOG_AGG_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_ratelimitInterval_set,
         "logging aggregation aging-time <"SYS_STR_CONST(BOND_LOG_AGGREGATION_INTERVAL_MIN)"-"SYS_STR_CONST(BOND_LOG_AGGREGATION_INTERVAL_MAX)">",
        LOG_DESC
        LOG_AGG_AGING_DESC
        LOG_ARG_AGG_AGING_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_target_set,
         "logging host (A.B.C.D|X:X::X:X|HOSTNAME) [port <"SYS_STR_CONST(BOND_LOG_HOST_PORT_MIN)"-"SYS_STR_CONST(BOND_LOG_HOST_PORT_MAX)">] [severity <0-7>] [facility (local0|local1|local2|local3|local4|local5|local6|local7)]",
        LOG_DESC
        LOG_HOST_DESC
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
        LOG_PORT_DESC
        LOG_ARG_PORT_DESC
        LOG_SEVERITY_DESC
        LOG_ARG_SEVERITY_DESC
        LOG_FACILITY_DESC
        LOG_ARG_FACILITY_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_target_set,
         "logging (buffered|console|file) [severity <0-7>]",
        LOG_DESC
        "Buffered logging\n"
        "Console logging\n"
        "File logging\n"
        LOG_SEVERITY_DESC
        LOG_ARG_SEVERITY_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_noTarget_set,
         "no logging (buffered|console|file)",
        NO_DESC
        LOG_DESC
        "Buffered logging\n"
        "Console logging\n"
        "File logging\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_log_noTarget_set,
         "no logging host (A.B.C.D|X:X::X:X|HOSTNAME)",
        NO_DESC
        LOG_DESC
        LOG_HOST_DESC
        ARG_IPADDR_DESC
        ARG_HOSTNAME_DESC
    },
#endif 

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemName_set,
         "system name WORD<1-32>",
        SYSTEM_DESC
        "Set host name\n"
        "System name. If string has blank, use \"\" to quote it.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemLocation_set,
         "system location LOCATION",
        SYSTEM_DESC
        "Set host location\n"
        "Location. If string has blank, use \"\" to quote it.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemManufacturer_set,
         "system manufacturer MANUFACTURER",
        SYSTEM_DESC
        "Set manufacturer or company name\n"
        "System manufacturer or company name. If string has blank, use \"\" to quote it.\n"
    },
        {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemSupportInfo_set,
         "system support SUPPORTINFO",
        SYSTEM_DESC
        "Set support info\n"
        "System support info. If string has blank, use \"\" to quote it.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemTelephone_set,
         "system telephone NUMBER",
        SYSTEM_DESC
        "Set Telephone or Mobile number\n"
        "Telephone or Mobile number. If string has blank, use \"\" to quote it.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_systemContact_set,
         "system contact CONTACT",
        SYSTEM_DESC
        "Set host contact\n"
        "System contact. If string has blank, use \"\" to quote it.\n"
    },
#ifndef CONFIG_SYS_L3_ARCHITECTURE
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipAddr_set,
         "ip address A.B.C.D [mask A.B.C.D]",
        IP_STR
        "IPv4 Address\n"
        ARG_IPADDR_DESC
        "Netmask\n"
        "Netmask\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipDfltGateway_set,
         "ip default-gateway A.B.C.D",
        IP_STR
        "Set default gateway IP address\n"
        "Default gateway IP address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipAddr_sub_set,
         "ip sub-address A.B.C.D [mask A.B.C.D] ",
        IP_STR
        "Sub IPv4 Address\n"
        ARG_IPADDR_DESC
        "Netmask\n"
        "Netmask\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipAddr_sub_set,
         "no ip sub-address",
        NO_DESC
        IP_STR
        "Sub IPv4 Address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipDfltGateway_set,
         "no ip default-gateway",
        NO_STR
        IP_STR
        "Remove default gateway address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dhcp_set,
         "[no] ip dhcp",
        NO_DESC
        IP_STR
        DHCP_DESC
    },
#endif
#ifdef CONFIG_SYS_IPV6
  #ifdef CONFIG_SYS_APP_DHCP6C
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dhcp6c_set,
         "[no] ipv6 dhcp",
        NO_DESC
        IPV6_STR
        "Set IPv6 DHCP Client\n"
    },
  #endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Enable_set,
         "[no] ipv6",
        NO_DESC
        IPV6_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Autoconf_set,
         "[no] ipv6 autoconfig",
        NO_DESC
        IPV6_STR
        "Enable Ipv6 auto-configuration\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Addr_set,
         "ipv6 address X:X::X:X prefix <0-128>",
        IPV6_STR
        "Set IPv6 address and prefix\n"
        "IPv6 address\n"
        "Prefix length\n"
        "lengh value\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Addr_set,
         "no ipv6 address X:X::X:X",
        NO_DESC
        IPV6_STR
        "Set IPv6 address and prefix\n"
        "IPv6 address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Gateway_set,
         "ipv6 default-gateway X:X::X:X",
        IP_STR
        "Set IPv6 gateway\n"
        "IPv6 gateway\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_ipv6Gateway_set,
         "no ipv6 default-gateway",
        NO_DESC
        IP_STR
        "Set IPv6 gateway\n"
        "IPv6 gateway\n"
    },
#endif
#ifdef CONFIG_SYS_ADVANCE_DNS
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsLookup_set,
         "[no] ip domain lookup",
        NO_DESC
        IP_DESC
        SYS_DNS_DESC
        "Enable the IP Domain Naming System lookup\n"
    },

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsDomainName_set,
         "ip domain name HOSTNAME",
        IP_DESC
        SYS_DNS_DESC
        SYS_DNS_NAME_DESC
        "Default domain name used to complete unqualified host |names. Do not include the initial period that |separates an unqualified name from the domain name.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsDomainName_set,
         "no ip domain name",
        NO_DESC
        IP_DESC
        SYS_DNS_DESC
        SYS_DNS_NAME_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsNameServer_set,
         "[no] ip name-server (A.B.C.D|X:X::X:X) [(A.B.C.D|X:X::X:X)] [(A.B.C.D|X:X::X:X)] [(A.B.C.D|X:X::X:X)]",
        NO_DESC
        IP_DESC
        "To set the available name servers, use the ip |name-server global configuration command.\n"
        "Configure DNS server IPv4 address\n"
        "Configure DNS server IPv6 address\n"
        "Configure DNS server IPv4 address\n"
        "Configure DNS server IPv6 address\n"
        "Configure DNS server IPv4 address\n"
        "Configure DNS server IPv6 address\n"
        "Configure DNS server IPv4 address\n"
        "Configure DNS server IPv6 address\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsHostMapping_set,
         "ip host HOSTNAME (A.B.C.D|X:X::X:X)",
        IP_DESC
        "To define static host name-to-address mapping in the |host cache\n"
        "Host name\n"
        "IPv4 address of the host.\n"
        "IPv6 address of the host.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_dnsHostMapping_set,
         "no ip host HOSTNAME",
        NO_DESC
        IP_DESC
        "To define static host name-to-address mapping in the |host cache\n"
        "Host name\n"
    },
#endif
#ifdef CONFIG_SYS_APP_MSNTP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_sntpEnable_set,
         "clock source (local|sntp)",
        "Manage the system clock\n"
        "Configure an external time source for the system clock\n"
        "Local\nSNTP Server\n"
    },
#endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_timezone_set,
         "clock timezone ACRONYM HOUR-OFFSET [minutes <0-59>]",
        "Manage the system clock\n"
        "set the time zone for display purposes\n"
        "The acronym of the time zone (1-4 chars)\n"
        "<-12-13> Hours difference from UTC\n"
        "Minutes difference from UTC\n"
        "Minutes difference from UTC\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_timezone_set,
         "no clock timezone",
        NO_DESC
        "Manage the system clock\n"
        "set the time zone for display purposes\n"
    },
#ifdef CONFIG_SYS_APP_MSNTP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_sntp_set,
         "sntp host (A.B.C.D|HOSTNAME) [port <1-65535>]",
        SNTP_DESC
        "Configure SNTP server address.\n"
        ARG_IPADDR_DESC
        ARG_HOSTNAME_DESC
        "Configure SNTP server port.\n"
        "1~65535\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_sntp_set,
         "no sntp",
        NO_DESC
        SNTP_DESC
    },
#endif
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_nosort_install,
         cmd_time_dls_date_set,
         "clock summer-time ACRONYM date (jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) <1-31> <2000-2037> HH:MM (jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) <1-31> <2000-2037> HH:MM [<1-1440>]",
        "Manage the system clock\n"
        "Configure the system to automatically switch to summer time (daylight saving time)\n"
        "The acronym of the time zone to be displayed when summer time is in effect. If unspecified default to the timezone acronym. (1-4 chars)\n"
        "Start on the first specific date listed in the command and end on the second specific date in the command\n"
        TIME_MONTHS
        "Date to start\n"
        "Starting year\n"
        "Starting time (military format) in hours and minutes.\n"
        TIME_MONTHS
        "Date to end\n"
        "Ending year\n"
        "Ending time (military format) in hours and minutes.\n"
        "Number of minutes to add during summer time (default is 60).\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_dls_rec_set,
         "clock summer-time ACRONYM recurring (usa|eu) [<1-1440>]",
        "Manage the system clock\n"
        "Configure the system to automatically switch to summer time (daylight saving time)\n"
        "The acronym of the time zone to be displayed when summer time is in effect. If unspecified default to the timezone acronym. (1-4 chars)\n"
        "Summer time should start and end on the corresponding specified days every year.\n"
        "Summer time rules are the United States rules. Start: Second Sunday in March End: First Sunday in November Time: 2 am local time\n"
        "Summer time rules are the European Union rules. Start: Last Sunday in March End: Last Sunday in October Time: 1 am local time\n"
        "Number of minutes to add during summer time (default is 60).\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         cmd_ext_nosort_install,
         cmd_time_dls_rec_set,
         "clock summer-time ACRONYM recurring (<1-5>|first|last) (sun|mon|tue|wed|thu|fri|sat) (jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) HH:MM (<1-5>|first|last) (sun|mon|tue|wed|thu|fri|sat) (jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) HH:MM [<1-1440>]",
        "Manage the system clock\n"
        "Configure the system to automatically switch to summer time (daylight saving time)\n"
        "The acronym of the time zone to be displayed when summer time is in effect. If unspecified default to the timezone acronym. (1-4 chars)\n"
        "Summer time should start and end on the corresponding specified days every year.\n"
        "Number of the week in the month\n"
        "First week of the month\n"
        "Last week of the month\n"
        TIME_WDAYS
        TIME_MONTHS
        "Starting time (military format) in hours and minutes.\n"
        "Number of the week in the month\n"
        "First week of the month\n"
        "Last week of the month\n"
        TIME_WDAYS
        TIME_MONTHS
        "Ending time (military format) in hours and minutes.\n"
        "Number of minutes to add during summer time (default is 60).\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_time_dls_rec_set,
         "no clock summer-time",
        NO_DESC
        "Manage the system clock\n"
        "Configure the system to automatically switch to summer time (daylight saving time)\n"
    },

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_localUser_set,
#if 0 
         "username WORD<0-32> [privilege (admin|user|<0-15>)] password PASSWORD",
#else
         "username WORD<0-32> [privilege (admin|user)] password PASSWORD",
#endif
        AAA_LOCAL_USER_DESC
        ARG_LOCAL_USRNAME
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV_ADMIN
        ARG_LOCAL_PRIV_USER
#if 0 
        ARG_LOCAL_PRIV
#endif
        ARG_LOCAL_CLEAR
        ARG_LOCAL_PASSWD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_localUser_set,
#if 0 
         "username WORD<0-32> [privilege (admin|user|<0-15>)] secret [encrypted] PASSWORD",
#else
         "username WORD<0-32> [privilege (admin|user)] secret [encrypted] PASSWORD",
#endif
        AAA_LOCAL_USER_DESC
        ARG_LOCAL_USRNAME
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV_ADMIN
        ARG_LOCAL_PRIV_USER
#if 0 
        ARG_LOCAL_PRIV
#endif
        ARG_LOCAL_SECRET
        ARG_LOCAL_SECRET
        ARG_LOCAL_PASSWD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_localUserNoPasswd_set,
#if 0 
         "username WORD<0-32> [privilege (admin|user|<0-15>)] nopassword",
#else
         "username WORD<0-32> [privilege (admin|user)] nopassword",
#endif
        AAA_LOCAL_USER_DESC
        ARG_LOCAL_USRNAME
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV_ADMIN
        ARG_LOCAL_PRIV_USER
#if 0 
        ARG_LOCAL_PRIV
#endif
        ARG_LOCAL_NOPASSWD
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_localUser_set,
         "no username WORD<0-32>",
        NO_DESC
        AAA_LOCAL_USER_DESC
        ARG_LOCAL_USRNAME
    },
#if 0    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_enblPasswd_set,
#ifdef CONFIG_SYS_AAA
         "enable [privilege <1-15>] password PASSWORD",
#else
         "enable password PASSWORD",
#endif
        AAA_LOCAL_ENBL_DESC
        ARG_LOCAL_CLEAR
#ifdef CONFIG_SYS_AAA
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV
#endif
        "Configure the 'enable' password.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_enblPasswd_set,
#ifdef CONFIG_SYS_AAA
         "enable [privilege <1-15>] secret [encrypted] PASSWORD",
#else
         "enable secret [encrypted] PASSWORD",
#endif
        AAA_LOCAL_ENBL_DESC
        ARG_LOCAL_SECRET
#ifdef CONFIG_SYS_AAA
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV
#endif
        "Specifies the password as encrypted\n"
        "Configure the 'enable' password.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_enblPasswd_set,
#ifdef CONFIG_SYS_AAA
         "no enable [privilege <0-15>]",
#else
         "no enable",
#endif
        NO_DESC
        AAA_LOCAL_ENBL_DESC
#ifdef CONFIG_SYS_AAA
        AAA_LOCAL_PRIV_DESC
        ARG_LOCAL_PRIV
#endif
    },
#endif    
	{
			 NODE_CONFIG,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_sys_localUser_access_method_set,
#ifdef CONFIG_SYS_AAA
			 "username WORD<0-32> access-method (permit|deny) (console|ssh|telnet|http|https)",
#else
			 "username WORD<0-32> access-method (permit|deny) (console|ssh|telnet|http|https)",
#endif
			AAA_LOCAL_USER_DESC
			ARG_LOCAL_USRNAME
			"User access method;eg console, ssh"
			"Allow access"
			"No Access"
			LINE_CONSOLE_DESC 
			LINE_SSH_DESC 
			LINE_TELNET_DESC
			LINE_HTTP_DESC
			LINE_HTTPS_DESC
	},

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_cliLineType_set,
         "line (console|ssh|telnet)",
        LINE_DESC
        LINE_CONSOLE_DESC LINE_SSH_DESC LINE_TELNET_DESC
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_sys_cliHistoryCnt_set,
         "history <"SYS_STR_CONST(BOND_CLI_HISTORY_ENTRY_MIN)"-"SYS_STR_CONST(BOND_CLI_HISTORY_ENTRY_MAX)">",
        "Set the number of history commands\n"
        "Number of commands, range is "SYS_STR_CONST(BOND_CLI_HISTORY_ENTRY_MIN)"-"SYS_STR_CONST(BOND_CLI_HISTORY_ENTRY_MAX)"\n"
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         cmd_ext_parserOnly_install,
         cmd_sys_cliHistoryCnt_set,
         "no history",
        NO_STR
        "disable command history\n"
    },
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_cliSessionTimeout_set,
         "exec-timeout <"SYS_STR_CONST(BOND_SYS_LINE_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SYS_LINE_TIMEOUT_MAX)">",
        TIMEOUT_DESC
        "Timeout after specified minutes (0 means no timeout)\n"
    },
#endif
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_cliPasswordRetry_set,
         "password-thresh <"SYS_STR_CONST(BOND_CLI_PASSRETRY_MIN)"-"SYS_STR_CONST(BOND_CLI_PASSRETRY_MAX)">",
        PASSRETRY_DESC
        "The number of allowed password attempts. (Range: "SYS_STR_CONST(BOND_CLI_PASSRETRY_MIN)"-"SYS_STR_CONST(BOND_CLI_PASSRETRY_MAX)"; 0: no threshold)\n"
    },
    {
         NODE_VTY | NODE_VTY_CONSOLE,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_cliSilentTime_set,
         "silent-time <"SYS_STR_CONST(BOND_CLI_SILENTTIME_MIN)"-"SYS_STR_CONST(BOND_CLI_SILENTTIME_MAX)">",
        SILENT_DESC
        "The number of seconds to disable console response. (Range: "SYS_STR_CONST(BOND_CLI_SILENTTIME_MIN)"-"SYS_STR_CONST(BOND_CLI_SILENTTIME_MAX)"; 0: no silent-time)\n"
    },
#endif
#endif 

#ifdef CONFIG_SYS_APP_TELNETD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_telnetdEnable_set,
         "[no] ip telnet",
        NO_DESC
        IP_DESC
        TELNET_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_telnetd_port_set,
         "ip telnet port <"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)">",
        IP_DESC
        TELNET_STR
        "tcp port number; default port 23\n"
        "Port range "SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)"\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_telnetd_port_set,
         "no ip telnet port",
        NO_DESC
        IP_DESC
        TELNET_STR
        "tcp port number; default port 23\n"
    },
#endif
#ifdef CONFIG_SYS_APP_TELNET
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_telnet_exec,
         "ip telnet (A.B.C.D|HOSTNAME)",
        IP_DESC
        TELNET_STR
        "telnet server"ARG_IPADDR_DESC
        "telnet server host name\n"
    },
#endif
#ifdef CONFIG_USER_SSH_SSHD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_sshdEnable_set,
         "[no] ip ssh",
        NO_DESC
        IP_DESC
        SSH_STR
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_sshd_port_set,
         "ip ssh port <"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)">",
        IP_DESC
        SSH_STR
        "tcp port number; default port 22\n"
        "Port range "SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)"\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_sshd_port_set,
         "no ip ssh port",
        NO_DESC
        IP_DESC
        SSH_STR
        "tcp port number; default port 22\n"
    },
#endif
#ifdef CONFIG_USER_SSH_SSH
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_sys_ssh_exec,
		 "ip ssh (A.B.C.D|HOSTNAME)",
		IP_DESC
		SSH_STR
        "ssh server"ARG_IPADDR_DESC
        "ssh server host name\n"
	},
#endif

#ifdef CONFIG_SYS_APP_HTTPD
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_httpEnable_set,
         "[no] ip http",
        NO_DESC
        IP_DESC
        HTTP_DESC
    },
  #ifdef CONFIG_USER_BOA_WITH_SSL
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_httpsEnable_set,
         "[no] ip https",
        NO_DESC
        IP_DESC
        HTTPS_DESC
    },
  #endif
  #ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_sys_webSessionTimeout_set,
         "ip (http|https) session-timeout <"SYS_STR_CONST(BOND_SYS_LINE_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_SYS_LINE_TIMEOUT_MAX)">",
        IP_DESC
        HTTP_DESC
        HTTPS_DESC
        TIMEOUT_DESC
        "Timeout after specified minutes (0 means no timeout)\n"
    },
  #endif
#endif

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_trunk_hashAlgo_set,
         "lag load-balance (src-dst-mac|src-dst-mac-ip|dst-mac|dst-ip|src-mac|src-ip)",
        LAG_DESC
        TRUNK_LOADBALANCE_DESC
        "LAG load balancing is based on source and destination MAC address.\n"
        "LAG load balancing is based on source and destination of MAC and IP addresses.\n"
        "LAG load balancing is based on destination of MAC.\n"
        "LAG load balancing is based on destination of IP addresses.\n"
        "LAG load balancing is based on source of MAC.\n"
        "LAG load balancing is based on source of IP addresses.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_trunk_hashAlgo_set,
         "no lag load-balance",
        NO_DESC
        LAG_DESC
        TRUNK_LOADBALANCE_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_trunk_staticGroup_set,
         "lag <1-$DEF> mode static",
        LAG_DESC
        ARG_PORT_CHANNEL_NUM
        CHANNEL_GROUP_MODE_DESC
        ARG_CHANNEL_GROUP_STATIC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_trunk_staticGroup_set,
         "no lag",
        NO_DESC
        LAG_DESC
    },
#ifdef CONFIG_SYS_PROTO_LACP
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_systemPriority_set,
         "lacp system-priority <1-65535>",
        LACP_DESC
        "LACP system priority\n"
        "LACP system priority\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_systemPriority_set,
         "no lacp system-priority",
        NO_DESC
        LACP_DESC
        "LACP system priority\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_lacp_group_set,
         "lag <1-$DEF> mode (active|passive)",
        LAG_DESC
        ARG_PORT_CHANNEL_NUM
        CHANNEL_GROUP_MODE_DESC
        ARG_CHANNEL_GROUP_LACP
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_portPriority_set,
         "lacp port-priority <1-65535>",
        LACP_DESC
        "IEEE 802.3 link aggregation port priority\n"
        "Port-priority value\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_portPriority_set,
         "no lacp port-priority",
        NO_DESC
        LACP_DESC
        "IEEE 802.3 link aggregation port priority\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_portTimeout_set,
         "lacp timeout (long|short)",
        LACP_DESC
        "IEEE 802.3 link aggregation port timeout\n"
        "Long timeout value\nShort timeout value\n"
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_lacp_portTimeout_set,
         "no lacp timeout",
        NO_DESC
        LACP_DESC
        "IEEE 802.3 link aggregation port timeout\n"
    },
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS
    
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_tacplus_dftParam_set,
         "tacacs default-config [key TACPLUSKEY] [timeout <"SYS_STR_CONST(BOND_TACPLUS_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_TACPLUS_TIMEOUT_MAX)">]",
        AAA_TACPLUS_SRV_DESC
        AAA_TACPLUS_SRV_DFTPARAM_DESC
        AAA_TACPLUS_KEY_DESC
        ARG_TACPLUS_KEY
        AAA_TACPLUS_TIMEOUT_DESC
        ARG_TACPLUS_TIMEOUT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_tacplus_server_set,
         "tacacs host (A.B.C.D|X:X::X:X|HOSTNAME) [port <0-65535>] [key TACPLUSKEY] [priority <"SYS_STR_CONST(BOND_TACPLUS_PRI_MIN)"-"SYS_STR_CONST(BOND_TACPLUS_PRI_MAX)">] [timeout <"SYS_STR_CONST(BOND_TACPLUS_TIMEOUT_MIN)"-"SYS_STR_CONST(BOND_TACPLUS_TIMEOUT_MAX)">]",
        AAA_TACPLUS_SRV_DESC
        AAA_TACPLUS_HOST_DESC
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
        TCPUDP_PORT_DESC
        ARG_TCPUDP_PORT_DESC
        AAA_TACPLUS_KEY_DESC
        ARG_TACPLUS_KEY
        AAA_SERVER_PRIOR_DESC
        ARG_SERVER_PRIOR
        AAA_TACPLUS_TIMEOUT_DESC
        ARG_TACPLUS_TIMEOUT
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_tacplus_server_set,
         "no tacacs [host (A.B.C.D|X:X::X:X|HOSTNAME)]",
        NO_DESC
        AAA_TACPLUS_SRV_DESC
        AAA_TACPLUS_HOST_DESC
        ARG_IPADDR_DESC
        ARG_IPV6ADDR_DESC
        ARG_HOSTNAME_DESC
    },
#endif 

#ifdef CONFIG_SYS_PROTO_UDLD
   
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_interval_set,
         "udld message time <"SYS_STR_CONST(BOND_UDLD_INTERVAL_MIN)"-"SYS_STR_CONST(BOND_UDLD_INTERVAL_MAX)">",
        UDLD_GLOBAL_DESC
        UDLD_MSG_DESC
        UDLD_MSG_TIME_DESC
        UDLD_MSG_TIME_VAL_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_port_enable_set,
         "[no] udld",
        NO_STR
        UDLD_PORT_DESC
        UDLD_PORT_PORT_DESC
    },
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_port_aggressive_set,
         "[no] udld aggressive",
        NO_STR
        UDLD_PORT_DESC
        UDLD_PORT_PORT_DESC
        UDLD_PORT_AGSV_DESC
    },
#endif

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_create_set,
         "[no] vlan VLAN-LIST",
        NO_DESC
        VLAN_DESC
        ARG_VLANLIST_DESC
    },
#ifdef CONFIG_SYS_L3_ROUTE
    {
         NODE_VLAN_DB | NODE_IF_VLAN,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_vlan_name_set,
         "name NAME",
        NAME_DESC
        "VLAN name\n"
    },
    {
         NODE_VLAN_DB,
         ATTR_PRIV_15,
         cmd_ext_condHidden_install,
         cmd_vlan_name_set,
         "no name",
        NO_DESC
        NAME_DESC
    },
#else
    {
         NODE_VLAN_DB,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_name_set,
         "name NAME",
        NAME_DESC
        "VLAN name\n"
    },
    {
         NODE_VLAN_DB,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_name_set,
         "no name",
        NO_DESC
        NAME_DESC
    },
#endif
#ifdef CONFIG_SYS_VLAN_QINQ
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMode_set,
         "switchport mode (access|tunnel|hybrid)",
        SWITCHPORT_DESC
        "VLAN mode\n"
        "Access port\nDot1Q Tunnel port\nHybrid port\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMode_set,
         "switchport mode trunk [uplink]",
        SWITCHPORT_DESC
        "VLAN mode\n"
        "Trunk port\n"
        "Uplink mode\n"
    },
#else
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMode_set,
         "switchport mode (access|hybrid)",
        SWITCHPORT_DESC
        "VLAN mode\n"
        "Access port\nHybrid port\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMode_set,
         "switchport mode trunk",
        SWITCHPORT_DESC
        "VLAN mode\n"
        "Trunk port\n"
    },
#endif
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portMode_set,
         "no switchport mode",
        NO_DESC
        SWITCHPORT_DESC
        "VLAN mode\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portPvid_set,
         "switchport hybrid pvid <1-4094>",
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        "Port configured VLAN ID\n"
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portPvid_set,
         "no switchport hybrid pvid",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        "Port configured VLAN ID\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portIgrFilterEnable_set,
         "[no] switchport hybrid ingress-filtering",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        "Ingress filtering configuration\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portAcceptFrameType_set,
         "switchport hybrid acceptable-frame-type (all|tagged-only|untagged-only)",
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        "VLAN accept frame type\n"
        "Accept tagged and untagged frames\nOnly accept tagged frames\nOnly accept untagged and priority-tagged frames\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portAcceptFrameType_set,
         "no switchport hybrid acceptable-frame-type",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        "VLAN accept frame type\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portGeneralVlan_set,
         "switchport hybrid allowed vlan add VLAN-LIST [(tagged|untagged)]",
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        SWITCHPORT_ALLOW_DESC
        VLAN_DESC
        "Configure to add VLANs to a hybrid port.\n"
        ARG_VLANLIST_DESC
        "Tagged\nUntagged\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portGeneralVlan_set,
         "switchport hybrid allowed vlan remove VLAN-LIST",
        SWITCHPORT_DESC
        SWITCHPORT_GENERAL_DESC
        SWITCHPORT_ALLOW_DESC
        VLAN_DESC
        "Configure to removes VLANs from a hybrid port.\n"
        ARG_VLANLIST_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portAccessVlan_set,
         "switchport access vlan <1-4094>",
        SWITCHPORT_DESC
        SWITCHPORT_ACCESS_DESC
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portAccessVlan_set,
         "no switchport access vlan",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_ACCESS_DESC
        VLAN_DESC
    },
#ifdef CONFIG_SYS_VLAN_QINQ
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portDot1qtunnelVlan_set,
         "switchport tunnel vlan <1-4094>",
        SWITCHPORT_DESC
        SWITCHPORT_DOT1QTUNNEL_DESC
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portDot1qtunnelVlan_set,
         "no switchport tunnel vlan",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_DOT1QTUNNEL_DESC
        VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         cmd_ext_dynDef_install,
         cmd_vlan_portDot1qTunnelTpid_set,
         "switchport vlan tpid ($DEF1|$DEF2|$DEF3|$DEF4)",
        SWITCHPORT_DESC
        VLAN_DESC
        "TPID configuration\n"
        "Tag-protocol-id $DEF1\n"
        "Tag-protocol-id $DEF2\n"
        "Tag-protocol-id $DEF3\n"
        "Tag-protocol-id $DEF4\n"
    },
#endif
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portTrunkVlan_set,
         "switchport trunk native vlan <1-4094>",
        SWITCHPORT_DESC
        SWITCHPORT_TRUNK_DESC
        SWITCHPORT_TRUNK_DESC
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portTrunkVlan_set,
         "no switchport trunk native vlan",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_TRUNK_DESC
        SWITCHPORT_TRUNK_DESC
        VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portTrunkVlan_set,
         "switchport trunk allowed vlan (add|remove) (VLAN-LIST|all)",
        SWITCHPORT_DESC
        SWITCHPORT_TRUNK_DESC
        SWITCHPORT_ALLOW_DESC
        VLAN_DESC
        "Specify which VLAN to add to the port.\nSpecify the VLAN to remove from port\n"
        ARG_VLANLIST_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portDfltVlanTagged_set,
         "[no] switchport default-vlan tagged",
        NO_DESC
        SWITCHPORT_DESC
        VLAN_DFLTVLAN_DESC
        "Tagging\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portForbidDfltVlan_set,
         "[no] switchport forbidden default-vlan",
        NO_DESC
        SWITCHPORT_DESC
        SWITCHPORT_FORBID_DESC
        VLAN_DFLTVLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portForbidVlan_set,
         "switchport forbidden vlan (add|remove) VLAN-LIST",
        SWITCHPORT_DESC
        SWITCHPORT_FORBID_DESC
        VLAN_DESC
        "Specify which VLAN to add to the port.\nSpecify the VLAN to remove from port\n"
        ARG_VLANLIST_DESC
    },
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_pvlanGroup_set,
         "vlan protocol-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_PROTO_GROUP_NUM)"> frame-type (ethernet_ii|llc_other|snap_1042) protocol-value VALUE",
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        "Protocol group\n"
        "Group index\n"
        "Frame type used by this protocol\n"
        "Ethernet II\n802.3 LLC other\n802.3 SNAP 1042\n"
        "Protocol value\n"
        "Protocol value (0x0600-0xFFFE)\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_pvlanGroup_del,
         "no vlan protocol-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_PROTO_GROUP_NUM)">",
        NO_DESC
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        "Protocol group\n"
        "Group index\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_pvlanVlan_set,
         "vlan protocol-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_PROTO_GROUP_NUM)"> vlan <1-4094>",
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        "Protocol group\n"
        "Group index\n"
        VLAN_DESC
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_pvlanVlan_del,
         "no vlan protocol-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_PROTO_GROUP_NUM)">",
        NO_DESC
        VLAN_DESC
        VLAN_PROTOCOL_BASED_DESC
        "Protocol group\n"
        "Group index\n"
    },
#endif
#ifdef CONFIG_SYS_IP_SUBNET
		{
			 NODE_CONFIG,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_vlan_ip_subnet_group_set,
			 "vlan ip-subnet-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_IP_SUBNET_GROUP_NUM)"> ip A.B.C.D A.B.C.D vlan <1-4094> [priority <0-7>]",
			VLAN_DESC
			VLAN_IP_SUBNET_BASED_DESC
			"IP subnet group\n"
			"Group index\n"
			"IPv4 Address\n"
			ARG_IPADDR_DESC
			"Netmask\n"
			VLAN_DESC
			ARG_VLAN_DESC
			"802.1q priority\n"
			"priority value (e.g. 0)\n"
		},
		{
			 NODE_CONFIG,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_vlan_ip_subnet_group_del,
			 "no vlan ip-subnet-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_IP_SUBNET_GROUP_NUM)">",
			NO_DESC
			VLAN_DESC
			VLAN_IP_SUBNET_BASED_DESC
			"IP subnet group\n"
			"Group index\n"
		},
		{
			 NODE_IF,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_vlan_ip_subnet_vlan_port_bind_group,
			 "[no] vlan ip-subnet-vlan group <1-"SYS_STR_CONST(CAPA_VLAN_IP_SUBNET_GROUP_NUM)"> enable",
			NO_DESC
			VLAN_DESC
			VLAN_IP_SUBNET_BASED_DESC
			"IP subnet group\n"
			"Group index\n"
			"enable ip-subbet-vlan\n"
		},
		{
			 NODE_IF,
			 ATTR_PRIV_15,
			 NULL,
			 cmd_vlan_ip_subnet_vlan_port,
			 "[no] vlan ip-subnet-vlan enable",
			NO_DESC
			VLAN_DESC
			VLAN_IP_SUBNET_BASED_DESC
			"enable ip-subbet-vlan\n"
		},
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanState_set,
         "voice-vlan",
        VLAN_VOICEVLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanState_set,
         "no voice-vlan",
        NO_DESC
        VLAN_VOICEVLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanVid_set,
         "voice-vlan vlan <2-4094>",
        VLAN_VOICEVLAN_DESC
        VLAN_DESC
        "Specifies the Voice VLAN Identifier\n"
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanVid_set,
         "no voice-vlan vlan",
        NO_DESC
        VLAN_VOICEVLAN_DESC
        VLAN_DESC
    },

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanPri_set,
         "voice-vlan cos <0-7> [remark]",
        VLAN_VOICEVLAN_DESC
        "Voice VLAN Class Of Service settings\n"
        "Specify the voice VLAN Class Of Service\n"
        "voice VLAN Remark setting\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanPri_set,
         "no voice-vlan cos",
        NO_DESC
        VLAN_VOICEVLAN_DESC
        "Voice VLAN Class Of Service settings\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanAging_set,
         "voice-vlan aging-time <"SYS_STR_CONST(BOND_VLAN_VOICE_AGING_MIN)"-"SYS_STR_CONST(BOND_VLAN_VOICE_AGING_MAX)">",
        VLAN_VOICEVLAN_DESC
        "Voice VLAN aging time settings\n"
        "Specify the aging time in minutes\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanAging_set,
         "no voice-vlan aging-time",
        NO_DESC
        VLAN_VOICEVLAN_DESC
        "Voice VLAN aging time settings\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanOui_add,
         "voice-vlan oui-table A:B:C [DESCRIPTION]",
        VLAN_VOICEVLAN_DESC
        "OUI-Table configuration\n"
        "OUI address(xx:xx:xx)\n"
        "OUI description string\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_voiceVlanOui_del,
         "no voice-vlan oui-table [A:B:C]",
        VLAN_VOICEVLAN_DESC
        "OUI-Table configuration\n"
        "OUI address(xx:xx:xx)\n"
    },
        {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portVoiceVlanMode_set,
         "voice-vlan mode (auto|manual)",
        VLAN_VOICEVLAN_DESC
        "Voice Member Port Join Mode\n"
        "Voice Member Port Join Voice VLAN Automatically\nVoice Member Port Join Voice VLAN Manually By Administrator\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portVoiceVlanMode_set,
         "no voice-vlan mode",
        NO_DESC
        VLAN_VOICEVLAN_DESC
        "Voice Member Port Join Mode\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portvoiceVlancosMode_set,
         "voice-vlan cos (src|all)",
        VLAN_VOICEVLAN_DESC
        "Voice VLAN COS mode configuration\n"
        "QoS attributes are applied only on packets from IP phones\n"
        "QoS attributes are applied on all packets that are classified to the Voice VLAN"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portvoiceVlancosMode_set,
         "no voice-vlan cos",
        NO_DESC
        VLAN_VOICEVLAN_DESC
        "Voice VLAN COS mode configuration\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_portVoiceVlanEnable_set,
         "[no] voice-vlan",
        NO_DESC
        VLAN_VOICEVLAN_DESC
    },
#endif
#if defined(CONFIG_SYS_MANAGEMENT_VLAN) && !defined(CONFIG_SYS_MANAGEMENT_VLAN)
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mgmVid_set,
         "management-vlan vlan <1-4094>",
        VLAN_MGMTVLAN_DESC
        "VLAN configuration\n"
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mgmVid_set,
         "no management-vlan",
        NO_DESC
        VLAN_MGMTVLAN_DESC
    },
#endif
#ifdef CONFIG_SYS_MACVLAN
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_group_set,
         "vlan mac-vlan group <"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MIN)"-"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MAX)"> A:B:C:D:E:F mask <9-48>",
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        "MAC group\n"
        "Specify group ID\n"
        "Source MAC Address\n"
        "MAC address mask\n"
        "9 to 48 bits. 48 bits means full match.\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_group_set,
         "no vlan mac-vlan group A:B:C:D:E:F mask <9-48>",
        NO_DESC
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        "MAC group\n"
        "Source MAC Address\n"
        "MAC address mask\n"
        "9 to 48 bits. 48 bits means full match."
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_mapping_set,
         "vlan mac-vlan group <"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MIN)"-"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MAX)"> vlan <1-4094>",
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        "MAC group\n"
        "Specify group ID to map\n"
        "Specify mapping VLAN ID\n"
        ARG_VLAN_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_mac_mapping_set,
         "no vlan mac-vlan [group <"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MIN)"-"SYS_STR_CONST(BOND_VLAN_MAC_GROUP_MAX)">]",
        NO_DESC
        VLAN_DESC
        VLAN_MAC_BASED_DESC
        "MAC group\n"
        "Specify group ID to remove mapping\n"
    },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanState_set,
         "[no] surveillance-vlan",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanVid_set,
         "surveillance-vlan vlan <2-4094>",
        VLAN_SURVEILLANCE_DESC
        VLAN_DESC
        "Specifies the Surveillance VLAN ID\n"
        ARG_VLAN_DESC
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanVid_set,
         "no surveillance-vlan vlan",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
        VLAN_DESC
    },

    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanPri_set,
         "surveillance-vlan cos <0-7> [(remark)]",
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN Class Of Service settings\n"
        "Specify the Surveillance VLAN Class Of Service\n"
        "Surveillance VLAN Remark setting\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanPri_set,
         "no surveillance-vlan cos",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN Class Of Service settings\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanAging_set,
         "surveillance-vlan aging-time <"SYS_STR_CONST(BOND_VLAN_SURVEILLANCE_AGING_MIN)"-"SYS_STR_CONST(BOND_VLAN_SURVEILLANCE_AGING_MAX)">",
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN aging time settings\n"
        "Specify the aging time in minutes\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanAging_set,
         "no surveillance-vlan aging-time",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN aging time settings\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanOui_set,
         "surveillance-vlan oui-table A:B:C [DESCRIPTION]",
        VLAN_SURVEILLANCE_DESC
        "OUI-Table configuration\n"
        "OUI address(xx:xx:xx)\n"
        "OUI description string\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlanOui_set,
         "no surveillance-vlan oui-table [A:B:C]",
        VLAN_SURVEILLANCE_DESC
        "OUI-Table configuration\n"
        "OUI address(xx:xx:xx)\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_portEnable_set,
         "[no] surveillance-vlan",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_portMode_set,
         "surveillance-vlan mode (auto|manual)",
        VLAN_SURVEILLANCE_DESC
        "Surveillance Member Port Join Mode\n"
        "Surveillance Member Port Join Voice VLAN Automatically\nVoice Member Port Join Voice VLAN Manually By Administrator\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_portMode_set,
         "no surveillance-vlan mode",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
        "Surveillance Member Port Join Mode\n"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_portCosMode_set,
         "surveillance-vlan cos (src|all)",
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN COS mode configuration\n"
        "QoS attributes are applied only on packets from IP phones\n"
        "QoS attributes are applied on all packets that are classified to the Surveillance VLAN"
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_surveillance_vlan_portCosMode_set,
         "no surveillance-vlan cos",
        NO_DESC
        VLAN_SURVEILLANCE_DESC
        "Surveillance VLAN COS mode configuration\n"
    },

#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_enable_set,
		 "[no] erps",
		NO_DESC
		ERPS_DESC
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_inst_set,
		 "erps instance <0-"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)">",
		ERPS_DESC
		"Sets erps parameters of instances\n"
		"Instance ID (0~"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)")\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_inst_set,
		 "no erps instance  <0-"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)">",
		NO_DESC
		ERPS_DESC
		"Sets erps parameters of instances\n"
		"Instance ID (0~"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)")\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ctrlVlan_set,
		 "control-vlan <1-4094>",
		"Sets erps control vlan of instances\n"
		ARG_VLAN_DESC
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ctrlVlan_set,
		 "no control-vlan",
		NO_DESC
		"Sets erps control vlan of instances\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_wtrTimer_set,
		 "wtr-timer <1-12>",
		"Sets erps wtr timer of instances\n"
		"Valid range is 1-12 Min. Default is 5 Min.\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_wtrTimer_set,
		 "no wtr-timer",
		NO_DESC
		"Sets erps wtr timer of instances\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_guardTimer_set,
		 "guard-timer <100-2000>",
		"Sets erps guard timer of instances\n"
		"Valid range is 100-2000 ms. Default is 500 ms.\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_guardTimer_set,
		 "no guard-timer",
		NO_DESC
		"Sets erps guard timer of instances\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_workMode_set,
		 "work-mode (revertive|non_revertive)",
		"Sets erps work mode of instances\n"
		"Revertive mode\n" "Non revertive mode\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringId_set,
		 "ring <1-239>",
		"Sets erps ring id of instances\n"
		"Valid range is 1-239 \n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringLevel_set,
		 "ring-level <0-1>",
		"Sets erps ring level of instances\n"
		"Valid range is 0-1,(0-master ring, 1-sub ring)\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringPort_set,
		 "port0 IF_PORTS [(owner|neighbour|next-neighbour)]",
		"link 0\n"
		ARG_IF_PORT
		"onwer\n"
		"neihbour\n"
		"next-neighbour\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringPort_set,
		 "port1 IF_PORTS [(owner|neighbour|next-neighbour)]",
		"link 1\n"
		ARG_IF_PORT
		"onwer\n"
		"neihbour\n"
		"next-neighbour\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringPort_set,
		 "no (port0|port1)",
		NO_DESC
		"link 0\n" "link 1\n"
	},

	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ring_protected_instance_set,
		 "protected-instance <0-"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)">",
		"Sets erps protected instance(spanning-tree mst instance) of instances\n"
		"Instance ID (0~"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)")\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ring_protected_instance_set,
		 "no protected-instance <0-"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)">",
		NO_DESC
		"Sets erps protected instance(spanning-tree mst instance) of instances\n"
		"Instance ID (0~"SYS_STR_CONST(BOND_ERPS_MST_INSTANCE_MAX)")\n"
	},

	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_mel_set,
		 "mel <0-7>",
		"Sets mel level of erps instances\n"
		"Valid range is 0-7\n"
	},
	{
		 NODE_ERPS_INST,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_erps_ringState_set,
		 "ring (enable|disable)",
		"Sets erps ring state of instances\n"
		"Enable ring\n" "Disable ring\n"
	},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_proto_enable_set,
		 "[no] loopback",
		NO_DESC
		LOOPBACK_DESC
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_detectionTime_set,
		 "loopback detection-time <1-32767>",
		LOOPBACK_DESC
		"Detection time set\n"
		"Valid range is 1-32767\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_resumeTime_set,
		 "loopback resume-time <10-65535>",
		LOOPBACK_DESC
		"Resume time set\n"
		"Valid range is <10-65535>\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_port_enable_set,
		 "interface IF_PORT loopbacks enable",
		IF_DESC
		ARG_IF_PORT
		"Loopback\n"
		"Enbale\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_port_enable_set,
		 "no interface IF_PORT loopbacks enable",
		NO_DESC
		IF_DESC
		ARG_IF_PORT
		"Loopback\n"
		"Enbale\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_port_resume_mode_set,
		 "interface IF_PORT loopbacks resume-mode (automation|manual)",
		IF_DESC
		ARG_IF_PORT
		LOOPBACK_DESC
		"Resume mode\n"
		"Automatic resume\n"
		"Manual resume\n"
	},
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_loopback_port_resume_manual_set,
		 "interface IF_PORT loopbacks resume enable",
		IF_DESC
		ARG_IF_PORT
		LOOPBACK_DESC
		"Resume manual\n"
		"Enable\n"
	},
#endif

#ifdef CONFIG_SYS_POE
	 
	{
		 NODE_IF_NML,
		 ATTR_PRIV_15,
		 cmd_ext_deprecated_install,
		 cmd_poe_portEnable_set,
		 "[no] poe",
		NO_DESC
		POE_DESC
	},
	#if 0
	{
		 NODE_IF_NML,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_poe_portIcut_set,
		 "poe power-limit <"SYS_STR_CONST(BOND_POE_PORT_ICUT_MIN)"-"SYS_STR_CONST(BOND_POE_PORT_ICUT_MAX)">",
		POE_DESC
		"power limit\n"
		"Valid range is "SYS_STR_CONST(BOND_POE_PORT_ICUT_MIN)"-"SYS_STR_CONST(BOND_POE_PORT_ICUT_MAX)"W . Default is "SYS_STR_CONST(DFLT_POE_PORT_ICUT)".\n"
	},
	#endif
    {
         NODE_IF_NML,
         ATTR_PRIV_15,
         cmd_ext_deprecated_install,
         cmd_poe_portSchedule_set,
         "[no] poe schedule week (sun|mon|tue|wed|thu|fri|sat) hour <0-23>",
		NO_DESC
		POE_DESC
        "schedule\n"
		"week\n"
		"Sunday\nMonday\nTuesday\nWednesday\nThursday\nFriday\nSaturday\n"
        "hours\n"
        "Number of hours\n"
    },
	{
		 NODE_IF_NML,
		 ATTR_PRIV_15,
		 cmd_ext_deprecated_install,
		 cmd_poe_portSchedule_set,
		 "no poe schedule",
		NO_DESC
		POE_DESC
		"schedule\n"
	},
#ifdef CONFIG_SYS_POE_SWDOG
	{
		 NODE_IF_NML,
		 ATTR_PRIV_15,
		 cmd_ext_deprecated_install,
		 cmd_poe_port_watch_dog_Enable_set,
		 "[no] poe watch-dog",
		NO_DESC
		POE_DESC
		"poe port watch dog\n"
	},
#endif
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	{
		 NODE_CONFIG,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_node_mode,
		 "cwmp",
		"CWMP protocol\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_cpe_enable,
		 "[no] cpe",
		NO_DESC
		"CPE device enable\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_set,
		 "local interface vlan VLAN-LIST",
		"CPE device local config\n"
		"Interface Vlan \n"
		"CPE device local config\n"
		VLAN_DESC
		ARG_VLANLIST_DESC
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_set,
		 "local port <"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)">",
		"CPE device local config\n"
		"TCP port number\n"
		"Port range "SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MIN)"-"SYS_STR_CONST(BOND_SNMP_NOTIFY_PORT_MAX)"\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_set,
		 "local username WORD<1-32> password WORD<1-32>",
		"CPE device local config\n"
		"User name to be added\n"
		"Specify user name to be added\n"
		"Password of the user\n"
		"Specify password of the user\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_set,
		 "local authentication (digest | basic)",
		"CPE device local config\n"
		AUTH_INTF_DESC
		"authentication type is DM5\n"
		"authentication type is basic\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_local_set,
		 "local logging level (critic|warning|notice|info|debug)",
		"CPE device local config\n"
		"Logging info config\n"
		"Logging level config\n"
		"Level critic \n"
		"Level warning \n"
		"Level notice \n"
		"Level info \n"
		"Level debug \n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_acs_set,
		 "acs url WORD<1-64>",
		"CPE device acs config\n"
		"url remote ip address config\n"
		"acs ip address, eg:http://192.168.0.100:80/acs \n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_acs_set,
		 "acs username WORD<1-32> password WORD<1-32>",
		"CPE device acs config\n"
		"User name to be added\n"
		"Specify user name to be added\n"
		"Password of the user\n"
		"Specify password of the user\n"
	},
	{
		 NODE_CWMP,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_easycwmp_acs_set,
		 "[no] acs periodic [interval <0-300>]",
		NO_DESC
		"CPE device acs config\n"
		"periodic Heartbeat config\n"
		"Timing interval\n"
		"rang<0~300>, define 100\n"
	},
#endif

    {0, 0, NULL, NULL, NULL, NULL}
};

#ifdef CONFIG_SYS_PLAT_DBGMOD

const cmd_inst_tbl_t cmdDebugTbl[] =
{
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_debug_set,
         "[no] debug acl [(all|common|reserve)]",
        NO_DESC
        "debug\n"
        "acl\n"
        "all\n"
        "common\n"
        "reserve\n"
    },
#ifdef CONFIG_SYS_MGMT_ACL
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_debug_set,
         "[no] debug acl [(mgmt-acl|)]",
        NO_DESC
        "debug\n"
        "acl\n"
        "management acl\n"
    },
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_acl_debug_set,
         "[no] debug acl [(user-defined|)]",
        NO_DESC
        "debug\n"
        "acl\n"
        "user-defined\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_auth_debug_set,
         "[no] debug auth [(all|dot1x|macauth|webauth)]",
        NO_DESC
        "debug\n"
        "auth\n"
        "all\n"
        "dot1x\n"
        "macauth\n"
        "webauth\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_dai_debug_set,
         "[no] debug ip arp inspection",
        NO_DESC
        "debug\n"
        IP_DESC
        ARP_DESC
        DAI_DESC
    },
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_ds_debug_set,
         "[no] debug ip dhcp snooping",
        NO_DESC
        "debug\n"
        IP_DESC
        DHCP_DESC
        DHCP_SNOOPING_DESC
    },
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_isg_debug_set,
         "[no] debug ip source binding",
        NO_DESC
        "debug\n"
        IP_DESC
        ISG_DESC
        ISG_BINDING_DESC
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l2_debug_set,
         "[no] debug l2",
        NO_DESC
        "debug\n"
        "l2\n"
    },
#ifdef CONFIG_SYS_L3_ROUTE
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_l3_debug_set,
         "[no] debug l3 [(all|intf|route|fib|fpm)]",
        NO_DESC
        "debug\n"
        "l3\n"
        "all\n"
        "interface\n"
        "route\n"
        "forwarding information base\n"
        "forwarding plane manager\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_lldp_debug_set,
         "[no] debug lldp [(all|common|packet|poe|rx|tx)]",
        NO_DESC
        "debug\n"
        "lldp\n"
        "all\n"
        "common\n"
        "packet\n"
        "poe\n"
        "rx\n"
        "tx\n"
    },
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_psecure_debug_set,
         "[no] debug psecure",
        NO_DESC
        "debug\n"
        "port security\n"
    },
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_rmon_debug_set,
         "[no] debug rmon",
        NO_DESC
        "debug\n"
        "rmon\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_STP
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_debug_stp_set,
         "[no] debug spanning-tree",
        NO_DESC
        "debug\n"
        STP_DESC
        "all\n"
        "general\n"
        "root\n"
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_time_debug_set,
         "[no] debug time",
        NO_DESC
        "debug\n"
        "time\n"
    },
#ifdef CONFIG_SYS_PROTO_UDLD
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_udld_debug_set,
         "[no] debug udld",
        NO_DESC
        "debug\n"
        "udld\n"
    },
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(all|common)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "all\n"
        "common\n"
    },
#ifdef CONFIG_SYS_MACVLAN
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(mac|)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "mac based vlan\n"
    },
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(protocol|)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "protocol vlan\n"
    },
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(voice|)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "voice vlan\n"
    },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(surveillance|)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "surveillance vlan\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_vlan_debug_set,
         "[no] debug vlan [(gvrp|)]",
        NO_DESC
        "debug\n"
        "vlan\n"
        "gvrp protocol\n"
    },
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
	{
		 NODE_PRIVMODE,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_debug_erps_set,
		 "[no] debug erps",
		NO_DESC
		"debug\n"
		ERPS_DESC
	},
#endif
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_debug_lag_set,
         "[no] debug lag",
        NO_DESC
        "debug\n"
        "lag\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_debug_mcast_set,
         "[no] debug igmp",
        NO_DESC
        "debug\n"
        "igmp\n"
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_debug_platform_set,
         "[no] debug platform",
        NO_DESC
        "debug\n"
        "platform\n"
    },
#ifdef CONFIG_SYS_DHCP_SERVER
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_dhcp_server_debug_set,
         "[no] debug dhcp-server [(all|common|packet|cmd|rx|tx)]",
        NO_DESC
        "debug\n"
        "dhcp server\n"
        "all\n"
        "common\n"
        "packet info\n"
        "cmd config\n"
        "rx\n"
        "tx\n"
    },
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	{
		 NODE_PRIVMODE,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_rip_debug,
		 "debug rip (events|packet|zebra|all)",
		"Debug\n"
		"Rip debug\n"
		"Debug type :events\n"
		"Debug type :packet\n"
		"Debug type :zebra\n"
		"Debug type :all\n"
	},
	{
		 NODE_PRIVMODE,
		 ATTR_PRIV_15,
		 NULL,
		 cmd_rip_debug,
		 "no debug rip (events|packet|zebra|all)",
		NO_DESC
		"Debug\n"
		"Rip debug\n"
		"Debug type :events\n"
		"Debug type :packet\n"
		"Debug type :zebra\n"
		"Debug type :all\n"
	},
#endif
    
    {0, 0, NULL, NULL, NULL, NULL}
};
#endif

const cmd_save_tbl_t cmdSaveTbl[] =
{

    {CAT_ID_TRUNK,      cmd_trunk_save                          },
#ifdef CONFIG_SYS_PROTO_LACP
    {CAT_ID_LACP,       cmd_lacp_save                           },
#endif

#ifdef CONFIG_SYS_JUMBO_FRAME
    
    {CAT_ID_SWITCH,     cmd_switch_save                         },
#endif

    {CAT_ID_SYSTEM,     cmd_sys_save                            },
    {CAT_ID_SYSTEM,     cmd_sys_auth_save                       },

    {CAT_ID_TIME,       cmd_time_save                          },

    {CAT_ID_VLAN,       cmd_vlan_save                          },

#ifdef CONFIG_SYS_VOICEVLAN
    
    {CAT_ID_VLAN_VOICE,     cmd_vlan_voice_save                },
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    
    {CAT_ID_VLAN_PROTO,     cmd_vlan_proto_save                },
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	
	{CAT_ID_VLAN_IP_SUBNET,  cmd_vlan_ip_subnet_save    	   },
#endif

#ifdef CONFIG_SYS_MACVLAN
    
    {CAT_ID_VLAN_MAC,       cmd_vlan_mac_save                  },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    
    {CAT_ID_VLAN_SURVEILLANCE,  cmd_surveillance_vlan_save               },
#endif

#ifdef CONFIG_SYS_PORT_SECURITY
     
    {CAT_ID_PORT,       cmd_psecure_save},
#endif

    {CAT_ID_RATE,       cmd_rate_save                           },

    {CAT_ID_PORT,       cmd_port_save                           },

#ifdef CONFIG_SYS_PROTO_STP
    
    {CAT_ID_STP,        cmd_stp_save                            },
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
    
    {CAT_ID_ERPS,        cmd_erps_save                            },
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
    
    {CAT_ID_DOS,        cmd_dos_save                            },
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    {CAT_ID_IGMP,       cmd_igmp_save                           },

  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    
    {CAT_ID_MLD,         cmd_mld_save                           },
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
    
    {CAT_ID_UDLD,       cmd_udld_save                           },
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
    
    {CAT_ID_LLDP,       cmd_lldp_save                           },
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {CAT_ID_LLDP_MED,   cmd_lldp_med_save                       },
  #endif
#endif

#ifdef CONFIG_SYS_UI_SNMP
    
    {CAT_ID_SNMP,       cmd_snmp_save                           },
  #ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    {CAT_ID_RMON,       cmd_rmon_save                           },
  #endif
#endif

#ifdef CONFIG_SYS_LIB_RADIUS
    
    {CAT_ID_RADIUS,     cmd_radius_save                         },
#endif

#ifdef CONFIG_SYS_AAA
    
    {CAT_ID_AAA,        cmd_aaa_save                            },
#endif

#ifdef CONFIG_SYS_AAA
    
    {CAT_ID_AUTHMGR,    cmd_authmgr_save                        },
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS
    
    {CAT_ID_TACPLUS,    cmd_tacplus_save                        },
#endif

    {CAT_ID_SYSTEM,     cmd_sys_line_save                       },

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    
    {CAT_ID_ACL,        cmd_acl_save                            },
#endif

    {CAT_ID_QOS,        cmd_qos_save                            },

#ifdef CONFIG_SYS_PROTO_ISG
    
    {CAT_ID_ISG,        cmd_isg_save                            },
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    
    {CAT_ID_DHCP_SNOOPING, cmd_dhcp_snooping_save               },
#endif

#ifdef CONFIG_SYS_PROTO_DAI
    
    {CAT_ID_DAI,        cmd_dai_save                            },
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
    
    {CAT_ID_GVRP,       cmd_gvrp_save                           },
#endif

#ifdef CONFIG_SYS_PROTO_GARP
    
    {CAT_ID_GMRP,       cmd_gmrp_save                           },
#endif

#ifdef CONFIG_SYS_SYSLOG
    
    {CAT_ID_SYSLOG,     cmd_log_save                            },
#endif

#ifdef CONFIG_SYS_MGMT_ACL
    
    {CAT_ID_MGMTACL,    cmd_acl_mgmt_save                       },
#endif

#ifdef CONFIG_SYS_LIB_CUSTOM
    
    {CAT_ID_CUSTOM,     NULL                                    },
#endif

#ifdef CONFIG_SYS_L3_ROUTE
    
    {CAT_ID_IF_LO,      NULL                                    },

    {CAT_ID_IF_VLAN,    NULL                                    },
#endif

    {CAT_ID_IF_PORT,    NULL                                    },

    {CAT_ID_LINE,       NULL                                    },

#ifdef CONFIG_SYS_MIRROR
    {CAT_ID_MIRROR,     cmd_mirror_save                         },
#endif

    {CAT_ID_L2,         cmd_l2_save                             },

#ifdef CONFIG_SYS_L3_ROUTE
    
    {CAT_ID_L3,         cmd_l3_save                             },
#ifdef CONFIG_SYS_L3_ROUTE_ZEBRA
	{CAT_ID_L3_ROUTE_ZEBRA, cmd_l3_router_save 					},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	{CAT_ID_L3_ROUTE_RIP,   cmd_l3_router_rip_save				},
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	{CAT_ID_L3_ROUTE_OSPF,	cmd_l3_router_ospf_save				},
#endif
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    {CAT_ID_IGMP,       cmd_igmp_group_save                     },
#endif
#ifdef CONFIG_SYS_POE
	
	{CAT_ID_POE,		cmd_poe_save							},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	
	{CAT_ID_DHCP_SERVER,		cmd_dhcp_server_save	    	},
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
		
	{CAT_ID_LOOPBACCK,		 cmd_loopback_save					},
#endif

    {-1,                NULL                                    }
};

const cmd_save_intfPort_tbl_t cmdSaveIntfPortTbl[] =
{

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    
    {CAT_ID_ACL,        cmd_acl_port_save                       },
#endif

    {CAT_ID_TRUNK,      cmd_trunk_port_save                     },
#ifdef CONFIG_SYS_PROTO_LACP
    {CAT_ID_LACP,       cmd_lacp_port_save                      },
#endif

#ifdef CONFIG_SYS_EEE
    
    {CAT_ID_EEE,        cmd_eee_port_save                       },
#endif

    {CAT_ID_VLAN,       cmd_vlan_port_save                      },

#ifdef CONFIG_SYS_VOICEVLAN
    
    {CAT_ID_VLAN_VOICE,       cmd_vlan_voice_port_save          },
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    
    {CAT_ID_VLAN_PROTO,       cmd_vlan_proto_port_save          },
#endif

#ifdef CONFIG_SYS_IP_SUBNET
	
	{CAT_ID_VLAN_IP_SUBNET, 	cmd_vlan_ip_subnet_port_save	},
#endif

#ifdef CONFIG_SYS_MACVLAN
    
    {CAT_ID_VLAN_MAC,           cmd_vlan_mac_port_save          },
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    
    {CAT_ID_VLAN_SURVEILLANCE,  cmd_surveillance_vlan_port_save },
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    
    {CAT_ID_AUTHMGR,    cmd_authmgr_port_save                   },
#endif

#ifdef CONFIG_SYS_PROTO_STP
    
    {CAT_ID_STP,        cmd_stp_port_save                       },
#endif

    {CAT_ID_RATE,       cmd_rate_port_save                      },

    {CAT_ID_PORT,       cmd_port_port_save                      },

#ifdef CONFIG_SYS_PORT_SECURITY
    
    {CAT_ID_PORT_SECURITY, cmd_psecure_port_save               },
#endif

    {CAT_ID_QOS,        cmd_qos_port_save                       },

#ifdef CONFIG_SYS_PROTO_ISG
    
    {CAT_ID_ISG,        cmd_isg_port_save                       },
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    
    {CAT_ID_DHCP_SNOOPING, cmd_dhcp_snooping_port_save          },
#endif

#ifdef CONFIG_SYS_PROTO_DAI
    
    {CAT_ID_DAI,        cmd_dai_port_save                       },
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
    
    {CAT_ID_DOS,        cmd_dos_port_save                       },
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    {CAT_ID_IGMP,       cmd_igmp_port_save                      },

  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    
    {CAT_ID_MLD,        cmd_mld_port_save                      },
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
    
    {CAT_ID_LLDP,       cmd_lldp_port_save                      },
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    {CAT_ID_LLDP_MED,   cmd_lldp_med_port_save                  },
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_UDLD
   
    {CAT_ID_UDLD,       cmd_udld_port_save                      },
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
    
    {CAT_ID_GVRP,       cmd_gvrp_port_save                      },
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    
    {CAT_ID_GMRP,       cmd_gmrp_port_save                      },
#endif

#ifdef CONFIG_SYS_POE
	
	{CAT_ID_POE,		cmd_poe_port_save						},
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	
	{CAT_ID_DHCP_SERVER,		cmd_dhcp_relay_port_save						},
#endif
    
    {-1,                NULL                                    }
};

#ifdef CONFIG_SYS_L3_ROUTE
const cmd_save_intfVlan_tbl_t cmdSaveIntfVlanTbl[] =
{

    {CAT_ID_L3,        cmd_l3_vlan_save                         },

    {-1,                NULL                                    }
};

const cmd_save_intfLo_tbl_t cmdSaveIntfLoTbl[] =
{

    {CAT_ID_L3,        cmd_l3_lo_save                           },

    {-1,                NULL                                    }
};
#endif

const cmd_save_line_tbl_t cmdSaveLineTbl[] =
{

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    {CAT_ID_LINE,       cmd_sys_vty_line_save                   },
#endif
#ifdef CONFIG_SYS_AAA
    {CAT_ID_AAA,        cmd_aaa_line_save                       },
#endif

    {-1,                NULL                                    }
};

