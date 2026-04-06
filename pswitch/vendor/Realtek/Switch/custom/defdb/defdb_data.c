
#include <defdb/defdb.h>

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <authmgr/proto_authmgr.h>
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#include <dhcp/proto_dhcp.h>
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#include <isg/proto_isg.h>
#endif
#include <lldp/proto_lldp.h>
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <igmp/proto_igmp.h>
#endif
#include <rsd/rsd_rate.h>
#include <rsd/rsd_qos.h>
#ifdef CONFIG_SYS_PORT_SECURITY
#include <rsd/rsd_psecure.h>
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
#include <udld/proto_udld.h>
#endif
#ifdef CONFIG_SYS_PROTO_LACP
#include <lacp/proto_lacp.h>
#endif
#ifdef CONFIG_SYS_UI_SNMP
#include <libsal/sal_snmp.h>
#endif
#ifdef CONFIG_SYS_PROTO_STP
#include <stp/proto_stp.h>
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#include <surveillancevlan/proto_surveillancevlan.h>
#endif
#ifdef CONFIG_SYS_SYSLOG
#include <common/sys_log.h>
#endif
#include <rsd/rsd_trunk.h>
#ifdef CONFIG_SYS_VOICEVLAN
#include <voice_vlan/proto_voice_vlan.h>
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
#include <erps/proto_sys_erps.h>
#endif

#define DB_VAL(idx, val)                     \
do {                                         \
    if (dbAct == INIT_ACT_CALSIZE)           \
    {                                        \
        dbSize += sizeof(uint32);            \
    }                                        \
    else                                     \
    {                                        \
        if (val < 0)                         \
            defdb_data_add(DEF_TYPE_INT, idx, val, 0, NULL);\
        else                                 \
            defdb_data_add(DEF_TYPE_UINT, idx, 0, val, NULL);\
    }                                        \
} while(0)

#define DB_STR(idx, val)                     \
do {                                         \
    if (dbAct == INIT_ACT_CALSIZE)           \
    {                                        \
        dbSize += strlen(val) + 1;           \
    }                                        \
    else                                     \
    {                                        \
        defdb_data_add(DEF_TYPE_STR, idx, 0, 0, val);\
    }                                        \
} while(0)

extern void defdb_data_add(def_type_t type, def_t idx, int32 i, uint32 u, char *s);

