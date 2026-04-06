
typedef int32 (*cgi_func)(void);

typedef struct cgi_cmdmap_s
{
    char     *cmdstr;
    cgi_func cmdfunc;
} cgi_cmdmap_t;

typedef struct cgi_catmap_s
{
    char         *catstr;
    const cgi_cmdmap_t *cmdmap;
} cgi_catmap_t;

#define CGI_CMD                     "cmd"

#ifdef CONFIG_SYS_AAA
#define CGI_CAT_AAA                 "aaa"
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
#define CGI_CAT_ACL                 "acl"
#endif
#ifdef CONFIG_SYS_MGMT_ACL
#define CGI_CAT_ACL_MGMT            "aclMgmt"
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#define CGI_CAT_AUTHMGR             "authmgr"
#endif
#ifdef CONFIG_SYS_PROTO_DAI
#define CGI_CAT_DAI                 "dai"
#endif
#define CGI_CAT_DIAG                "diag"
#ifdef CONFIG_SYS_DOS_PROTECTION
#define CGI_CAT_DOS                  "dos"
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#define CGI_CAT_DS                  "ds"
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
#define CGI_CAT_DR                  "dr"
#endif
#define CGI_CAT_FILE                "file"
#ifdef CONFIG_SYS_PROTO_GVRP
#define CGI_CAT_GVRP                "gvrp"
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#define CGI_CAT_GMRP                "gmrp"
#endif
#define CGI_CAT_HOME                "home"
#ifdef CONFIG_SYS_PROTO_ISG
#define CGI_CAT_IPSG                "ipsg"
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#define CGI_CAT_L3                  "l3"
#endif
#define CGI_CAT_LAG                 "lag"
#ifdef CONFIG_SYS_PROTO_LLDP
#define CGI_CAT_LLDP                "lldp"
#endif
#ifdef CONFIG_SYS_SYSLOG
#define CGI_CAT_LOG                 "log"
#endif
#define CGI_CAT_MAC                 "mac"
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#define CGI_CAT_MCAST               "mcast"
#endif
#define CGI_CAT_PANEL               "panel"
#define CGI_CAT_PORT                "port"
#ifdef CONFIG_SYS_PORT_SECURITY
#define CGI_CAT_PSECURE             "psecure"
#endif
#define CGI_CAT_QOS                 "qos"
#ifdef CONFIG_SYS_LIB_RADIUS
#define CGI_CAT_RADIUS              "radius"
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#define CGI_CAT_RMON                "rmon"
#endif
#ifdef CONFIG_SYS_UI_SNMP
#define CGI_CAT_SNMP                "snmp"
#endif
#ifdef CONFIG_SYS_PROTO_STP
#define CGI_CAT_STP                 "stp"
#endif
#define CGI_CAT_STORM               "storm"
#define CGI_CAT_SYS                 "sys"
#ifdef CONFIG_SYS_LIB_TACPLUS
#define CGI_CAT_TACPLUS             "tacplus"
#endif
#define CGI_CAT_TIME                "time"
#ifdef CONFIG_SYS_PROTO_UDLD
#define CGI_CAT_UDLD                "udld"
#endif
#define CGI_CAT_VLAN                "vlan"
#ifdef CONFIG_SYS_MACVLAN
#define CGI_CAT_VLAN_MAC            "vlanmac"
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
#define CGI_CAT_VLAN_PROTOCOL       "vlanprotocol"
#endif
#ifdef CONFIG_SYS_IP_SUBNET
#define CGI_CAT_VLAN_IP_SUBNET       "vlanipsubnet"
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#define CGI_CAT_VLAN_SURVEILLANCE   "vlansurveillance"
#endif
#ifdef CONFIG_SYS_VOICEVLAN
#define CGI_CAT_VLAN_VOICE          "vlanvoice"
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
#define CGI_CAT_ERPS          "erps"
#endif
#ifdef CONFIG_SYS_POE
#define CGI_CAT_POE                 "poe"
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
#define CGI_CAT_RIP                 "rip"
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
#define CGI_CAT_OSPF                 "ospf"
#endif
#ifdef CONFIG_SYS_L3_VRRP
#define CGI_CAT_VRRP                 "vrrp"
#endif

#ifdef CONFIG_SYS_AAA

#define CGI_CMD_AAA_LOGIN           "login"
#define CGI_CMD_AAA_LOGINADD        "loginAdd"
#define CGI_CMD_AAA_LOGINEDIT       "loginEdit"
#define CGI_CMD_AAA_LOGINDEL        "loginDel"
#define CGI_CMD_AAA_LOGINAUTH       "loginAuth"
#endif

#ifdef CONFIG_SYS_USER_DEFINED_ACL

#define CGI_CMD_ACL_ACL            "acl"
#define CGI_CMD_ACL_ACL_DEL        "aclDel"
#define CGI_CMD_ACL_ACE            "ace"
#define CGI_CMD_ACL_ACEDEL         "aceDel"
#define CGI_CMD_ACL_BINDING        "binding"
#define CGI_CMD_ACL_BINDING_DEL    "bindingDel"
#endif

#ifdef CONFIG_SYS_MGMT_ACL

