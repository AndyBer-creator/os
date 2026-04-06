
#ifndef __CLI_DESC_H__
#define __CLI_DESC_H__

#include <common/sys_def.h>

#define ARG_IPADDR_DESC             "IP Address format is A.B.C.D where (A/B/C/D = 0 ~ 255)\n"
#define ARG_IPV6ADDR_DESC           "IPv6 Address format is X:X::X:X\n"
#define ARG_IPV6ADDRPFXLEN_DESC     "Specify IPv6 address and prefix length\n"
#define ARG_MACADDR_DESC            "MAC Address format is AA:BB:CC:DD:EE:FF\n"
#define ARG_PORT_DESC               "Port Number (e.g. fa5)\n"
#define ARG_VLAN_DESC               "VLAN ID (e.g. 100)\n"
#define ARG_VLANLIST_DESC           "VLAN List (e.g. 3,6-8): The range of VLAN ID is 1 to 4094\n"
#define ARG_VLANRANGE_DESC          "VLAN Range (e.g. 1-6): The range of VLAN ID is 1 to 4094\n"
#define ARG_QUEUELIST_DESC          "QUEUE List (e.g. 3,5-7)\n"
#define ARG_FILENAME_DESC           "File name\n"
#define ARG_USERNAME_DESC           "Login user name\n"
#define ARG_TCPUDP_PORT_DESC        "TCP/UDP port number (0 ~ 65535)\n"
#define ARG_HOSTNAME_DESC           "Host name\n"

#define ARG_PORTLIST_DESC           "Port List\n"
#define ARG_PORTLIST_ALL_DESC       "Port List\nAll ports\n"

#define ARG_NML_PORTLIST_DESC       "Port List (all normal ports)\n"
#define ARG_NML_PORTLIST_ALL_DESC   "Port List (all normal ports)\nAll ports\n"

#define ARG_NML_WOTRKMBR_PORTLIST_ALL_DESC "Port List (normal port without turnk members)\nAll ports\n"

#define SHOW_DESC                   "Show running system information\n"
#define EXIT_DESC                   "Exit from current context\n"
#define DEBUG_DESC                  "Debug Options\n"
#define CREATE_DESC                 "Create an entity or an entry\n"
#define ADD_DESC                    "Add an Entry\n"
#define CLEAR_DESC                  "Reset functions\n"
#define NO_DESC                     "Negate command\n"
#define COPY_DESC                   "Copy from one file to another\n"
#define PORT_DESC                   "Port configuration\n"
#define EEE_DESC                    "EEE configuration\n"
#define VLAN_DESC                   "VLAN configuration\n"
#define VLAN_RANGE_DESC             "VLAN Range configuration\n"
#define NAME_DESC                   "Name configuration\n"
#define IGMP_DESC                   "IGMP configuration\n"
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define MLD_DESC                    "MLD configuration\n"
#endif
#define QINQ_DESC                   "QinQ configuration\n"
#define QOS_DESC                    "QoS configuration\n"
#define MIRROR_DESC                 "Mirror configuration\n"
#define SESSION_DESC                "Mirror Session configuration\n"
#define ARG_SESSION_DESC            "Session ID (e.g. 1-4)configuraton\n"
#define MAC_DESC                    "MAC configuration\n"
#define MAC_ADDRESS_TABLE_DESC      "MAC address table configuration\n"
#define DYNAMIC_MAC_DESC            "dynamic addresses\n"
#define STP_DESC                    "Spanning-tree configuration\n"
#define LLDP_DESC                   "Lldp configuration\n"
#define SNMP_STR                    "SNMP information\n"
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#define RMON_STR                    "RMON information\n"
#endif
#ifdef CONFIG_SYS_SNMPV3
#define SNMPV3_STR                  "SNMPv3 information\n"
#endif
#ifdef CONFIG_SYS_APP_SSH
#define SSH_STR                     "SSH (Secure Shell) configuration\n"
#endif
#define TELNET_STR                  "Telnet daemon configuration\n"
#define TCPUDP_PORT_DESC            "TCP/UDP port\n"
#define ARP_DESC                    "ARP configuration\n"
#define IP_DESC                     "IP configuration\n"
#define GARP_DESC                   "Gratuitous ARP\n"
#define STORM_DESC                  "Storm control configuration\n"
#define BWCTRL_DESC                 "Bandwidth control configuration\n"
#define RATE_LIMIT_DESC             "Rate limit configuration of the specified incoming traffic\n"
#define RL_INPUT_DESC               "Rate limit args ingress configuration\n"
#define RL_OUTPUT_DESC              "Rate limit args egress configuration\n"
#define TRAFFIC_SHAPE_DESC          "Configure shaper on an egress port\n"
#define RATE_LIMIT_INFORMATION      "Rate limit configuration information\n"

#ifdef CONFIG_SYS_PROTO_GVRP
#define GVRP_DESC                   "GVRP configuration\n"
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#define GMRP_DESC                   "GMRP configuration\n"
#endif

#define CUSTOM_DESC                 "Custom Module configuration\n"
#define SWITCHPRT_DESC              "Switch Port configuration\n"
#define MANAGEMENT_DESC             "IP Management\n"
#define DEFAULT_DESC                "Set a command to its defaults\n"

#define ERPS_DESC                 "Ethernet Ring Protection Switching\n"
#define LOOPBACK_DESC             "Ethernet  Loopback configure\n"

#define IF_DESC                   "Select an interface to configure\n"
#define IFS_DESC                  "Interface status and configuration\n"
#define IFS_CONF_DESC             "Interface configuration\n"
#define IF_RANGE_DESC             "interface range command\n"
#define ARG_IF_FASTETH            "Fast ethernet interface to configure\n"
#define ARG_IF_FASTETH_DEV        "FastEthernet device number\n"
#define ARG_IF_GIGAETH            "Gigabit ethernet interface to configure\n"
#define ARG_IF_GIGAETH_DEV        "GigabitEthernet device number\n"
#define ARG_IF_TRUNK              "IEEE 802.3 Link Aggregateion interface\n"
#define ARG_IF_TRUNK_DEV          "Port-Channel interface number\n"
#define ARG_IF_VLAN               "Vlan interface\n"
#define ARG_IF_VLAN_DEV           "VLAN interface number\n"
#define ARG_IF_PORT               ""
#define ARG_IF_NMLPORT            ""

