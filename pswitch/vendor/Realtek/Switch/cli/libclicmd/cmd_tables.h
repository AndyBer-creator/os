
#ifndef __CMD_TABLES_H__
#define __CMD_TABLES_H__

typedef enum cat_id_e
{
    
    CAT_ID_SPECIAL_START = 0,
    CAT_ID_IF_PORT,
#ifdef CONFIG_SYS_L3_ROUTE
    CAT_ID_IF_VLAN,
    CAT_ID_IF_LO,
#endif
    CAT_ID_LINE,
    CAT_ID_CUSTOM,
    CAT_ID_SPECIAL_END,
    
#ifdef CONFIG_SYS_AAA
    CAT_ID_AAA,
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    CAT_ID_ACL,
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    CAT_ID_AUTHMGR,
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    CAT_ID_DAI,
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    CAT_ID_DHCP_SNOOPING,
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
    CAT_ID_DOS,
#endif
#ifdef CONFIG_SYS_EEE
    CAT_ID_EEE,
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    CAT_ID_GVRP,
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    CAT_ID_GMRP,
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    CAT_ID_IGMP,
  #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    CAT_ID_IGMP_PROFILE,
  #endif
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    CAT_ID_ISG,
#endif
    CAT_ID_PORT,
#ifdef CONFIG_SYS_PORT_SECURITY
    CAT_ID_PORT_SECURITY,
#endif
    CAT_ID_L2,
#ifdef CONFIG_SYS_L3_ROUTE
    CAT_ID_L3,
#endif
#ifdef CONFIG_SYS_PROTO_LACP
    CAT_ID_LACP,
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    CAT_ID_LLDP,
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    CAT_ID_LLDP_MED,
  #endif
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    CAT_ID_MGMTACL,
#endif
#ifdef CONFIG_SYS_MIRROR
    CAT_ID_MIRROR,
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    CAT_ID_MLD,
#endif
    CAT_ID_QOS,
#ifdef CONFIG_SYS_LIB_RADIUS
    CAT_ID_RADIUS,
#endif
    CAT_ID_RATE,
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    CAT_ID_RMON,
#endif
#ifdef CONFIG_SYS_UI_SNMP
    CAT_ID_SNMP,
#endif
#ifdef CONFIG_SYS_PROTO_STP
    CAT_ID_STP,
#endif
    CAT_ID_SWITCH,
#ifdef CONFIG_SYS_SYSLOG
    CAT_ID_SYSLOG,
#endif
    CAT_ID_SYSTEM,
#ifdef CONFIG_SYS_LIB_TACPLUS
    CAT_ID_TACPLUS,
#endif
    CAT_ID_TIME,
    CAT_ID_TRUNK,
#ifdef CONFIG_SYS_PROTO_UDLD
    CAT_ID_UDLD,
#endif
    CAT_ID_VLAN,
#ifdef CONFIG_SYS_VOICEVLAN
    CAT_ID_VLAN_VOICE,
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    CAT_ID_VLAN_PROTO,
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	CAT_ID_VLAN_IP_SUBNET,
#endif
#ifdef CONFIG_SYS_MACVLAN
    CAT_ID_VLAN_MAC,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    CAT_ID_VLAN_SURVEILLANCE,
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
		CAT_ID_ERPS,
#endif
#ifdef CONFIG_SYS_POE
	CAT_ID_POE,
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	CAT_ID_DHCP_SERVER,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_ZEBRA
	CAT_ID_L3_ROUTE_ZEBRA,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	CAT_ID_L3_ROUTE_RIP,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	CAT_ID_L3_ROUTE_OSPF,
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
	CAT_ID_EASYCWMP,
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	CAT_ID_LOOPBACCK,
#endif	

    CAT_ID_END
} cat_id_t;

typedef struct cat_tbl_s
{
    cat_id_t id;
    char     *name;
} cat_tbl_t;