#define CGI_CMD_ACL_MGMT_ACL            "acl"
#define CGI_CMD_ACL_MGMT_ACL_DEL        "aclDel"
#define CGI_CMD_ACL_MGMT_ACL_ACT        "aclActive"
#define CGI_CMD_ACL_MGMT_ACL_DEACT      "aclDeactive"
#define CGI_CMD_ACL_MGMT_ACE            "ace"
#define CGI_CMD_ACL_MGMT_ACEEDIT        "aceEdit"
#define CGI_CMD_ACL_MGMT_ACEDEL         "aceDel"
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

#define CGI_CMD_AUTHMGR_PROP         "prop"
#define CGI_CMD_AUTHMGR_PROPEDIT     "propEdit"
#define CGI_CMD_AUTHMGR_PORT         "port"
#define CGI_CMD_AUTHMGR_PORTEDIT     "portEdit"
  #ifdef CONFIG_SYS_PROTO_MACAUTH
#define CGI_CMD_AUTHMGR_MACACCT      "macAcct"
#define CGI_CMD_AUTHMGR_MACACCTADD   "macAcctAdd"
#define CGI_CMD_AUTHMGR_MACACCTEDIT  "macAcctEdit"
#define CGI_CMD_AUTHMGR_MACACCTDEL   "macAcctDel"
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
#define CGI_CMD_AUTHMGR_WEBACCT      "webAcct"
#define CGI_CMD_AUTHMGR_WEBACCTADD   "webAcctAdd"
#define CGI_CMD_AUTHMGR_WEBACCTEDIT  "webAcctEdit"
#define CGI_CMD_AUTHMGR_WEBACCTDEL   "webAcctDel"
  #endif
#define CGI_CMD_AUTHMGR_SESS         "sess"
#define CGI_CMD_AUTHMGR_SESSCLEAR    "sessClear"
#endif

#ifdef CONFIG_SYS_PROTO_DAI

#define CGI_CMD_DAI_PROP            "prop"
#define CGI_CMD_DAI_PORTEDIT        "portEdit"
#define CGI_CMD_DAI_STATISTIC       "statistics"
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

#define CGI_CMD_DS_PROP            "prop"
#define CGI_CMD_DS_PORTEDIT        "portEdit"
#define CGI_CMD_DS_STATISTIC       "statistics"
#define CGI_CMD_DS_82_PROP         "option82Prop"
#define CGI_CMD_DS_82_PORTEDIT     "option82PortEdit"
#define CGI_CMD_DS_82_CID          "option82CID"
#define CGI_CMD_DS_82_CID_DEL      "option82CIDDel"

#endif

#ifdef CONFIG_SYS_DHCP_SERVER

#define CGI_CMD_DR_PROP            "prop"
#define CGI_CMD_DR_PORTEDIT        "portEdit"
#define CGI_CMD_DR_STATISTIC       "statistics"
#define CGI_CMD_DR_82_PROP         "option82Prop"
#define CGI_CMD_DR_82_PORTEDIT     "option82PortEdit"
#define CGI_CMD_DR_82_CID          "option82CID"
#define CGI_CMD_DR_82_CID_DEL      "option82CIDDel"
#define CGI_CMD_DR_IPPOOL          "ipPool"
#define CGI_CMD_DR_IPPOOLEDIT      "ipPoolEdit"
#define CGI_CMD_DR_IPPOOLDEL       "ipPoolDel"
#define CGI_CMD_DR_IPPOOLADD       "ipPoolAdd"
#define CGI_CMD_DR_VLANIFIPPOOL    "vlanifipPool"
#define CGI_CMD_DR_GROUP_EDIT      "groupEdit"
#define CGI_CMD_DR_GROUP_ADD       "groupAdd"
#define CGI_CMD_DR_GROUP_DEL       "groupDel"
#define CGI_CMD_DR_CLIENTS         "clientLists"
#define CGI_CMD_DR_STATIC_BIND     "staticBind"
#define CGI_CMD_DR_STATIC_BIND_EDIT   "staticBindEdit"
#define CGI_CMD_DR_STATIC_BIND_ADD   "staticBindAdd"
#define CGI_CMD_DR_STATIC_BIND_DEL   "staticBindDel"

#endif