#define SYSTEM_DESC                 "System information\n"
#define SNTP_DESC                   "Simple Network Time Protocol\n"
#define FLASH_DESC                  "Flash Operations\n"
#define BOOT_DESC                   "Booting Operations\n"
#define BOOTVAR_DESC                "Show boot attributes\n"
#define DELETE_DESC                 "Delete Operations\n"
#define CONFIG_DESC                 "Startup config configurations\n"
#define CONFIG_NAME_DESC            "Configuration file name\n"
#define IMAGE_DESC                  "Run time firmware image\n"
#define JFFS2_DESC                  "JFFS2 file system partition\n"
#define SELECT_DESC                 "Select file\n"
#define REMOVE_DESC                 "Remove file\n"
#define ERASE_DESC                  "Erase flash\n"
#define INFO_DESC                   "Information\n"
#define TFTP_DESC                   "TFTP operation\n"
#define TFTPSRV_DESC                "TFTP server configurations\n"
#define TIMEOUT_DESC                "Session timeout configuration\n"
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
#define PASSRETRY_DESC              "CLI login password intrusion threshold\n"
#define SILENT_DESC                 "CLI login fail silent time\n"
#define ACCESS_CONSOLE_DESC         "Access CLI from console\n"
#define ACCESS_TELNET_DESC          "Access CLI from telnet\n"
#define ACCESS_SSH_DESC             "Access CLI from ssh\n"
#endif
#define REMOTE_HOST_DESC            "Remote host address\n"
#define HTTP_DESC                   "HTTP server configuration\n"
#define HTTPS_DESC                  "HTTPS server configuration\n"
#define TIME_WDAYS                  "Sunday\nMonday\nTuesday\nWednesday\nThursday\nFriday\nSaturday\n"
#define TIME_MONTHS                 "Month January\nMonth February\nMonth March\nMonth April\nMonth May\nMonth June\nMonth July\nMonth August\nMonth September\nMonth October\nMonth November\nMonth December\n"
#define SYS_DNS_DESC                "IP Domain Naming System\n"
#define SYS_DNS_NAME_DESC           "To define a default domain names that the software |uses to complete unqualified host names\n"

#define JUMBO_DESC                  "Jumbo Frame configuration\n"

#define IF_DESCP_DESC             "Interface specific description\n"
#define ARG_IF_DESCP              "Up to " SYS_STR_CONST(CAPA_PORTDESC_STR_LEN) " characters descriping this interface\n"

#define PORT_SHUTDOWN_DESC          "Shutdown the selected interface\n"
#define PORT_MEDIA_TYPE_DESC        "Configure media type\n"
#define PORT_SPEED_DESC             "Configure speed operation\n"
#define PORT_SPEED_AUTO_DESC        "Enable AUTO speed configuration\n"
#define PORT_FIBERSPEED_DESC        "Configure speed operation for combo fiber port\n"
#define PORT_DUPLEX_DESC            "Configure duplex operation\n"
#define PORT_FLOWCTL_DESC           "Configure flow-control mode\n"
#define PORT_SWITCHPORT_DESC        "Set switching mode characteristics\n"
#define PORT_PROTECTED_DESC         "Configure an interface to be a protected port\n"
#define PORT_SECURITY_DESC          "Security related command\n"
#define PORT_SECURITY_MAXADDR_DESC  "Max secure addresses\n"
#define ARG_MEDIA_TYPE_AUTO         "Use whichever connector is attached\n"
#define ARG_MEDIA_TYPE_RJ45         "Use RJ45 connector\n"
#define ARG_MEDIA_TYPE_SFP          "Use SFP connector\n"
#define ARG_PORT_SPEED_NO_NEGO      "Do not negotiate speed\n"
#define ARG_PORT_SPEED_10_100       "Force 10 Mbps operation\nForce 100 Mbps operation\n"
#define ARG_PORT_SPEED_1000         "Force 1000 Mbps operation\n"
#define ARG_PORT_SPEED_2500         "Force 2500 Mbps operation\n"
#define ARG_PORT_SPEED_10000        "Force 10 Gbps operation\n"
#define ARG_PORT_SPEED_10000_1000   "Force 10 Gbps operation\nForce 1 Gbps operation"
#define ARG_PORT_AUTO_SPEED_10_100  "Include 10 Mbps in auto-negotiation advertisement\nInclude 100 Mbps in auto-negotiation advertisement\n"
#define ARG_PORT_AUTO_SPEED_1000    "Include 1000 Mbps in auto-negotiation advertisement\n"
#define ARG_PORT_AUTO_SPEED_2500    "Include 2500 Mbps in auto-negotiation advertisement\n"
#define ARG_PORT_FIBERSPEED         "Force 100 Mbps operation\nForce 1000 Mbps operation\n"
#define ARG_PORT_AUTO_FIBERSPEED    "Include 1000 Mbps in auto-negotiation advertisement\n"
#define ARG_PORT_DUPLEX             "Enable AUTO duplex configuration\nForce full duplex operation\nForce half-duplex operation\n"
#define ARG_PORT_FLOWCTL            "Enable AUTO flow-control configuration\nForce flow-control as disabled\nForce flow-control as enabled\n"
#define ARG_PORT_SECURITY_MAXADDR   "Maximum addresses\n"
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
#define PORT_ERRDISABLE_DESC        "Error Disable\n"
#define PORT_ERRDISABLE_RECV_DESC   "Recovery from error disable\n"
#define PORT_ERRDISABLE_INTV_DESC   "Recovery interval\n"
#define PORT_ERRDISABLE_CAUSE_DESC  "Error Disabled caused reason\n"
#define PORT_ERRDISABLE_CAUSE_DESC  "Error Disabled caused reason\n"
#define ARG_PORT_ERRDISABLE_CAUSE   "Enable timer to recover from all causes\n" \
                                    "Enable timer to recover from acl causes\n" \
                                    "Enable timer to recover from arp rate limit causes\n" \
                                    "Enable timer to recover from broadcast flood causes\n" \
                                    "Enable timer to recover from bpdu guard causes\n" \
                                    "Enable timer to recover from dhcp rate limit causes\n" \
                                    "Enable timer to recover from port security causes\n" \
                                    "Enable timer to recover from unicast flood causes\n" \
                                    "Enable timer to recover from unknown multicast flood causes\n" \
                                    "Enable timer to recover from selfloop causes\n"
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL

#define ACL_DESC                    "ACL configuration\n"
#define ACE_DESC                    "ACE configuration\n"
#define ACL_ID_RANGE_DESC           "ACL id range configuration\n"
#define TEMPLATE_DESC               "ACL template configuration\n"
#define ACL_SEQ_DESC                "Specify sequence of access control entry\n"
#define ACL_PERMIT_DESC             "A permit condition forward traffic if the conditions define in the permit statement are matched.\n"
#define ACL_DENY_DESC               "A deny condition blocks traffic if the conditions defined in the deny statement are matched.\n"
#define ACL_DENYIP_DESC             "A deny-ip condition blocks traffic if the conditions define in the deny statement are matched.\n"
#define ACL_DENYIPV6_DESC           "A deny condition blocks traffic if the conditions defined in the deny statement are matched.\n"
#define ACL_MAC_VLAN_DESC           "Specify the packet VLAN.\n"
#define ACL_MAC_COS_DESC            "Specify the packet cos value (0-7)\n"
#define ACL_MAC_ETHER_DESC          "Specify the packet EtherType.\n"
#define ACL_SHUTDOWN                "The Ethernet interface would be disabled if the condition is matched\n"
#define ARG_ACL_IP_PARAM            "Specify the IP protocol number.\n" \
                                    "IP in IP (encapsulation) Protocol (4).\n" \
                                    "Exterior Gateway Protocol (8).\n" \
                                    "interior Gateway Protocol (9).\n" \
                                    "Host Monitoring Protocol (20).\n" \
                                    "Reliable Data Protocol (27).\n" \
                                    "IPv6 in IP encapsulation (41).\n" \
                                    "Routing Header for IPv6 (43).\n" \
                                    "Fragment Header for IPv6 (44).\n" \
                                    "Reservation Protocol (46).\n" \
                                    "IPv6 internet control message protocol (58).\n" \
                                    "Open Shortest Path Protocol (89).\n" \
                                    "Protocol Independent Multicast (103).\n" \
                                    "Layer Two Tunneling Protocol (115).\n" \
                                    "Specify for any Internet Protocol.\n"