typedef enum cmd_install_attr_e
{
    CMD_ATTR_PRIV_0 = 0,
    CMD_ATTR_PRIV_1,
    CMD_ATTR_PRIV_2,
    CMD_ATTR_PRIV_3,
    CMD_ATTR_PRIV_4,
    CMD_ATTR_PRIV_5,
    CMD_ATTR_PRIV_6,
    CMD_ATTR_PRIV_7,
    CMD_ATTR_PRIV_8,
    CMD_ATTR_PRIV_9,
    CMD_ATTR_PRIV_10,
    CMD_ATTR_PRIV_11,
    CMD_ATTR_PRIV_12,
    CMD_ATTR_PRIV_13,
    CMD_ATTR_PRIV_14,
    CMD_ATTR_PRIV_15,
    CMD_ATTR_NOSORT,
    CMD_ATTR_COND_HIDDEN,
} cmd_install_attr_t;

typedef int32 (*cmd_set_func)(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
typedef int32 (*cmd_ext_func)(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc);
typedef int32 (*cmd_ext_hidmatch_func)(struct cmd_element *cmd, struct vty *vty);
typedef int32 (*cmd_save_func)(FILE *fp);
typedef int32 (*cmd_save_intfPort_func)(FILE *fp, sys_logic_port_t port);
#ifdef CONFIG_SYS_L3_ROUTE
typedef int32 (*cmd_save_intfVlan_func)(FILE *fp, sys_vid_t vid);
typedef int32 (*cmd_save_intfLo_func)(FILE *fp, uint32 id);
#endif
typedef int32 (*cmd_save_line_func)(FILE *fp, sys_line_cliType_t line);

typedef struct cmd_inst_tbl_s
{
    uint32           nodemask;
    uint32           attrmask;
    cmd_ext_func     cmdext;
    cmd_set_func     cmdset;
    char             *cmdstr;
    char             *cmddesc;
} cmd_inst_tbl_t;

typedef struct cmd_save_tbl_s
{
    cat_id_t      id;
    cmd_save_func cmdsave;
} cmd_save_tbl_t;

typedef struct cmd_save_intfPort_tbl_s
{
    cat_id_t               id;
    cmd_save_intfPort_func cmdsave;
} cmd_save_intfPort_tbl_t;

#ifdef CONFIG_SYS_L3_ROUTE
typedef struct cmd_save_intfVlan_tbl_s
{
    cat_id_t               id;
    cmd_save_intfVlan_func cmdsave;
} cmd_save_intfVlan_tbl_t;

typedef struct cmd_save_intfLo_tbl_s
{
    cat_id_t               id;
    cmd_save_intfLo_func cmdsave;
} cmd_save_intfLo_tbl_t;
#endif

typedef struct cmd_save_line_tbl_s
{
    cat_id_t           id;
    cmd_save_line_func cmdsave;
} cmd_save_line_tbl_t;

extern const cat_tbl_t catSaveNameTbl[];
extern const cat_tbl_t catNameTbl[];
extern const cmd_inst_tbl_t cmdCommonTbl[];
extern const cmd_inst_tbl_t cmdSetTbl[];
#ifdef CONFIG_SYS_PLAT_DBGMOD
extern const cmd_inst_tbl_t cmdDebugTbl[];
#endif
extern const cmd_save_tbl_t cmdSaveTbl[];
extern const cmd_save_intfPort_tbl_t cmdSaveIntfPortTbl[];

#define NODE_VIEW           (1 << VIEW_NODE)
#define NODE_PRIVMODE       (1 << ENABLE_NODE)
#define NODE_CONFIG         (1 << CONFIG_NODE)
#define NODE_IF             ((1 << IF_NODE) | (1 << IF_TRK_NODE))
#define NODE_IF_NML         (1 << IF_NODE)
#define NODE_IF_TRK         (1 << IF_TRK_NODE)
#ifdef CONFIG_SYS_L3_ROUTE
#define NODE_IF_VLAN        (1 << IF_VLAN_NODE)
#define NODE_IF_LO          (1 << IF_LO_NODE)
#define NODE_IF_ALL         (NODE_IF | NODE_IF_VLAN | NODE_IF_LO)
#else
#define NODE_IF_ALL         (NODE_IF)
#endif
#define NODE_VTY            (1 << VTY_NODE)
#define NODE_VTY_CONSOLE    (1 << VTY_CONSOLE_NODE)
#define NODE_VLAN_DB        (1 << VLAN_DB_NODE)
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
#define NODE_IGMP_PROFILE   (1 << IGMP_PROFILE_NODE)
#else
#define NODE_IGMP_PROFILE   0
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define NODE_MLD_PROFILE    (1 << MLD_PROFILE_NODE)
#else
#define NODE_MLD_PROFILE    0
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
#define NODE_ACL            ((1 << ACL_MAC_NODE) | (1 << ACL_IPV4_NODE) | (1 << ACL_IPV6_NODE))
#define NODE_ACL_MAC        (1 << ACL_MAC_NODE)
#define NODE_ACL_IPV4       (1 << ACL_IPV4_NODE)
#define NODE_ACL_IPV6       (1 << ACL_IPV6_NODE)
#else
#define NODE_ACL            0
#define NODE_ACL_MAC        0
#define NODE_ACL_IPV4       0
#define NODE_ACL_IPV6       0
#endif
#ifdef CONFIG_SYS_MGMT_ACL
#define NODE_MGMT_ACL       (1 << MGMT_ACL_NODE)
#else
#define NODE_MGMT_ACL       0
#endif
#ifdef CONFIG_SYS_PROTO_STP_MSTP
#define NODE_MST_CFG        (1 << MST_CFG_NODE)
#else
#define NODE_MST_CFG        0
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
#define NODE_ERPS_INST        (1 << ERPS_INST_NODE)
#else
#define NODE_ERPS_INST        0
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
#define NODE_IP_POOL    (1 << IP_POOL_NODE)
#else
#define NODE_IP_POOL    0
#endif

#ifdef CONFIG_SYS_L3_ROUTE_RIP
#define NODE_RIP    (1 << RIP_NODE)
#else
#define NODE_RIP    0
#endif

#ifdef CONFIG_SYS_L3_ROUTE_OSPF
#define NODE_OSPF    (1 << OSPF_NODE)
#define NODE_AREA    (1 << AREA_NODE)
#else
#define NODE_OSPF    0
#define NODE_AREA    0
#endif

#ifdef CONFIG_SYS_L3_ROUTE_BGP
#define NODE_BGP    (1 << BGP_NODE)
#else
#define NODE_BGP    0
#endif

#ifdef CONFIG_SYS_APP_EASYCWMP
#define NODE_CWMP    (1 << CWMP_NODE)
#else
#define NODE_CWMP    0
#endif

#define NODE_TASK_SCHED     0

#define NODE_CONFIG_ALLSUB  (NODE_CONFIG | NODE_IF_ALL | NODE_VTY | NODE_VTY_CONSOLE | NODE_VLAN_DB | NODE_IGMP_PROFILE |  \
							NODE_MLD_PROFILE | NODE_ACL | NODE_MGMT_ACL | NODE_MST_CFG | NODE_TASK_SCHED | NODE_IP_POOL |  \
							NODE_RIP | NODE_OSPF | NODE_AREA | NODE_BGP | NODE_CWMP | NODE_ERPS_INST)