#ifdef CONFIG_SYS_CABLE_DIAG
#define CGI_CMD_DIAG_COPPER             "copper"
#define CGI_CMD_DIAG_COPPER_RESULT      "copperResult"
#endif
#ifdef CONFIG_SYS_MIRROR
#define CGI_CMD_DIAG_MIRROR             "mirror"
#define CGI_CMD_DIAG_MIRROREDIT         "mirrorEdit"
#endif
#ifdef CONFIG_SYS_PING_TEST
#define CGI_CMD_DIAG_PING               "ping"
#define CGI_CMD_DIAG_PING_ABORT         "pingAbort"
#define CGI_CMD_DIAG_PING_RESULT        "pingResult"
#define CGI_CMD_DIAG_PING_RUNNING       "pingRunning"
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
#define CGI_CMD_DIAG_TECH_GEN           "techGen"
#define CGI_CMD_DIAG_TECH_GEN_STATUS    "techGenStatus"
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
#define CGI_CMD_DIAG_TRACEROUTE         "traceroute"
#define CGI_CMD_DIAG_TRACEROUTE_ABORT   "tracerouteAbort"
#define CGI_CMD_DIAG_TRACEROUTE_RESULT  "tracerouteResult"
#define CGI_CMD_DIAG_TRACEROUTE_RUNNING "tracerouteRunning"
#endif
#ifdef CONFIG_SYS_OPTICALMOD_STATUS
#define CGI_CMD_DIAG_OPTICAL            "optical"
#define CGI_CMD_DIAG_OPTICAL_INTF       "opticalIntf"
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
#define CGI_CMD_DOS_PROP            "prop"
#define CGI_CMD_DOS_PORT            "port"
#define CGI_CMD_DOS_PORTEDIT        "portEdit"
#endif

#define CGI_CMD_FILE_IMG                "img"
#define CGI_CMD_FILE_CFG                "cfg"
#define CGI_CMD_FILE_BACKUP             "backup"
#define CGI_CMD_FILE_BACKUP_STATUS      "bakStatus"
#define CGI_CMD_FILE_UPGRADE            "upgrade"
#define CGI_CMD_FILE_UPGRADE_STATUS     "upStatus"
#define CGI_CMD_FILE_DUAL_IMAGE         "dualImg"
#define CGI_CMD_FILE_CFG_SAVE           "savecfg"
#define CGI_CMD_FILE_CFG_SAVE_STATUS    "saveStatus"

#ifdef CONFIG_SYS_PROTO_GVRP

#define CGI_CMD_GVRP                    "prop"
#define CGI_CMD_GVRP_INTFEDIT           "propEdit"
#define CGI_CMD_GVRP_MEMBER            "member"
#define CGI_CMD_GVRP_INTFCNT            "portCnt"
#define CGI_CMD_GVRP_INTFCNTCLR       "portClr"
#endif

#ifdef CONFIG_SYS_PROTO_GARP

#define CGI_CMD_GMRP                    "prop"
#define CGI_CMD_GMRP_INTFEDIT           "propEdit"
#endif

#define CGI_CMD_HOME_LOGIN          "login"
#define CGI_CMD_HOME_LOGINAUTH      "loginAuth"
#define CGI_CMD_HOME_LOGINSTATUS    "loginStatus"
#define CGI_CMD_HOME_SAVE           "save"
#define CGI_CMD_HOME_LOGOUT         "logout"
#define CGI_CMD_HOME_MAIN           "main"
#define CGI_CMD_HOME_MONITOR        "monitor"
#define CGI_CMD_HOME_EMPTY          "empty"
#define CGI_CMD_HOME_STARTING       "starting"
#define CGI_CMD_HOME_REFRESH        "refresh"
#define CGI_CMD_HOME_DEBUG          "debug"
#define CGI_CMD_HOME_DEBUGDEL       "debugDel"
#define CGI_CMD_HOME_SYSTEMSET      "systemSet"
#define CGI_CMD_HOME_SYSOEMSET      "sysOemSet"

#ifdef CONFIG_SYS_PROTO_ISG

#define CGI_CMD_IPSG_PROP            "prop"
#define CGI_CMD_IPSG_PORTEDIT        "portEdit"
#define CGI_CMD_IPSG_BINDING         "binding"
#define CGI_CMD_IPSG_BINDINGDEL      "bindingDel"
#define CGI_CMD_IPSG_DATABASE        "database"
#endif

#ifdef CONFIG_SYS_L3_ROUTE

#define CGI_CMD_L3_V4INTF           "v4Intf"
#define CGI_CMD_L3_V4INTFADD        "v4IntfAdd"
#define CGI_CMD_L3_V4INTFEDIT       "v4IntfEdit"
#define CGI_CMD_L3_V4INTFDEL        "v4IntfDel"
#define CGI_CMD_L3_V4ROUTE          "v4Route"
#define CGI_CMD_L3_V4ROUTEADD       "v4RouteAdd"
#define CGI_CMD_L3_V4ROUTEEDIT      "v4RouteEdit"
#define CGI_CMD_L3_V4ROUTEDEL       "v4RouteDel"
#define CGI_CMD_L3_V4ARP            "v4Arp"
#define CGI_CMD_L3_V4ARPADD         "v4ArpAdd"
#define CGI_CMD_L3_V4ARPEDIT        "v4ArpEdit"
#define CGI_CMD_L3_V4ARPDEL         "v4ArpDel"
#define CGI_CMD_L3_V4ARPPROXY       "v4ArpProxy"