#define ARG_ACL_IP_DSCP_FILTER      "Configure DSCP filtering.\n" "Configure IP-PRECEDENCE filtering.\n"
#define ARG_ACL_IP_DSCP_FILTER_VAL  "Specify value for DSCP<0-63> or IP-PRECEDENCE<0-7>.\n"
#define ARG_ACL_IP_ICMP             "Internet Control Message Protocol (1).\n" "NOUSE\n"
#define ARG_ACL_IP_ICMP_PARAM       "Specify a number for Icmp\n" \
                                    "Set Icmp type to echo-reply\n" \
                                    "Set Icmp type to destination-unreachable\n" \
                                    "Set Icmp type to source-quench\n" \
                                    "Set Icmp type to echo-request\n" \
                                    "Set Icmp type to router-advertisement\n" \
                                    "Set Icmp type to router-solicitation\n" \
                                    "Set Icmp type to time-exceeded\n" \
                                    "Set Icmp type to timestamp\n" \
                                    "Set Icmp type to timestamp-reply\n" \
                                    "Set Icmp type to traceroute\n" \
                                    "Set Icmp type to any\n"
#define ARG_ACL_IP_ICMP_CODE        "Specify a number for Icmp code\n" "Specify Any\n"
#define ARG_ACL_IP_IGMP             "Internet Group Management Protocol (2).\n" "NOUSE\n"
#define ARG_ACL_IP_IGMP_PARAM       "Specify a number for Igmp\n" \
                                    "Set Igmp type to host-query\n" \
                                    "Set Igmp type to host-report version 1\n" \
                                    "Set Igmp type to dvmrp\n" \
                                    "Set Igmp type to pim\n" \
                                    "Set Igmp type to trace\n" \
                                    "Set Igmp type to host-report version 2\n" \
                                    "Set Igmp type to host-leave version 2\n" \
                                    "Set Igmp type to host-report version 3\n" \
                                    "Set Igmp type to any\n"
#define ARG_ACL_IP_TCP              "Transmission Control Protocol (6).\n" "NOUSE\n"
#define ARG_ACL_IP_TCP_PARAM        "Configure TCP port filtering.\n" \
                                    "echo (7)\n" \
                                    "discard (9)\n" \
                                    "daytime (13)\n" \
                                    "ftp-data (20)\n" \
                                    "ftp (21)\n" \
                                    "telnet (23)\n" \
                                    "smtp (25)\n" \
                                    "time (37)\n" \
                                    "hostname (42)\n" \
                                    "whois (43)\n" \
                                    "tacacs-ds (49)\n" \
                                    "domain (53)\n" \
                                    "www (80)\n" \
                                    "pop2 (109)\n" \
                                    "pop3 (110)\n" \
                                    "syslog (514)\n" \
                                    "talk (517)\n" \
                                    "klogin (543)\n" \
                                    "kshell (544)\n" \
                                    "sunrpc (111)\n" \
                                    "drip (3949)\n" \
                                    "Configure TCP port filtering range (Format: start-end).\n" \
                                    "Define Any\n"
#define ARG_ACL_IP_UDP              "User Datagram Protocol (17).\n" "NOUSE\n"
#define ARG_ACL_IP_UDP_PARAM        "Configure UDP port filtering.\n" \
                                    "echo (7)\n" \
                                    "discard (9)\n" \
                                    "time (37)\n" \
                                    "nameserver (42)\n" \
                                    "tacacs-ds (49)\n" \
                                    "domain (53)\n" \
                                    "bootps (67)\n" \
                                    "bootpc (68)\n" \
                                    "tftp (69)\n" \
                                    "sunrpc (111)\n" \
                                    "ntp (123)\n" \
                                    "netbios-ns (137)\n" \
                                    "snmp (161)\n" \
                                    "snmptrap (162)\n" \
                                    "who (513)\n" \
                                    "syslog (514)\n" \
                                    "talk (517)\n" \
                                    "rip (520)\n" \
                                    "Configure UDP port filtering range (Format: start-end).\n" \
                                    "Define Any\n"
#define ARG_ACL_IPV6                "Specify the IPv6 protocol number.\n" "Specify for any Internet Protocol.\n"
#define ARG_ACL_IPV6_SRC            "Specify IPv6 source address and prefix length.\n" "Use any for all IP addresses.\n"
#define ARG_ACL_IPV6_DST            "Specify IPv6 destination address and prefix length.\n" "Use any for all IP addresses.\n"
#define ARG_ACL_IPV6_ICMP_PARAM     "Specify a number for Icmpv6\n" \
                                    "Set Icmp type to destination-unreachable\n" \
                                    "Set Icmp type to packet-too-big\n" \
                                    "Set Icmp type to time-exceeded\n" \
                                    "Set Icmp type to parameter-problem\n" \
                                    "Set Icmp type to echo-request\n" \
                                    "Set Icmp type to echo-reply\n" \
                                    "Set Icmp type to router-solicitation\n" \
                                    "Set Icmp type to router-advertisement\n" \
                                    "Set Icmp type to nd-ns\n" \
                                    "Set Icmp type to nd-na\n" \
                                    "Set Icmp type to any\n"
#define ARG_ACL_MAC_SRC             "Specify MAC source address and mask. For example, to set 00:00:00:00:10:XX use mac 00:00:00:00:10:00 with mask FF:FF:FF:FF:FF:00\n" "Specify any MAC source address\n"
#define ARG_ACL_MAC_DST             "Specify MAC destination address. For example, to set 00:00:00:00:10:XX use mac 00:00:00:00:10:00 with mask FF:FF:FF:FF:FF:00\n" "Specify any MAC destination address\n"
#define ARG_ACL_MAC_COSMASK         "Specify the packet cos mask (0-7).One in mask is an hit. For example, setting cos value to 7 (0111) with mask 5 (0101) will match cos values: 5 (0101), 7 (0111)\n"
#define ARG_ACL_IP_SRC              "Specify IP source address and mask. For example, to set 176.212.xx.xx use addr 176.212.0.0 with mask 255.255.0.0\n" "Use any for all IP addresses.\n"
#define ARG_ACL_IP_DST              "Specify IP destination address and mask. For example, to set 176.212.xx.xx use addr 176.212.0.0 with mask 255.255.0.0\n" "Use any for all IP addresses.\n"
#endif 

