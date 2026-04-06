#include <common/sys_def.h>
#include <common/sys_log_desc.h>
const log_msg_t log_msg_aaa[LOG_AAA_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_AAA_CONNECT] =
    {
         LOG_SEV_NOTICE,
         "CONNECT",
         "New %s connection for user %s, source %s ACCEPTED"
    },
    [LOG_AAA_DISCONNECT] =
    {
         LOG_SEV_NOTICE,
         "DISCONNECT",
         "%s connection for user %s, source %s TERMINATED"
    },
    [LOG_AAA_REJECT] =
    {
         LOG_SEV_WARNING,
         "REJECT",
         "New %s connection, source %s REJECTED"
    },
    [LOG_AAA_USER_REJECT] =
    {
         LOG_SEV_WARNING,
         "USER_REJECT",
         "New %s connection for user %s, source %s REJECTED"
    },
    [LOG_AAA_ENABLE_SUCCESS] =
    {
         LOG_SEV_INFO,
         "ENABLE_SUCCESS",
         "User %s from %s enter privileged mode level %d ACCEPTED"
    },
    [LOG_AAA_ENABLE_FAIL] =
    {
         LOG_SEV_NOTICE,
         "ENABLE_FAIL",
         "User %s from %s enter privileged mode level %d REJECTED"
    },
    [LOG_AAA_USER_ADD] =
    {
         LOG_SEV_INFO,
         "USER_ADD",
         "Add user %s with privilege %d and password type %s"
    },
    [LOG_AAA_USER_EDIT] =
    {
         LOG_SEV_DEBUG,
         "USER_EDIT",
         "Edit user %s with privilege %d and password type %s"
    },
    [LOG_AAA_USER_DEL] =
    {
         LOG_SEV_INFO,
         "USER_DEL",
         "Delete user %s"
    },
  #ifdef CONFIG_SYS_AAA
    [LOG_AAA_METHOD_NEW] =
    {
         LOG_SEV_INFO,
         "METHOD_NEW",
         "New method %s is used"
    },
    [LOG_AAA_METHOD_FAIL] =
    {
         LOG_SEV_INFO,
         "METHOD_FAIL",
         "Method %s failed"
    },
    [LOG_AAA_AUTH_SUCCESS] =
    {
         LOG_SEV_DEBUG,
         "AUTH_SUCCESS",
         "Authentication with method %s for %s succeeded"
    },
    [LOG_AAA_AUTH_FAIL] =
    {
         LOG_SEV_DEBUG,
         "AUTH_FAIL",
         "Authentication failed for %s"
    },
    #ifdef CONFIG_SYS_LIB_RADIUS
    [LOG_AAA_RADIUS_EMPTY] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_EMPTY",
         "No RADIUS server is configured"
    },
    [LOG_AAA_RADIUS_CONN_FAIL] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_CONN_FAIL",
         "Connect to RADIUS server host %s port %d failed"
    },
    [LOG_AAA_RADIUS_CONN_SUCCESS] =
    {
         LOG_SEV_DEBUG,
         "RADIUS_CONN_SUCCESS",
         "Connect to RADIUS server host %s port %d succeeded"
    },
    #endif
    #ifdef CONFIG_SYS_LIB_TACPLUS
    [LOG_AAA_TACACS_EMPTY] =
    {
         LOG_SEV_NOTICE,
         "TACACS_EMPTY",
         "No TACACS+ server is configured"
    },
    [LOG_AAA_TACACS_CONN_FAIL] =
    {
         LOG_SEV_NOTICE,
         "TACACS_CONN_FAIL",
         "Connect to TACACS+ server host %s port %d failed"
    },
    [LOG_AAA_TACACS_CONN_SUCCESS] =
    {
         LOG_SEV_DEBUG,
         "TACACS_CONN_SUCCESS",
         "Connect to TACACS+ server host %s port %d succeeded"
    },
    [LOG_AAA_TACACS_USER_INVALID] =
    {
         LOG_SEV_WARNING,
         "TACACS_USER_INVALID",
         "User %s is invalid"
    },
    [LOG_AAA_TACACS_REPLYMSG] =
    {
         LOG_SEV_WARNING,
         "TACACS_REPLYMSG",
         "Received unsupported authentication REPLY msg status=%d (server msg: %s)"
    },
    #endif
  #endif
#endif
};