#define CGI_CMD_L3_V6INTF           "v6Intf"
#define CGI_CMD_L3_V6INTFADD        "v6IntfAdd"
#define CGI_CMD_L3_V6INTFEDIT       "v6IntfEdit"
#define CGI_CMD_L3_V6INTFDEL        "v6IntfDel"
#define CGI_CMD_L3_V6ADDR           "v6Addr"
#define CGI_CMD_L3_V6ADDRADD        "v6AddrAdd"
#define CGI_CMD_L3_V6ADDRDEL        "v6AddrDel"
#define CGI_CMD_L3_V6ROUTE          "v6Route"
#define CGI_CMD_L3_V6ROUTEADD       "v6RouteAdd"
#define CGI_CMD_L3_V6ROUTEEDIT      "v6RouteEdit"
#define CGI_CMD_L3_V6ROUTEDEL       "v6RouteDel"
#define CGI_CMD_L3_V6NEIGH          "v6Neigh"
#define CGI_CMD_L3_V6NEIGHADD       "v6NeighAdd"
#define CGI_CMD_L3_V6NEIGHEDIT      "v6NeighEdit"
#define CGI_CMD_L3_V6NEIGHDEL       "v6NeighDel"
#endif

#define CGI_CMD_LAG_MGMT            "mgmt"
#define CGI_CMD_LAG_MGMTEDIT        "mgmtEdit"
#define CGI_CMD_LAG_PORT            "port"
#define CGI_CMD_LAG_PORTEDIT        "portEdit"
#ifdef CONFIG_SYS_PROTO_LACP
#define CGI_CMD_LAG_LACP            "lacp"
#define CGI_CMD_LAG_LACPEDIT        "lacpEdit"
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
#define CGI_CMD_GET_ERPS             "erpsGet"
#define CGI_CMD_GET_ERPS_EDIT        "erpsGetEdit"
#define CGI_CMD_SET_ERPS_EDIT        "erpsSetEdit"
#define CGI_CMD_SET_ERPS_INSTANCE    "erpsSetInstance"
#define CGI_CMD_SET_ERPS_GLOBAL_STATUS    "erpsGetGlobalStatus"

#define CGI_CMD_GET_ERPS_VAL         "erps"
#define CGI_CMD_GET_ERPS_RING_INS    "ring"
#define CGI_CMD_SET_ERPS_VAL         "erps"
#define CGI_CMD_SET_ERPS             "erpsSet"
#define CGI_CMD_DEL_ERPS_INSTANCE     "erpsDelInstance"

#define CGI_CMD_STATUS_ERPS          "erpsStatus"
#define CGI_CMD_ERPS_ALL_ARREAY      "rings"
#define	CGI_CMD_ERPS_INSTANCE        "erpsInstance"
#define	CGI_CMD_ERPS_INSTANCE_ENABLE "erpsInstanceEnable"
#define	CGI_CMD_ERPS_INSTANCE_EXIST  "erpsInstanceExist"
#define	CGI_CMD_ERPS_RINGSTATUS      "erpsRingStatus"
#define	CGI_CMD_ERPS_RINGMEL         "erpsRingMel"
#define	CGI_CMD_ERPS_RINGCONTROLVLAN "erpsRingControlVlan"
#define	CGI_CMD_ERPS_RINGWTRTIME     "erpsRingWtrTime"
#define	CGI_CMD_ERPS_RINGGUARDTIME   "erpsRingGuardTime"
#define	CGI_CMD_ERPS_RINGWORKINGMODE "erpsRingWorkingMode"
#define	CGI_CMD_ERPS_RINGID          "erpsRingId"
#define	CGI_CMD_ERPS_RINGRINGLEVEL   "erpsRingRingLevel"
#define	CGI_CMD_ERPS_RINGRINGPROINST "erpsRingRingProtectedInstance"

#define	CGI_CMD_ERPS_RINGPORT0       "erpsRingPort0"
#define	CGI_CMD_ERPS_RINGPORT0ROLE   "erpsRingPort0Role"
#define	CGI_CMD_ERPS_RINGPORT0STATUS "erpsRingPort0Status"
#define	CGI_CMD_ERPS_RINGPORT1       "erpsRingPort1"
#define	CGI_CMD_ERPS_RINGPORT1ROLE   "erpsRingPort1Role"
#define	CGI_CMD_ERPS_RINGPORT1STATUS "erpsRingPort1Status"
#define	CGI_CMD_ERPS_RINGNODESTATUS  "erpsRingNodeStatus"
#define	CGI_CMD_ERPS_RINGNODEINSTANCEID        "instance"
#define	CGI_CMD_ERPS_RINGNODEINSTANCEIDMIN     "instance_min"
#define	CGI_CMD_ERPS_RINGNODEINSTANCEIDMAX     "instance_max"
#define	CGI_CMD_ERPS_RINGNODEINSTANCEIDDEF     "instance_def"

#define	CGI_CMD_ERPS_INSTANCE_EDIT_SEL_RING    "selRing"
#define	CGI_CMD_ERPS_INSTANCE_ADD_AND_DEL      "erpsInstanceAddOrDel"
#define	CGI_CMD_ERPS_RINGPORTROLEVALUE1           "erpsRingPortRoleValue1"
#define	CGI_CMD_ERPS_RINGPORTROLEVALUE0           "erpsRingPortRoleValue0"
#endif

#ifdef CONFIG_SYS_PROTO_LLDP