#define LOG_DESC                    "Log Configuration\n"
#define LOG_LOCAL_DESC              "Flash \nRAM \n"
#define LOG_HOST_DESC               "Remote syslog host\n"
#define LOG_PORT_DESC               "Remote server Port, default 514\n"
#define LOG_ARG_PORT_DESC           "UDP port number\n"
#define LOG_SEVERITY_DESC           "Specify logging level.\n"
#define LOG_ARG_SEVERITY_DESC       "Minimum severity <0-7> (EMEGR-DEBUG).\n"
#define LOG_FACILITY_DESC           "Specify facility parameter for syslog messages.\n"
#define LOG_ARG_FACILITY_DESC       "Local use\nLocal use\nLocal use\nLocal use\nLocal use\nLocal use\nLocal use\nLocal use\n"
#define LOG_CATEGORY_DESC           "Log category level\n"
#define LOG_ARG_CATEGORY_DESC       "Log category 0-22 (e.g. 0,11-15). Use \"show logging category-severity-table\" to see mapping index.\n"
#define LOG_AGG_DESC                "aggregation of syslog messages\n"
#define LOG_AGG_ENABLE_DESC         "Enable aggregation of syslog messages\n"
#define LOG_AGG_AGING_DESC          "Specify aging-time for syslog aggregated message\n"
#define LOG_ARG_AGG_AGING_DESC      "Specify aging-time in sec\n"

#define FILE_MGMT_STARTUPCFG_DESC  "Startup configuration\n"
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
#define FILE_MGMT_BACKUPCFG_DESC   "Backup configuration\n"
#endif
#define FILE_MGMT_RUNNINGCFG_DESC  "Running configuration\n"
#define FILE_MGMT_MIRRORCFG_DESC   "Mirror configuration\n"

#define LINE_DESC                   "To identify a specific line for configuration\n"
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
#define LINE_CONSOLE_DESC           "Console terminal line\n"
#define LINE_TELNET_DESC            "Virtual terminal for remote console access (Telnet)\n"
#define LINE_SSH_DESC               "Virtual terminal for secured remote console access (SSH)\n"
#define LINE_HTTP_DESC              "Access web from http\n"
#define LINE_HTTPS_DESC             "Access web from https\n"
#define LINE_CONSOLE_SPEED_DESC     "To set the baud rate of the line.\n"
#endif

#ifdef CONFIG_SYS_AAA

#define AAA_DESC                    "Authentication, Authorization, Accounting\n"
#define AAA_AUTHEN_DESC             "Authentication\n"
#define AAA_AUTHOR_DESC             "Authorization\n"
#define AAA_ACCOUNT_DESC            "Accounting\n"
#define AAA_LOGIN_DESC              "Login Authentication\n"
#define AAA_ENABLE_DESC             "Enable Authentication\n"
#define AAA_ACCEXEC_DESC            "EXEC Accounting\n"
#define AAA_EXEC_DESC               "Exec Authorization\n"
#define AAA_CMDS_DESC               "Commands Authorization\n"
#define AAA_AUTHLIST_DESC           "Auth Method List\n"
#define AAA_ACCLIST_DESC            "Accounting Method List\n"
#define AAA_SERVER_PRIOR_DESC       "Server priority\n"
#define AAA_LINE_ACCT_DESC          "Accounting parameters\n"
#define AAA_LINE_AUTHOR_DESC        "Authorization parameters\n"

#define AAA_TACPLUS_SRV_DFTPARAM_DESC   "TACACS+ server default parameters\n"
#define AAA_TACPLUS_SRV_DESC        "TACACS+ server information\n"
#define AAA_TACPLUS_HOST_DESC       "TACACS+ server host\n"
#define AAA_TACPLUS_HOSTNAME_DESC   "TACACS+ server host name\n"
#define AAA_TACPLUS_KEY_DESC        "TACACS+ key\n"
#define AAA_TACPLUS_TIMEOUT_DESC    "TACACS+ timeout\n"

#define AAA_PRIV_DESC               "Command privilege parameters\n"
#define AAA_PRIV_CONFIG_DESC        "Configuration mode\n"
#define AAA_PRIV_EXEC_DESC          "Exec nide\n"
#define AAA_PRIV_LEVEL_DESC         "Set privilege level of command\n"
#define AAA_PRIV_RESET_DESC         "Reset privilege level of command\n"
#define ARG_PRIV_LEVEL              "Privilege level\n"
#define ARG_PRIV_CMDLINE            "Initial keywords of the command to modify\n"

#define ARG_AUTHLIST_DFLT           "Default Auth Method List\n"
#define ARG_AUTHLIST_NAME           "Auth Method List Name\n"
#define ARG_ACCLIST_DFLT            "Default Accounting Method List\n"
#define ARG_ACCLIST_NAME            "Accouting Method List Name\n"
#define ARG_AUTH_LOGIN_METHOD       "Use enable password for authentication\nUse local username authentication\nNO authentication\nUse all Radius servers\nUse all Tacacs+ servers\n"
#define ARG_AUTH_ENABLE_METHOD      "Use enable password for authentication\nNO authentication\nUse all Radius servers\nUse all Tacacs+ servers\n"
#define ARG_AUTHOR_CMD_METHOD       "Use local username authentication\nNO authentication\nUse all Radius servers\nUse all Tacacs+ servers\n"
#define ARG_AUTHOR_EXEC_METHOD      "Use local username authentication\nNO authentication\nUse all Radius servers\nUse all Tacacs+ servers\n"
#define ARG_TACPLUS_AUTOINDEX       "Select Tacacs+ server index automatically\n"
#define ARG_TACPLUS_INDEX           "Tacacs+ server information index\n"
#define ARG_TACPLUS_KEY             "Tacacs+ server key\n"
#define ARG_TACPLUS_TIMEOUT         "Tacacs+ server timeout\n"
#define AAA_TACPLUS_DFTPARAM_DESC   "Tacacs+ server default configurations\n"
#endif 
#define AAA_LOCAL_USER_DESC         "Local User\n"
#define AAA_LOCAL_PRIV_DESC         "Local user privilege level\n"
#define AAA_LOCAL_PASS_DESC         "Local user password\n"
#define AAA_LOCAL_ENBL_DESC         "Local Enable Password\n"
#define ARG_LOCAL_USRNAME           "Local user name\n"
#define ARG_LOCAL_PRIV              "Privilege level\n"
#define ARG_LOCAL_PRIV_ADMIN        "Admin privilege\n"
#define ARG_LOCAL_PRIV_USER         "User privilege\n"
#define ARG_LOCAL_CLEAR             "Use clear text password\n"
#define ARG_LOCAL_NOPASSWD          "No password for this user\n"
#define ARG_LOCAL_SECRET            "Use encrypted password\n"
#define ARG_LOCAL_PASSWD            "Local user password\n"

#ifdef CONFIG_SYS_LIB_RADIUS
#define RADIUS_SRV_DESC         "RADIUS server information\n"
#define RADIUS_SRV_DFTPARAM_DESC    "RADIUS server default configurations\n"
#define RADIUS_HOST_DESC        "RADIUS server host\n"
#define RADIUS_HOSTNAME_DESC    "RADIUS server host name\n"
#define RADIUS_KEY_DESC         "RADIUS key\n"
#define RADIUS_TIMEOUT_DESC     "Time to wait for this RADIUS server to reply(default is 3)\n"
#define RADIUS_AUTHPORT_DESC    "UDP port for RADIUS authentication server (default is 1812)\n"
#define RADIUS_ACCTPORT_DESC    "UDP port for RADIUS accounting server (default is 1813)\n"
#define RADIUS_RETRANS_DESC     "Specify the number of retransmit to active server\n"
#define RADIUS_DEADTIME_DESC    "Time to wait before a non-responsive RADIUS server is bypassed\n"
#define RADIUS_USGTYPE_DESC     "Usage type\n"
#define SERVER_PRIOR_DESC       "Server priority\n"