#define NODE_CONFIG_SHOW  ( NODE_PRIVMODE | NODE_CONFIG)

#define ATTR_PRIV_0         (1 << CMD_ATTR_PRIV_0)
#define ATTR_PRIV_1         (1 << CMD_ATTR_PRIV_1)
#define ATTR_PRIV_2         (1 << CMD_ATTR_PRIV_2)
#define ATTR_PRIV_3         (1 << CMD_ATTR_PRIV_3)
#define ATTR_PRIV_4         (1 << CMD_ATTR_PRIV_4)
#define ATTR_PRIV_5         (1 << CMD_ATTR_PRIV_5)
#define ATTR_PRIV_6         (1 << CMD_ATTR_PRIV_6)
#define ATTR_PRIV_7         (1 << CMD_ATTR_PRIV_7)
#define ATTR_PRIV_8         (1 << CMD_ATTR_PRIV_8)
#define ATTR_PRIV_9         (1 << CMD_ATTR_PRIV_9)
#define ATTR_PRIV_10        (1 << CMD_ATTR_PRIV_10)
#define ATTR_PRIV_11        (1 << CMD_ATTR_PRIV_11)
#define ATTR_PRIV_12        (1 << CMD_ATTR_PRIV_12)
#define ATTR_PRIV_13        (1 << CMD_ATTR_PRIV_13)
#define ATTR_PRIV_14        (1 << CMD_ATTR_PRIV_14)
#define ATTR_PRIV_15        (1 << CMD_ATTR_PRIV_15)
#define ATTR_COND_HIDDEN    (1 << CMD_ATTR_COND_HIDDEN)

#endif 