#define CGI_CMD_LLDP_PROP           "prop"
#define CGI_CMD_LLDP_PORT           "port"
#define CGI_CMD_LLDP_PORTEDIT       "portEdit"
#ifdef CONFIG_SYS_PROTO_LLDP_MED
#define CGI_CMD_LLDP_MEDPOLICY      "medPolicy"
#define CGI_CMD_LLDP_MEDPOLICYADD   "medPolicyAdd"
#define CGI_CMD_LLDP_MEDPOLICYEDIT  "medPolicyEdit"
#define CGI_CMD_LLDP_MEDPOLICYDEL   "medPolicyDel"
#define CGI_CMD_LLDP_MEDPORT        "medPort"
#define CGI_CMD_LLDP_MEDPORTEDIT    "medPortEdit"
#endif
#define CGI_CMD_LLDP_LOCAL          "local"
#define CGI_CMD_LLDP_LOCALDETAIL    "localDetail"
#define CGI_CMD_LLDP_NEIGHBOR       "neighbor"
#define CGI_CMD_LLDP_NEIGHBORDETAIL "neighborDetail"
#define CGI_CMD_LLDP_NEIGHBORCLEAR "neighborClear"
#define CGI_CMD_LLDP_NEIGHBORREFRESH "neighborRefresh"
#define CGI_CMD_LLDP_STATISTIC      "statistic"
#define CGI_CMD_LLDP_GLOBALSTATISTICSCLEAR "globalStatisticsClear"
#define CGI_CMD_LLDP_PORTSTATISTICSCLEAR "portStatisticsClear"
#define CGI_CMD_LLDP_STATISTICREFRESH "statisticRrefresh"
#define CGI_CMD_LLDP_OVERLOAD       "overload"
#define CGI_CMD_LLDP_OVERLOADDETAIL "overloadDetail"
#define CGI_CMD_LLDP_OVERLOADREFRESH "overloadRefresh"
#endif

#ifdef CONFIG_SYS_SYSLOG

#define CGI_CMD_LOG_SYSLOG          "syslog"
#define CGI_CMD_LOG_GLOBAL          "global"
#define CGI_CMD_LOG_REMOTE          "remote"
#define CGI_CMD_LOG_REMOTE_ADD      "remoteAdd"
#define CGI_CMD_LOG_REMOTE_EDIT     "remoteEdit"
#define CGI_CMD_LOG_REMOTE_DEL      "remoteDel"
#define CGI_CMD_LOG_CLEAR           "clear"
#endif

#define CGI_CMD_MAC_MISCSTATUS      "miscStatus"
#define CGI_CMD_MAC_DYNAMIC         "dynamic"
#define CGI_CMD_MAC_DYNAMICAGING    "dynamicAging"
#define CGI_CMD_MAC_DYNAMICTOSTATIC "dynamicToStatic"
#define CGI_CMD_MAC_STATIC          "static"
#define CGI_CMD_MAC_STATICADD       "staticAdd"
#define CGI_CMD_MAC_STATICEDIT      "staticEdit"
#define CGI_CMD_MAC_STATICDELETE    "staticDelete"
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
#define CGI_CMD_MAC_FILTER          "filter"
#define CGI_CMD_MAC_FILTERADD       "filterAdd"
#define CGI_CMD_MAC_FILTEREDIT      "filterEdit"
#define CGI_CMD_MAC_FILTERDELETE    "filterDelete"
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

#define CGI_CMD_MCAST_GEN_PROP        "genProp"
#define CGI_CMD_MCAST_GEN_GROUP        "genGroup"
#define CGI_CMD_MCAST_GEN_GROUPADD        "genGroupAdd"
#define CGI_CMD_MCAST_GEN_GROUPEDIT        "genGroupEdit"
#define CGI_CMD_MCAST_GEN_GROUPDEL        "genGroupDel"

#define CGI_CMD_MCAST_GEN_ROURTER        "genRouter"
#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
#define CGI_CMD_MCAST_GEN_ROURTERADD        "genRouterAdd"
#define CGI_CMD_MCAST_GEN_ROURTEREDIT        "genRouterEdit"
#define CGI_CMD_MCAST_GEN_ROURTERDEL        "genRouterDel"
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
#define CGI_CMD_MCAST_GEN_FWDALL        "genFwdAll"
#define CGI_CMD_MCAST_GEN_FWDALLADD        "genFwdAllAdd"
#define CGI_CMD_MCAST_GEN_FWDALLEDIT        "genFwdAllEdit"
#define CGI_CMD_MCAST_GEN_FWDALLDEL        "genFwdAllDel"
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
#define CGI_CMD_MCAST_GEN_THROT        "genThrot"
#define CGI_CMD_MCAST_GEN_THROTEDIT        "genThrotEdit"
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
#define CGI_CMD_MCAST_GEN_PROFILE        "genProfile"
#define CGI_CMD_MCAST_GEN_PROFILEADD        "genProfileAdd"
#define CGI_CMD_MCAST_GEN_PROFILEEDIT        "genProfileEdit"
#define CGI_CMD_MCAST_GEN_PROFILEDEL        "genProfileDel"

#define CGI_CMD_MCAST_GEN_FILTER        "genFilter"
#define CGI_CMD_MCAST_GEN_FILTEREDIT        "genFilterEdit"
#endif