#define ARG_RADIUS_AUTOINDEX        "Select Radius server index automatically\n"
#define ARG_RADIUS_INDEX            "Radius server information index\n"
#define ARG_RADIUS_KEY              "Radius server key\n"
#define ARG_RADIUS_TIMEOUT          "Timeout value in seconds to wait for server to reply\n"
#define ARG_RADIUS_RETRANS          "The number of retransmit. The default is 3 retries\n"
#define ARG_RADIUS_DEADTIME         "Deadtime value in minutes that elapse before a non-responsive RADIUS server is bypassed\n"
#define ARG_RADIUS_USGTYPE          "Usage type (login, 802.1X, all)\n"
#define ARG_SERVER_PRIOR            "Server priority vlaue in Server Group\n"
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION

#define DOS_DESC                    "DoS information\n"
#define ARG_DOS_TYPE                "TCP fragment packet with offset equals to one\n" \
                                    "SYNC and RST bits set in the packet\n" \
                                    "SYN and FIN bits set in the packet\n" \
                                    "Xmascan: sequence number is zero and the FIN, URG and PSH bits are set\n" \
                                    "NULL Scan Attacks\n" \
                                    "SYN packets with sport less than 1024\n" \
                                    "Check minimum TCP header\n" \
                                    "Smurf Attacks\n" \
                                    "Check ICMPv6 ping maximum packets size\n" \
                                    "Check ICMPv4 ping maximum packets size\n" \
                                    "Fragmented ICMP packets\n" \
                                    "Check minimum size of IPv6 fragments\n" \
                                    "Ping of Death Attacks\n" \
                                    "Source TCP port equals to destination TCP port\n" \
                                    "Source UDP port equals to destination UDP port\n" \
                                    "Source IP equals to destination IP\n" \
                                    "Destination MAC equals to source MAC\n"
#endif

#ifdef CONFIG_SYS_STORM_CTRL

#define BCAST_STORM_DESC            "Broadcast storm control\n"
#define MCAST_STORM_DESC            "Multicast storm control\n"
#define UUCAST_STORM_DESC           "Unknown-unicast storm control\n"
#define UMCAST_STORM_DESC           "Unknown-multicast storm control\n"
#define STORM_RATE_DESC             "Storm control rate(pps/Kbps). (Value 1-1000000)\n"
#define STORM_ACTION_DESC           "Storm control action after exceed threshold\n"
#define STORM_ACTION_NONE           "No action after exceed storm control threshold\n"
#define STORM_ACTION_DROP           "Drop packets after exceed storm control threshold\n"
#define STORM_ACTION_SHUTDOWN       "Shut down port after exceed storm control threshold\n"
#endif

#define QOS_MODE                    "Set system QoS advance mode.\nSet system QoS basic mode.\n"
#define QOS_QUEUE_DESC              "Queue configuration\n"
#define QOS_PORTCOS_DESC            "Configure the default CoS value for a port. Use the no form of the command to return to the default setting.\n"
#define ARG_QOS_PORTCOS             "Specify the default VPT value.\n"
#define QOS_TRUST_DESC              "Configure the global trust mode . Use the no form to return untrusted state.\n"
#define QOS_PORT_TRUST_DESC         "Configure each port to trust state while the system is in basic mode. Use the no form of the command to disable trust state on each port.\n"
#define ARG_QOS_TRUST_MODE          "Specify trust mode cos.\nSpecify trust mode Cos-DSCP.\nSpecify trust mode DSCP.\nSpecify trust mode precedence\n"
#define QOS_SPQ_NUM_DESC            "Set priority queue parameters\n"
#define QOS_SPQ_OUT_DESC            "Enable the egress queues to be expedite queue\n"
#define QOS_SPQ_NUMQ_DESC           "Configure the number of strict priority queues\n"
#define ARG_QOS_SPQ_NUM             "Assign the number of queues to be expedite queues. The expedite queues would be the queues with higher indexes\n"
#define QOS_WRR_DESC                "Configure queue drop mechanism on egress port.\n"
#define QOS_BW_DESC                 "Configure weights to egress queues. Use no form to return to default values.\n"
#define ARG_BW_SEQ                  "Specify weight in the range weight1 - weight8. Enter the frequency ratio in which the scheduler drops the packet in every queue. Separate each value by space. The ratio is as follows: For queue number N the ratio is weightN / SumOf{weight1 - weight8}.\n"
#define QOS_COS_MAP_DESC            "Map assigned CoS values to select an egress queue. Use the command no form to return to the default values.\n"
#define ARG_COS_MAP_QUEUE           "The queue number to which the following CoS values are mapped.\n"
#define ARG_COS_MAP_SEQ             "Enter up to eight CoS values for mapping to a specific queue.\n"
#define QOS_MAP_DESC                "Configure the QoS maps.\n"
#define QOS_MAP_TO_DESC             "Mapping to\n"
#define QOS_MAP_DSCP_Q_DESC         "Modify the DSCP to queue map.\n"
#define ARG_MAP_DSCP_Q_SEQ          "dscp-list Specify 8 DSCP values , separate each DSCP with space.\n"
#define ARG_MAP_DSCP_Q_QUE          "Enter the queue number to which the DSCP value correspond.\n"
#define QOS_MAP_IPP_Q_DESC          "Modify the IP Precedence to queue map.\n"
#define ARG_MAP_IPP_Q_SEQ           "ip precedence list , separate each precedence with space.\n"
#define ARG_MAP_IPP_Q_QUE           "Enter the queue number to which the IP Precedence value correspond.\n"
#define QOS_MAP_Q_COS_MAP_DESC      "Modify the queue to CoS map.\n"
#define ARG_MAP_Q_COS_MAP_SEQ       "Enter up to eight queue id for mapping to a specific CoS value.\n"
#define ARG_MAP_Q_COS_COS           "Enter the cos value to which the queue id correspond.\n"
#define QOS_MAP_Q_DSCP_MAP_DESC     "Modify the queue to DSCP map.\n"
#define ARG_MAP_Q_DSCP_MAP_SEQ      "Enter up to eight queue id for mapping to a specific DSCP value.\n"
#define ARG_MAP_Q_DSCP_DSCP         "Enter the dscp value to which the queue id correspond.\n"
#define QOS_MAP_Q_IPP_MAP_DESC      "Modify the queue to ip precedence map.\n"
#define ARG_MAP_Q_IPP_MAP_SEQ       "Enter up to eight queue id for mapping to a specific ip precedence value.\n"
#define ARG_MAP_Q_IPP_COS           "Enter the ip precedence value to which the queue id correspond.\n"
#define QOS_QUEUEING_DESC           "Display quality of service (QoS) queuing information\n"
#define QOS_PORT_REMARK_DESC        "Configure remarking state of each port\n"
#define QOS_PORT_REMARK_TYPE        "Remarking CoS value\nRemarking DSCP value\nRemarking ip precedence value\n"