uint32 defdb_data_init(uint32 dbAct)
{
    uint32 dbSize = 0;

#ifdef CONFIG_SYS_LIB_TACPLUS
    
    DB_VAL(DEF_TACPLUS_HOST_NUM,                 8);
    DB_VAL(DEF_TACPLUS_TIMEOUT,                  5);
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
    DB_VAL(DEF_RADIUS_HOST_NUM,                  8);
    DB_VAL(DEF_RADIUS_RETRY,                     3);
    DB_VAL(DEF_RADIUS_TIMEOUT,                   3);
#endif

#ifdef CONFIG_SYS_MGMT_ACL
    
    DB_STR(DEF_ACL_MGMT_PROFILE,                 "");
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    
    DB_VAL(DEF_AUTHMGR_SESSION_NUM,              256);
  #ifdef CONFIG_SYS_PROTO_DOT1X
    DB_VAL(DEF_AUTHMGR_DOT1X_EBL,                DISABLED);
  #endif
  #ifdef CONFIG_SYS_PROTO_MACAUTH
    DB_VAL(DEF_AUTHMGR_MAC_EBL,                  DISABLED);
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    DB_VAL(DEF_AUTHMGR_WEB_EBL,                  DISABLED);
  #endif
    DB_VAL(DEF_AUTHMGR_GUEST_VLAN,               0);
  #ifdef CONFIG_SYS_PROTO_MACAUTH
    DB_VAL(DEF_AUTHMGR_RADIUS_USER_DELIMIT,      AUTHMGR_DELIMITER_NONE);
    DB_VAL(DEF_AUTHMGR_RADIUS_USER_GAP,          2);
    DB_VAL(DEF_AUTHMGR_RADIUS_USER_CASE,         AUTHMGR_MACCASE_UPPER);
  #endif
    DB_VAL(DEF_AUTHMGR_PORT_CTRL,                AUTHMGR_PORTCTRL_DISABLE);
  #ifdef CONFIG_SYS_PROTO_DOT1X
    DB_VAL(DEF_AUTHMGR_PORT_DOT1X_EBL,           DISABLED);
  #endif
  #ifdef CONFIG_SYS_PROTO_MACAUTH
    DB_VAL(DEF_AUTHMGR_PORT_MAC_EBL,             DISABLED);
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    DB_VAL(DEF_AUTHMGR_PORT_WEB_EBL,             DISABLED);
  #endif
    DB_VAL(DEF_AUTHMGR_PORT_ORDER_FIRST,         AUTHMGR_TYPE_DOT1X);
    DB_VAL(DEF_AUTHMGR_PORT_METHOD_ORDER_FIRST,  AUTHMGR_METHOD_RADIUS);
    DB_VAL(DEF_AUTHMGR_PORT_MODE,                AUTHMGR_MODE_MULTI_AUTH);
    DB_VAL(DEF_AUTHMGR_PORT_REAUTH_EBL,          DISABLED);
    DB_VAL(DEF_AUTHMGR_PORT_GUEST_VLAN_EBL,      DISABLED);
    DB_VAL(DEF_AUTHMGR_PORT_MAX_HOSTS,           256);
    DB_VAL(DEF_AUTHMGR_PORT_VLAN_ASSIGN_MODE,    AUTHMGR_VLAN_ASSIGN_STATIC);
    DB_VAL(DEF_AUTHMGR_PORT_REAUTH_PERIOD,       3600);
    DB_VAL(DEF_AUTHMGR_PORT_INACT_PERIOD,        60);
    DB_VAL(DEF_AUTHMGR_PORT_QUIET_PERIOD,        60);
  #ifdef CONFIG_SYS_PROTO_DOT1X
    DB_VAL(DEF_AUTHMGR_PORT_MAX_REQ,             2);
    DB_VAL(DEF_AUTHMGR_PORT_EAP_TX_PERIOD,       30);
    DB_VAL(DEF_AUTHMGR_PORT_SUPP_TIMEOUT,        30);
    DB_VAL(DEF_AUTHMGR_PORT_SRV_TIMEOUT,         30);
  #endif
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    DB_VAL(DEF_AUTHMGR_PORT_WEBAUTH_LOGIN_ATTEMPT,3);
  #endif
#endif

    DB_VAL(DEF_CLI_MAX_LINES_NUM,                24);
    DB_VAL(DEF_CLI_LOGIN_IDLE_TIMEOUT,           90);
    DB_STR(DEF_CLI_RECOVERY_PASSWORD,            "ewindnet.com");
    DB_STR(DEF_CLI_DEBUG_PASSWORD,               "ewindnet");
    DB_VAL(DEF_CLI_REMOTE_DEBUG,                 ENABLED);
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    DB_VAL(DEF_CLI_SILENTTIME_CONSOLE,           0);
    DB_VAL(DEF_CLI_SILENTTIME_TELNET,            0);
    DB_VAL(DEF_CLI_SILENTTIME_SSH,               0);
#endif
    DB_VAL(DEF_CLI_HISTORY_MAX_ENTRY,            128);

#ifdef CONFIG_SYS_PROTO_DAI
    
    DB_VAL(DEF_DAI_EBL,                          DISABLED);
    DB_VAL(DEF_DAI_PORT_TRUST,                   DISABLED);
    DB_VAL(DEF_DAI_SMAC_CHK,                     DISABLED);
    DB_VAL(DEF_DAI_DMAC_CHK,                     DISABLED);
    DB_VAL(DEF_DAI_IPMAC_CHK,                    DISABLED);
    DB_VAL(DEF_DAI_IPZEROALLOW_CHK,              DISABLED);
    DB_VAL(DEF_DAI_RATELIMIT_RATE,               BOND_RATE_UNLIMITED);
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    
    DB_VAL(DEF_DHCP_SNOOPING_EBL,                DISABLED);
    DB_VAL(DEF_DHCP_SNOOPING_PORT_TRUST,         DISABLED);
    DB_VAL(DEF_DHCP_SNOOPING_PORT_OPTION82_ENABLE, DISABLED);
    DB_VAL(DEF_DHCP_SNOOPING_PORT_OPTION82_ACTION, DHCP_OPTION82_DROP);
    DB_VAL(DEF_DHCP_SNOOPING_RATE_LIMIT,         BOND_RATE_UNLIMITED);
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
    
    DB_VAL(DEF_DOS_TYPE_EBL,                     ENABLED);
    DB_VAL(DEF_DOS_PORT_ATTACKPREVENT_EBL,       DISABLED);
#endif

#ifdef CONFIG_SYS_PROTO_ISG
    
    DB_VAL(DEF_IPSG_ENTRY_NUM,                   256);
    DB_VAL(DEF_IPSG_PORT_EBL,                    DISABLED);
    DB_VAL(DEF_IPSG_PORT_MAX_ENTRY,              IPSG_ENTRY_UNLIMITED);
    DB_VAL(DEF_IPSG_DB_TIMEOUT,                  300);
    DB_VAL(DEF_IPSG_DB_WRITE_DELAY,              300);
#endif

    DB_VAL(DEF_L2_STATIC_MAC_NUM,                256);
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    DB_VAL(DEF_L2_FILTER_MAC_NUM,                256);
#endif
    DB_VAL(DEF_L2_MAC_AGING_MIN,                 10);
    DB_VAL(DEF_L2_MAC_AGING_MAX,                 630);
    DB_VAL(DEF_L2_AGING_TIME,                    300);

    DB_VAL(DEF_LAG_NUM,                          8);
    DB_VAL(DEF_LAG_HASH_ALGO,                    LAG_HASH_ALGO_MAC);
#ifdef CONFIG_SYS_PROTO_LACP
    DB_VAL(DEF_LACP_EBL,                         ENABLED);
    DB_VAL(DEF_LACP_PORT_TIMEOUT,                LACP_TIMEOUT_LONG);
#endif

    DB_VAL(DEF_LLDP_EBL,                         ENABLED);
    DB_VAL(DEF_LLDP_PORT_STATUS,                 LLDP_PORT_TXRX);
    DB_VAL(DEF_LLDP_DISABLE_ACTION,              LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS);
    DB_VAL(DEF_LLDP_PORT_PVID_TLV_EBL,           ENABLED);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    DB_VAL(DEF_LLDP_PORT_MED_EBL,                ENABLED);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    DB_VAL(DEF_MCAST_GROUP_NUM,                  CONFIG_SYS_MCAST_GROUP_NUM);
    DB_VAL(DEF_MCAST_PORT_LIMIT,                 CONFIG_SYS_MCAST_GROUP_NUM);
    DB_VAL(DEF_IGMP_MVR_GROUP_NUM,               128);
    DB_VAL(DEF_IGMP_EBL,                         DISABLED);
    DB_VAL(DEF_IGMP_SUPPRE_EBL,                  ENABLED);
    DB_VAL(DEF_IGMP_VLAN_EBL,                    DISABLED);
    DB_VAL(DEF_IGMP_IMMEDIATE_LEAVE,             DISABLED);
    DB_VAL(DEF_IGMP_UNKNOWN_MCAST_ACTION,        IGMP_UNKNOWN_MCAST_ACTION_FLOOD);
    DB_VAL(DEF_IGMP_OPERATION_VER,               IGMP_VERSION_V2);
    DB_VAL(DEF_IGMP_MAX_GROUP_ACT,               IGMP_ACT_DENY);
  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    DB_VAL(DEF_MLD_EBL,                          DISABLED);
    DB_VAL(DEF_MLD_SUPPRE_EBL,                   ENABLED);
    DB_VAL(DEF_MLD_VLAN_EBL,                     DISABLED);
    DB_VAL(DEF_MLD_IMMEDIATE_LEAVE,              DISABLED);
    DB_VAL(DEF_MLD_UNKNOWN_MCAST_ACTION,         IGMP_UNKNOWN_MCAST_ACTION_FLOOD);
    DB_VAL(DEF_MLD_OPERATION_VER,                MLD_VERSION_V1);
    DB_VAL(DEF_MLD_MAX_GROUP_ACT,                IGMP_ACT_DENY);
  #endif
  #ifdef CONFIG_SYS_PROTO_MVR
    DB_VAL(DEF_IGMP_MVR_IMMEDIATE_LEAVE,         DISABLED);
  #endif
#endif

    DB_VAL(DEF_MIRROR_SESSION_NUM,               4);

    DB_VAL(DEF_PORT_FLOW_CTRL_AUTO_ABILITY,      DISABLED);
    DB_VAL(DEF_PORT_FLOW_CTRL_STATUS,            DISABLED);
    DB_VAL(DEF_PORT_BACK_PRESSURE,               ENABLED);
    DB_STR(DEF_PORT_DESCRIPTION,                 "");
    DB_VAL(DEF_PORT_FIBER_NEGO_EBL,              ENABLED);
#ifdef CONFIG_SYS_EEE
    DB_VAL(DEF_PORT_EEE_EBL,                     DISABLED);
#endif
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    DB_VAL(DEF_PORT_ERRDISABLE_RECOVERY,         DISABLED);
    DB_VAL(DEF_PORT_ERRDISABLE_RECOVERY_INTERVAL,300);
#endif
#ifdef CONFIG_SYS_PORT_ISOLATION
    DB_VAL(DEF_PORT_PROTECTED,                   DISABLED);
#endif
#ifdef CONFIG_SYS_JUMBO_FRAME
    DB_VAL(DEF_PORT_MAX_FRAME_SIZE,              1522);
#endif
    DB_STR(DEF_PORT_PREFIX_BRIEF_FA,             "fa");
    DB_STR(DEF_PORT_PREFIX_BRIEF_GE,             "gi");
    DB_STR(DEF_PORT_PREFIX_BRIEF_MGE,            "mgi");
    DB_STR(DEF_PORT_PREFIX_BRIEF_TE,             "te");
    DB_STR(DEF_PORT_PREFIX_BRIEF_LAG,            "lag");
    DB_STR(DEF_PORT_PREFIX_WEB_FA,               "FE");
    DB_STR(DEF_PORT_PREFIX_WEB_GE,               "GE");
    DB_STR(DEF_PORT_PREFIX_WEB_MGE,              "MGE");
    DB_STR(DEF_PORT_PREFIX_WEB_TE,               "TE");
    DB_STR(DEF_PORT_PREFIX_WEB_LAG,              "LAG");
    DB_STR(DEF_PORT_PREFIX_FA,                   "FastEthernet");
    DB_STR(DEF_PORT_PREFIX_GE,                   "GigabitEthernet");
    DB_STR(DEF_PORT_PREFIX_MGE,                  "MultiGigabitEthernet");
    DB_STR(DEF_PORT_PREFIX_TE,                   "TenGigabitEthernet");
    DB_STR(DEF_PORT_PREFIX_LAG,                  "LAG");

#ifdef CONFIG_SYS_PORT_SECURITY
    
    DB_VAL(DEF_PSECURE_LIMIT_NUM,                256);
    DB_VAL(DEF_PSECURE_EBL,                      DISABLED);
    DB_VAL(DEF_PSECURE_RATELIMIT,                100);
    DB_VAL(DEF_PSECURE_PORT_EBL,                 DISABLED);
    DB_VAL(DEF_PSECURE_PORT_LIMITNUM,            1);
    DB_VAL(DEF_PSECURE_PORT_VIOLATION,           SYS_PSECURE_VIOLATION_PROTECT);
    DB_VAL(DEF_PSECURE_PORT_STICKY,              DISABLED);
#endif

    DB_VAL(DEF_QOS_MODE,                         SYS_QOS_MODE_DISABLE);
    DB_VAL(DEF_QOS_STRICT_PRI_QUEUE_NUM,         8);
    DB_VAL(DEF_QOS_TRUST_TYPE,                   SYS_QOS_TRUST_COS);
    DB_VAL(DEF_QOS_PORT_TRUST_STATE,             ENABLED);
    DB_VAL(DEF_QOS_PORT_COS,                     0);
    DB_VAL(DEF_QOS_PORT_REMARK_COS,              DISABLED);
    DB_VAL(DEF_QOS_PORT_REMARK_DSCP,             DISABLED);
    DB_VAL(DEF_QOS_PORT_REMARK_IPP,              DISABLED);

    DB_VAL(DEF_RATE_STORM_MODE,                  BASED_ON_BYTE);
    DB_VAL(DEF_RATE_STORM_PREABMLE,              DISABLED);
    DB_VAL(DEF_RATE_STORM_PORT_EBL,              DISABLED);
    DB_VAL(DEF_RATE_STORM_BCAST_EBL,             DISABLED);
    DB_VAL(DEF_RATE_STORM_UMCAST_EBL,            DISABLED);
    DB_VAL(DEF_RATE_STORM_UUCAST_EBL,            DISABLED);
    DB_VAL(DEF_RATE_STORM_CTRL,                  10000);
    DB_VAL(DEF_RATE_STORM_ACTION,                STORM_ACTION_NONE);
    DB_VAL(DEF_RATE_FWD_IGR_PREABMLE,            DISABLED);
    DB_VAL(DEF_RATE_PORT_IGR_EBL,                DISABLED);
    DB_VAL(DEF_RATE_PORT_EGR_EBL,                DISABLED);

#ifdef CONFIG_SYS_UI_SNMP
    
    DB_VAL(DEF_SNMP_HOST_NUM,                    8);
    DB_VAL(DEF_SNMP_EBL,                         DISABLED);
  #ifdef CONFIG_SYS_MIB_TRAP
    DB_VAL(DEF_SNMP_TRAP_EBL,                    ENABLED);
  #endif
  #ifdef CONFIG_SYS_SNMPV3
    DB_STR(DEF_SNMP_ENTERPRISE,                  "006a92");
    DB_STR(DEF_SNMP_COMMUNITY,                   "public");
    DB_VAL(DEF_SNMP_COMMUNITY_ACCESS,            SNMP_COMMUNITY_ACCESS_READ_ONLY);
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_STP
    
    #ifdef CONFIG_SYS_PROTO_STP_DEFINE_OPEN
    DB_VAL(DEF_STP_EBL,                          ENABLED);
    DB_VAL(DEF_STP_PORT_EBL,                     ENABLED);
    #else
    DB_VAL(DEF_STP_EBL,                          DISABLED);
    DB_VAL(DEF_STP_PORT_EBL,                     DISABLED);
    #endif
    DB_VAL(DEF_STP_BPDU_FORWARD_EBL,             ENABLED);
    DB_VAL(DEF_STP_FORCE_VERSION,                STP_VERSION_RSTP);
    DB_VAL(DEF_STP_PORT_BPDU_FORWARD_EBL,        ENABLED);
    DB_VAL(DEF_STP_PORT_EDGE,                    STP_PORT_CONF_AUTO);
    DB_VAL(DEF_STP_PORT_P2P,                     STP_PORT_CONF_AUTO);
  #ifdef CONFIG_SYS_STP_BPDU_FILTER
    DB_VAL(DEF_STP_PORT_BPDU_FILTER,             STP_PORT_CONF_FALSE);
  #endif
  #ifdef CONFIG_SYS_STP_BPDU_GUARD
    DB_VAL(DEF_STP_PORT_BPDU_GUARD,              STP_PORT_CONF_FALSE);
  #endif
#endif

#ifdef CONFIG_SYS_SYSLOG
    
    DB_VAL(DEF_LOG_HOST_NUM,                     8);                 
    DB_VAL(DEF_LOG_EBL,                          ENABLED);
    DB_VAL(DEF_LOG_BUFFERED_EBL,                 ENABLED);
    DB_VAL(DEF_LOG_CONSOLE_EBL,                  ENABLED);
    DB_VAL(DEF_LOG_FILE_EBL,                     DISABLED);
    DB_VAL(DEF_LOG_AGGREGATION_EBL,              DISABLED);
    DB_VAL(DEF_LOG_BUFFERED_SEVERITY,            CONFIG_SYS_LOG_SEVERITY_LEVEL);    
    DB_VAL(DEF_LOG_BUFFERED_TOTAL,               3000);    
    DB_VAL(DEF_LOG_CONSOLE_SEVERITY,             CONFIG_SYS_LOG_SEVERITY_LEVEL);    
    DB_VAL(DEF_LOG_FILE_SEVERITY,                CONFIG_SYS_LOG_SEVERITY_LEVEL);    
    DB_VAL(DEF_LOG_FILE_TOTAL,                   3000);    
    DB_VAL(DEF_LOG_HOST_SEVERITY,                CONFIG_SYS_LOG_SEVERITY_LEVEL);    
#endif

    DB_STR(DEF_SYS_LOGIN_LOGO,                   SYS_OEM_LOGIN_LOG_NAME);
    DB_STR(DEF_SYS_HOME_LOGO,                    SYS_OEM_HOME_LOG_NAME);
    DB_STR(DEF_SYS_SUPPORT_INFO,                 SYS_OEM_SUPPORT_INFO);
    DB_STR(DEF_SYS_CONTACT_TELE,                 SYS_OEM_CONTACT_TELE);
    DB_STR(DEF_SYS_VERSION,                      PRODUCT_VERSION);
    DB_STR(DEF_SYS_PLATFORM,                     PLATFORM_NAME);
    DB_STR(DEF_SYS_COPYRIGHT,                    COPYRIGHT);
    DB_STR(DEF_SYS_MANUFACTURER,                 SYS_OEM_MANUFACTURER);
    DB_STR(DEF_SYS_VENDOR_URL,                   VENDOR_URL);
    DB_STR(DEF_SYS_NAME,                         SYS_OEM_NAME);
    DB_STR(DEF_SYS_LOCATION,                     SYS_OEM_LOCATION);
    DB_STR(DEF_SYS_CONTACT,                      SYS_OEM_CONTACT);
    DB_STR(DEF_SYS_SN_VAR,                       "sn");
    DB_STR(DEF_SYS_HWVER_VAR,                    "hwver");
    DB_STR(DEF_SYS_IMG_EXTNAME,                  "");
    DB_STR(DEF_SYS_CFG_EXTNAME,                  ".cfg");
    DB_STR(DEF_SYS_CFG_HEAD_START,               "SYSTEM CONFIG FILE ::= BEGIN");
    DB_STR(DEF_SYS_CFG_HEAD_END,                 "");
    DB_STR(DEF_SYS_FW_NAME,                      "system-firmware.bin");
    DB_STR(DEF_SYS_ADMIN_USERNAME,               SYS_OEM_USERNAME);
    DB_STR(DEF_SYS_ADMIN_PASSWORD,               SYS_OEM_PASSWORD);
    DB_STR(DEF_SYS_ENABLE_PASSWORD,              "");
    DB_VAL(DEF_SYS_TELNET_EBL,                   DISABLED);
#ifdef CONFIG_USER_SSH_SSHD
    DB_VAL(DEF_SYS_SSH_EBL,                      DISABLED);
#endif
    DB_VAL(DEF_SYS_HTTP_EBL,                     ENABLED);
    DB_VAL(DEF_SYS_HTTPS_EBL,                    DISABLED);
    DB_VAL(DEF_SYS_SESSION_TIMEOUT_HTTP,         10);
    DB_VAL(DEF_SYS_SESSION_TIMEOUT_HTTPS,        10);
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    DB_VAL(DEF_SYS_SESSION_TIMEOUT_CONSOLE,      10);
    DB_VAL(DEF_SYS_SESSION_TIMEOUT_TELNET,       10);
    DB_VAL(DEF_SYS_SESSION_TIMEOUT_SSH,          10);
#endif
	DB_VAL(DEF_SYS_USER_ACCESS_METH,		     CONFIG_SYS_USER_ACCESS_METH_VALUE);   

    DB_STR(DEF_L3_IPV4_ADDR,                     "192.168.0.1");
    DB_STR(DEF_L3_IPV4_NETMASK,                  "255.255.255.0");
    DB_VAL(DEF_L3_IPV4_PREFIXLEN,                24);
    DB_STR(DEF_L3_IPV4_GW,                       "192.168.0.254");
    DB_VAL(DEF_L3_IPV4_DHCPC_EBL,                ENABLED);
    DB_VAL(DEF_L3_IPV6_FORWARDING_EBL,           DISABLED);
    DB_STR(DEF_L3_IPV6_ADDR,                     "");
    DB_VAL(DEF_L3_IPV6_PREFIX_LEN,               0);
    DB_STR(DEF_L3_IPV6_GW,                       "::");
    DB_VAL(DEF_L3_IPV6_AUTOCONF_EBL,             ENABLED);
    DB_VAL(DEF_L3_IPV6_DADATTEMPT,               1);
    DB_VAL(DEF_L3_IPV6_DHCPC_EBL,                DISABLED);

    DB_VAL(DEF_SYS_DNS_EBL,                      ENABLED);
    DB_STR(DEF_SYS_IPV4_DNS1,                    "");
    DB_STR(DEF_SYS_IPV4_DNS2,                    "");
    DB_STR(DEF_SYS_IPV6_DNS1,                    "");
    DB_STR(DEF_SYS_IPV6_DNS2,                    "");

#ifdef CONFIG_SYS_APP_MSNTP
    DB_VAL(DEF_SYS_SNTP_EBL,                     DISABLED);
#endif
    DB_VAL(DEF_SYS_TIME_ZONE,                    SYS_OEM_TIME_ZONE);
    DB_STR(DEF_SYS_TIME_ZONE_ACRONYM,            "");

#ifdef CONFIG_SYS_PROTO_UDLD
    
    DB_VAL(DEF_UDLD_PORT_MODE,                   UDLD_MODE_DISABLE);
    DB_VAL(DEF_UDLD_INTERVAL_TIME,               15);
#endif

    DB_VAL(DEF_VLAN_ADMIN_ENTRY_NUM,             CONFIG_SYS_VLAN_MAX_NUM);
#ifdef CONFIG_SYS_MACVLAN
    DB_VAL(DEF_VLAN_MAC_GROUP_NUM,               256);
#endif
    DB_STR(DEF_VLAN_NAME_PREFIX,                 "VLAN");
#ifdef CONFIG_SYS_VLAN_PORT_MODE_ACCESS
    DB_VAL(DEF_VLAN_PORT_MODE,                   SYS_VLAN_PORT_MODE_ACCESS);
#endif
#ifdef CONFIG_SYS_VLAN_PORT_MODE_HYBRID
	DB_VAL(DEF_VLAN_PORT_MODE,					 SYS_VLAN_PORT_MODE_GENERAL);
#endif
#ifdef CONFIG_SYS_VLAN_PORT_MODE_TRUNK
	DB_VAL(DEF_VLAN_PORT_MODE,					 SYS_VLAN_PORT_MODE_TRUNK);
#endif
    DB_VAL(DEF_VLAN_PORT_ROLE_UPLINK_EN,         DISABLED);
    DB_VAL(DEF_VLAN_PORT_MODE_CHK,               ENABLED);
    DB_VAL(DEF_VLAN_IFILTER_EN,                  ENABLED);
    DB_VAL(DEF_VLAN_ACCEPT_FRAME_TYPE,           ACCEPT_FRAME_TYPE_ALL);
    DB_VAL(DEF_VLAN_PVID,                        1);
#ifdef CONFIG_SYS_MANAGEMENT_VLAN
    DB_VAL(DEF_VLAN_MGMVLAN_VID,                 1);
#endif
    DB_STR(DEF_DEFAULT_VLAN_NAME,                "default");
#ifdef CONFIG_SYS_VLAN_QINQ
    DB_VAL(DEF_VLAN_DOT1QTUNNEL_TPID_0,          0x8100);
    DB_VAL(DEF_VLAN_DOT1QTUNNEL_TPID_1,          0x88A8);
    DB_VAL(DEF_VLAN_DOT1QTUNNEL_TPID_2,          0x9100);
    DB_VAL(DEF_VLAN_DOT1QTUNNEL_TPID_3,          0x9200);
#endif

#ifdef CONFIG_SYS_VOICEVLAN
    DB_VAL(DEF_VLAN_VOICE_ENABLE,                VOICE_VLAN_DISABLED);
    DB_VAL(DEF_VLAN_VOICE_PORT_ENABLE,           DISABLED);
    DB_VAL(DEF_VLAN_VOICE_PORT_MODE,             VOICE_VLAN_MODE_AUTO);
    DB_VAL(DEF_VLAN_VOICE_PORT_COS_MODE,         VOICE_VLAN_COS_SRC);
    DB_VAL(DEF_VLAN_VOICE_REMARK_ENABLE,         DISABLED);
    DB_VAL(DEF_VLAN_VOICE_VID,                   DFLT_VLAN_RESERVED_VLAN);
    DB_VAL(DEF_VLAN_VOICE_PRI,                   6);
    DB_VAL(DEF_VLAN_VOICE_AGING,                 1440);
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    DB_VAL(DEF_VLAN_SURVEILLANCE_ENABLE,         DISABLED);
    DB_VAL(DEF_VLAN_SURVEILLANCE_REMARK_ENABLE,  DISABLED);
    DB_VAL(DEF_VLAN_SURVEILLANCE_VID,            DFLT_VLAN_RESERVED_VLAN);
    DB_VAL(DEF_VLAN_SURVEILLANCE_PRI,            6);
    DB_VAL(DEF_VLAN_SURVEILLANCE_AGING,          1440);
    DB_VAL(DEF_VLAN_SURVEILLANCE_PORT_ENABLE,    DISABLED);
    DB_VAL(DEF_VLAN_SURVEILLANCE_PORT_MODE,      SURVEILLANCE_VLAN_PORT_MODE_AUTO);
    DB_VAL(DEF_VLAN_SURVEILLANCE_PORT_COS_MODE,  SURVEILLANCE_VLAN_PORT_COS_SRC);
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
    DB_VAL(DEF_GVRP_EBL,                         DISABLED);
    DB_VAL(DEF_GVRP_PORT_EBL,                    DISABLED);
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    DB_VAL(DEF_GMRP_EBL,                         DISABLED);
    DB_VAL(DEF_GMRP_PORT_EBL,                    DISABLED);
#endif

#ifdef CONFIG_SYS_PROTO_ERPS
	DB_VAL(DEF_ERPS_EBL,                         DISABLED);
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	DB_VAL(DEF_LOOPBACK_EBL,                     DISABLED);
#endif
#ifdef CONFIG_SYS_POE
	
	DB_VAL(DEF_POE_PORT_ENABLE,					 ENABLED);
	
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
		
	DB_VAL(DEF_DHCP_SNOOPING_EBL,				 DISABLED);
#endif

    return dbSize;
}