#define CGI_CMD_MCAST_IGMP_PROP        "igmpProp"
#define CGI_CMD_MCAST_IGMP_PROPEDIT        "igmpPropEdit"
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
#define CGI_CMD_MCAST_IGMP_QUERIER        "igmpQuerier"
#define CGI_CMD_MCAST_IGMP_QUERIEREDIT        "igmpQuerierEdit"
#endif
#define CGI_CMD_MCAST_IGMP_STATIC             "igmpStat"
#define CGI_CMD_MCAST_IGMP_STATICCLR             "igmpStatClear"

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define CGI_CMD_MCAST_MLD_PROP        "mldProp"
#define CGI_CMD_MCAST_MLD_PROPEDIT        "mldPropEdit"
#define CGI_CMD_MCAST_MLD_STATIC             "mldStat"
#define CGI_CMD_MCAST_MLD_STATICCLR             "mldStatClear"
#endif 

#ifdef CONFIG_SYS_PROTO_MVR
#define CGI_CMD_MCAST_MVR_PROP        "mvrProp"
#define CGI_CMD_MCAST_MVR_GROUP        "mvrGroup"
#define CGI_CMD_MCAST_MVR_GROUPADD        "mvrGroupAdd"
#define CGI_CMD_MCAST_MVR_GROUPEDIT        "mvrGroupEdit"
#define CGI_CMD_MCAST_MVR_GROUPDEL        "mvrGroupDel"
#define CGI_CMD_MCAST_MVR_PORT           "mvrPort"
#define CGI_CMD_MCAST_MVR_PORTEDIT        "mvrPortEdit"
#endif 
#endif 

#define CGI_CMD_PANEL_LAYOUT        "layout"
#define CGI_CMD_PANEL_INFO          "info"

#define CGI_CMD_PORT_PORT           "port"
#define CGI_CMD_PORT_PORTEDIT       "portEdit"
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
#define CGI_CMD_PORT_ERRDISABLESTATUS     "errDisableStatus"
#define CGI_CMD_PORT_ERRDISABLE     "errDisable"
#endif
#define CGI_CMD_PORT_JUMBO          "jumbo"
#ifdef CONFIG_SYS_EEE
#define CGI_CMD_PORT_EEE            "eee"
#define CGI_CMD_PORT_EEEEDIT        "eeeEdit"
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION
#define CGI_CMD_PORT_PROTECT        "protect"
#define CGI_CMD_PORT_PROTECTEDIT    "protectEdit"
#endif
#define CGI_CMD_PORT_CNT            "cnt"
#define CGI_CMD_PORT_CNTCLEAR       "cntClear"
#ifdef CONFIG_SYS_BW_UTILIZATION
#define CGI_CMD_PORT_BWUTLIZ        "bwutilz"
#endif

#ifdef CONFIG_SYS_PORT_SECURITY

#define CGI_CMD_PSECURE_PROP        "prop"
#define CGI_CMD_PSECURE_PORTEDIT    "portEdit"
#define CGI_CMD_PSECURE_MAC         "mac"
#define CGI_CMD_PSECURE_MACADD      "macAdd"
#define CGI_CMD_PSECURE_MACEDIT     "macEdit"
#define CGI_CMD_PSECURE_MACDEL      "macDel"
#endif

#define CGI_CMD_QOS_PROP                "prop"
#define CGI_CMD_QOS_PORTEDIT            "portEdit"
#define CGI_CMD_QOS_QUEUE               "queue"
#define CGI_CMD_QOS_MAP_COS             "mapCos"
#define CGI_CMD_QOS_MAP_DSCP            "mapDscp"
#define CGI_CMD_QOS_MAP_IPP             "mapIpp"
#ifdef CONFIG_SYS_BW_CTRL
#define CGI_CMD_QOS_RATE_CTRL           "rateCtrl"
#define CGI_CMD_QOS_RATE_CTRLEDIT       "rateCtrlEdit"
#define CGI_CMD_QOS_RATE_EGRQUE         "rateEgrQue"
#define CGI_CMD_QOS_RATE_EGRQUEEDIT     "rateEgrQueEdit"
#endif

#ifdef CONFIG_SYS_LIB_RADIUS

#define CGI_CMD_RADIUS                  "srv"
#define CGI_CMD_RADIUSADD               "srvAdd"
#define CGI_CMD_RADIUSEDIT              "srvEdit"
#define CGI_CMD_RADIUSDEL               "srvDel"
#endif

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#define CGI_CMD_RMON_STATS          "statistics"
#define CGI_CMD_RMON_STATS_INTF     "statisticsIntf"
#define CGI_CMD_RMON_STATS_CLEAR    "statisticsClear"
#define CGI_CMD_RMON_HISTORY        "history"
#define CGI_CMD_RMON_HISTORY_ADD    "historyAdd"
#define CGI_CMD_RMON_HISTORY_EDIT   "historyEdit"
#define CGI_CMD_RMON_HISTORY_DEL    "historyDel"
#define CGI_CMD_RMON_HISTORY_LOG    "historyLog"