#define LLDP_GLOBAL_DESC            "Global LLDP configuration subcommands\n"
#define LLDP_ITF_DESC               "LLDP interface subcommands\n"
#define LLDP_TLV_DESC               "LLDP optional TLV, pick from: port-desc, sys-name, sys-desc, sys-cap, mac-phy, lag, max-frame-size, management-addr\n"
#define LLDP_TLV_8021_DESC          "802.1 optional TLVs\n"
#define LLDP_TLV_8021_PVID_DESC     "Specify to advertise the PVID of interface\n"
#define LLDP_TLV_8021_VLAN_DESC     "Add/remove VLAN for advertise\n"
#ifdef CONFIG_SYS_PROTO_LLDP_MED
#define LLDP_MED_DESC               "Configure LLDP MED extension\n"
#define LLDP_MED_TLV_DESC           "LLDP MED optional TLV (network-policy, location, inventory)\n"
#define LLDP_MED_NETWORK_POLICY_DESC    "LLDP MED network policy configuration\n"
#endif
#define LLDP_ENABLE_DESC            "Enable LLDP\n"
#define LLDP_DISABLE_DESC           "Disable LLDP\n"
#define LLDP_REINIT_STR             "Delay (in sec) for LLDP initialization on any interface\n"
#define LLDP_REINIT_VAL_STR         "Specify the delay (in secs) for LLDP to initialize\n"
#define LLDP_TRANSMIT_DESC          "Enable LLDP transmission on interface\n"
#define LLDP_RECEIVE_DESC           "Enable LLDP reception on interface\n"
#define LLDP_HOLDTIME_DESC          "Configuration of multiplier used for calculating the |LLDP discovery packet hold time\n"
#define LLDP_HOLDTIME_VAL_STR       "Multiplier used for calculating the LLDP discovery |packet hold time\n"
#define LLDP_TXINTERVAL_DESC        "Specify the rate at which LLDP packets are sent (in sec)\n"
#define LLDP_TXINTERVAL_VAL_STR     "Rate at which LLDP packets are sent (in sec)\n"
#define LLDP_L_DEVICE_DESC          "LLDP information that is advertised from a specific port\n"
#define LLDP_R_DEVICE_DESC          "Neighbor information\n"
#define LLDP_STATISTICS_DESC        "LLDP Statistics information\n"
#define LLDP_TLV_OVERLOAD_DESC      "display LLDP TLVs overloading\n"

#define SNMP_COMMUNITY_NAME         "Community name (maximum length is " SYS_STR_CONST(CAPA_SNMP_COMMUNITY_LEN) " characters)\n"
#ifdef CONFIG_SYS_SNMPV3
#define SNMP_VIEW_NAME              "View name (maximum length is " SYS_STR_CONST(CAPA_SNMP_VIEW_NAME_LEN) " characters)\n"
#define SNMP_VIEW_TREE_OID          "Subtree OID (maximum OID length is " SYS_STR_CONST(CAPA_SNMP_VIEW_SUBTREE_LEN) ")\n"
#define SNMP_GROUP_NAME             "Group name (maximum length is " SYS_STR_CONST(CAPA_SNMP_GROUP_NAME_LEN) " characters)\n"
#define SNMP_USER_NAME              "Uer name (maximum length is " SYS_STR_CONST(CAPA_SNMP_USER_NAME_LEN) " characters)\n"
#define SNMP_AUTH_PASSWORD          "Auth password(length " SYS_STR_CONST(BOND_SNMP_AUTH_KEY_LEN_MIN) "~" SYS_STR_CONST(BOND_SNMP_AUTH_KEY_LEN_MAX) ") for authenticaste\n"
#define SNMP_PRIV_PASSWORD          "Priv password(length " SYS_STR_CONST(BOND_SNMP_PRIV_KEY_LEN_MIN) "~" SYS_STR_CONST(BOND_SNMP_PRIV_KEY_LEN_MAX) ") for privacy key\n"
#define SNMP_SECURITY_NAME          "Community or User name\n"
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI

#define RMON_STATISTICS_DESC        "rmon statistics\n"
#define RMON_EVENT_DESC             "rmon event\n"
#define RMON_ALARM_DESC             "rmon alarm\n"
#define RMON_HISTORY_DESC           "rmon history\n"
#endif

#define BRIDGE_DESC                 "Global bridge table configuration\n"
#define MCAST_STR                   "Multicast Configuration\n"

#define SWITCHPORT_DESC             "Set switching mode characteristics\n"
#define SWITCHPORT_GENERAL_DESC     "Configure switchport in hybrid mode\n"
#define SWITCHPORT_ACCESS_DESC      "Vlan aware port\n"
#define SWITCHPORT_TRUNK_DESC       "Vlan aware port\n"
#define SWITCHPORT_DOT1QTUNNEL_DESC "802.1Q tunnel port\n"
#define SWITCHPORT_ALLOW_DESC       "Configure switchport allowed parameters\n"
#define SWITCHPORT_FORBID_DESC      "Forbidden VLAN\n"

#define PORT_CHANNEL_DESC           "Configure port-channel\n"
#define CHANNEL_GROUP_DESC          "Add port to Port-channel\n"
#define CHANNEL_GROUP_MODE_DESC     "Set LAG mode\n"
#define TRUNK_DESC                  "Trunk Configuration\n"
#define LAG_DESC                    "Link Aggregation Group Configuration\n"
#define LACP_DESC                   "LACP Configuration\n"
#define ETHERCHANNEL_DESC           "Ethernet channel\n"
#define TRUNK_LOADBALANCE_DESC      "Configure load balancing policy of the trunk\n"
#define ARG_PORT_CHANNEL_NUM        "LAG number\n"
#define ARG_CHANNEL_GROUP_STATIC    "Enable Static Only\n"
#define ARG_CHANNEL_GROUP_LACP      "Enable LACP unconditionally\nEnable LACP only if a LACP device is detected\n"

#ifdef CONFIG_SYS_PROTO_UDLD

#define UDLD_GLOBAL_DESC            "Configure global UDLD setting\n"
#define UDLD_INFO_DESC              "UDLD information\n"
#define UDLD_FIBR_AGSV_DESC         "Enable UDLD protocol in aggressive mode on fiber ports except where locally configured\n"
#define UDLD_FIBR_ENABLE_DESC       "Enable UDLD protocol on fiber ports except where locally configured\n"
#define UDLD_MSG_DESC               "Set UDLD message parameters\n"
#define UDLD_MSG_TIME_DESC          "Set UDLD message time period\n"
#define UDLD_MSG_TIME_VAL_DESC      "Time in seconds between sending of messages in steady state\n"
#define UDLD_PORT_DESC              "Configure UDLD enabled or disabled and ignore global UDLD setting\n"
#define UDLD_PORT_PORT_DESC         "Enable UDLD protocol on this interface\n"
#define UDLD_PORT_AGSV_DESC         "Enable UDLD protocol in aggressive mode on this interface\n"
#define UDLD_PORT_DISABLE_DESC      "Disable UDLD protocol on this interface despite global UDLD setting\n"
#define UDLD_RESET_DESC             "Reset all interfaces which have been shutdown by UDLD\n"
#endif