#ifdef CONFIG_SYS_PROTO_AUTHMGR
const log_msg_t log_msg_authmgr[LOG_AUTHMGR_MAX] =
{
#ifdef __KERNEL__
    [LOG_AUTHMGR_START] =
    {
         LOG_SEV_NOTICE,
         "START",
         "Starting %s for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_FAIL] =
    {
         LOG_SEV_NOTICE,
         "FAIL",
         "Authentication failed for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_SUCCESS] =
    {
         LOG_SEV_NOTICE,
         "SUCCESS",
         "Authentication successful for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_VLAN_ASSIGN] =
    {
         LOG_SEV_NOTICE,
         "VLAN_ASSIGN",
         "VLAN %d assigned to client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_SECURITY_VIOLATION] =
    {
         LOG_SEV_NOTICE,
         "SECURITY_VIOLATION",
         "Security violation on interface %s, new MAC address (%02X:%02X:%02X:%02X:%02X:%02X) is seen"
    },
    [LOG_AUTHMGR_MAC_MOVE] =
    {
         LOG_SEV_NOTICE,
         "MAC_MOVE",
         "Client (%02X:%02X:%02X:%02X:%02X:%02X) move from interface %s to interface %s"
    },
    [LOG_AUTHMGR_LOCK] =
    {
         LOG_SEV_NOTICE,
         "LOCK",
         "Client (%02X:%02X:%02X:%02X:%02X:%02X) is locked on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_GUESTVLAN] =
    {
         LOG_SEV_NOTICE,
         "GUESTVLAN",
         "Client (%02X:%02X:%02X:%02X:%02X:%02X) joins guest vlan %d on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_FAILOVER] =
    {
         LOG_SEV_DEBUG,
         "FAILOVER",
         "Failing over from %s for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_NOMORE_METHODS] =
    {
         LOG_SEV_DEBUG,
         "NOMORE_METHODS",
         "Exhausted all authentication methods for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_TIMEOUT_INACT] =
    {
         LOG_SEV_DEBUG,
         "TIMEOUT_INACT",
         "Inactive timeout for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_TIMEOUT_REAUTH] =
    {
         LOG_SEV_DEBUG,
         "TIMEOUT_REAUTH",
         "Reauthentication period id up for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_TIMEOUT_QUIET] =
    {
         LOG_SEV_DEBUG,
         "TIMEOUT_QUIET",
         "Quiet period is up for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s SessionID %08X%08X"
    },
    [LOG_AUTHMGR_DOT1X_FAIL] =
    {
         LOG_SEV_NOTICE,
         "DOT1X_FAIL",
         "Authentication failed for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
    [LOG_AUTHMGR_DOT1X_SUCCESS] =
    {
         LOG_SEV_INFO,
         "DOT1X_SUCCESS",
         "Authentication successful for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
    [LOG_AUTHMGR_MACAUTH_FAIL] =
    {
         LOG_SEV_NOTICE,
         "MACAUTH_FAIL",
         "Authentication failed for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
    [LOG_AUTHMGR_MACAUTH_SUCCESS] =
    {
         LOG_SEV_INFO,
         "MACAUTH_SUCCESS",
         "Authentication successful for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
    [LOG_AUTHMGR_WEBAUTH_FAIL] =
    {
         LOG_SEV_NOTICE,
         "WEBAUTH_FAIL",
         "Authentication failed for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
    [LOG_AUTHMGR_WEBAUTH_SUCCESS] =
    {
         LOG_SEV_INFO,
         "WEBAUTH_SUCCESS",
         "Authentication successful for client (%02X:%02X:%02X:%02X:%02X:%02X) on interface %s"
    },
#else
    [LOG_AUTHMGR_FORCE_UNAUTH] =
    {
         LOG_SEV_NOTICE,
         "FORCE_UNAUTH",
         "Interface %s force unauthorized"
    },
    [LOG_AUTHMGR_FORCE_AUTH] =
    {
         LOG_SEV_NOTICE,
         "FORCE_AUTH",
         "Interface %s force authorized"
    },
    [LOG_AUTHMGR_RADIUS_EMPTY] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_EMPTY",
         "No RADIUS server is configured"
    },
    [LOG_AUTHMGR_RADIUS_CONN_FAIL] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_CONN_FAIL",
         "Connect to RADIUS server host %s port %d failed"
    },
    [LOG_AUTHMGR_RADIUS_CONN_SUCCESS] =
    {
         LOG_SEV_DEBUG,
         "RADIUS_CONN_SUCCESS",
         "Connect to RADIUS server host %s port %d succeeded"
    },
    [LOG_AUTHMGR_RADIUS_VLAN_INVALID] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_VLAN_INVALID",
         "The VLAN %d is invalid and cannot be assigned"
    },
    [LOG_AUTHMGR_RADIUS_VLAN_NOTFOUND] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_VLAN_NOTFOUND",
         "Attempt to assign non-existent VLAN '%s'"
    },
    [LOG_AUTHMGR_RADIUS_TUNNELMEDIUMTYPE_INVALID] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_TUNNELTYPE_INVALID",
         "Got an invalid value %d for TUNNEL_MEDIUM_TYPE"
    },
    [LOG_AUTHMGR_RADIUS_TUNNELTYPE_INVALID] =
    {
         LOG_SEV_NOTICE,
         "RADIUS_INVALIDTUNNELTYPE",
         "Got an invalid value %d for TUNNEL_TYPE"
    },
#endif
};
#endif

const log_msg_t log_msg_aclmgr[LOG_ACLMGR_MAX] =
{
#ifdef __KERNEL__
  #ifdef CONFIG_SYS_MGMT_ACL
    [LOG_ACLMGR_MGMT_HTTP_DENY] =
    {
         LOG_SEV_WARNING,
         "MGMT_HTTP_DENY",
         "Management ACL HTTP deny. Packet drop: interface %s, SMAC %s and SIP %s"
    },
    [LOG_ACLMGR_MGMT_HTTPS_DENY] =
    {
         LOG_SEV_WARNING,
         "MGMT_HTTPS_DENY",
         "Management ACL HTTPS deny. Packet drop: interface %s, SMAC %s and SIP %s"
    },
    [LOG_ACLMGR_MGMT_SSH_DENY] =
    {
         LOG_SEV_WARNING,
         "MGMT_SSH_DENY",
         "Management ACL SSH deny. Packet drop: interface %s, SMAC %s and SIP %s"
    },
    [LOG_ACLMGR_MGMT_TELNET_DENY] =
    {
         LOG_SEV_WARNING,
         "MGMT_TELNET_DENY",
         "Management ACL TELNET deny. Packet drop: interface %s, SMAC %s and SIP %s"
    },
    [LOG_ACLMGR_MGMT_SNMP_DENY] =
    {
         LOG_SEV_WARNING,
         "MGMT_SNMP_DENY",
         "Management ACL SNMP deny. Packet drop: interface %s, SMAC %s and SIP %s"
    },
    [LOG_ACLMGR_MGMT_INSPECTION_START] =
    {
         LOG_SEV_INFO,
         "MGMT_INSPECTION_START",
         "Management ACL %s inspection is started"
    },
    [LOG_ACLMGR_MGMT_INSPECTION_STOP] =
    {
         LOG_SEV_INFO,
         "MGMT_INSPECTION_STOP",
         "Management ACL inspection is stopped"
    },
  #endif
  #ifdef CONFIG_SYS_USER_DEFINED_ACL
    [LOG_ACLMGR_USER_ERRDISABLE] =
    {
         LOG_SEV_WARNING,
         "USER_ERRDISABLE",
         "User-defined ACL errdisable on port %s"
    },
    [LOG_ACLMGR_USER_SHUTDOWN] =
    {
         LOG_SEV_WARNING,
         "USER_SHUTDOWN",
         "User-defined ACL disabled on port %s"
    },
  #endif
    [LOG_ACLMGR_RSV_BLOCK] =
    {
         LOG_SEV_WARNING,
         "RSV_BLOCK",
         "Reserve ACL type %s blocking trigger due to receive %u packet in 1 second"
    },
    [LOG_ACLMGR_RSV_RESTORE] =
    {
         LOG_SEV_WARNING,
         "RSV_RESTORE",
         "Reserve ACL type %s restore from blocking"
    },
    [LOG_ACLMGR_RSV_ENABLE] =
    {
         LOG_SEV_DEBUG,
         "RSV_ENABLE",
         "Reserve ACL type %u enabled, action is 0x%x"
    },
    [LOG_ACLMGR_RSV_DISABLE] =
    {
         LOG_SEV_DEBUG,
         "RSV_DISABLE",
         "Reserve ACL type %u disabled, action is 0x%x"
    },
    [LOG_ACLMGR_RSV_REMOVE] =
    {
         LOG_SEV_DEBUG,
         "RSV_REMOVE",
         "Reserve ACL type %u has been removed"
    },
    [LOG_ACLMGR_RSV_RULE_UPDATE] =
    {
         LOG_SEV_DEBUG,
         "RSV_RULE_UPDATE",
         "Reserve ACL type %u updated"
    },
    [LOG_ACLMGR_RSV_ACTION_UPDATE] =
    {
         LOG_SEV_DEBUG,
         "RSV_ACTION_UPDATE",
         "Reserve ACL type %u action updated to 0x%x"
    },
    [LOG_ACLMGR_TCAM_FULL] =
    {
         LOG_SEV_WARNING,
         "TCAM_FULL",
         "TCAM full. ACL entry can't be set to ASIC"
    },
    [LOG_ACLMGR_L4PORTRANGE_FULL] =
    {
         LOG_SEV_WARNING,
         "L4PORTRANGE_FULL",
         "ASIC resource L4 port range table full"
    },
    [LOG_ACLMGR_VLANRANGE_FULL] =
    {
         LOG_SEV_WARNING,
         "VLANRANGE_FULL",
         "ASIC resource VLAN range full"
    },
    [LOG_ACLMGR_COUNTER_FULL] =
    {
         LOG_SEV_WARNING,
         "COUNTER_FULL",
         "ASIC resource counter table full"
    },
    [LOG_ACLMGR_METER_FULL] =
    {
         LOG_SEV_WARNING,
         "METER_FULL",
         "ASIC resource meter table full"
    },
    [LOG_ACLMGR_RANGE_ENTRY_FULL] =
    {
         LOG_SEV_WARNING,
         "RANGE_ENTRY_FULL",
         "ASIC resource range check entry full"
    },
    [LOG_ACLMGR_PROTO_REGISTER] =
    {
         LOG_SEV_DEBUG,
         "PROTO_REGISTER",
         "Protocol %u registered"
    },
#endif
};

const log_msg_t log_msg_cfgmgr[LOG_CFGMGR_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_CFGMGR_CFG_INVALID] =
    {
         LOG_SEV_WARNING,
         "CFG_INVALID",
         "Configuration format is invalid"
    },
    [LOG_CFGMGR_COMMAND_FAIL] =
    {
         LOG_SEV_WARNING,
         "COMMAND_FAIL",
         "Execution of command failed: %s"
    },
    [LOG_CFGMGR_UPGRADE_SUCCESS] =
    {
         LOG_SEV_INFO,
         "UPGRADE_SUCCESS",
         "Upgrade %s succeeded"
    },
    [LOG_CFGMGR_UPGRADE_FAIL] =
    {
         LOG_SEV_ERR,
         "UPGRADE_FAIL",
         "Upgrade %s failed"
    },
    [LOG_CFGMGR_BACKUP_SUCCESS] =
    {
         LOG_SEV_INFO,
         "BACKUP_SUCCESS",
         "Backup %s succeeded"
    },
    [LOG_CFGMGR_BACKUP_FAIL] =
    {
         LOG_SEV_ERR,
         "BACKUP_FAIL",
         "Backup %s failed"
    },
    [LOG_CFGMGR_SAVE_SUCCESS] =
    {
         LOG_SEV_INFO,
         "SAVE_SUCCESS",
         "Save %s from %s succeeded"
    },
    [LOG_CFGMGR_SAVE_FAIL] =
    {
         LOG_SEV_ERR,
         "SAVE_FAIL",
         "Save %s to %s failed"
    },
#endif
};

#ifdef CONFIG_SYS_PROTO_DAI
const log_msg_t log_msg_dai[LOG_DAI_MAX] =
{
#ifdef __KERNEL__
    [LOG_DAI_RATE_EXCEED] =
    {
         LOG_SEV_WARNING,
         "RATE_EXCEED",
         "ARP packet rate exceeded. Packet Drop. %u packets received in 1 second on interface %s"
    },
    [LOG_DAI_ERRDISABLE] =
    {
         LOG_SEV_WARNING,
         "ERRDISABLE",
         "DAI errdisable on interface %s by rate limit violation"
    },
    [LOG_DAI_SMAC_DROP] =
    {
         LOG_SEV_NOTICE,
         "SMAC_DROP",
         "SMAC check failure. Packet drop: VLAN %u, interface %s and SMAC %s not equals sender MAC %s"
    },
    [LOG_DAI_DMAC_DROP] =
    {
         LOG_SEV_NOTICE,
         "DMAC_DROP",
         "DMAC check failure. Packet drop: VLAN %u, interface %s and DMAC %s not equals target MAC %s"
    },
    [LOG_DAI_SIP_DROP] =
    {
         LOG_SEV_NOTICE,
         "SIP_DROP",
         "SIP check failure. Packet drop: VLAN %u, interface %s and sender IP %s"
    },
    [LOG_DAI_DIP_DROP] =
    {
         LOG_SEV_NOTICE,
         "DIP_DROP",
         "DIP check failure. Packet drop: VLAN %u, interface %s and target IP %s"
    },
    [LOG_DAI_TARGET_IPMAC_BIND_DROP] =
    {
         LOG_SEV_NOTICE,
         "TARGET_IPMAC_BIND_DROP",
         "IP & MAC Mismatch. Packet drop: VLAN %u, interface %s and entry not found of DMAC %s and target IP %s"
    },
    [LOG_DAI_SENDER_IPMAC_BIND_DROP] =
    {
         LOG_SEV_NOTICE,
         "SENDER_IPMAC_BIND_DROP",
         "IP & MAC Mismatch. Packet drop: VLAN %u, interface %s and entry not found of SMAC %s and sender IP %s"
    },
    [LOG_DAI_GLOBAL_DROP] =
    {
         LOG_SEV_INFO,
         "GLOBAL_DROP",
         "DAI disabled. Packet drop: VLAN %u, interface %s, SMAC %s and sender IP %s"
    },
    [LOG_DAI_VLAN_DROP] =
    {
         LOG_SEV_INFO,
         "VLAN_DROP",
         "DAI disabled on VLAN. Packet drop: VLAN %u, interface %s, SMAC %s and sender IP %s"
    },
    [LOG_DAI_STP_DROP] =
    {
         LOG_SEV_INFO,
         "STP_DROP",
         "STP state not forward. Packet drop: VLAN %u, interface %s, SMAC %s and sender IP %s"
    },
#endif
};
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
const log_msg_t log_msg_dhcp_snooping[LOG_DHCP_SNOOPING_MAX] =
{
#ifdef __KERNEL__
    [LOG_DHCP_SNOOPING_ALLOCATE_FAIL] =
    {
         LOG_SEV_ERR,
         "ALLOCATE_FAIL",
         "Resource allocate failed. Packet drop: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_RATE_EXCEED] =
    {
         LOG_SEV_WARNING,
         "RATE_EXCEED",
         "DHCP packet rate exceeded. Packet drop. %u packets received in 1 second on interface %s"
    },
    [LOG_DHCP_SNOOPING_ERRDISABLE] =
    {
         LOG_SEV_WARNING,
         "ERRDISABLE",
         "DHCP snooping errdisable on interface %s by rate limit violation"
    },
    [LOG_DHCP_SNOOPING_TRANSACTION_FULL] =
    {
         LOG_SEV_WARNING,
         "TRANSACTION_FULL",
         "DHCP snooping cannot process new bindings because transaction pool is full. Packet forward"
    },
    [LOG_DHCP_SNOOPING_TRANSACTION_FAIL] =
    {
         LOG_SEV_WARNING,
         "TRANSACTION_FAIL",
         "DHCP packet ACK cannot match in transaction pool. Packet forward: VLAN %u, interface %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_RELEASE_INVALID] =
    {
         LOG_SEV_WARNING,
         "RELEASE_INVALID",
         "DHCP release cannot match in database. Packet drop VLAN %u, interface %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_UNTRUSTED_DROP] =
    {
         LOG_SEV_NOTICE,
         "UNTRUSTED_DROP",
         "Untrusted deny. Packet drop: VLAN %u, interface %s, chaddr %s and opcode %s"
    },
    [LOG_DHCP_SNOOPING_CHADDR_DROP] =
    {
         LOG_SEV_NOTICE,
         "CHADDR_DROP",
         "DHCP packet chaddr check failure. Packet drop: VLAN %u, interface %s and chaddr %s not equals SMAC %s"
    },
    [LOG_DHCP_SNOOPING_OPTION_INVALID] =
    {
         LOG_SEV_NOTICE,
         "OPTION_INVALID",
         "DHCP packet option invalid. Packet drop: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_OPTION82_DROP] =
    {
         LOG_SEV_NOTICE,
         "OPTION82_DROP",
         "DHCP packet option82 not allow. Packet drop: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_GLOBAL_DROP] =
    {
         LOG_SEV_INFO,
         "GLOBAL_DROP",
         "DHCP snooping disabled. Packet drop: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_VLAN_FORWARD] =
    {
         LOG_SEV_INFO,
         "VLAN_FORWARD",
         "DHCP snooping disabled on VLAN. Packet forward: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
    [LOG_DHCP_SNOOPING_STP_DROP] =
    {
         LOG_SEV_INFO,
         "STP_DROP",
         "STP state not forward. Packet drop: VLAN %u, interface %s, SMAC %s and chaddr %s"
    },
#endif
};
#endif

const log_msg_t log_msg_imagemgr[LOG_IMAGEMGR_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_IMAGEMGR_FLASH_IMG_BROKEN] =
    {
         LOG_SEV_CRIT,
         "FLASH_IMG_BROKEN",
         "Flash erased but writing failed. Image %d is broken"
    },
    [LOG_IMAGEMGR_MEMORY_LACK] =
    {
         LOG_SEV_ERR,
         "MEMORY_LACK",
         "Memory not enough to upgrade image"
    },
    [LOG_IMAGEMGR_IMG_INVALID] =
    {
         LOG_SEV_WARNING,
         "IMG_INVALID",
         "Image header is invalid"
    },
    [LOG_IMAGEMGR_IMG_CHKSUM] =
    {
         LOG_SEV_WARNING,
         "IMG_CHKSUM",
         "Image checksum error"
    },
    [LOG_IMAGEMGR_IMG_LOCKED] =
    {
         LOG_SEV_WARNING,
         "IMG_LOCKED",
         "Operation still in progress. Action is forbidden"
    },
    [LOG_IMAGEMGR_UPGRADE_SUCCESS] =
    {
         LOG_SEV_INFO,
         "UPGRADE_SUCCESS",
         "Upgrade image %d with name %s by %s/%s succeeded"
    },
    [LOG_IMAGEMGR_UPGRADE_FAIL] =
    {
         LOG_SEV_ERR,
         "UPGRADE_FAIL",
         "Upgrade image %d by %s/%s failed"
    },
    [LOG_IMAGEMGR_BACKUP_SUCCESS] =
    {
         LOG_SEV_INFO,
         "BACKUP_SUCCESS",
         "Backup image %d by %s/%s succeeded"
    },
    [LOG_IMAGEMGR_BACKUP_FAIL] =
    {
         LOG_SEV_ERR,
         "BACKUP_FAIL",
         "Backup image %d by %s/%s failed"
    },
    [LOG_IMAGEMGR_IMG_ACTIVE] =
    {
         LOG_SEV_INFO,
         "IMG_ACTIVE",
         "Image %d as active image on next boot"
    },
#endif
};

#ifdef CONFIG_SYS_PROTO_ISG
const log_msg_t log_msg_ipsg[LOG_IPSG_MAX] =
{
#ifdef __KERNEL__
    [LOG_IPSG_BINDING_COLLISION] =
    {
         LOG_SEV_WARNING,
         "BINDING_COLLISION",
         "Binding collision. Entry ignored: VLAN %u and MAC %s"
    },
    [LOG_IPSG_DB_FULL] =
    {
         LOG_SEV_WARNING,
         "DB_FULL",
         "Binding database full. Entry Ignored: VLAN %u and MAC %s"
    },
    [LOG_IPSG_ENTRY_FULL] =
    {
         LOG_SEV_WARNING,
         "ENTRY_FULL",
         "Maximum binding entry of port full. Entry ignored: VLAN %u and MAC %s"
    },
#else
    [LOG_IPSG_DB_DOWNLOAD_FAIL] =
    {
         LOG_SEV_NOTICE,
         "DB_DOWNLOAD_FAIL",
         "Binding database download from %s failed"
    },
    [LOG_IPSG_DB_DOWNLOAD_SUCCESS] =
    {
         LOG_SEV_NOTICE,
         "DB_DOWNLOAD_SUCCESS",
         "Binding database download from %s succeeded"
    },
    [LOG_IPSG_DB_UPLOAD_FAIL] =
    {
         LOG_SEV_NOTICE,
         "DB_UPLOAD_FAIL",
         "Binding database upload to %s failed"
    },
    [LOG_IPSG_DB_UPLOAD_SUCCESS] =
    {
         LOG_SEV_NOTICE,
         "DB_UPLOAD_SUCCESS",
         "Binding database upload to %s succeeded"
    },
    [LOG_IPSG_DB_READ_FAIL] =
    {
         LOG_SEV_NOTICE,
         "DB_READ_FAIL",
         "Binding database read from file failed"
    },
    [LOG_IPSG_DB_WRITE_FAIL] =
    {
         LOG_SEV_NOTICE,
         "DB_WRITE_FAIL",
         "Binding database write to file failed"
    },
    [LOG_IPSG_DB_RESTORE_SUCCESS] =
    {
         LOG_SEV_INFO,
         "DB_RESTORE_SUCCESS",
         "Binding database restore succeeded"
    },
    [LOG_IPSG_DB_INTF_NOTEXIST] =
    {
         LOG_SEV_INFO,
         "DB_INTF_NOTEXIST",
         "Interface of entry from read file not valid. Entry ignored"
    },
#endif
};
#endif

const log_msg_t log_msg_l2[LOG_L2_MAX] =
{
#ifdef __KERNEL__
    [LOG_L2_TABLE_FULL] =
    {
         LOG_SEV_WARNING,
         "TABLE_FULL",
         "Dynamic MAC address table full (%d entries)"
    },
    [LOG_L2_STATIC_TIMEOUT] =
    {
         LOG_SEV_NOTICE,
         "STATIC_TIMEOUT",
         "Static entry (%02x:%02x:%02x:%02x:%02x:%02x) timeout delete"
    },
#else
    
#endif
};

#ifdef CONFIG_SYS_L3_ROUTE
const log_msg_t log_msg_l3[LOG_L3_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_L3_IP_SUBNET_OVERLAP] =
    {
         LOG_SEV_WARNING,
         "IPSBNTOVRLP",
         "A specified subnet %s/%d is overlapped with subnet %s/%d"
    },
    [LOG_L3_IP_SUBNET_DUPLICATE] =
    {
         LOG_SEV_DEBUG,
         "IPSBNTDUP",
         "A specified subnet %s/%d is duplicated with subnet %s/%d"
    },
    [LOG_L3_IPV6_INTFIP_DUPLICATE] =
    {
         LOG_SEV_WARNING,
         "DAD",
         "Duplicated IPv6 address %s detected on interface %s"
    },
#endif
};
#endif

#ifdef CONFIG_SYS_PROTO_LLDP
const log_msg_t log_msg_lldp[LOG_LLDP_MAX] =
{
#ifdef __KERNEL__
    [LOG_LLDP_ETHERTYPE_INVALID] =
    {
         LOG_SEV_NOTICE,
         "ETHERTYPE_INVALID",
         "Received invalid EtherType on port %s"
    },
    [LOG_LLDP_LEN_INVALID] =
    {
         LOG_SEV_NOTICE,
         "LEN_INVALID",
         "Received invalid length on port %s"
    },
    [LOG_LLDP_NEIGHBOR_LIMIT] =
    {
         LOG_SEV_NOTICE,
         "NEIGHBOR_LIMIT",
         "Maximum count (%d) reached. Neighbor is no longer allowed on port %s"
    },

    [LOG_LLDP_FRAME_DROP] =
    {
         LOG_SEV_NOTICE,
         "FRAME_DROP",
         "Drop invalid packet on port %s"
    },
    [LOG_LLDP_TLV_UNRECOGNIZE] =
    {
         LOG_SEV_NOTICE,
         "TLV_UNRECOGNIZE",
         "Drop unrecognized TLV on port %s"
    },
    [LOG_LLDP_DISABLE] =
    {
         LOG_SEV_INFO,
         "DISABLE",
         "LLDP is disabled. %s LLDP PDU"
    },
    [LOG_LLDP_NEIGHBOR_DISCOVER] =
    {
         LOG_SEV_INFO,
         "NEIGHBOR_DISCOVER",
         "New neighbor on port %s: Chassis ID %s, Port ID %s"
    },
    [LOG_LLDP_NEIGHBOR_DEL] =
    {
         LOG_SEV_INFO,
         "NEIGHBOR_DEL",
         "Neighbor deleted on port %s: Chassis ID %s, Port ID %s"
    },
    [LOG_LLDP_NEIGHBOR_TIMEOUT] =
    {
         LOG_SEV_INFO,
         "NEIGHBOR_TIMEOUT",
         "Neighbor timeout on Port %s: Chassis ID %s, Port ID %s"
    },
    [LOG_LLDP_FRAME_ERROR] =
    {
         LOG_SEV_DEBUG,
         "FRAME_ERROR",
         "Received error frame on port %s"
    },
    [LOG_LLDP_TLV_DROP] =
    {
         LOG_SEV_DEBUG,
         "TLV_DROP",
         "Drop invalid TLV on port %s"
    },
#else
    [LOG_LLDP_TXOVERLOAD] =
    {
         LOG_SEV_NOTICE,
         "TX_OVERLOAD",
         "LLDP TLVsTxOverloading: %s Overloading state on port %s\n"
    },
#endif
};
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
const log_msg_t log_msg_mcast[LOG_MCAST_MAX] =
{
#ifdef __KERNEL__
    [LOG_MCAST_DB_UPDATE_FAIL] =
    {
         LOG_SEV_ERR,
         "DB_UPDATE_FAIL",
         "Database update failed: VLAN %u group %s"
    },
    [LOG_MCAST_ASIC_DMAC_UPDATE_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DMAC_UPDATE_FAIL",
         "ASIC update failed: VLAN %u DMAC "MAC_PRINT""
    },
     [LOG_MCAST_ASIC_DIP_UPDATE_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DIP_UPDATE_FAIL",
         "ASIC update failed: VLAN %u DIP %s"
    },
    [LOG_MCAST_DB_ADD_FAIL] =
    {
         LOG_SEV_ERR,
         "DB_ADD_FAIL",
         "Database add failed: VLAN %u group %s"
    },
    [LOG_MCAST_ASIC_DMAC_ADD_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DMAC_ADD_FAIL",
         "ASIC add failed: VLAN %u DMAC "MAC_PRINT""
    },
     [LOG_MCAST_ASIC_DIP_ADD_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DIP_ADD_FAIL",
         "ASIC add failed: VLAN %u DIP %s"
    },
    [LOG_MCAST_DB_DEL_FAIL] =
    {
         LOG_SEV_ERR,
         "DB_DEL_FAIL",
         "Database delete failed: VLAN %u group %s"
    },
    [LOG_MCAST_ASIC_DMAC_DEL_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DMAC_DEL_FAIL",
         "ASIC delete failed: VLAN %u DMAC "MAC_PRINT""
    },
    [LOG_MCAST_ASIC_DIP_DEL_FAIL] =
    {
         LOG_SEV_ERR,
         "ASIC_DIP_DEL_FAIL",
         "ASIC delete failed: VLAN %u DIP %s"
    },
    [LOG_MCAST_DB_FULL] =
    {
         LOG_SEV_WARNING,
         "DB_FULL",
         "Database is full"
    },
    [LOG_MCAST_DMAC_COLLISION] =
    {
         LOG_SEV_NOTICE,
         "DMAC_COLLISION",
         "ASIC collision: VLAN %u DMAC "MAC_PRINT""
    },
     [LOG_MCAST_DIP_COLLISION] =
    {
         LOG_SEV_NOTICE,
         "DIP_COLLISION",
         "ASIC collision: VLAN %u DIP %s"
    },
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    [LOG_MCAST_LIMIT_EXCEED] =
    {
         LOG_SEV_NOTICE,
         "LIMIT_EXCEED",
         "%s maximum group (%u) reached on interface %s"
    },
    #endif
    [LOG_MCAST_PKT_CHKSUM_DROP] =
    {
         LOG_SEV_NOTICE,
         "PKT_CHKSUM_DROP",
         "Received %s checksum error: VLAN %u, interface %s"
    },
    [LOG_MCAST_VLAN_DISABLE] =
    {
         LOG_SEV_NOTICE,
         "VLAN_DISABLE",
        "%s disabled on VLAN %u"
    },

    [LOG_MCAST_RTPORT_PIM_ADD] =
    {
         LOG_SEV_INFO,
         "RTPORT_PIM_ADD",
         "Interface %s added to %s router port on VLAN %u by PIM/DVMRP/MOSPF"
    },
    [LOG_MCAST_RTPORT_ADD] =
    {
         LOG_SEV_INFO,
         "RTPORT_ADD",
         "Interface %s added to %s router port on VLAN %u"
    },
    [LOG_MCAST_VLAN_DISABLE_FLOOD] =
    {
         LOG_SEV_INFO,
         "VLAN_DISABLE_FLOOD",
         "%s not in operation on VLAN. Packet flood: VLAN %u, interface %s"
    },
    [LOG_MCAST_RTPORT_TIMEOUT] =
    {
         LOG_SEV_INFO,
         "RTPORT_TIMEOUT",
        "Interface %s %s router port timeout on VLAN %u"
    },
    [LOG_MCAST_GROUP_RANGE_INVALID] =
    {
         LOG_SEV_DEBUG,
         "GROUP_RANGE_INVALID",
        "Received invalid group range %s"
    },
    [LOG_MCAST_STP_DROP] =
    {
         LOG_SEV_DEBUG,
         "STP_DROP",
        "STP state not forward. Packet drop: VLAN %u, interface %s (%s)"
    },
    [LOG_MCAST_VLAN_DROP] =
    {
         LOG_SEV_DEBUG,
         "VLAN_DROP",
         "Not VLAN member. Packet drop: VLAN %u, interface %s (%s)"
    },
    [LOG_MCAST_VER_FLOOD] =
    {
         LOG_SEV_DEBUG,
         "VER_FLOOD",
         "%s version %d incompatible, Packet flood: VLAN %u"
    },
    [LOG_MCAST_GROUP_NOTEXSIT_DROP] =
    {
         LOG_SEV_DEBUG,
         "GROUP_NOTEXSIT_DROP",
         "%s leave group not exist"
    },
    #ifdef CONFIG_SYS_PROTO_MVR
    [LOG_MCAST_MVR_VLAN_FLOOD] =
    {
         LOG_SEV_DEBUG,
         "MVR_VLAN_FLOOD",
         "Not MVR group. Packet flood: VLAN %u"
    },
    [LOG_MCAST_MVR_TYPE_DROP] =
    {
         LOG_SEV_DEBUG,
         "MVR_TYPE_DROP",
         "No MVR type. Packet drop: VLAN %u, interface %s"
    },
    [LOG_MCAST_MVR_RECEIVER_DROP] =
    {
         LOG_SEV_DEBUG,
         "MVR_RECEIVER_DROP",
         "MVR type not receiver. Packet drop: VLAN %u, interface %s"
    },
    [LOG_MCAST_MVR_NOTSRC_DROP] =
    {
         LOG_SEV_DEBUG,
         "MVR_NOTSRC_DROP",
         "MVR type not source. Packet drop: VLAN %u, interface %s"
    },
    #endif
#endif
};
#endif

const log_msg_t log_msg_logging[LOG_LOGGING_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_LOGGING_BUF_START] =
    {
         LOG_SEV_INFO,
         "BUF_START",
         "Buffer logging is started with minimum severity %s"
    },
    [LOG_LOGGING_BUF_STOP] =
    {
         LOG_SEV_INFO,
         "BUF_STOP",
         "Buffer logging is stopped and the messages stored on the internal buffer would be clear after reboot"
    },
    [LOG_LOGGING_CONSOLE_START] =
    {
         LOG_SEV_INFO,
         "CONSOLE_START",
         "Console logging is started with minimum severity %s"
    },
    [LOG_LOGGING_CONSOLE_STOP] =
    {
         LOG_SEV_INFO,
         "CONSOLE_STOP",
         "Console logging is stopped"
    },
    [LOG_LOGGING_FILE_START] =
    {
         LOG_SEV_INFO,
         "FILE_START",
         "File logging is started with minimum severity %s"
    },
    [LOG_LOGGING_FILE_STOP] =
    {
         LOG_SEV_INFO,
         "FILE_STOP",
         "File logging is stopped"
    },
    [LOG_LOGGING_START] =
    {
         LOG_SEV_INFO,
         "START",
         "Logging is started"
    },
    [LOG_LOGGING_STOP] =
    {
         LOG_SEV_INFO,
         "STOP",
         "Logging is stopped"
    },
    [LOG_LOGGING_AGG_AGETIME] =
    {
         LOG_SEV_DEBUG,
         "AGG_AGETIME",
         "Logging aggregation aging time is set to %u seconds"
    },
    [LOG_LOGGING_AGG_START] =
    {
         LOG_SEV_DEBUG,
         "AGG_START",
         "Logging aggregation is started"
    },
    [LOG_LOGGING_AGG_STOP] =
    {
         LOG_SEV_DEBUG,
         "AGG_STOP",
         "Logging aggregation is stopped"
    },
    [LOG_LOGGING_HOST_ADD] =
    {
         LOG_SEV_DEBUG,
         "HOST_ADD",
         "Logging host %s (%s) with port %u is added with minimum severity %s"
    },
    [LOG_LOGGING_HOST_DEL] =
    {
         LOG_SEV_DEBUG,
         "HOST_DEL",
         "Logging host %s is deleted"
    },
#endif
};

#ifdef CONFIG_SYS_MIRROR
const log_msg_t log_msg_mirror[LOG_MIRROR_MAX] =
{
#ifdef __KERNEL__
    [LOG_MIRROR_MONITOR_ON] =
    {
         LOG_SEV_INFO,
         "ON",
         "Monitor session %d is on"
    },
    [LOG_MIRROR_MONITOR_OFF] =
    {
         LOG_SEV_INFO,
         "OFF",
         "Monitor session %d is off"
    },

#endif
};
#endif

const log_msg_t log_msg_platform[LOG_PLATFORM_MAX] =
{
#ifdef __KERNEL__
    [LOG_PLATFORM_OPTICAL_INSERT] =
    {
         LOG_SEV_INFO,
         "OPTICAL_INSERT",
         "Optical module inserted port %s, %s %s %s"
    },
    [LOG_PLATFORM_OPTICAL_REMOVE] =
    {
         LOG_SEV_INFO,
         "OPTICAL_REMOVE",
         "Optical module removed port %s"
    },
#else
    [LOG_PLATFORM_OPTICAL_AUTOSPEED] =
    {
         LOG_SEV_DEBUG,
         "OPTICAL_AUTOSPEED",
         "Optical module bitrate %u bps, port %s speed is configured as %s"
    },
#endif
};

const log_msg_t log_msg_port[LOG_PORT_MAX] =
{
#ifdef __KERNEL__
    [LOG_PORT_LINK_DOWN] =
    {
         LOG_SEV_NOTICE,
         "LINK_DOWN",
          "Interface %s link down"
    },
    [LOG_PORT_LINK_UP] =
    {
         LOG_SEV_NOTICE,
         "LINK_UP",
          "Interface %s link up"
    },
    [LOG_PORT_MEDIA_CHANGE] =
    {
         LOG_SEV_NOTICE,
         "MEDIA_CHANGE",
          "Media changed from %s to %s on %s"
    },
    [LOG_PORT_SPEED_DUPLEX] =
    {
         LOG_SEV_INFO,
         "SPEED_DUPLEX",
          "Interface %s link speed %s duplex %s"
    },
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    [LOG_PORT_ERR_DISABLE] =
    {
         LOG_SEV_WARNING,
         "ERR_DISABLE",
         "%s error detected on %s, putting %s in err-disable state"
    },
    [LOG_PORT_ERR_RECOVERY] =
    {
         LOG_SEV_WARNING,
         "ERR_RECOVER",
         "Attempting to recover from %s err-disable state on %s"
    },
#endif
#endif
};

#ifdef CONFIG_SYS_PORT_SECURITY
const log_msg_t log_msg_psecure[LOG_PSECURE_MAX] =
{
#ifdef __KERNEL__
    [LOG_PSECURE_VIOLATION] =
    {
         LOG_SEV_CRIT,
         "PSECURE_VIOLATION",
         "Security violation occurred, caused by MAC address (%02x:%02x:%02x:%02x:%02x:%02x) on %s"
    },
#else
    [LOG_PSECURE_COUNT_LIMIT] =
    {
         LOG_SEV_DEBUG,
         "COUNT_LIMIT",
         "Maximum secure address count (%d) changed on port %s"
    },
#endif
};
#endif

const log_msg_t log_msg_qosmgr[LOG_QOSMGR_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_QOSMGR_STRICT_PRI_QUEUE] =
    {
         LOG_SEV_DEBUG,
         "STRICT_PRI_QUEUE",
         "Strict priority queue number is set to %u"
    },
    [LOG_QOSMGR_QUEUE_WEIGHT] =
    {
         LOG_SEV_DEBUG,
         "QUEUE_WEIGHT",
         "Queue weight is set to %d %d %d %d %d %d %d %d"
    },
#endif
};

#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
const log_msg_t log_msg_rmon[LOG_RMON_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_RMON_ALARM_RISING] =
    {
         LOG_SEV_INFO,
         "ALARM_RISING",
         "Rising trap generated. Value %u of %s.%u over rising-threshold %u"
    },
    [LOG_RMON_ALARM_FALLING] =
    {
         LOG_SEV_INFO,
         "ALARM_FALLING",
         "Falling trap generated. Value %u of %s.%u under falling-threshold %u"
    },
#endif
};
#endif

#ifdef CONFIG_SYS_UI_SNMP
const log_msg_t log_msg_snmp[LOG_SNMP_MAX] =
{
#ifdef __KERNEL__
   
#else
    [LOG_SNMP_AUTH_FAIL] =
    {
         LOG_SEV_WARNING,
         "AUTH_FAIL",
         "Authentication failure for \"%s\""
    },
    [LOG_SNMP_TRAP_CONN_FAIL] =
    {
         LOG_SEV_NOTICE,
         "TRAP_CONN_FAIL",
         "SNMP trap server failed: host %s error:%d"
    },
    [LOG_SNMP_TRAP_DROP] =
    {
         LOG_SEV_DEBUG,
         "TRAP_DROP",
         "Attempt to generate SNMP trap failed"
    },
    #ifdef CONFIG_SYS_SNMPV3
    [LOG_SNMP_USER_REJECT] =
    {
         LOG_SEV_WARNING,
         "USER_REJECT",
         "User connection rejected:%s"
    },
    #endif
#endif
};
#endif

const log_msg_t log_msg_storm_control[LOG_STORM_CONTROL_MAX] =
{
#ifdef __KERNEL__
    [LOG_STORM_CONTROL_DROP] =
    {
         LOG_SEV_ERR,
         "DROP",
         "A %s storm detected on %s. Drop excess traffic"
    },
    [LOG_STORM_CONTROL_SHUTDOWN] =
    {
         LOG_SEV_ERR,
         "SHUTDOWN",
         "A %s storm detected on %s. The interface has been disabled"
    },
#endif
};

#ifdef CONFIG_SYS_PROTO_STP
const log_msg_t log_msg_stp[LOG_STP_MAX] =
{
#ifdef __KERNEL__
  #ifdef CONFIG_SYS_STP_BPDU_GUARD
    [LOG_STP_BPDUGUARD] =
    {
         LOG_SEV_CRIT,
         "BPDUGUARD",
         "Received BPDU on port %s with BPDU Guard enabled. Disabling port"
    },
  #endif
    [LOG_STP_SELFLOOP] =
    {
         LOG_SEV_ERR,
         "SELFLOOP",
         "Port %s disabled. Received BPDU SMAC (%02X:%02X:%02X:%02X:%02X:%02X) same as that of interface"
    },
  #ifdef CONFIG_SYS_STP_BPDU_FILTER
    [LOG_STP_BPDUFILTER] =
    {
         LOG_SEV_NOTICE,
         "BPDUFILTER",
         "Dropping BPDU on port %s with BPDU Filter enabled"
    },
  #endif
    [LOG_STP_EDGEPORT] =
    {
         LOG_SEV_INFO,
         "EDGEPORT",
         "BPDU is received on port %s which is configured as the edge port"
    },
    [LOG_STP_MSTI_PORT_STATE] =
    {
         LOG_SEV_INFO,
         "MSTI_PORT_STATE",
         "Port %s instance %u moving from %s to %s"
    },
    [LOG_STP_PORT_STATE] =
    {
         LOG_SEV_INFO,
         "PORT_STATE",
         "Port %s moving from %s to %s"
    },
#else
    [LOG_STP_DISABLE] =
    {
         LOG_SEV_INFO,
         "DISABLE",
         "STP is disabled. %s BPDU"
    },
    [LOG_STP_ENABLE] =
    {
         LOG_SEV_INFO,
         "ENABLE",
         "STP is enabled"
    },
    [LOG_STP_MIGRATION] =
    {
         LOG_SEV_INFO,
         "MIGRATION",
         "Port %s migration check"
    },
    [LOG_STP_PORT_DISABLE] =
    {
         LOG_SEV_INFO,
         "PORT_DISABLE",
         "Port %s STP is disabled. %s BPDU"
    },
    [LOG_STP_PORT_ENABLE] =
    {
         LOG_SEV_INFO,
         "PORT_ENABLE",
         "Port %s STP is enabled"
    },
    [LOG_STP_RSTP] =
    {
         LOG_SEV_INFO,
         "RSTP",
         "Operating in RSTP-Compatible mode"
    },
    [LOG_STP_STP] =
    {
         LOG_SEV_INFO,
         "STP",
         "Operating in STP-Compatible mode"
    },
  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    [LOG_STP_MSTI_VLAN_ADD] =
    {
         LOG_SEV_INFO,
         "MSTI_VLAN_ADD",
         "VLAN %s added MST instance %u"
    },
    [LOG_STP_MSTI_VLAN_DEL] =
    {
         LOG_SEV_INFO,
         "MSTI_VLAN_DEL",
         "VLAN %s deleted MST instance %u"
    },
    [LOG_STP_MSTP] =
    {
         LOG_SEV_INFO,
         "MSTP",
         "Operating in MSTP mode"
    },
  #endif
#endif
};
#endif

const log_msg_t log_msg_time[LOG_TIME_MAX] =
{
#ifdef __KERNEL__
    
#else
  #ifdef CONFIG_SYS_APP_MSNTP
    [LOG_TIME_CONN_FAIL] =
    {
         LOG_SEV_ERR,
         "CONN_FAIL",
         "SNTP server %s is unreachable"
    },
    [LOG_TIME_PKT_ERR] =
    {
         LOG_SEV_NOTICE,
         "PKT_ERR",
         "SNTP packet format is illegal"
    },
    [LOG_TIME_START] =
    {
         LOG_SEV_INFO,
         "START",
         "SNTP client is started"
    },
    [LOG_TIME_STOP] =
    {
         LOG_SEV_INFO,
         "STOP",
         "SNTP client is stopped"
    },
    [LOG_TIME_TIME_SYNC] =
    {
         LOG_SEV_INFO,
         "TIME_SYNC",
         "SNTP sync: time %s server %s"
    },
    [LOG_TIME_CONN_SUCCESS] =
    {
         LOG_SEV_DEBUG,
         "CONN_SUCCESS",
         "SNTP server %s is reachable"
    },
  #endif
#endif
};

const log_msg_t log_msg_system[LOG_SYSTEM_MAX] =
{
#ifdef __KERNEL__
    
#else
    [LOG_SYSTEM_REBOOT] =
    {
         LOG_SEV_WARNING,
         "REBOOT",
         "System reboot"
    },
    [LOG_SYSTEM_RESTORE] =
    {
         LOG_SEV_NOTICE,
         "RESTORE",
         "System restore to default"
    },
    [LOG_SYSTEM_FACTORY_RESTORE] =
    {
         LOG_SEV_NOTICE,
         "FACTORY_RESTORE",
         "System restore to factory default"
    },
    [LOG_SYSTEM_INTF_RESTORE] =
    {
         LOG_SEV_NOTICE,
         "INTF_RESTORE",
         "Restore default on interface %s"
    },
    [LOG_SYSTEM_WARMSTART] =
    {
         LOG_SEV_NOTICE,
         "WARMSTART",
         "Warm startup"
    },
    [LOG_SYSTEM_COLDSTART] =
    {
         LOG_SEV_NOTICE,
         "COLDSTART",
         "Cold startup"
    },
    [LOG_SYSTEM_NAME] =
    {
         LOG_SEV_DEBUG,
         "NAME",
         "System name is set to %s"
    },
    [LOG_SYSTEM_LOCATION] =
    {
         LOG_SEV_DEBUG,
         "LOCATION",
         "System location is set to %s"
    },
    [LOG_SYSTEM_CONTACT] =
    {
         LOG_SEV_DEBUG,
         "CONTACT",
         "System contact is set to %s"
    },
#ifdef CONFIG_SYS_APP_TELNETD
    [LOG_SYSTEM_TELNET_START] =
    {
         LOG_SEV_INFO,
         "TELNET_START",
         "TELNET service is started"
    },
    [LOG_SYSTEM_TELNET_STOP] =
    {
         LOG_SEV_INFO,
         "TELNET_STOP",
         "TELNET service is stopped"
    },
    [LOG_SYSTEM_TELNETD_START_FAIL] =
    {
         LOG_SEV_ERR,
         "TELNETD_START_FAIL",
         "Start TELNET daemon failed"
    },
#endif
#ifdef CONFIG_USER_SSH_SSHD
    [LOG_SYSTEM_SSH_START] =
    {
         LOG_SEV_INFO,
         "SSH_START",
         "SSH service is started"
    },
    [LOG_SYSTEM_SSH_STOP] =
    {
         LOG_SEV_INFO,
         "SSH_STOP",
         "SSH service is stopped"
    },
    [LOG_SYSTEM_SSHD_START_FAIL] =
    {
         LOG_SEV_ERR,
         "SSHD_START_FAIL",
         "Start SSH daemon failed"
    },
#endif
    [LOG_SYSTEM_HTTP_START] =
    {
         LOG_SEV_INFO,
         "HTTP_START",
         "HTTP service is started"
    },
    [LOG_SYSTEM_HTTP_STOP] =
    {
         LOG_SEV_INFO,
         "HTTP_STOP",
         "HTTP service is stopped"
    },
    [LOG_SYSTEM_HTTPS_START] =
    {
         LOG_SEV_INFO,
         "HTTPS_START",
         "HTTPS service is started"
    },
    [LOG_SYSTEM_HTTPS_STOP] =
    {
         LOG_SEV_INFO,
         "HTTPS_STOP",
         "HTTPS service is stopped"
    },
    [LOG_SYSTEM_HTTPD_START_FAIL] =
    {
         LOG_SEV_ERR,
         "HTTPD_START_FAIL",
         "Start HTTP daemon failed"
    },
    [LOG_SYSTEM_CLI_SESSION_FULL] =
    {
         LOG_SEV_NOTICE,
         "CLI_SESSION_FULL",
         "CLI session number %d exceed, connection ABORTED"
    },
    [LOG_SYSTEM_CLI_TIMEOUT] =
    {
         LOG_SEV_INFO,
         "CLI_TIMEOUT",
         "CLI session timeout: user %s, source %s"
    },
    [LOG_SYSTEM_WEB_TIMEOUT] =
    {
         LOG_SEV_INFO,
         "WEB_TIMEOUT",
         "GUI session timeout: user %s, source %s"
    },
    [LOG_SYSTEM_TFTP_ERROR] =
    {
         LOG_SEV_ERR,
         "TFTP_ERROR",
         "TFTP error: %s (%d)"
    },
    [LOG_SYSTEM_FLASH_OPERATE_ERROR] =
    {
         LOG_SEV_ERR,
         "FLASH_OPERATE_ERROR",
         "Read or write flash device error"
    },
    [LOG_SYSTEM_SYSINFO_CHKSUM_ERROR] =
    {
         LOG_SEV_ERR,
         "SYSINFO_CHKSUM_ERROR",
         "partition checksum error"
    },
    [LOG_SYSTEM_SYSINFO_CFG] =
    {
         LOG_SEV_DEBUG,
         "SYSINFO_CFG",
         "%s is set to %s"
    },
    [LOG_SYSTEM_BDINFO_CHKSUM_ERROR] =
    {
         LOG_SEV_ERR,
         "BDINFO_CHKSUM_ERROR",
         "partition checksum error"
    },
    [LOG_SYSTEM_BDINFO_CFG] =
    {
         LOG_SEV_DEBUG,
         "BDINFO_CFG",
         "%s is set to %s"
    },
    [LOG_SYSTEM_DHCPC_START] =
    {
         LOG_SEV_INFO,
         "DHCPC_START",
         "DHCP client is started"
    },
    [LOG_SYSTEM_DHCPC_STOP] =
    {
         LOG_SEV_INFO,
         "DHCPC_STOP",
         "DHCP client is stopped"
    },
    [LOG_SYSTEM_DHCP6C_START] =
    {
         LOG_SEV_INFO,
         "DHCP6C_START",
         "DHCPv6 client is started"
    },
    [LOG_SYSTEM_DHCP6C_STOP] =
    {
         LOG_SEV_INFO,
         "DHCP6C_STOP",
         "DHCPv6 client is stopped"
    },
    [LOG_SYSTEM_IP] =
    {
         LOG_SEV_INFO,
         "IP",
         "IP address is configured as %s with netmask %s"
    },
    [LOG_SYSTEM_GW] =
    {
         LOG_SEV_INFO,
         "GW",
         "Default gateway is configured as %s"
    },
    [LOG_SYSTEM_IPV6] =
    {
         LOG_SEV_INFO,
         "IPV6",
         "IPv6 address is configured as %s/%u"
    },
    [LOG_SYSTEM_IPV6_GW] =
    {
         LOG_SEV_INFO,
         "IPV6_GW",
         "IPv6 default gateway is configured as %s"
    },
    [LOG_SYSTEM_IPV6_AUTOCONFIG_START] =
    {
         LOG_SEV_INFO,
         "IPV6_AUTOCONFIG_START",
         "IPv6 auto-configuration is enabled"
    },
    [LOG_SYSTEM_IPV6_AUTOCONFIG_STOP] =
    {
         LOG_SEV_INFO,
         "IPV6_AUTOCONFIG_STOP",
         "IPv6 auto-configuration is disabled"
    },
    [LOG_SYSTEM_DNS_IPV4] =
    {
         LOG_SEV_DEBUG,
         "DNS_IPV4",
         "Static DNS server %u is configured as %s"
    },
    [LOG_SYSTEM_DNS_IPV6] =
    {
         LOG_SEV_DEBUG,
         "DNS_IPV6",
         "Static IPv6 DNS server %u is configured as %s"
    },
    [LOG_SYSTEM_DNS_DHCPC] =
    {
         LOG_SEV_DEBUG,
         "DNS_DHCPC",
         "Dynamic DNS server %u from DHCP is configured as %s"
    },
    [LOG_SYSTEM_DNS_DHCP6C] =
    {
         LOG_SEV_DEBUG,
         "DNS_DHCP6C",
         "Dynamic DNS server %u from DHCPv6 is configured as %s"
    },
  #ifdef CONFIG_SYS_UI_SNMP
    [LOG_SYSTEM_SNMP_START] =
    {
         LOG_SEV_INFO,
         "SNMP_START",
         "SNMP service is started"
    },
    [LOG_SYSTEM_SNMP_STOP] =
    {
         LOG_SEV_INFO,
         "SNMP_STOP",
         "SNMP service is stopped"
    },
  #endif
#endif
};

const log_msg_t log_msg_lag[LOG_LAG_MAX] =
{
#ifdef __KERNEL__
#ifdef CONFIG_SYS_PROTO_LACP
    [LOG_LAG_LACP_MBR_TIMEOUT] =
    {
         LOG_SEV_DEBUG,
         "LACP_MBR_TIMEOUT",
         "LACP member %s is timeout"
    },
    [LOG_LAG_LACP_MBR_EXCEED] =
    {
         LOG_SEV_DEBUG,
         "LACP_MBR_EXCEED",
         "Maximum aggregate limit (%d) reached"
    },
#endif
#else
#ifdef CONFIG_SYS_PROTO_LACP
    [LOG_LAG_LACP_ACTIVE] =
    {
         LOG_SEV_DEBUG,
         "ACTIVE",
         "LACP member %s with active mode enabled"
    },
    [LOG_LAG_LACP_PASSIVE] =
    {
         LOG_SEV_DEBUG,
         "PASSIVE",
         "LACP member %s with passive mode enabled"
    },
#endif
#endif
};

#ifdef CONFIG_SYS_PROTO_UDLD
const log_msg_t log_msg_udld[LOG_UDLD_MAX] =
{
#ifdef __KERNEL__
    [LOG_UDLD_ALLOCATE_FAIL] =
    {
         LOG_SEV_WARNING,
         "ALLOCATE_FAIL",
         "Can't allocate resource for new packet"
    },
    [LOG_UDLD_AGGRESSIVE_FAIL] =
    {
         LOG_SEV_WARNING,
         "AGGRESSIVE_FAIL",
         "UDLD errdisable on interface %s by aggressive mode failure detected"
    },
    [LOG_UDLD_LOOP_DETECT] =
    {
         LOG_SEV_WARNING,
         "LOOP_DETECT",
         "UDLD errdisable on interface %s by transmit/receive loop detected"
    },
    [LOG_UDLD_NEIGHBOR_MISMATCH_DETECT] =
    {
         LOG_SEV_WARNING,
         "NEIGHBOR_MISMATCH_DETECT",
         "UDLD errdisable on interface %s by neighbor mismatch detected"
    },
    [LOG_UDLD_UNIDIRECTIONAL_DETECT] =
    {
         LOG_SEV_WARNING,
         "UNIDIRECTIONAL_DETECT",
         "UDLD errdisable on interface %s by unidirectional link detected"
    },
    [LOG_UDLD_PKT_CHKSUM_DROP] =
    {
         LOG_SEV_NOTICE,
         "PKT_CHKSUM_DROP",
         "Header checksum error. Packet drop: SMAC %s and port %s"
    },
    [LOG_UDLD_PKT_INVALID_DROP] =
    {
         LOG_SEV_NOTICE,
         "PKT_INVALID_DROP",
         "Format error. Packet drop: SMAC %s and port %s"
    },
    [LOG_UDLD_PKT_PORT_DROP] =
    {
         LOG_SEV_INFO,
         "PKT_PORT_DROP",
         "UDLD disabled on port %s. Packet dropped: SMAC %s"
    },
    [LOG_UDLD_RESET] =
    {
         LOG_SEV_DEBUG,
         "RESET",
         "UDLD reset on port %s"
    },
#endif
};
#endif

const log_msg_t log_msg_vlan[LOG_VLAN_MAX] =
{
#ifdef __KERNEL__
    [LOG_VLAN_TYPE] =
    {
         LOG_SEV_INFO,
         "TYPE",
         "VLAN %u changed from %s to %s"
    },
  #ifdef CONFIG_SYS_PROTO_GVRP
    [LOG_VLAN_GVRP_CREATE] =
    {
         LOG_SEV_INFO,
         "GVRP_CREATE",
         "Dynamic VLAN %d created by GVRP"
    },
    [LOG_VLAN_GVRP_DESTROY] =
    {
         LOG_SEV_INFO,
         "GVRP_DESTROY",
         "Dynamic VLAN %d destroyed by GVRP"
    },
    [LOG_VLAN_GVRP_MBR_ADD] =
    {
         LOG_SEV_INFO,
         "GVRP_MBR_ADD",
         "Interface %s added to VLAN %d by GVRP"
    },
    [LOG_VLAN_GVRP_MBR_DEL] =
    {
         LOG_SEV_INFO,
         "GVRP_MBR_DEL",
         "Interface %s removed from VLAN %d by GVRP"
    },

    [LOG_VLAN_GVRP_PROTO_ID_INVALID] =
    {
         LOG_SEV_NOTICE,
         "GVRP_PROTO_ID_INVALID",
         "Protocol ID is not 0x0001"
    },
    [LOG_VLAN_GVRP_ATTR_TYPE_INVALID] =
    {
         LOG_SEV_NOTICE,
         "GVRP_ATTR_TYPE_INVALID",
         "Attribute type of message is invalid"
    },
    [LOG_VLAN_GVRP_ATTR_LEN_INVALID] =
    {
         LOG_SEV_NOTICE,
         "GVRP_ATTR_LEN_INVALID",
         "Attribute length of message is invalid"
    },
    [LOG_VLAN_GVRP_ATTR_VALUE_INVALID] =
    {
         LOG_SEV_NOTICE,
         "GVRP_ATTR_VALUE_INVALID",
         "Attribute value of message is invalid"
    },
    [LOG_VLAN_GVRP_ATTR_EVENT_INVALID] =
    {
         LOG_SEV_DEBUG,
         "GVRP_ATTR_EVENT_INVALID",
         "Attribute event of message is unknown"
    },
    [LOG_VLAN_GVRP_DB_FULL] =
    {
         LOG_SEV_NOTICE,
         "GVRP_DB_FULL",
         "GVRP database is full"
    },
    [LOG_VLAN_GVRP_DROP] =
    {
         LOG_SEV_DEBUG,
         "GVRP_DROP",
         "Drop packet due to member %s disabled"
    },
  #endif
  #ifdef CONFIG_SYS_MACVLAN
    [LOG_VLAN_MAC_RESOURCE_LACK] =
    {
         LOG_SEV_ERR,
         "MAC_RESOURCE_LACK",
         "Lack of system resource"
    },
    [LOG_VLAN_MAC_BIND_FAIL] =
    {
         LOG_SEV_ERR,
         "MAC_BIND_FAIL",
         "Binding port with MAC VLAN entry failed"
    },
    [LOG_VLAN_MAC_UNBIND_FAIL] =
    {
         LOG_SEV_ERR,
         "MAC_UNBIND_FAIL",
         "Unbinding port with MAC VLAN entry failed"
    },
    [LOG_VLAN_MAC_UPDATE_FAIL] =
    {
         LOG_SEV_ERR,
         "MAC_UPDATE_FAIL",
         "Update port and MAC VLAN binding failed"
    },
  #endif
  #ifdef CONFIG_SYS_PROTOCOL_VLAN
    [LOG_VLAN_PROTO_RESOURCE_LACK] =
    {
         LOG_SEV_ERR,
         "PROTO_RESOURCE_LACK",
         "Lack of system resource"
    },
    [LOG_VLAN_PROTO_BIND_FAIL] =
    {
         LOG_SEV_ERR,
         "PROTO_BIND_FAIL",
         "Binding port with protocol VLAN entry failed"
    },
    [LOG_VLAN_PROTO_UNBIND_FAIL] =
    {
         LOG_SEV_ERR,
         "PROTO_UNBIND_FAIL",
         "Unbinding port with protocol VLAN entry failed"
    },
    [LOG_VLAN_PROTO_UPDATE_FAIL] =
    {
         LOG_SEV_ERR,
         "PROTO_UPDATE_FAIL",
         "Update port and protocol VLAN binding failed"
    },
  #endif
  #ifdef CONFIG_SYS_VOICEVLAN
    [LOG_VLAN_VOICE_RESOURCE_LACK] =
    {
         LOG_SEV_ERR,
         "VOICE_RESOURCE_LACK",
         "Lack of system resource"
    },
    [LOG_VLAN_VOICE_CONFIG_FAIL] =
    {
         LOG_SEV_ERR,
         "VOICE_CONFIG_FAIL",
         "Configuration update failed"
    },
    [LOG_VLAN_VOICE_GLOBAL_DROP] =
    {
         LOG_SEV_INFO,
         "VOICE_GLOBAL_DROP",
         "Voice VLAN disabled. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_VOICE_INTF_DROP] =
    {
         LOG_SEV_INFO,
         "VOICE_INTF_DROP",
         "Voice VLAN disabled on interface. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_VOICE_OUI_DROP] =
    {
         LOG_SEV_INFO,
         "VOICE_OUI_DROP",
         "OUI MAC not exist. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_VOICE_MBR_ADD] =
    {
         LOG_SEV_INFO,
         "VOICE_INTF_MBR_ADD",
         "Interface %s added to voice VLAN %u, SMAC %s"
    },
    [LOG_VLAN_VOICE_MBR_DEL] =
    {
         LOG_SEV_INFO,
         "VOICE_MBR_DEL",
         "Interface %s removed from voice VLAN %u"
    },
    [LOG_VLAN_VOICE_MBR_LLDP_ADD] =
    {
         LOG_SEV_INFO,
         "VOICE_MBR_LLDP_ADD",
         "Interface %s added to voice VLAN %u by LLDP"
    },
    [LOG_VLAN_VOICE_MBR_LLDP_DEL] =
    {
         LOG_SEV_INFO,
         "VOICE_MBR_LLDP_DEL",
         "Interface %s removed from voice VLAN %u by LLDP"
    },
  #endif
  #ifdef CONFIG_SYS_SURVEILLANCEVLAN
    [LOG_VLAN_SURVEILLANCE_RESOURCE_LACK] =
    {
         LOG_SEV_ERR,
         "SURVEILLANCE_RESOURCE_LACK",
         "Lack of system resource"
    },
    [LOG_VLAN_SURVEILLANCE_CONFIG_FAIL] =
    {
         LOG_SEV_ERR,
         "SURVEILLANCE_CONFIG_FAIL",
         "Configuration update failed"
    },
    [LOG_VLAN_SURVEILLANCE_GLOBAL_DROP] =
    {
         LOG_SEV_INFO,
         "SURVEILLANCE_GLOBAL_DROP",
         "Surveillance VLAN disabled. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_SURVEILLANCE_INTF_DROP] =
    {
         LOG_SEV_INFO,
         "SURVEILLANCE_INTF_DROP",
         "Surveillance VLAN disabled on interface. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_SURVEILLANCE_OUI_DROP] =
    {
         LOG_SEV_INFO,
         "SURVEILLANCE_OUI_DROP",
         "OUI MAC not exists. Packet drop: interface %s and SMAC %s"
    },
    [LOG_VLAN_SURVEILLANCE_MBR_ADD ] =
    {
         LOG_SEV_INFO,
         "SURVEILLANCE_MBR_ADD",
         "Interface %s added to surveillance VLAN %u, SMAC %s"
    },
    [LOG_VLAN_SURVEILLANCE_MBR_DEL ] =
    {
         LOG_SEV_INFO,
         "SURVEILLANCE_MBR_DEL",
         "Interface %s removed from surveillance VLAN %u"
    },
  #endif
#else
    [LOG_VLAN_CREATE_FAIL] =
    {
         LOG_SEV_ERR,
         "CREATE_FAIL",
         "Create VLAN %u failed"
    },
    [LOG_VLAN_DESTROY_FAIL] =
    {
         LOG_SEV_ERR,
         "DESTROY_FAIL",
         "Destroy VLAN %u failed"
    },
    [LOG_VLAN_DEFAULT_VLAN] =
    {
         LOG_SEV_INFO,
         "DEFAULT_VLAN",
         "Default VLAN is %u"
    },
    [LOG_VLAN_CREATE] =
    {
         LOG_SEV_INFO,
         "CREATE",
         "Static VLAN %u (%s) is created"
    },
    [LOG_VLAN_DESTROY] =
    {
         LOG_SEV_INFO,
         "DESTROY",
         "Static VLAN %u is destroyed"
    },
#endif
};
const sys_log_desc_t log_cat_desc[LOG_CAT_ALL] =
{
    [LOG_CAT_AAA] =
    {
        .text   = "AAA",
        .msgnum = LOG_AAA_MAX,
        .msgtbl = log_msg_aaa,
    },
    [LOG_CAT_ACLMGR] =
    {
        .text   = "ACLMGR",
        .msgnum = LOG_ACLMGR_MAX,
        .msgtbl = log_msg_aclmgr,
    },
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    [LOG_CAT_AUTHMGR] =
    {
        .text   = "AUTHMGR",
        .msgnum = LOG_AUTHMGR_MAX,
        .msgtbl = log_msg_authmgr,
    },
#endif
#ifdef CONFIG_SYS_PROTO_DAI
    [LOG_CAT_DAI] =
    {
        .text   = "DAI",
        .msgnum = LOG_DAI_MAX,
        .msgtbl = log_msg_dai,
    },
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    [LOG_CAT_DHCP_SNOOPING] =
    {
        .text   = "DHCP_SNOOPING",
        .msgnum = LOG_DHCP_SNOOPING_MAX,
        .msgtbl = log_msg_dhcp_snooping,
    },
#endif
    [LOG_CAT_IMAGEMGR] =
    {
        .text   = "IMAGEMGR",
        .msgnum = LOG_IMAGEMGR_MAX,
        .msgtbl = log_msg_imagemgr,
    },
#ifdef CONFIG_SYS_PROTO_ISG
    [LOG_CAT_IPSG] =
    {
        .text   = "IPSG",
        .msgnum = LOG_IPSG_MAX,
        .msgtbl = log_msg_ipsg,
    },
#endif
    [LOG_CAT_L2] =
    {
        .text   = "L2",
        .msgnum = LOG_L2_MAX,
        .msgtbl = log_msg_l2,
    },
#ifdef CONFIG_SYS_L3_ROUTE
    [LOG_CAT_L3] =
    {
        .text   = "L3",
        .msgnum = LOG_L3_MAX,
        .msgtbl = log_msg_l3,
    },
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    [LOG_CAT_LLDP] =
    {
        .text   = "LLDP",
        .msgnum = LOG_LLDP_MAX,
        .msgtbl = log_msg_lldp,
    },
#endif
    [LOG_CAT_LOGGING] =
    {
        .text   = "LOGGING",
        .msgnum = LOG_LOGGING_MAX,
        .msgtbl = log_msg_logging,
    },
    [LOG_CAT_MCAST] =
    {
        .text   = "MCAST",
        .msgnum = LOG_MCAST_MAX,
        .msgtbl = log_msg_mcast,
    },
#ifdef CONFIG_SYS_MIRROR
    [LOG_CAT_MIRROR] =
    {
        .text   = "MIRROR",
        .msgnum = LOG_MIRROR_MAX,
        .msgtbl = log_msg_mirror,
    },
#endif
    [LOG_CAT_PLATFORM] =
    {
        .text   = "PLATFORM",
        .msgnum = LOG_PLATFORM_MAX,
        .msgtbl = log_msg_platform,
    },
    [LOG_CAT_PORT] =
    {
        .text   = "PORT",
        .msgnum = LOG_PORT_MAX,
        .msgtbl = log_msg_port,
    },
#ifdef CONFIG_SYS_PORT_SECURITY
    [LOG_CAT_PSECURE] =
    {
        .text   = "PSECURE",
        .msgnum = LOG_PSECURE_MAX,
        .msgtbl = log_msg_psecure,
    },
#endif
    [LOG_CAT_QOSMGR] =
    {
        .text   = "QOSMGR",
        .msgnum = LOG_QOSMGR_MAX,
        .msgtbl = log_msg_qosmgr,
    },
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    [LOG_CAT_RMON] =
    {
        .text   = "RMON",
        .msgnum = LOG_RMON_MAX,
        .msgtbl = log_msg_rmon,
    },
#endif
#ifdef CONFIG_SYS_UI_SNMP
    [LOG_CAT_SNMP] =
    {
        .text   = "SNMP",
        .msgnum = LOG_SNMP_MAX,
        .msgtbl = log_msg_snmp,
    },
#endif
    [LOG_CAT_TIME] =
    {
        .text   = "TIME",
        .msgnum = LOG_TIME_MAX,
        .msgtbl = log_msg_time,
    },
#ifdef CONFIG_SYS_PROTO_STP
    [LOG_CAT_STP] =
    {
        .text   = "STP",
        .msgnum = LOG_STP_MAX,
        .msgtbl = log_msg_stp,
    },
#endif
    [LOG_CAT_STORM_CONTROL] =
    {
        .text   = "STORM_CONTROL",
        .msgnum = LOG_STORM_CONTROL_MAX,
        .msgtbl = log_msg_storm_control,
    },
    [LOG_CAT_SYSTEM] =
    {
        .text   = "SYSTEM",
        .msgnum = LOG_SYSTEM_MAX,
        .msgtbl = log_msg_system,
    },
    [LOG_CAT_LAG] =
    {
        .text   = "LAG",
        .msgnum = LOG_LAG_MAX,
        .msgtbl = log_msg_lag,
    },
#ifdef CONFIG_SYS_PROTO_UDLD
    [LOG_CAT_UDLD] =
    {
        .text   = "UDLD",
        .msgnum = LOG_UDLD_MAX,
        .msgtbl = log_msg_udld,
    },
#endif
    [LOG_CAT_VLAN] =
    {
        .text   = "VLAN",
        .msgnum = LOG_VLAN_MAX,
        .msgtbl = log_msg_vlan,
    },
};