#define CGI_CMD_RMON_EVENT          "event"
#define CGI_CMD_RMON_EVENT_ADD      "eventAdd"
#define CGI_CMD_RMON_EVENT_EDIT     "eventEdit"
#define CGI_CMD_RMON_EVENT_DEL      "eventDel"
#define CGI_CMD_RMON_EVENT_LOG      "eventLog"

#define CGI_CMD_RMON_ALARM          "alarm"
#define CGI_CMD_RMON_ALARM_ADD      "alarmAdd"
#define CGI_CMD_RMON_ALARM_EDIT     "alarmEdit"
#define CGI_CMD_RMON_ALARM_DEL      "alarmDel"
#endif

#ifdef CONFIG_SYS_PROTO_STP

#define CGI_CMD_STP_GLOBAL              "global"
#define CGI_CMD_STP_INTF                "intf"
#define CGI_CMD_STP_INTF_EDIT           "intfEdit"
#define CGI_CMD_STP_INTF_MIGRATE        "intfMigrate"
#define CGI_CMD_STP_MSTI                "msti"
#define CGI_CMD_STP_MSTI_EDIT           "mstiEdit"
#define CGI_CMD_STP_MSTI_INTF           "mstiIntf"
#define CGI_CMD_STP_MSTI_INTF_EDIT      "mstiIntfEdit"
#define CGI_CMD_STP_STATS               "statistics"
#define CGI_CMD_STP_STATS_INTF          "statisticsIntf"
#define CGI_CMD_STP_STATS_INTF_CLEAR    "statisticsIntfClear"
#endif

#ifdef CONFIG_SYS_STORM_CTRL

#define CGI_CMD_STORM                 "control"
#define CGI_CMD_STORMEDIT             "controlEdit"
#endif

#define CGI_CMD_SYS_SYSINFO         "sysinfo"
#define CGI_CMD_SYS_SYSINFOEDIT     "sysinfoEdit"
#define CGI_CMD_SYS_CPUMEM          "cpumem"
#define CGI_CMD_SYS_IP              "ip"
#define CGI_CMD_SYS_IPV4            "ipv4"
#define CGI_CMD_SYS_IPV6            "ipv6"
#define CGI_CMD_SYS_ACCT            "acct"
#define CGI_CMD_SYS_ACCTADD         "acctAdd"
#define CGI_CMD_SYS_ACCTEDIT        "acctEdit"
#define CGI_CMD_SYS_ACCTDEL         "acctDel"
#define CGI_CMD_SYS_REBOOT          "reboot"
#define CGI_CMD_SYS_RESTORE         "restore"
#define CGI_CMD_SYS_LINE            "line"
#ifdef CONFIG_SYS_ADVANCE_DNS
#define CGI_CMD_SYS_DNS             "dns"
#define CGI_CMD_SYS_DNS_ADD         "dnsAdd"
#define CGI_CMD_SYS_DNS_DEL         "dnsDel"
#define CGI_CMD_SYS_DNS_HOSTS       "dnsHosts"
#define CGI_CMD_SYS_DNS_HOSTS_ADD   "dnsHostsAdd"
#define CGI_CMD_SYS_DNS_HOSTS_DEL   "dnsHostsDel"
#endif
#define CGI_CMD_SYS_SYSTIME         "sysTime"

#define CGI_CMD_TIME_TIME            "time"

#ifdef CONFIG_SYS_UI_SNMP
#define CGI_CMD_SNMP_COMMUNITY          "community"
#define CGI_CMD_SNMP_COMMUNITYADD           "communityAdd"
#define CGI_CMD_SNMP_COMMUNITYEDIT           "communityEdit"
#define CGI_CMD_SNMP_COMMUNITYDEL           "communityDel"

#define CGI_CMD_SNMP_TRAPEVENT              "trapEvent"

#define CGI_CMD_SNMP_NOTIFY          "notify"
#define CGI_CMD_SNMP_NOTIFYADD          "notifyAdd"
#define CGI_CMD_SNMP_NOTIFYEDIT          "notifyEdit"
#define CGI_CMD_SNMP_NOTIFYDEL          "notifyDel"

#ifdef CONFIG_SYS_SNMPV3
#define CGI_CMD_SNMP_VIEW                    "view"
#define CGI_CMD_SNMP_VIEWADD                    "viewAdd"
#define CGI_CMD_SNMP_VIEWDEL                      "viewDel"

#define CGI_CMD_SNMP_GROUP                           "group"
#define CGI_CMD_SNMP_GROUPADD                       "groupAdd"
#define CGI_CMD_SNMP_GROUPEDIT                       "groupEdit"
#define CGI_CMD_SNMP_GROUPDEL           "groupDel"

#define CGI_CMD_SNMP_USER               "user"
#define CGI_CMD_SNMP_USERADD            "userAdd"
#define CGI_CMD_SNMP_USEREDIT            "userEdit"
#define CGI_CMD_SNMP_USERDEL            "userDel"

#define CGI_CMD_SNMP_ENGINEID    "engineID"
#define CGI_CMD_SNMP_ENGINEIDADD    "engineIDAdd"
#define CGI_CMD_SNMP_ENGINEIDEDIT    "engineIDEdit"
#define CGI_CMD_SNMP_ENGINEIDDEL    "engineIDDel"