#define VLAN_DFLTVLAN_DESC          "Default VLAN\n"
#ifdef CONFIG_SYS_PROTOCOL_VLAN
#define VLAN_PROTOCOL_BASED_DESC    "Protocol-based VLAN configuration\n"
#endif
#ifdef CONFIG_SYS_IP_SUBNET
#define VLAN_IP_SUBNET_BASED_DESC    "ip-subnet-based VLAN configuration\n"
#endif
#ifdef CONFIG_SYS_VOICEVLAN
#define VLAN_VOICEVLAN_DESC         "Voice VLAN configuration\n"
#endif
#ifdef CONFIG_SYS_MACVLAN
#define VLAN_MAC_BASED_DESC         "MAC-based VLAN configuration\n"
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#define VLAN_SURVEILLANCE_DESC      "Surveillance VLAN configuration\n"
#endif
#define VLAN_MGMTVLAN_DESC          "Management VLAN configuration\n"
#define VLAN_MGMTPORT_DESC          "Management port configuration\n"
#define VLAN_TXLATE_DESC            "VLAN Mapping configuration\n"

#define CABLEDIAG_COPPER_DESC           "Cable Diagnostics\n"
#define CABLEDIAG_CABLE_LENGTH          "Cable Length\n"
#define CABLEDIAG_OPTICAL_DESC          "Fiber ports diagnositics\n"
#define CABLEDIAG_OPTICAL_STATUS        "Displays fiber ports diagnostics\n"
#define CABLEDIAG_OPTICAL_DETAIL_STATUS "Displays fiber ports detailed diagnostics\n"

#ifdef CONFIG_SYS_TECH_SUPPORT
#define TECH_INFO_DESC            "Show system information\n"
#define TECH_FILE_DESC            "Tech Support file\n"
#endif

#define DHCP_DESC                   "DHCP configuration\n"
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#define DHCP_SNOOPING_DESC          "Snopping information\n"
#define DHCP_L2RELAY_DESC           DHCP_DESC"L2 relay information\n"
#define DHCP_INFO_DESC              "DHCP information\n"
#define DHCP_OPTION82_DESC          "DHCP-Option82 configuration\n"
#define DHCP_LIMIT                  "DHCP limit\n"
#define DHCP_LIMIT_RATE             "Rate limit of dhcp packets\n"
#endif

#define DHCPSERVER_DESC             "DHCP relay and server configuration\n"
#define DHCPSERVERGROUP_DESC        "DHCP server group configuration\n"
#define DHCPRELAY_DESC              "DHCP relay configuration\n"
#ifdef CONFIG_SYS_DHCP_SERVER
#define DHCP_SERVER_DESC           "dhcp relay and server information\n"
#define DHCP_SERVER_GROUP_DESC     "server group information\n"
#define DHCP_RELAY_DESC            "dhcp relay information\n"
#endif

#ifdef CONFIG_SYS_PROTO_DAI

#define DAI_DESC                    "ARP Inspection information\n"
#define DAI_LIMIT                   "ARP limit\n"
#define DAI_LIMIT_RATE              "Rate limit of arp packets.\n"
#endif

#define IGMP_STR                    "IGMP Configuration\n"
#define IGMP_SNOOPING_DESC          "IGMP Snooping Configuration\n"
#define IGMP_IMMEDLEAVE_STR          "IGMP snooping immediate-leave function\n"
#define IGMP_DBG_STR                "IGMP snooping debug function\n"
#define IGMP_QRY_STR                "IGMP snooping querier function\n"

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

#define MLD_STR                     "MLD Configuration\n"
#define MLD_SNOOPING_DESC           "MLD Snooping Configuration\n"
#define MLD_IMMEDLEAVE_STR           "MLD snooping immediate-leave function\n"
#define MLD_DBG_STR                 "MLD snooping debug function\n"
#define MLD_QRY_STR                 "MLD snooping querier function\n"
#define IGMP_CONF_UNKMCAST          "Unregistered multicast group traffic\n"
#define IGMP_CONF_UNKMCAST_ACT      "Action for unregistered multicast group traffic\n"
#define IGMP_CONF_UNKMCAST_FLOOD    "Flood unknown multicast traffic in VLAN\n"
#define IGMP_CONF_UNKMCAST_DROP     "Drop unknown multicast traffic\n"
#define IGMP_CONF_UNKMCAST_ROUTER   "Forward unknown multicast traffic to router port\n"
#define IGMP_CONF_UNKMCAST_ACT_DFLT "Set unknown multicast action to default (flood)\n"
#endif
#ifdef CONFIG_SYS_PROTO_MVR

#define MVR_STR                "MVR Configuration\n"
#endif

#ifdef CONFIG_SYS_PROTO_ISG

#define ISG_DESC                    "IP Source Guard Configuration\n"
#define ISG_BINDING_DESC            "IP Source Guard Binding Table\n"
#endif
#ifdef CONFIG_SYS_MGMT_ACL

#define  MGMT_ACL_IPMGMT_CFG_DESC   "IP management\n"
#define  MGMT_ACL_LIST_CFG_DESC     "Use this command to configure a management access list.\n"
#define  MGMT_ACL_LIST_NAME_DESC    "Specifies the access list name.\n"
#define  MGMT_ACL_ACE_PRIDEL_DESC   "Specify priority of  management access control entry to delete\n"
#define  MGMT_ACL_ACE_PERMIT_CFG_DESC  " To set conditions for the management access list, use the permit command in management access-list\n"
#define  MGMT_ACL_ACE_DENY_CFG_DESC    " To set conditions for the management access list, use the deny command in management access-list \n"
#define  MGMT_ACL_ACE_IP_SOURCE_DESC   "To restrict conditions for source ipv4 or ipv6 address  \n"
#define  MGMT_ACL_ACE_IP_SOURCE_IPV4_MASK_DESC "Set source ipv4 mask\n"
#define  MGMT_ACL_ACE_IP_SOURCE_IPV4_MASK_VALUE_DESC "Set source IPv4 mask value\n"
#define  MGMT_ACL_ACE_IP_SOURCE_IPV6_PRE_DESC "Set source IPv6 prefix\n "
#define  MGMT_ACL_ACE_IP_SOURCE_IPV6_PRE_VALUE_DESC  "Value of source IPv6 prefix value\n "
#define  MGMT_ACL_ACE_SERVICE_DESC     " Define service type condition.\n"
#define  MGMT_ACL_ACE_SERVICE_DETAIL_DESC  "Support ALL service \nSupport HTTP service \nSupport HTTPS service\nSupport SNMP service\nSupport SSH service\nSupport Telnet  service\n"
#define  MGMT_ACL_ACE_SEQ_DESC             "Sequence of  management  access control entry \n"
#define  MGMT_ACL_ACE_SEQ_VALUE_DESC       "Value of sequence ,that is from 1 to 65535\n"
#define  MGMT_ACL_CLASS_CFG_DESC           "Use this command to choose the active access-list.\n"
#define  MGMT_ACL_CLASS_DETAIL_DESC        "Specify the name of the access list.\n"
#define  MGMT_ACL_IPMGMT_SHOW_DESC         "Specify management restrictions configuration\n"
#define  MGMT_ACL_LIST_SHOW_DESC           "Show management access list or lists definition.\n"
#define  MGMT_ACL_CLASS_SHOW_DESC          "Show management active list definition. \n"
#endif