#endif 
#endif

#ifdef CONFIG_SYS_LIB_TACPLUS

#define CGI_CMD_TACPLUS                 "srv"
#define CGI_CMD_TACPLUSADD              "srvAdd"
#define CGI_CMD_TACPLUSEDIT             "srvEdit"
#define CGI_CMD_TACPLUSDEL              "srvDel"
#endif

#ifdef CONFIG_SYS_PROTO_UDLD

#define CGI_CMD_UDLD_PROP            "prop"
#define CGI_CMD_UDLD_PORTEDIT        "portEdit"
#define CGI_CMD_UDLD_NEIGHBOR        "neighbor"
#endif

#define CGI_CMD_VLAN_CREATE          "create"
#define CGI_CMD_VLAN_EDIT            "edit"
#define CGI_CMD_VLAN_DEL             "del"
#define CGI_CMD_VLAN_CONF            "conf"
#define CGI_CMD_VLAN_PORT            "port"
#define CGI_CMD_VLAN_PORTEDIT        "portEdit"
#define CGI_CMD_VLAN_MEMBERSHIP      "membership"
#define CGI_CMD_VLAN_MEMBERSHIPEDIT  "membershipEdit"

#ifdef CONFIG_SYS_MANAGEMENT_VLAN
#define CGI_CMD_VLAN_MGMT            "mgmt"
#endif

#ifdef CONFIG_SYS_MACVLAN
#define CGI_CMD_VLAN_MAC_GROUP         "group"
#define CGI_CMD_VLAN_MAC_GROUP_ADD     "groupAdd"
#define CGI_CMD_VLAN_MAC_GROUPDEL      "groupDel"
#define CGI_CMD_VLAN_MAC_BINDING       "binding"
#define CGI_CMD_VLAN_MAC_BINDINGDEL    "bindingDel"
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
#define CGI_CMD_VLAN_PROTOCOL_GROUP         "group"
#define CGI_CMD_VLAN_PROTOCOL_GROUPDEL      "groupDel"
#define CGI_CMD_VLAN_PROTOCOL_BINDING       "binding"
#define CGI_CMD_VLAN_PROTOCOL_BINDINGDEL    "bindingDel"
#endif

#ifdef CONFIG_SYS_IP_SUBNET
#define CGI_CMD_VLAN_IPSUBNET_GROUP         "group"
#define CGI_CMD_VLAN_IPSUBNET_GROUPDEL      "groupDel"
#define CGI_CMD_VLAN_IPSUBNET_BINDING       "binding"
#define CGI_CMD_VLAN_IPSUBNET_BINDINGDEL    "bindingDel"
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#define CGI_CMD_VLAN_SURVEILLANCEVLAN_PROP      "prop"
#define CGI_CMD_VLAN_SURVEILLANCEVLAN_PORTEDIT  "portEdit"
#define CGI_CMD_VLAN_SURVEILLANCEVLAN_OUI       "oui"
#define CGI_CMD_VLAN_SURVEILLANCEVLAN_OUIDEL    "ouiDel"
#endif

#ifdef CONFIG_SYS_VOICEVLAN
#define CGI_CMD_VLAN_VOICE_PROP      "prop"
#define CGI_CMD_VLAN_VOICE_PORTEDIT  "portEdit"
#define CGI_CMD_VLAN_VOICE_OUI       "oui"
#define CGI_CMD_VLAN_VOICE_OUIDEL    "ouiDel"
#endif

#ifdef CONFIG_SYS_POE

#define CGI_CMD_POE_PORT           "poe"
#define CGI_CMD_POE_PORTEDIT       "poeEdit"
#define CGI_CMD_POE_PORTTIMER      "poeTimer"
#define CGI_CMD_POE_PORTREBOOTTIMER  "poeRebootTimer"
#define CGI_CMD_POE_PORTREBOOTEDIT  "poeTimerEdit"
#define CGI_CMD_POE_PORTWATCHDOG   "poeWatchDog"
#endif

#ifdef CONFIG_SYS_L3_ROUTE_RIP

#define CGI_CMD_RIP_CFG            "ripconfig"
#define CGI_CMD_RIP_NETWORK_ADD    "ripNetworkAdd"
#define CGI_CMD_RIP_NETWORK_DEL    "ripNetworkDel"

#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF

#define CGI_CMD_OSPF_CFG            "ospfconfig"
#define CGI_CMD_OSPF_AREA_NETWORK_ADD    "ospfAreaNetworkAdd"
#define CGI_CMD_OSPF_AREA_NETWORK_DEL    "ospfAreaNetworkDel"
#endif

#ifdef CONFIG_SYS_L3_VRRP

#define CGI_CMD_VRRP_V4INTF           "ipv4Vrrp"
#define CGI_CMD_VRRP_V4INTFADD        "ipv4VrrpAdd"
#define CGI_CMD_VRRP_V4INTFEDIT       "ipv4VrrpEdit"
#define CGI_CMD_VRRP_V4INTFDEL        "ipv4VrrpDel"
#endif