#define CPUINFO_DESC                "Displays information about the system CPU utilization.\n"
#define CPUINFO_INPUT_DESC          "Show rate of input frames to CPU.\n"
#define CPUINFO_INPUT_RATE_DESC     "Show rate of input frames to CPU.\n"
#define CPUINFO_UTILIZATION_DESC    "Displays information about the system CPU utilization.\n"

#define MEMINFO_DESC                "Memory statistics.\n"
#define MEMINFO_STATISTICS_DESC     "Memory statistics.\n"

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#define AUTH_GLB_DESC               "Auth Manager Global Configuration Commands\n"
#define AUTH_INTF_DESC              "Auth Manager Port Configuration Commands\n"
#define AUTH_SHOW_DESC              "Shows Auth Manager configurations or sessions\n"
#define AUTH_CLEAR_DESC             "Clear Auth Manager sessions\n"
#define AUTH_TYPE_DOT1X_DESC        "802.1X authentication\n"
#define AUTH_TYPE_MAC_DESC          "MAC-Based authentication\n"
#define AUTH_TYPE_WEB_DESC          "Web-Based authentication\n"
#define AUTH_HOSTMODE_DESC          "Set the Host mode for authentication on this port\n"
#define AUTH_PORTCTRL_DESC          "Set the port-control value\n"
#define ARG_AUTH_PORTCTRL           "PortState will be set to AUTO\n" \
                                    "PortState will be set to Authorized\n" \
                                    "PortState will be set to UnAuthorized\n"
#define AUTH_ORDER_DESC             "Add authentication type to the order list\n"
#define AUTH_METHOD_DESC            "MAC-Based/Web-Based authentication method\n"
#define ARG_AUTH_METHOD_RADIUS      "Use RADIUS to authenticate\n"
#define ARG_AUTH_METHOD_LOCAL       "Use Local DB to authenticate\n"
#define AUTH_REAUTH_DESC            "Enable/Disabel Reauthentication for this port\n"
#define AUTH_MAXHOST_DESC           "Maximum number of authenticated hosts allowed on the port.|Max hosts only take effect on multi-auth mode.\n"
#define AUTH_MAXLOGIN_DESC          "Maximum number of allowed login attempts on the interface\n"
#define AUTH_RADATTR_DESC           "Enable or Disable treatment to Radius Accept message |attributes\n"
#define AUTH_RADATTR_VLAN_DESC      "VLAN assignment to 802.1x-authenticated users through Radius server\n"
#define AUTH_TIMER_DESC             "Set authentication timer values\n"
#define AUTH_TIME_REAUTH_DESC       "Time in seconds after which an automatic re-authentication |should be initiated\n"
#define AUTH_TIME_INACT_DESC        "Interval in seconds after which if there is no activity from |the client then it will be unauthorized\n"
#define AUTH_TIME_QUIET_DESC        "Interval in seconds to wait following a failed authentication exchange\n"

#define AUTH_SESS_DESC              "Auth Manager session information\n"

#define DOT1X_DESC                  "802.1x configuration\n"
#define DOT1X_TIMEOUT_STR           "Various Timeouts\n"
#define GUEST_VLAN_DESC             "Guest VLAN configuration\n"
#define ARG_DOT1X_PORT_MODE         "PortState will be set to AUTO\n" \
                                    "PortState will be set to Authorized\n" \
                                    "PortState will be set to UnAuthorized\n"
#define AUTH_MAC_RADIUS             "User format send to RADIUS for MAC-Based authentication\n"
#define AUTH_MAC_DELIMITER          "MAC address delimiter used for Radius user ID format\n"
#define ARG_AUTH_MAC_DELIMITER      "Use ':' as delimiter\nUse '.' as delimiter\nUse '-' as delimiter\nNo delimiter\n"
#define AUTH_MAC_DELIMITER_GAP      "The gap of each delimiter\n"
#define ARG_AUTH_GAP                "Insert delimiter every 2 chars\nInsert delimiter every 4 chars\nInsert delimiter every 6 chars\n"
#define AUTH_MAC_CASE               "MAC address case used for Radius user ID format\n"
#define ARG_AUTH_CASE               "Use lower case\nUse upper case\n"
#define AUTH_MAC_LOCAL              "Local daba base for MAC-Based authentication\n"
#define AUTH_WEB_LOCAL              "Local daba base for WEB-Based authentication\n"
#endif

#ifdef CONFIG_SYS_PORT_SECURITY
#define PSECURE_DESC                "Port Security\n"
#define PSECURE_RATELIMIT_DESC      "Rate limiter to protect the CPU against excessive load\n"
#define PSECURE_ADDRLIMIT_DESC      "MAC address limitation\n"
#define PSECURE_VIOLATION_DESC      "Action to be taken when limitation is reached\n"
#define PSECURE_MACADDR_DESC        "Sticky MAC address\n"
#define PSECURE_CLEAR_TYPE          "All secure mac addresses\n" \
                                    "Configured secure mac addresses\n" \
                                    "Secure MAC address auto-learned by hardware\n" \
                                    "Secure MAC address either auto-learned or configured\n"
#endif

#ifdef CONFIG_SYS_POE

#define POE_DESC                    "Power Over Ethernet\n"
#endif
#ifdef CONFIG_SYS_L3_ROUTE
#define L3_INTF_IPV4_ADDR_DESC           "Configure IPv4 Address on interface\n"
#define L3_INTF_IPV6_ADDR_DESC           "Configure IPv6 Address on interface\n"
#define L3_IP_DST_PRFIX_DESC             "Destination prefix\n"
#define L3_IP_DST_PRFIX_LEN_DESC         "Destination prefix and length\n"
#define L3_IP_DST_PRFIX_MASK_DESC        "Destination prefix mask\n"
#define L3_STATIC_ROUTE_DESC             "Establish static routes\n"
#define L3_STATIC_ROUTE_GW_DESC          "Forwarding router's address\n"
#define L3_STATIC_ROUTE_METRIC_DESC      "Distance metric for this route\n"
#define L3_INTF_DHCPV6_DESC              "DHCPv6 commands\n"
#define L3_INTF_DHCPV6_CLIENT_DESC       "DHCPv6 client commands\n"
#define L3_INTF_DHCPV6_INFO_DESC         "IPv6 DHCP Client information refresh commands\n"
#define L3_INTF_DHCPV6_REFRESH_DESC      "Sets IPv6 DHCP Client information refresh timeout\n"
#define L3_INTF_DHCPV6_REFRESH_MIN_DESC  "Sets IPv6 DHCP Client information refresh timeout minimum\n"
#define L3_INTF_ND_DESC                  "Neighbor Discovery configuration\n"
#define L3_INTF_DAD_DESC                 "Duplicate Address Detection\n"
#define L3_INTF_DAD_ATTEMPT_DESC         "Set IPv6 Duplicate Address Detection Transmits\n"

#endif

#endif 

