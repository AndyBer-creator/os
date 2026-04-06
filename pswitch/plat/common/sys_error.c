#include <common/sys_def.h>
#define CMD_IPMASK_STR "  A.B.C.D/A.B.C.D  IP/IP mask"
#define CMD_MACMASK_STR "  A:B:C:D:E:F/A:B:C:D:E:F  MAC/MAC mask"
static ski_sysErr_t sysError[] =
{
    { RT_ERR_FAILED,                SYS_ERR_FAILED,                  "errFailed",   "FAILED" },
    { RT_ERR_OK,                    SYS_ERR_OK,                      "errSuccess",   "SUCCESS" },

    { RT_ERR_NOT_ALLOWED,           SYS_ERR_ACT_NOT_ALLOWED,         "",   "Action not allowed" },
    { RT_ERR_NOT_FINISH,            SYS_ERR_ACT_NOT_FINISH,          "",   "Action not finished" },
    { RT_ERR_END,                   SYS_ERR_BUF_SIZE,                "",  "Buffer size error" },
    { RT_ERR_CHIP_NOT_SUPPORTED,    SYS_ERR_CHIP_NOT_SUPPORTED,       "",  "Chip not supported" },
    { RT_ERR_DROP_PRECEDENCE,       SYS_ERR_DROP_PRECEDENCE,         "",   "Invalid drop precedence" },
    { RT_ERR_END,                   SYS_ERR_DSCP_ID,                 "",   "Invalid dscp id" },
    { RT_ERR_ENTRY_EXIST,           SYS_ERR_DUPLICATE_ENTRY,          "",  "The entry already exists" },
    { RT_ERR_END,                   SYS_ERR_DUPLICATE_NAME,           "",  "The name already exists" },
    { RT_ERR_ENTRY_INDEX,           SYS_ERR_ENTRY_INDEX,              "",  "Invalid entry index" },
    { RT_ERR_ENTRY_NOTFOUND,        SYS_ERR_ENTRY_NOT_FOUND,          "",  "Specified entry doesn't exist" },
    { RT_ERR_END,                   SYS_ERR_ENTRY_IS_EXIST,          "",   "entry is exist  " },
    { RT_ERR_END,                   SYS_ERR_ENTRY_FULL,              "errEntryFull",   "database is full " },
    { RT_ERR_ETHER_TYPE,            SYS_ERR_ETHER_TYPE,              "",   "Invalid ether type" },
    { RT_ERR_END,                   SYS_ERR_FIBER_CONFIG_PHY,        "",   "A pure fiber port can not configure speed/duplex" },
    { RT_ERR_END,                   SYS_ERR_FIBER_NOT_SUPPORT_10_100, "",  "Fiber only supports 1 gigabit speed" },
    { RT_ERR_END,                   SYS_ERR_GATEWAY_NOT_FOUND,        "",  "Gateway not found" },
    { RT_ERR_INPUT,                 SYS_ERR_INPUT,                    "",  "Input Parameter Error" },
    { RT_ERR_IPV4_ADDRESS,          SYS_ERR_IPV4_ADDR,                "",  "Invalid IPv4 address" },
    { RT_ERR_END,                   SYS_ERR_IPV4_SUBNET,              "",  "Invalid IPv4 address mask" },
    { RT_ERR_END,                   SYS_ERR_IPV4_GATEWAY_MISMATCH,    "",  "Ipv4 address and gateway not in the same subnet" },
    { RT_ERR_IPV6_ADDRESS,          SYS_ERR_IPV6_ADDR,                "",  "Invalid IPv6 address" },
    { RT_ERR_END,                   SYS_ERR_IPV6_GATEWAY_MISMATCH,    "",  "IPv6 address and gateway must in the same subnet" },
    { RT_ERR_END,                   SYS_ERR_HOSTNAME,                 "",  "Invalid host name" },
    { RT_ERR_MAC,                   SYS_ERR_MAC,                      "",  "Invalid MAC address" },
    { RT_ERR_OUT_OF_RANGE,          SYS_ERR_OUT_OF_RANGE,             "",  "Specified value is out of range" },
    { RT_ERR_END,                   SYS_ERR_RANGE,                    "",  "Specified range is invalid" },
    { RT_ERR_PORT_ID,               SYS_ERR_PORT_ID,                  "",  "Invalid port id" },
    { RT_ERR_PRIORITY,              SYS_ERR_PRIORITY,                 "",  "Invalid priority" },
    { RT_ERR_QUEUE_ID,              SYS_ERR_QUEUE_ID,                 "",  "Invalid queue id" },
    { RT_ERR_END,                   SYS_ERR_SNTP_UPDATE_FAIL,         "",  "SNTP update failed" },
    { RT_ERR_END,                   SYS_ERR_STRING,                   "",  "Invalid string"},
    { RT_ERR_END,                   SYS_ERR_TIME_DLS_RANGE_ERR,       "",  "Error! Start Time greater than End Time." },
    { RT_ERR_END,                   SYS_ERR_TIME_DLS_RANGE_OFFSET_ERR, "", "Error! The difference between daylight savings From and To must be greater than Time Set Offset" },
    { RT_ERR_END,                   SYS_ERR_STR_LEN_EMPTY,             "", "Empty string input" },
    { RT_ERR_END,                   SYS_ERR_STR_LEN_EXCEED,           "",  "Exceed recommended maximum string length" },
    { RT_ERR_END,                   SYS_ERR_TCPUDP_PORT,              "",  "Invalid TCP/UDP port number" },
    { RT_ERR_TIMEOUT,               SYS_ERR_TIMEOUT,                  "",  "Operation time out" },
    { RT_ERR_END,                   SYS_ERR_UNKNOWN_HOST,             "",  "Unknown host" },
    { RT_ERR_END,                   SYS_ERR_CFG_SYNC_OK,              "",  "Syncronize config success" },
    { RT_ERR_END,                   SYS_ERR_CFG_SYNC_FAILED,          "",  "Syncronize config failed" },
    { RT_ERR_END,                   SYS_ERR_CFG_EXTNAME,              "",  "Configuration file extension name is invalid" },
    { RT_ERR_END,                   SYS_ERR_CFG_INVALID,              "",  "Invalid config file" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_INVALID,            "",  "Flash image is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_MAGIC_INVALID,      "",  "Image signature is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_ERASE_ACTIVE,       "",  "Do not allow to earse active image partition" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_OUT_OF_MEMORY,      "",  "System do not have sufficient memory to do firmware image operation. Please turn off some memory consuming features such as SNMP to make operation available" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_EXTNAME,            "",  "Image extension name is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_PROCESSING,         "",  "Image is processing, do not allow to process another image" },
    { RT_ERR_END,                   SYS_ERR_DNS_RESOLVE,              "", 	"DNS resolution failed. Please check DNS server setting or host name" },
    { RT_ERR_END,                   SYS_ERR_DNS_ENTRY_FULL,           "errDNSR", "DNS resolution failed. Please check DNS server setting or host name" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_FULL,        "", "Exceed max support user number" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_NOT_EXIST,   "", "The user does not exist" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_EMPTY,       "", "Please specify a user name" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_DEFAULT_DEL, "", "Default user name cannot be deleted." },

    { RT_ERR_END,                   SYS_ERR_USERNAME_ACCESSMETH,      "", "This user has no access rights" },
    { RT_ERR_END,                   SYS_ERR_LAST_LOCALUSER,        "", "The last user" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_PASSWORD_LEN,"", "User password length is invalid" },
    { RT_ERR_NOT_ALLOWED,           SYS_ERR_ACT_NOT_ALLOWED,          "errActionNoAllowed",    		"Action not allowed" },
    { RT_ERR_NOT_FINISH,            SYS_ERR_ACT_NOT_FINISH,           "errActionNoFinished",   		"Action not finished" },
    { RT_ERR_END,                   SYS_ERR_BUF_SIZE,                 "errBufferSizeError",  		"Buffer size error" },
    { RT_ERR_CHIP_NOT_SUPPORTED,    SYS_ERR_CHIP_NOT_SUPPORTED,       "errChipNoSupported",  		"Chip not supported" },
    { RT_ERR_DROP_PRECEDENCE,       SYS_ERR_DROP_PRECEDENCE,          "errInvalidDropPrecedence",   "Invalid drop precedence" },
    { RT_ERR_END,                   SYS_ERR_DSCP_ID,                  "errInvalidDscpId",   		"Invalid dscp id" },
    { RT_ERR_ENTRY_EXIST,           SYS_ERR_DUPLICATE_ENTRY,          "errEntryAlreadyExist",  		"The entry already exists" },
    { RT_ERR_END,                   SYS_ERR_DUPLICATE_NAME,           "errNameAlreadyExist",  		"The name already exists" },
    { RT_ERR_ENTRY_INDEX,           SYS_ERR_ENTRY_INDEX,              "errInvalidEntryIndex",  		"Invalid entry index" },
    { RT_ERR_ENTRY_NOTFOUND,        SYS_ERR_ENTRY_NOT_FOUND,          "errSEntryNoExist",  			"Specified entry doesn't exist" },
    { RT_ERR_END,                   SYS_ERR_ENTRY_IS_EXIST,           "errEntryIsExist",   			"entry is exist  " },
    { RT_ERR_END,                   SYS_ERR_ENTRY_FULL,               "errDatabaseIsFull",   		"database is full " },
    { RT_ERR_ETHER_TYPE,            SYS_ERR_ETHER_TYPE,               "errInvalidEtherType",   		"Invalid ether type" },
    { RT_ERR_END,                   SYS_ERR_FIBER_CONFIG_PHY,         "errFiberPortNoSetSD",   		"A pure fiber port can not configure speed/duplex" },
    { RT_ERR_END,                   SYS_ERR_FIBER_NOT_SUPPORT_10_100, "errFiberOnlySupport",  		"Fiber only supports 1 gigabit speed" },
    { RT_ERR_END,                   SYS_ERR_GATEWAY_NOT_FOUND,        "errGatewayNoFound",  		"Gateway not found" },
    { RT_ERR_INPUT,                 SYS_ERR_INPUT,                    "errInputParameterErr",  		"Input Parameter Error" },
    { RT_ERR_IPV4_ADDRESS,          SYS_ERR_IPV4_ADDR,                "errInvalidIPAddr",  			"Invalid IPv4 address" },
    { RT_ERR_END,                   SYS_ERR_IPV4_SUBNET,              "errInvalidIPMask",  			"Invalid IPv4 address mask" },
    { RT_ERR_END,                   SYS_ERR_IPV4_GATEWAY_MISMATCH,    "errIPAndGWNoInSameSub",  	"Ipv4 address and gateway not in the same subnet" },
    { RT_ERR_IPV6_ADDRESS,          SYS_ERR_IPV6_ADDR,                "errInvalidIPv6Addr",  		"Invalid IPv6 address" },
    { RT_ERR_END,                   SYS_ERR_IPV6_GATEWAY_MISMATCH,    "errIPv6AddrAndGWMust",  		"IPv6 address and gateway must in the same subnet" },
    { RT_ERR_END,                   SYS_ERR_HOSTNAME,                 "errInvalidHostName",  		"Invalid host name" },
    { RT_ERR_MAC,                   SYS_ERR_MAC,                      "errInvalidMacAddr",  		"Invalid MAC address" },
    { RT_ERR_OUT_OF_RANGE,          SYS_ERR_OUT_OF_RANGE,             "errSpecifiedValueOut",  		"Specified value is out of range" },
    { RT_ERR_END,                   SYS_ERR_RANGE,                    "errSpecifiedInvalid",  		"Specified range is invalid" },
    { RT_ERR_PORT_ID,               SYS_ERR_PORT_ID,                  "errInvalidPid",  			"Invalid port id" },
    { RT_ERR_PRIORITY,              SYS_ERR_PRIORITY,                 "errInvlaidPri",  			"Invalid priority" },
    { RT_ERR_QUEUE_ID,              SYS_ERR_QUEUE_ID,                 "errInvalidQueueId",  		"Invalid queue id" },
    { RT_ERR_END,                   SYS_ERR_SNTP_UPDATE_FAIL,         "errSNTPUpFail", 	 			"SNTP update failed" },
    { RT_ERR_END,                   SYS_ERR_STRING,                   "errInvlaidStr",  			"Invalid string"},
    { RT_ERR_END,                   SYS_ERR_TIME_DLS_RANGE_ERR,       "errTimeErr",  				"Error! Start Time greater than End Time." },
    { RT_ERR_END,                   SYS_ERR_TIME_DLS_RANGE_OFFSET_ERR,"errTimeDaylight", 			"Error! The difference between daylight savings From and To must be greater than Time Set Offset" },
    { RT_ERR_END,                   SYS_ERR_STR_LEN_EMPTY,            "errEmptyStrInput", 			"Empty string input" },
    { RT_ERR_END,                   SYS_ERR_STR_LEN_EXCEED,           "errExceedStrLen",  			"Exceed recommended maximum string length" },
    { RT_ERR_END,                   SYS_ERR_TCPUDP_PORT,              "errInvlaidPortNum", 	 		"Invalid TCP/UDP port number" },
    { RT_ERR_TIMEOUT,               SYS_ERR_TIMEOUT,                  "errOperationTimeOut",  		"Operation time out" },
    { RT_ERR_END,                   SYS_ERR_UNKNOWN_HOST,             "errUnknownHost",  			"Unknown host" },
    { RT_ERR_END,                   SYS_ERR_CFG_SYNC_OK,              "errConfigSuccess",  			"Syncronize config success" },
    { RT_ERR_END,                   SYS_ERR_CFG_SYNC_FAILED,          "errConfigFailed",  			"Syncronize config failed" },
    { RT_ERR_END,                   SYS_ERR_CFG_EXTNAME,              "errConfigNameErr",  			"Configuration file extension name is invalid" },
    { RT_ERR_END,                   SYS_ERR_CFG_INVALID,              "errConfigFailErr",  			"Invalid config file" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_INVALID,            "errFlashInvalid",  			"Flash image is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_MAGIC_INVALID,      "errImageInvalid",  			"Image signature is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_ERASE_ACTIVE,       "errimageNoAllow",  			"Do not allow to earse active image partition" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_OUT_OF_MEMORY,      "errSystemImageErr",  		"System do not have sufficient memory to do firmware image operation. Please turn off some memory consuming features such as SNMP to make operation available" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_EXTNAME,            "errSystemNameInvalid",  		"Image extension name is invalid" },
    { RT_ERR_END,                   SYS_ERR_IMAGE_PROCESSING,         "errSystemAllowIErr",  		"Image is processing, do not allow to process another image" },
    { RT_ERR_END,                   SYS_ERR_DNS_RESOLVE,              "errDnsFailed", 				"DNS resolution failed. Please check DNS server setting or host name" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_FULL,           "errUserNumMaxErr", 			"Exceed max support user number" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_NOT_EXIST,      "errUserNoExist", 			"The user does not exist" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_EMPTY,          "errSpecifyUserName", 		"Please specify a user name" },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_DEFAULT_DEL,    "errDefaultUserErr", 			"Default user name cannot be deleted." },
    { RT_ERR_END,                   SYS_ERR_LOCALUSER_CURRENT_DEL,    "errCurrentUserErr", 			"The current user name cannot be deleted." },
    { RT_ERR_END,                   SYS_ERR_USERNAME_INVALID,         "errUseNaINvalidErr", 		"User name is invalid" },
    { RT_ERR_END,                   SYS_ERR_PASSWORD_INVALID,         "errPasswordInErr", 			"Password is invalid" },
    { RT_ERR_END,                   SYS_ERR_USERNAME_LENGTH_INVALID,  "errUseNaLengthErr", 			"The length of the username is invalid" },
    { RT_ERR_END,                   SYS_ERR_PASSWORD_LENGTH_INVALID,  "errPasswordLengthErr", 		"The length of the password is invalid" },

#ifdef CONFIG_SYS_AAA
    
    { RT_ERR_END,                   SYS_ERR_AAA_AUTHLIST_FULL,        "errAuthLiFull",   			"Auth Lists are full" },
    { RT_ERR_END,                   SYS_ERR_AAA_AUTHLIST_NOT_EXIST,   "errAuthLiExist",   			"Auth List not exist" },
    { RT_ERR_END,                   SYS_ERR_AAA_AUTHLIST_EMPTY,       "errAuthLiName", 				"Please specify a Auth List name" },
    { RT_ERR_END,                   SYS_ERR_AAA_HTTP_NOT_SUPPORTED_ENABLE,       "errAuthType", 		"It is not allowed enable auth for 'http' or https" },

#endif

    { RT_ERR_END,                   SYS_ERR_ACL_PHASE_INVALID,        "errACLPhInvalid",  			"Phase is invalid"},
    { RT_ERR_END,                   SYS_ERR_ACL_BLK_AGGTYPE_INVALID,  "errACLTypeIsInvalid",  		"Block aggregator type is invalid"},
    { RT_ERR_END,                   SYS_ERR_ACL_BLK_USING,            "errACLBlUsedPr",  			"Block is using by protocol"},
    { RT_ERR_END,                   SYS_ERR_ACL_BLK_ID_INVALID,       "errACLBlInInvalid",  		"Block index is invalid"},
    { RT_ERR_END,                   SYS_ERR_ACL_BLK_FULL,             "errACLBlACLFull",   			"Block in ACL is full"},
    { RT_ERR_END,                   SYS_ERR_ACL_TMPLTE_FULL,          "errACLTempIsFulle",  		"Template is fulle"},
    { RT_ERR_END,                   SYS_ERR_ACL_TMPLTE_ID_INVALID,    "errACLTempIdInvalid",  		"Template id is invalid"},
    { RT_ERR_END,                   SYS_ERR_ACL_FIELD_NOT_EXIST,      "errACLFieldNOExist",  		"Field does not exist in the template"},
    { RT_ERR_END,                   SYS_ERR_ACL_RATE_ENTRY_ID_INVALID,"errACLRateRnIdErr", 			"Rate entry id is invalid"},
    { RT_ERR_END,                   SYS_ERR_ACL_RATE_ENTRY_FULL,       "errACLPoleFull",			"Policer table is full"},
    { RT_ERR_END,                   SYS_ERR_ACL_VLAN_RANGE_ENTRY_FULL, "errACLVlanEnOccupie",		"All vlan range entries are occupied"},
    { RT_ERR_END,                   SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL,"errACLEnRangeFull", 		"Cannot create new entry in the Port Range table since the table is full"},
    { RT_ERR_END,                   SYS_ERR_ACL_LOG_ENTRY_FULL,        "errACLLogTableFull", 		"Log table is full" },
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    { RT_ERR_END,                   SYS_ERR_ACL_ENTRY_EMPTY,           "errACLEmpty", "ACE does not exist." },
    { RT_ERR_END,                   SYS_ERR_ACL_ENTRY_EXISTS,          "errACLExists", "ACE already exists." },
    { RT_ERR_END,                   SYS_ERR_ACL_ENTRY_PRIORITY,        "errACLPri", "MIB Index is out of range.Index must be bigger then 0 and Existing ifindex." },
    { RT_ERR_END,                   SYS_ERR_ACL_LIST_EMPTY,           "errACLlistEmpty",  "ACL does not exist." },
    { RT_ERR_END,                   SYS_ERR_ACL_LIST_EXISTS,          "errACLlistexists",  "Different type of ACL with the same name already exists."  },
    { RT_ERR_END,                   SYS_ERR_ACL_LIST_BINDING,         "errACLlistbinding",  "ACL list is binding"  },
    { RT_ERR_END,                   SYS_ERR_ACL_EMPTY_BINDING,        "errACLemptybind",  "Empty ACL Can Not be Attached to Class-map or interface." },
    { RT_ERR_END,                   SYS_ERR_ACL_ILLEGAL_BINDING,      "errACLillegalEmpty",  "Type of ACL is illegal." },
    { RT_ERR_END,                   SYS_ERR_ACL_INSUFF_PIEENTRY,      "errACLinsuff",  "Insufficient PIE entries" },
    { RT_ERR_END,                   SYS_ERR_ACL_DSCP_INVALID,         "errACDscp",  "DSCP value is invalid. DSCP value should be 0-63" },
    { RT_ERR_END,                   SYS_ERR_ACL_TCPFLAG_INVALID,      "errACLTCPFlag",  "Illegal Tcp Flags String." },
    { RT_ERR_END,                   SYS_ERR_ACL_PRECEDENCE_INVALID,   "errACLPrecedence",  "IP-PRECEDENCE value is invalid.! IP-PRECEDENCE value should be 0-7" },
    { RT_ERR_END,                   SYS_ERR_ACL_PORT_RANGE_INVALID,   "errACLrange",  "Port range value is invalid" },
    { RT_ERR_END,                   SYS_ERR_ACL_PORT_HAS_BINDED_TYPE, "errACLPortType",  "Can't apply acl since another same type acl already applied." },
    { RT_ERR_END,                   SYS_ERR_ACL_PORT_HAS_BINDED_ACL,  "errACLPortAcl",  "Can't apply acl since another acl already applied." },
    { RT_ERR_END,                   SYS_ERR_ACL_PORT_TRKMBR_BIND_ACL, "errACLPortTRUNK",  "Port-Channel member port is not allowed to bind/unbind ACL" },
    { RT_ERR_END,                   SYS_ERR_ACL_ENTRY_FULL,           "errACLEntryFULl",  "Cannot create new entry in acl table since the table is full"},
    { RT_ERR_END,                   SYS_ERR_ACE_ENTRY_FULL,          "errACEEntryTaFull",   "Cannot create new ace entry in acl table since the table is full"},
    { RT_ERR_END,                   SYS_ERR_ACE_ENTRY_POOL_FULL,      "errACLEntryPOOLFULL",  "Cannot create new ace entry since the table is full"},
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    { RT_ERR_END,                   SYS_ERR_AUTH_PORT_TRUNKMBR,            "",     "Authentication manager is not able to enabled on trunk member port"},
    { RT_ERR_END,                   SYS_ERR_DOT1X_VLAN_MODE,                "",    "802.1x is not allowed in VLAN hybrid mode or 802.1q-tunnel mode" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_TRUNK_MODE,               "",    "Port belongs to a port-channel" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VID_GUESTVLAN,             "",    "A Guest VLAN can not be configured as Default VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_DEL,           "",    "Not allowed to delete Guest VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_RSPAN_VLAN,    "",    "An RSPAN VLAN can not be configured as Guest VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_VOICE_VLAN,     "",   "A Voice VLAN can not be configured as Guest VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_SURVEILLANCE_VLAN, "", "A Surveillance VLAN can not be configured as Guest VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_MGMT_VLAN,        "",  "A Management VLAN can not be configured as Guest VLAN" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_PORT_VLAN_CONFIG,  "", "Not allowed to configure Guest VLAN for Guest VLAN enabled ports" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_GUEST_VLAN_PORT_PVID_CONFIG,  "", "Not allowed to configure PVID for Guest VLAN enabled ports" },
    { RT_ERR_END,                   SYS_ERR_DOT1X_MGMT_VLAN_GUEST_VLAN,        "",  "A Guest VLAN can not be configured as Management VLAN" },
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    { RT_ERR_END,                   SYS_ERR_WEBAUTH_WEBAUTH_MULTIHOST,         "", "Web-based authentication can not be enabled since port is in multi-host mode" },
    { RT_ERR_END,                   SYS_ERR_WEBAUTH_MULTIHOST_WEBAUTH,        "",  "Host mode can not be changed to multi-host since Web-based authentication is enabled" },
  #endif
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    
    { RT_ERR_END,                   SYS_ERR_IGMP_REMOVE_PORT_OK,      "errIgmpPort", "Mcast remove port OK"},
    { RT_ERR_END,                   SYS_ERR_IGMP_RSV_ADDR,             "errIgmpAddr",     "Invalid multicast address, it is reserved"},
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    { RT_ERR_END,                   SYS_ERR_IGMP_MAX_GROUP_ACT,        "errIgmpgroup", "Invalid IGMP max-group action"},
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    { RT_ERR_END,                   SYS_ERR_IGMP_QUERIER_CHANGE,      "errIgmpQuerierChange",  "Querier is not self,can not modify"},
    { RT_ERR_END,                   SYS_ERR_IGMP_QUERIER_VERSION,     "errIgmpQuerierVersion",  "Querier version can not larger than igmp version"},
 #endif
    { RT_ERR_END,                   SYS_ERR_IGMP_QUERY_INTVL,         "errIgmpQuery",  "Invalid IGMP Query interval" },
    { RT_ERR_END,                   SYS_ERR_IGMP_RESPONSE_TIME,       "errIgmpResponse",  "Invalid IGMP Response time" },
    { RT_ERR_END,                   SYS_ERR_IGMP_ROUTER_TIMEOUT,      "errIgmpRouter",  "Invalid IGMP Router timeout" },
    { RT_ERR_END,                   SYS_ERR_IGMP_LSTMBRQUERY_INTVL,   "errIgmpINTVl",  "Invalid IGMP Last member query interval" },
    { RT_ERR_END,                   SYS_ERR_IGMP_ROBUSTNESS_VAR,      "errIgmpVar",  "Invalid IGMP Robustness variable" },
    { RT_ERR_END,                   SYS_ERR_IGMP_STATIC_ENTRY_EXIST,  "errIgmpstaticEntry",  "IGMP Static Group is exist"},
    { RT_ERR_END,                   SYS_ERR_IGMP_GROUP_ENTRY_NOT_EXIST, "errIgmpGroupEntry", "IGMP Group entry does not exist"},

    { RT_ERR_END,                   SYS_ERR_IGMP_UNKNOWN_ROUTERPORT_ACT, "errIgmpRouterPort", "Invalid unknown multicast action,when multicast snooping is disable,action router-port can not be set"},
    { RT_ERR_END,                   SYS_ERR_MCAST_GROUP_TYPE,            "errMCASTGroupType",   "Multicast Group type error "},
    { RT_ERR_END,                   SYS_ERR_MCAST_ADDR,              "errMCASTADDR", "Multicast Address error "},
    { RT_ERR_END,                   SYS_ERR_MCAST_IPTYPE,            "errMCASTIPType",   "Multicast ip type error "},
    { RT_ERR_END,                   SYS_ERR_MCAST_VLAN_NOT_EXIST,    "errMCASTVlan",   "Multicast vlan not exist"},
    { RT_ERR_END,                   SYS_ERR_MCAST_DATABASE_FULL,     "errMCASTDATA",   "Multicast group database is full"},

 #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    { RT_ERR_END,                   SYS_ERR_IGMP_PROFILE_ID,        "errIgmpProID",    "IGMP profile ID not allowed"},
    { RT_ERR_END,                   SYS_ERR_IGMP_PROFILE_TYPE,      "errIgmpProType",  "IGMP profile type is error"},
    { RT_ERR_END,                   SYS_ERR_IGMP_PROFILE_USED,      "errIgmpProUsed",    "IGMP profile ID is used, can not delete and modify"},
    { RT_ERR_END,                   SYS_ERR_IGMP_PROFILE_NOT_EXIST,  "errIgmpProNotExist",   "IGMP profile does not exist"},
    { RT_ERR_END,                   SYS_ERR_IGMP_PROFILE_EMPTY,       "errIgmpProEmpty",  "IGMP profile is empty"},
    { RT_ERR_END,                   SYS_ERR_MCAST_PROFILE_IPTYPE_NOT_MATCH,  "errIgmpProIPType", "Multicast profile ip type is not match"},
 #endif

    #ifdef CONFIG_SYS_PROTO_MVR
    { RT_ERR_END,                   SYS_ERR_MVR_ENABLE,                  "errMVRenable",   "MVR is Disabled, can not configure the setting of mvr"},
    { RT_ERR_END,                   SYS_ERR_MVR_TYPE,                   "errMVRType",     "MVR error port type"},
    { RT_ERR_END,                   SYS_ERR_MVR_PORT_MODE,           "errMVRPortMode",  "MVR port type can not configure for this vlan port mode"},
    { RT_ERR_END,                   SYS_ERR_MVR_VLAN_PORT_TYPE,    "errMVRVlanPortType",  "VLAN membership can not configure the MVR port type (source port must in MVR VLAN,receiver port must not be MVR VLAN)"},
    { RT_ERR_END,                   SYS_ERR_MVR_VLAN_PORT_SOURCE, "errMVRVlanPortSource", "MVR source port must in mvr vlan"},
    { RT_ERR_END,                   SYS_ERR_MVR_VLAN_PORT_RECEIVER,      "errMVRVlanPortReceiver","MVR revciever port can not be mvr vlan member"},
    { RT_ERR_END,                   SYS_ERR_MVR_STATIC_GROUP_CONFLICT, "errMVRStaticGroupConflict",  "MVR static group conflict with MVR group range"},
    { RT_ERR_END,                   SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE, "errMVRStaticGroupPortType", "MVR static group port member,In compatible mode, applies to only receiver ports. In dynamic mode, it applies to receiver ports and source ports"},
    { RT_ERR_END,                   SYS_ERR_MVR_VLAN_STATIC_GROUP_SET,  "errMVRVlanStaticSet",  "MVR static group vlan id is not the MVR VLAN ID"},
    { RT_ERR_END,                   SYS_ERR_MVR_IGMP_STATIC_GROUP_SET,  "errMVRigmpStaticSet",  "IGMP Snooping static group can not use MVR group"},
    { RT_ERR_END,                   SYS_ERR_STATIC_GROUP_SET,             "errStaticGroup",       "MVR is enabled and the Static group vlan is mvr vlan"},
    #endif
#endif

#ifdef CONFIG_SYS_PROTO_ISG
    
    { RT_ERR_END,                   SYS_ERR_ISG_MAX_ENTRY_REACH,      "errIsgEntryReached",  "Maximun number of entries of port has reached"},
    { RT_ERR_END,                   SYS_ERR_ISG_MAX_ENTRY_INVALID,    "errIsgBindingNumber",  "Maximun number can't smaller than current binding number."},
    { RT_ERR_END,                   SYS_ERR_ISG_DB_FULL,              "errIsgDataFull",  "IP Source Guard database is full"},
    { RT_ERR_END,                   SYS_ERR_ISG_SAME_ENTRY,           "errIsgIpSourceEx",  "IP Source Guard Entry is exist"},
    { RT_ERR_END,                   SYS_ERR_ISG_MULTI_INVALID,        "errIsgMacAddr",  "MAC address can not be multicast address"},
#endif
    
    { RT_ERR_END,                   SYS_ERR_L2_MAC_CLEARTYPE_INVALID, "errMacEntryCl",  "Error type of mac entry clear" },
    { RT_ERR_END,                   SYS_ERR_L2_MAC_CONFIT_WITH_STATIC, "errMacStaticTable", "Mac entry exist in static table" },
    { RT_ERR_L2_NO_EMPTY_ENTRY,     SYS_ERR_L2_MAC_FULL,              "errMacFull",  "Mac table is full" },
    { RT_ERR_L2_ENTRY_NOTFOUND,     SYS_ERR_L2_MAC_NOT_EXIST,         "errMacEntryNoExist",  "Mac entry does not exist" },
    { RT_ERR_L2_ENTRY_EXIST,        SYS_ERR_L2_MAC_IS_EXIST,           "errMacEntryExist", "Mac entry does already exist"},
    { RT_ERR_END,                   SYS_ERR_L2_MAC_UCAST_PORT_INVALID, "errMacInvalidSet", "Invalid unicast port setting" },
    { RT_ERR_END,                   SYS_ERR_L2_MAC_UCAST_ADDR,        "errMacInvalidUnAddr",  "Invalid unicast mac address" },
    { RT_ERR_END,                   SYS_ERR_L2_STATIC_AZERO_INVALID,  "errStaticMacZero",  "Static entry cannot be all zero MAC address" },
    { RT_ERR_END,                   SYS_ERR_L2_STATIC_EQ_SYSMAC,      "errStaticMacAddr",  "Static entry cannot be system MAC address" },
    { RT_ERR_END,                   SYS_ERR_L2_STATIC_BE_MCAST,       "errStaticMulPort",  "Static entry cannot be multicast MAC address" },
    { RT_ERR_END,                   SYS_ERR_L2_STATIC_PORT_TRUNKMBR_INVALID,  "errStaticNoTrunkMem",  "Static entry port can NOT be trunk member port" },
    { RT_ERR_END,                   SYS_ERR_L2_STATIC_PORT_EMPTYTRUNK_INVALID, "errStaticNoEmPort",   "Static entry port can NOT be empty trunk port" },
    { RT_ERR_END,                   SYS_ERR_L2_DB_BUSY,               "errDataIsBusy",  "MAC Address Database is busy. Please try it later" },
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    { RT_ERR_END,                   SYS_ERR_L2_MAC_CONFIT_WITH_FILTER,"errBlackEntryEx",  "Mac entry exist in filter table" },
    { RT_ERR_END,                   SYS_ERR_L2_FILTER_AZERO_INVALID,  "errBlackZebroAddr",  "Filter entry cannot be all zero MAC address" },
    { RT_ERR_END,                   SYS_ERR_L2_FILTER_EQ_SYSMAC,     "errBlackSysMac",   "Filter entry cannot be system MAC address" },
    { RT_ERR_END,                   SYS_ERR_L2_FILTER_BE_MCAST,      "errBlackEntrMuMac",   "Filter entry cannot be multicast MAC address" },
#endif
#ifdef CONFIG_SYS_MIRROR
    { RT_ERR_END,                   SYS_ERR_MIRROR_GROUP_INVALID,    "errMirrorNoSession",   "Invalid mirror session id" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_TARGET_EXIST,     "errMirrorDstPort",   "A port can not be monitor destination port for multiple sessions" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_TARGET_NOT_EXIST,  "errMirrorDstNoExist",  "Monitor destination port does not exist" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_TARGET,            "errMirrorPortErr",  "Monitor destination port Error" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_SELF_MIRROR,       "errMirrorSourNoList",  "Monitor Destination port can NOT appears in source port list" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_MONITOR_PORT_MIRRORED,"errMirrorDstMiPor", "Monitor Destination port can not be mirrored port" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_TRUNK,            "errMirrorTrunkPort",   "Monitor Destination port can NOT trunk port" },
    { RT_ERR_END,                   SYS_ERR_MIRROR_TRUNK_MEMBER,     "errMirrorMemPort",   "Monitor Destination port can NOT be trunk member port" },
    #ifdef CONFIG_SYS_PROTO_GVRP
    { RT_ERR_END,                   SYS_ERR_MIRROR_GVRP_ENABLE,      "errMirrorGvrpEnable",   "Monitor Destination port is GVRP Enabled" },
    #endif
    { RT_ERR_END,                   SYS_ERR_MIRROR_NOT_PORTBASED,    "errMirrorEnableState",   "Mirror session is not in port based enable state" },
#endif

    { RT_ERR_END,                   SYS_ERR_L3_INTFTYPE_INVALID,       "errL3InterfaceInval",     "Interface type invalid" },
    { RT_ERR_END,                   SYS_ERR_L3_DB_FULL,                "errL3DatabaseFull",     "Database is full" },
    { RT_ERR_END,                   SYS_ERR_L3_DB_NOT_FOUND,           "errL3EntryNodata",     "Entry is not found in database" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_NUM_EXCEED,        "errL3NumInterNum",     "Number of maximum interface number exceed" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_LO_ID_INVALID,     "errL3NoInterfaceId",     "Invalid loopback intreface id" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_NOT_EXIST,     "errL3IpNoExist",      "IP address does not exist on interface" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_PRIMARY_IP_NOT_EXIST, "errL3IpPriNoExist",     "The primary IP address does not exist, please configure the primary IP address first" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_EXIST,          "errL3IpIsExist",     "IP address already exists" },
	{ RT_ERR_END,					SYS_ERR_L3_INTF_PRIMARY_IP_EXIST,  "errL3IpPriIsExist",	 "IP address already exists, This IP address is the current primary IP address" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_SUB_EXIST,      "errL3IpSubIsExist",     "Error: Please delete the sub address in the interface view first." },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_DEL_PRIMARY,      "errL3IpIsPri",     "Error: The specified address cannot be deleted because it is not the primary address of this interface" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_DEL_SUB,         "errL3IpIsSub",     "Error: The specified address cannot be deleted because it is not the sub address of this interface" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_IP_NUM_EXCEED,     "errL3IpNumInterface",     "IP address number exceed on interface" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_NOT_ROUTING,       "errL3RouteInterface",     "Not L3 routing interface" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_DISABLE_IPV6_ADDR_EXIST,  "errL3NoIpv6InterSet", "Cannot disable IPv6 processing on an interface that is configured with an explicit IPv6 address" },
    { RT_ERR_END,                   SYS_ERR_L3_IP_DUPLICATE,             "errL3SameSubnet",   "Duplicated IP on the same subnet" },
    { RT_ERR_END,                   SYS_ERR_L3_IP_OVERLAP,              "errL3IpTheSubnet",    "Overlapped IP on the subnet" },
    { RT_ERR_END,                   SYS_ERR_L3_IP_ADDR_INVALID,         "errL3InvaliIpAddr",    "Invalid IP address" },
    { RT_ERR_END,                   SYS_ERR_L3_IP_MASK_INVALID,         "errL3InvalidIpMaskErr",    "Invalid IP mask" },
    { RT_ERR_END,                   SYS_ERR_L3_VLAN_IS_MGMT_VLAN,        "",   "This VLAN is the management VLAN,IP configuration is not allowed" },
    { RT_ERR_END,                   SYS_ERR_MGMT_VLAN_IS_L3_IP,           "",  "Configuration failed, please delete the IP configuration of the Layer 3 interface" },
    { RT_ERR_END,                   SYS_ERR_L3_IPV6_PREFIX_INVALID,    "errL3Ipv6NoValid",     "IPv6 prefix is not valid" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_GW_INTF_IP,       "errL3GWAddrNoDevi",     "Gateway cannot be one of the addresses configured on this device" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_GW_NOT_DIRECT,    "errL3RouterDefGW",     "Only a directly connected router can be defined as the gateway" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_GW_DHCP_ENBL,      "errL3GWDHCPInter",    "Gateway cannot on the DHCP enabled interface" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_GW_MGMT_SUBNET,   "errL3GWSameVlan",     "Gateway cannot be the same subnet as the management VLAN" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_ON_LO_INTF,        "errL3RouteViaInter",    "IP static route cannot be defined via loopback interface" },
    { RT_ERR_END,                   SYS_ERR_L3_ROUTE_MAX_NH_NUM_EXCEED, "errL3HopsNetExceed",    "Max number of config next hops for this network has been exceeded" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_ROUTE_FULL,       "errL3StaticFull",    "Static route database is full" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST,  "errL3StaticExist",    "Static route does not exist" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGH_MAC,        "errL3StaticMustAddr",    "ARP MAC address must be Unicast address" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGH_MAC_SAMEDUT, "errL3MacAddrNoErr",   "MAC address can not same as device mac." },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV4_NOT_MATCH, "errL3ArpNoMatch",   "ARP interface does not match IP interface port" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV4_NOT_EXIST, "errL3ArpNwDir",   "ARP IP must be from a directly connected network" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV4_SAME_DUT,   "errL3ArpDefDevice",  "ARP IP cannot be an IP interface defined on this device." },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV4_INTF_NOT_EXIST,"errL3NoSuchExist", "No such instance exists" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV4_INTF_DHCP,  "errL3SetARPOverBy",    "Setting ARP static configuration over IP interface obtained by DHCP is not allowed" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV6_INTF_NOTON, "errL3Ipv6NoEnbaleInter",  "IPv6 is not enabled on the interface" },
    { RT_ERR_END,                   SYS_ERR_L3_STATIC_NEIGHV6_NOT_MATCH,  "errL3NeighNoCreate",  "Neighbor entry cannot be created - subnet is not defined on the device" },
    { RT_ERR_END,                   SYS_ERR_L3_INTF_EXIST,               "errL3VlanRelay",   "VLAN can not be deleted because IP Interface or DHCP relay is defined on the VLAN." },

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    { RT_ERR_END,                   SYS_ERR_MLD_MAX_GROUP_ACT,      "errIgmpProGroup",   "Invalid MLD max-group action"},
    #endif
    { RT_ERR_END,                   SYS_ERR_MLD_PROFILE_ID,         "errIgmpProAllow",   "MLD profile ID not allowed"},
    { RT_ERR_END,                   SYS_ERR_MLD_PROFILE_USED,       "errIgmpProMldMod",   "MLD profile ID is used, can not delete and modify"},
    { RT_ERR_END,                   SYS_ERR_MLD_PROFILE_NOT_EXIST,  "errIgmpProMldExist",   "MLD profile does not exist"},
    { RT_ERR_END,                   SYS_ERR_MLD_PROFILE_EMPTY,       "errIgmpProMldZero",  "MLD profile is empty"},
    { RT_ERR_END,                   SYS_ERR_MCAST_GROUP_MAX,         "errIgmpProMulMaximum",              "Multicast port group arrived maximum"},
#endif

#ifdef CONFIG_SYS_PROTO_LACP
    
    { RT_ERR_END,                   SYS_ERR_LACP_SYSTEM_PRIORITY,   "errLacpPri",    "Invalid LACP system priority" },
    { RT_ERR_END,                   SYS_ERR_LACP_ACTIVE_PORTS,      "errLacpActive",    "Invalid LACP active ports" },
    { RT_ERR_END,                   SYS_ERR_LACP_NOT_MEMBER_PORTS,   "errLacpNotMember",   "Some ports are not LACP member ports"},
    { RT_ERR_END,                   SYS_ERR_LACP_STATIC_TRUNK_GROUP, "errLacpStatic",   "Can not add LACP trunk member to static trunk"},
#endif

    { RT_ERR_PORT_LINKDOWN,         SYS_ERR_PORT_LINKDOWN,         "errPortLinkDown",     "Port link down" },
    { RT_ERR_PHY_AUTO_ABILITY,      SYS_ERR_PORT_AUTO_SPEED,       "errPortAutoSpeed",     "Port speed capability invalid" },
    { RT_ERR_END,                   SYS_ERR_PORT_RANGE,            "",     "Port range invalid" },
    { RT_ERR_PHY_SPEED,             SYS_ERR_PORT_SPEED,             "errPortSpeed",    "Port speed invalid" },
    { RT_ERR_PHY_DUPLEX,            SYS_ERR_PORT_DUPLEX,            "errPortDuplex",    "Port duplex invalid" },
    { RT_ERR_END,                   SYS_ERR_PORT_1000M_NOT_SUPPORT,   "errPort1000NO",  "Fast Ethernet port does not support 1000M" },
    { RT_ERR_END,                   SYS_ERR_PORT_1000M_HALF_NOT_SUPPORT,   "errPort1000HalfNO",  "Giga Ethernet port does not support 1000/Half" },
    { RT_ERR_END,                   SYS_ERR_PORT_FIBER_SPEED,         "errFiberSpeed",  "Fiber port speed invalid" },
    { RT_ERR_END,                   SYS_ERR_PORT_FIBER_DUPLEX,        "errFiberDuplex",  "Fiber port duplex invalid" },
    { RT_ERR_END,                   SYS_ERR_PORT_AUTO_NEGO_NOT_ALLOW, "errPortAuto",  "Port do not allow auto negotiation" },
    { RT_ERR_END,                   SYS_ERR_PORT_NOTJOINIVLAN,        "errPortNoVlan",  "Port is not the member port of the VLAN" },
    { RT_ERR_END,                   SYS_ERR_PORT_SET_FIBER_TO_COPPER, "",  "Port is copper only, do not allow fiber configuration" },

    { RT_ERR_PHY_RTCT_NOT_FINISH,   SYS_ERR_PORT_RTCT_NOT_FINISH,     "",           "Port has not finish RTCT yet" },
    { RT_ERR_END,                   SYS_ERR_PORT_RTCT_NOT_COPPER,      "",          "The cable length test can only be performed on copper cables. Do not perform this test on combo ports when fiber-optic cables are connected" },
    { RT_ERR_END,                   SYS_ERR_PORT_EEE_NOT_FIBER,       "errFiberNoEEE",         "Fiber port do not support EEE function" },
    { RT_ERR_END,                   SYS_ERR_PORT_NOT_FIBER,       "errNotFiberPort",         "This is not fiber port" },
    { RT_ERR_END,                   SYS_ERR_PORT_AUTO_FLOW_NOT_ALLOW_COMBO_FIBER,  "errPortNoFlowCtrl", "Flow Control AutoNegotiation is not supported on combo or fiber ports" },

#ifdef CONFIG_SYS_PORT_SECURITY
    
    { RT_ERR_END,                   SYS_ERR_PSECURE_MAX_LESS_CURRENT,  "errSecurityLess",      "Maximum is less than number of currently secured mac-addresses" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_MAC_NOT_EXIST,    "errSecurityMAC",       "Secure MAC address not exist" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_MAC_FULL,         "errSecurityMACFull",       "Total secure mac-addresses on port has reached maximum limit" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_TRUNK,            "errSecuritTrunk",       "Port belongs to a LAG" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_TYPE_INVALID,     "errSecurityType",       "Invalid port security mac address type" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_AZERO_INVALID,     "errSecurityAzero",      "Secure MAC address cannot be all zero MAC address" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_EQ_SYSMAC,       "errSecuritySYSMAC",        "Secure MAC address cannot be system MAC address" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_BE_MCAST,        "errSecurityMCAST",        "Secure MAC address cannot be multicast MAC address" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_MAC_CONFIT_WITH_PSECURE, "errSecurityMACConfit", "Mac entry exist in port security table" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_FORBID_STATIC,        "errSecurityForbid",   "Cannot add static address on port because port security is enabled" },
    { RT_ERR_END,                   SYS_ERR_PSECURE_PORT_STATIC_MAC_EXIST, "errSecurityStaticMac",   "Port has static address" },
#endif

    { RT_ERR_END,                   SYS_ERR_QOS_MODE,               "errQosMode",    "Currently not in QoS mode" },
    { RT_ERR_END,                   SYS_ERR_QOS_MODE_BASIC,        "errQosNoBasic",     "Currently not in QoS basic mode" },
    { RT_ERR_END,                   SYS_ERR_QOS_PRI_INVALID,        "errQosPriVal",    "Invalid priority value" },
    { RT_ERR_END,                   SYS_ERR_QOS_WEIGHT_INVALID,     "errQosInvalidVal",    "Invalid queue weight value" },
    { RT_ERR_END,                   SYS_ERR_QOS_COS_INVALID,        "errQosInvalidCos",    "Invalid CoS value" },
    { RT_ERR_END,                   SYS_ERR_QOS_DSCP_INVALID,       "errQosInvalidDscp",    "Invalid DSCP value" },
    { RT_ERR_END,                   SYS_ERR_QOS_IPP_INVALID,        "errQosPriVal",    "Invalid IP Precedence value" },
    { RT_ERR_END,                   SYS_ERR_QOS_QUEUE_INVALID,     "errQosInvalQueId",     "Invalid queue id" },
    { RT_ERR_END,                   SYS_ERR_QOS_REMARK_CONFLICT,   "errQosRemarkCon",     "DSCP and IP precedence remarking state conflict" },

    { RT_ERR_RATE,                  SYS_ERR_RATE_OUT_OF_RANGE,                 "errRateRange",    "Rate invalid: out of range" },
    { RT_ERR_END,                   SYS_ERR_RATE_NOT_MULTIPLE_OF_16,            "errRateInvalidMul",   "Rate invalid: must be a multiple of 16" },
    { RT_ERR_END,                   SYS_ERR_RATE_BURSTSIZE_NOT_MULTIPLE_OF_128, "errRateMultiBur",   "BurstSize invalid: must be a multiple of 128" },
    { RT_ERR_END,                   SYS_ERR_RATE_BURSTSIZE_NOT_MULTIPLE_OF_1024,"errRateBurMulti",   "BurstSize invalid: must be a multiple of 1024" },

    { RT_ERR_END,                   SYS_ERR_SNMP_STR,            "errSnmpInvalStr",           "Invalid snmp string"},
    { RT_ERR_END,                   SYS_ERR_SNMP_TARGET_FULL,    "errSnmpTableFull",       "Trap host table is full" },
    { RT_ERR_END,                   SYS_ERR_SNMP_COMMUNITY_FULL,  "errSnmpConmmIsFull",    "Community table is full" },
    { RT_ERR_END,                   SYS_ERR_SECURITY_LEVEL,       "errSnmpSecLelErr",      "Security level is error"},
    { RT_ERR_END,                   SYS_ERR_TRAP_TYPE,           "errSnmpNotiErr",          "Notifaction type is error"},
    { RT_ERR_END,                   SYS_ERR_SNMP_INFORM,         "errSnmpNotiMatch",        "Notifaction type inform can not  match the version 1(default version is 1)"},
    { RT_ERR_END,                   SYS_ERR_SNMP_COMMUNITY_NAME,   "errSnmpNameCommErr",   "SNMP communtiy name is error"},
    #ifdef CONFIG_SYS_SNMPV3
    
    { RT_ERR_END,                   SYS_ERR_SNMP_VIEW_FULL,      "errSnmpV3ViewFul",       "view table is full" },
    { RT_ERR_END,                   SYS_ERR_SNMP_VIEW_DEFAULT,   "errSnmpV3DefaultView",    "view name is default view, can not add and delete" },
    { RT_ERR_END,                   SYS_ERR_SNMP_VIEW_NAME,       "errSnmpV3SpecialName",     "special view name, can not be added"},
    { RT_ERR_END,                   SYS_ERR_SNMP_GROUP_FULL,       "errSnmpV3SecurityFull",     "security group table is full" },
    { RT_ERR_END,                   SYS_ERR_SNMP_ACCESS_FULL,      "errSnmpV3AccessFull",     "access group table is full" },
    { RT_ERR_END,                   SYS_ERR_SNMP_ACCESS_GROUP_NAME, "errSnmpV3GroupName", "SNMP group name error"},
    { RT_ERR_END,                   SYS_ERR_SNMP_USER_FULL,     "errSnmpV3TableFull",        "user table is full" },
    { RT_ERR_END,                   SYS_ERR_SNMP_REMOTE_FULL,    "errSnmpV3RemoteTabl",     "remote engine table is full"},
    { RT_ERR_END,                   SYS_ERR_OID,                  "errSnmpV3DefaultViNa",      "Invalid oid" },
    { RT_ERR_END,                   SYS_ERR_OID_DFLT,            "errSnmpV3UseNaVi",            "Invalid use default view name" },
    { RT_ERR_END,                   SYS_ERR_OID_LEN,            "errSnmpV3InvalOid",        "Invalid oid length" },
    { RT_ERR_END,                   SYS_ERR_OID_MASK,           "errSnmpV3NoOidMask",     "Invalid oid mask"  },
    { RT_ERR_END,                   SYS_ERR_OID_MASK_LEN,        "errSnmpV3InvalidMaskLen",  "Invalid oid mask length" },
    { RT_ERR_END,                   SYS_ERR_COMNUNITY_NOT_FOUND, "errSnmpV3CommunFound",       "Community is not found" },
    { RT_ERR_END,                   SYS_ERR_GROUP_EXIST,           "errSnmpV3GrouExist",     "Group is exist" },
    { RT_ERR_END,                   SYS_ERR_GROUP_NOT_EXIST,       "errSnmpV3SecMoMath",     "Group does not exist or security mode not match"},
    { RT_ERR_END,                   SYS_ERR_GROUP_VIEW_CONFLICT,    "errSnmpV3AcceCon",    "Access group exist but the view conflict"},
    { RT_ERR_END,                   SYS_ERR_READ_VIEW_NOT_EXIST,    "errSnmpV3ReadExis",    "Read View does not exist"},
    { RT_ERR_END,                   SYS_ERR_READ_VIEW_MUST_EXIST,  "errSnmpV3MustAdd",    "Must add read view"},
    { RT_ERR_END,                   SYS_ERR_WRITE_VIEW_NOT_EXIST,   "errSnmpV3WritExi",    "Write View does not exist"},
    { RT_ERR_END,                   SYS_ERR_NOTIFY_VIEW_NOT_EXIST,   "errSnmpV3NotViNotEx",   "NotifyView does not exist"},
    { RT_ERR_END,                   SYS_ERR_ACCESS_GROUP_USED,      "errSnmpV3AcessGrCom",    "Acess group is used by community or user"},
    { RT_ERR_END,                   SYS_ERR_ENGINEID_ERR,          "errSnmpV3EngForErr",     "Engine id format is error"},
    { RT_ERR_END,                   SYS_ERR_ENGINEID_LEN_ERR,     "errSnmpV3EngHexad",   "EngineID must contain 10 through 64 hexadecimal digits, must divided by 2"},
    { RT_ERR_END,                   SYS_ERR_USER_NOT_FOUND,       "errSnmpV3UsNameNoFind",              "User name is not found"},
    { RT_ERR_END,                   SYS_ERR_SNMPv3_USERNAME_ERR,   "errSnmpV3UserPLErr",     "Username error" },
    { RT_ERR_END,                   SYS_ERR_SNMPv3_PASSWORD_ERR,   "errSnmpV3PassWokErr",     "Password error" },
    { RT_ERR_END,                   SYS_ERR_SNMPv3_AUTH_ERR,        "errSnmpV3AuthProNo",    "No authetication protocol" },
    { RT_ERR_END,                   SYS_ERR_SNMPv3_ENCRYPT_ERR,     "errSnmpV3EncryProhf",    "No encryption protocol" },
    #endif 

#ifdef CONFIG_SYS_PROTO_LLDP
    
  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    { RT_ERR_END,                   SYS_ERR_LLDP_DISABLE_ACTION_FLOODING_MSTP,    "errMstpActiFldWhe",   "Can not congiure LLDP disable action to flooding when MSTP is running"},
  #endif
    { RT_ERR_END,                   SYS_ERR_LLDP_HOLDTIME_MUL_TX_INTERVAL,        "errMstpInterMul",   "LLDP transmission interval multiplies holdtme multiplier should be smaller than 65535" },
    { RT_ERR_END,                   SYS_ERR_LLDP_TX_DELAY_TX_INTERVAL,            "errMstpDelayThan",   "LLDP transmission delay should not be larger than LLDP transmission interval * 0.25" },
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    { RT_ERR_END,                   SYS_ERR_LLDP_NETWORK_POLICY_NOT_EXIST,        "errMstpNetExi",   "Network policy does not exist"},
    { RT_ERR_END,                   SYS_ERR_LLDP_NETWORK_POLICY_DUPLICATE,       "errMstpTypeDup",    "Network policy application type duplicated"},
    { RT_ERR_END,                   SYS_ERR_LLDP_NETWORK_POLICY_BINDED,          "errMstpBindInter",    "Network policy binded to an interface can not be modified or removed"},
    { RT_ERR_END,                   SYS_ERR_LLDP_NETWORK_POLICY_VOICE_WHEN_AUTO, "errMstpNetConBec",    "A network policy of type Voice cannot be configured because the Voice VLAN is in automatic update mode" },
    { RT_ERR_END,                   SYS_ERR_LLDP_NETWORK_POLICY_AUTO_WHEN_VOICE,  "errMstpVoiceAu",   "Voice VLAN cannot be in automatic update mode because there is a network policy of type VOICE" },
    { RT_ERR_END,                   SYS_ERR_LLDP_LOCATION_FORMAT,                "errMstpParisHex",    "Invalid input. Input should be paris of hexadecimal value without \":\", ex: 1122AB" },
  #endif
#endif

#ifdef CONFIG_SYS_LIB_RADIUS
    
    { RT_ERR_END,                   SYS_ERR_RADIUS_SRV_ERR,        "errRadiusSerErr", "radius server error" },
    { RT_ERR_END,                   SYS_ERR_RADIUS_SRV_EMPTY,     "errRadiusSerTheNo",  "There is no radius server" },
    { RT_ERR_END,                   SYS_ERR_RADIUS_SRV_FULL,       "errRadiusOverSerNum", "Over max radius server number" },
    { RT_ERR_END,                   SYS_ERR_RADIUS_SRV_EXIST,     "errRadiusSerAlEx",  "Radius server already exist" },
    { RT_ERR_END,                   SYS_ERR_RADIUS_SRV_PRIORITY_EXIST, "errRadiusSetUniqueVal", "Setting unique priority value is needed" },
    { RT_ERR_END,                   SYS_ERR_RADIUS_AUTH_ERR,      "errRadiusAuthCatErr",  "Authentication error" },
#endif

    { RT_ERR_END,                   SYS_ERR_STP_HELLO_TIME,          "errstpHello",   "Invalid STP parameter of hello-time" },
    { RT_ERR_END,                   SYS_ERR_STP_FORWARD_DELAY,      "errstpforward",    "Invalid STP parameter of forward-delay" },
    { RT_ERR_END,                   SYS_ERR_STP_MAXIMUM_AGE,        "errstpage",    "Invalid STP parameter of maximum-age" },
    { RT_ERR_END,                   SYS_ERR_STP_TIMER_RELATIONSHIPS, "errstptimer",   "Wrong STP timer value. Timers should comply with the following formula:\n 2*(Forward-Delay - 1) >= Max-Age and Max-Age >= 2*(Hello-Time + 1)" },
    { RT_ERR_END,                   SYS_ERR_STP_BRIDGE_PRIORITY,     "errstpbridge",   "Invalid STP parameter of bridge priority, it must multiples of 4096" },
    { RT_ERR_END,                   SYS_ERR_STP_PORT_PRIORITY,       "errstpPortpri",   "Invalid STP parameter of port priority, it must multiples of 16" },
    { RT_ERR_END,                   SYS_ERR_STP_CST_DEL,             "errstpCst",   "Not allowed to delete CST" },
    { RT_ERR_END,                   SYS_ERR_STP_CST_VLAN_DEL,        "errstpCstVlan",   "Not allowed to delete VLAN from CST" },
#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_STP_MSTP)
    { RT_ERR_END,                   SYS_ERR_STP_MSTP_LLDP_FLOODING,  "errstpMstpLldp",   "LLDP PDU Flooding Enabled prevents executing MSTP Enable" },
#endif
    { RT_ERR_END,                   SYS_ERR_STP_COST_SHORT_RANGE,    "errstpshort",   "Current Pathcost method is short,the valid max cost value is 65535" },

    { RT_ERR_END,                   SYS_ERR_LOG_SERVER_IP,           "errSyslogSerIp",   "Syslog server ip error"},
    { RT_ERR_END,                   SYS_ERR_LOG_SERVER_FULL,        "errSyslogRemoFul",    "Syslog remote host is full"},
    { RT_ERR_END,                   SYS_ERR_LOG_SERVER_EXIST,       "errSyslogNoExist",    "Syslog remote host is exist"},
    { RT_ERR_END,                   SYS_ERR_LOG_SERVER_NOT_EXIST,    "errSyslogHostNo",   "Syslog remote host does not exist"},
    { RT_ERR_END,                   SYS_ERR_LOG_SEVERITY_ERROR,     "errSyslogInputSev",    "Syslog input severity error"},
    { RT_ERR_END,                   SYS_ERR_LOG_CATEGORY_ERROR,     "errSyslogCateErr",    "Syslog input category error"},

#ifdef CONFIG_SYS_LIB_TACPLUS
    
    { RT_ERR_END,                   SYS_ERR_TACPLUS_SRV_ERR,     "errTacSerErr",   "tacacs+ server error" },
    { RT_ERR_END,                   SYS_ERR_TACPLUS_SRV_EMPTY,   "errTacNoSer",   "There is no tacacs+ server" },
    { RT_ERR_END,                   SYS_ERR_TACPLUS_SRV_FULL,    "errTacMaxSerNum",   "Over max tacacs+ server number" },
    { RT_ERR_END,                   SYS_ERR_TACPLUS_SRV_EXIST,   "errTacSerAlEx",   "Tacacs+ server already exist" },
    { RT_ERR_END,                   SYS_ERR_TACPLUS_SRV_PRIORITY_EXIST, "errTacSetPriVal", "Setting unique priority value is needed" },
#endif

    { RT_ERR_END,                   SYS_ERR_TFTP_DOWNLOAD_IMAGE,     "",   "Download firmware image from TFTP server failed"},
    { RT_ERR_END,                   SYS_ERR_TFTP_DOWNLOAD_IMAGE_MAGIC, "", "Wrong image"},
    { RT_ERR_END,                   SYS_ERR_TFTP_DOWNLOAD_CONFIG,    "",   "Download config file from TFTP server failed"},
    { RT_ERR_END,                   SYS_ERR_TFTP_UPLOAD_IMAGE,       "",   "Upload firmware image to TFTP server failed"},
    { RT_ERR_END,                   SYS_ERR_TFTP_UPLOAD_CONFIG,     "",    "Upload config file to TFTP server failed"},
    { RT_ERR_END,                   SYS_ERR_TFTP_FILE_NAME,          "",   "Image file name error"},
    { RT_ERR_END,                   SYS_ERR_TFTP_FILE_NOT_FOUND,     "",   "TFTP file not found"},
    { RT_ERR_END,                   SYS_ERR_TFTP_SERVER_IP,          "",   "Tftp server ip error"},
    { RT_ERR_END,                   SYS_ERR_TFTP_TIMEOUT,            "",   "Tftp server time out"},
    { RT_ERR_END,                   SYS_ERR_TFTP_UDP,               "",    "Tftp server use udp error"},
    { RT_ERR_END,                   SYS_ERR_TFTP_BCAST_NORESP,      "",    "Tftp broadcast without response"},

    { RT_ERR_END,                   SYS_ERR_CMD_AMBIGUOUS,          "",    "Ambiguous command" },
    { RT_ERR_END,                   SYS_ERR_CMD_INCOMPLETE,         "",    "Incomplete command" },
    { RT_ERR_END,                   SYS_ERR_CMD_NOT_MATCH,          "",    "Unknown command" },
    { RT_ERR_END,                   SYS_ERR_CMD_BAD_PARAM,          "",    "Bad parameter value" },
    { RT_ERR_END,                   SYS_ERR_CMD_SLAVE_PORT,         "",    "Trunk slave port can not be configured"  },
    { RT_ERR_END,                   SYS_ERR_CMD_TRUNK_MBR,          "",    "Trunk member port can not be configured" },
    { RT_ERR_END,                   SYS_ERR_CMD_WARNING,            "",    "Warning" },
    { RT_ERR_END,                   SYS_ERR_CMD_NOT_ALLOW,         "",     "Not authorized to execute this command" },
    { RT_ERR_END,                   SYS_ERR_CMD_PORTLIST,           "",    "% bad parameter value" },
    { RT_ERR_END,                   SYS_ERR_CMD_VALUE,               "",   "Invalid decimal or hex value, hex need '0x' prefix" },
    { RT_ERR_END,                   SYS_ERR_CMD_SEQUENCE_TOKEN,     "",    "Invalid sequence token" },
    { RT_ERR_END,                   SYS_ERR_CMD_RANGE_FORMAT,       "",    "Invalid range syntax" },
    { RT_ERR_END,                   SYS_ERR_CMD_RANGE,              "",    "Out of range" },
    { RT_ERR_END,                   SYS_ERR_CMD_IPMASK,            "",     CMD_IPMASK_STR },
    { RT_ERR_END,                   SYS_ERR_CMD_MACMASK,            "",    CMD_MACMASK_STR },
    { RT_ERR_END,                   SYS_ERR_PORT_STR_FMT,          "",     "% bad parameter value" },

    { RT_ERR_LA_TRUNK_ID,           SYS_ERR_TRUNK_INVALID,         "errTrunkid",     "Invalid trunk ID" },
    { RT_ERR_LA_PORTMASK,           SYS_ERR_TRUNK_MBR,            "errTrunkPorts",      "Invalid trunk member ports" },
    { RT_ERR_LA_HASHMASK,           SYS_ERR_TRUNK_HASHMASK,        "errTrunkHash",     "Invalid trunk hash algorithm" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_LACP_GROUP,       "errTrunkGroup",    "Can not add static trunk member to LACP trunk" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_LESS_THAN_2_PORTS, "errTrunkLess",   "2 trunk member ports at least" },
    { RT_ERR_LA_PORTNUM_DUMB,       SYS_ERR_TRUNK_MORE_THAN_4_PORTS,  "errTrunkMore4",  "4 trunk member ports at most" },
    { RT_ERR_LA_PORTNUM_NORMAL,     SYS_ERR_TRUNK_MORE_THAN_8_PORTS,  "errTrunkMore8",  "8 trunk member ports at most" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_MORE_THAN_16_PORTS, "errTrunkMore16",  "16 trunk member ports at most" },
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_HAS_AUTH,       "errTrunkAuth", "Port should not be authentication enabled" },
#endif
    { RT_ERR_LA_MEMBER_OVERLAP,     SYS_ERR_TRUNK_PORT_USED_BY_TRK,    "errTrunkAnother", "Port already in another trunk" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_VLAN_CONFLICT,  "errTrunkVlan", "Joined VLAN/Tag of trunk member ports are different" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_MIRRORING,      "errTrunkMirror", "Mirroring port cannot be a trunk member port" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_RATE_LIMITED,  "errTrunkRate",  "Port should not be ingress/egress rate limited" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_NEGO,          "errTrunkNego",  "Trunk member ports must all have the same negotiation mode" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_SPEED,         "errTrunkSpeed",  "Trunk member ports must all have the same speed" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_DUPLEX,         "errTrunkDuplex", "Trunk member ports must all be full duplex" },
    { RT_ERR_LA_TRUNK_NOT_EXIST,    SYS_ERR_TRUNK_TRK_NO_EXIST,        "errTrunkNoExist", "Trunk does not exist" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_BACKUPPORT_USED_BY_TRK, "errTrunkBackupPort", "Port already used as trunk member" },
#ifdef CONFIG_SYS_PORT_SECURITY
    { RT_ERR_END,                   SYS_ERR_TRUNK_PORT_HAS_PORT_SECURITY,  "errTrunkSecurity", "Port should not be port security enabled" },
#endif
    { RT_ERR_END,                   SYS_ERR_TRUNK_EMPTY,             "errTrunkEmpty",   "No member ports in trunk" },
    { RT_ERR_END,                   SYS_ERR_TRUNK_MEMBER_SKIP,        "",  "Skip configuring on trunk member " },

#ifdef CONFIG_SYS_PROTO_UDLD
    
    { RT_ERR_END,                   SYS_ERR_UDLD_AGSV,                    "",           "UDLD is globally enabled in aggressive mode" },
    { RT_ERR_END,                   SYS_ERR_UDLD_NOT_AGSV,                 "",          "UDLD is not globally enabled in aggressive mode" },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_NML_NOT,            "",           "UDLD is not enabled on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_NML_BUT_AGSV,       "",           "UDLD is in aggressive mode on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_NML_BUT_DBL,        "",           "UDLD is disabled (not enabled) on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_AGSV_NOT,           "",           "UDLD is not in aggressive mode on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_AGSV_BUT_NML,       "",           "UDLD is enabled (not in aggressive mode) on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_AGSV_BUT_DBL,       "",           "UDLD is disabled (not in aggressive mode) on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_DBL_NOT,           "",            "UDLD is not disabled on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_DBL_BUT_NML,        "",           "UDLD is enabled (not disabled) on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_DBL_BUT_AGSV,        "",          "UDLD is in aggressive mode (not disabled) on this port." },
    { RT_ERR_END,                   SYS_ERR_UDLD_PORT_NOT_COMBO_FIBER,     "",          "UDLD can be configured to follow global configuration only on combo or fiber port" },
    { RT_ERR_END,                   SYS_ERR_UDLD_ONLY_FIBER_SUPPORT,       "",          "UDLD port disable command only supports fiber or combo ports" },
#endif

    { RT_ERR_END,                   SYS_ERR_VLAN_DFLTVLAN_DEL,           "errVlanDef",   "Not allowed to delete Default VLAN" },
    { RT_ERR_VLAN_EXIST,            SYS_ERR_VLAN_EXIST,                 "errVlanExists",    "VLAN exists" },
    { RT_ERR_END,                   SYS_ERR_VLAN_FRAMETYPE,              "errVlanFrame",   "Invalid VLAN frame type" },
    { RT_ERR_VLAN_VID,              SYS_ERR_VLAN_ID,                    "errVlanID",    "VID is out of range" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VID_MGMTVLAN,          "errVlanMGMT",    "A Management VLAN can not be configured as Default VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_MEMBERSHIP,            "errVlanMemvership",    "VLAN membership error" },
    { RT_ERR_END,                   SYS_ERR_VLAN_MGMTVLAN_DEL,          "errVlanMGMTDel",    "Not allowed to delete Management VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_MGMTVLAN_EMPTY,        "errVlanMGMTempty",    "Management VLAN can not be created because member port is empty" },
    { RT_ERR_END,                   SYS_ERR_VLAN_NAME_NULL,            "errVlanNameFull",     "Bad VLAN name length" },
    { RT_ERR_END,                   SYS_ERR_VLAN_NAME_EXIST,           "errVlanNameExist",     "VLAN name exists" },
    { RT_ERR_VLAN_ENTRY_NOT_FOUND,  SYS_ERR_VLAN_NOT_EXIST,             "errVlanNotExist",    "VLAN does not exist" },
    { RT_ERR_END,                   SYS_ERR_VLAN_NOT_STATIC,             "errVlanNotStatic",   "VLAN is not static" },
    { RT_ERR_END,                   SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER, "errVlanNotCreated",  "VLAN was not created by user" },
    { RT_ERR_END,                   SYS_ERR_VLAN_TRKMBR_PORT,             "errVlanTrunk",  "Port belongs to a port-channel" },
#ifdef CONFIG_SYS_VLAN_QINQ
    { RT_ERR_END,                   SYS_ERR_VLAN_PORTMODE_NOT_UPLINK,    "errVlanuplink",   "Port mode is not uplink port" },
#endif
    { RT_ERR_END,                   SYS_ERR_VLAN_MVR_VID_DEL,            "errVlanMVRDel",   "MVR VLAN Can not be allowed to destory"},
    { RT_ERR_END,                   SYS_ERR_VLAN_NUM_NOT_SUPPORT,        "errVlanNotSupport",   "VLAN over max entry number" },

#ifdef CONFIG_SYS_VOICEVLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_VID_VOICEVLAN,          "errVoiceID",    "A Voice VLAN can not be configured as Default VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_EXIST,    "errVoiceOUIexist",    "Voice VLAN OUI entry is exist in database" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_MCAST,    "errVoiceOUImcast",    "OUI MAC must be unicast" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_ZERO,    "errVoiceOUIzero",     "Voice VLAN OUI entry is all zero " },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT,    "errVoiceauto",    "Voice VLAN auto mode port can not be edited in Voice VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_PORT,     "errVoiceOUIport",    "Voice VLAN State is not OUI-enabled"},
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_PORT_MODE,    "errVoiceportMode",    "Port must be in hybrid or trunk mode"},
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_PORT_STATIC,     "errVoicePortStaic", "static member cannot be a candidate in a Voice VLAN"},
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_ENABLED_PORT,"errVoiceOUIenable", "Voice VLAN  has enabled ports "},
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_OUI_DFLT_VID,    "errVoiceOUIdef", "A Default VLAN can not be configured as Voice VLAN in OUI mode" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_VID_DEL,         "errVoiceVidDel", "Voice VLAN Can not be allowed to delete" },
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_GUEST_VLAN,     "errVoiceGustVlan",  "A guest VLAN can not be configured as voice VLAN"},
  #endif
  #ifdef CONFIG_SYS_SURVEILLANCEVLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_SURVEILLANCE_VLAN,"errVoiceSurveillance", "A surveillance VLAN can not be configured as voice VLAN" },
  #endif
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_VID_NOT_EXIST,   "errVoiceVidNotExist", "Can't enable voice VLAN becasue voice VLAN ID isn't assgined"},
    { RT_ERR_END,                   SYS_ERR_VLAN_VOICEVLAN_VID_ENABLE,      "errVoiceVidEnable", "Can't restore voice VLAN ID to none becasue voice VLAN is enabled"},
#endif

#ifdef CONFIG_SYS_PROTOCOL_VLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_PROVLAN_PORT_REF,         "errProvlanPort",  "Protocol VLAN : the Protocol VLAN is configured in the port" },
    { RT_ERR_END,                   SYS_ERR_VLAN_PROVLAN_PORT_MODE,        "errProvlanPortMode",  "Port must be in hybrid Mode" },
    { RT_ERR_END,                   SYS_ERR_VLAN_PROVLAN_ENABLED_PORT,      "errProvlanEnablePort", "Port has bound Protocol VLAN Group" },
#endif

#ifdef CONFIG_SYS_MACVLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_MAX,                 "errVlanMac",  "Number of ranges exceeds the maximum. " },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_NOT_UNI,             "errVlanMacNotUni",  "Range contains Multicast addresses." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_OVERLAP,            "errVlanMacOverlap",   "The range or part of it is already defined in the database." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_LAST_DEL,           "errVlanMacLastDel",   "Cannot remove last range from mapped group." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_NO_RESOURCE,         "errVlanMacNoResource",  "There are no resources for this range." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_NO_GID,             "errVlanMacNoGid",   "Attempting to map illegal group number." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_GENERAL_MODE,       "errVlanMacGeneral",   "Port mode is not hybrid." },
    { RT_ERR_END,                   SYS_ERR_VLAN_MAC_PORT_MODE,          "errVlanMacPortMode",   "Enable MAC VLAN port must be in hybrid mode" },
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_OUI_MCAST,           "errSVOUIMcast",   "OUI MAC must be unicast" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_OUI_EXIST,           "errSVOUIExist",   "Surveillance VLAN OUI entry is exist in database" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_OUI_ZERO,            "errSVOUIzero",   "Surveillance VLAN OUI can't be all zero" },
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_EQ_GUEST,            "errSVGuest",   "A Guest VLAN can not be configured as Surveillance VLAN" },
  #endif
  #ifdef CONFIG_SYS_VOICEVLAN
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_EQ_VOICE,            "errSVVoice",   "A Voice VLAN can not be configured as Surveillance VLAN" },
  #endif
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_EQ_DEFAULT,          "errSVDefailt",   "A Default VLAN can not be configured as Surveillance VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_NOT_EXIST,           "errSVNotExist",   "Can't enable Surveillance VLAN becasue Surveillance VLAN ID isn't assgined" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_VID_ENABLE,          "errSVVidEnable",   "Can't restore Surveillance VLAN ID to none becasue Surveillance VLAN is enabled" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_ENABLED_PORT,        "errSVEnablePort",   "Surveillance VLAN has enabled ports" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_PORT_MODE,           "errSVPortMode",   "Port must be in hybrid or trunk mode" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_PORT_STATIC,         "errSVPortStatic",   "Port can't be static member port of a Surveillance VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_GLOBAL_DISABLE,      "errSVGlobalDisable",   "Surveillance VLAN global state is not enabled" },
    { RT_ERR_END,                   SYS_ERR_VLAN_VID_SURVEILLANCEVLAN,   "errVidSV",   "A Surveillance VLAN can not be configured as Default VLAN" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_VID_DEL,             "errSVVidDel",   "Surveillance VLAN Can not be allowed to destory" },
    { RT_ERR_END,                   SYS_ERR_VLAN_SV_USED_PORT,           "errSVUsedPort",   "Port in surveillance VLAN auto mode can not be edited in VLAN" },
#endif

#ifdef CONFIG_SYS_PROTO_GVRP
    
    { RT_ERR_END,                   SYS_ERR_GVRP_NOT_ENABLE,           "errGvrpNoenable", "GVRP global not enable"},
    { RT_ERR_END,                   SYS_ERR_GVRP_DB_FULL,             "errGvrpDB",  "GVRP vlan db is full"},
    { RT_ERR_END,                   SYS_ERR_GVRP_PORT_NOT_TRUNK,     "errGvrpportNoTrunk",   "Port mode is not trunk, can't enable gvrp"},
    { RT_ERR_END,                   SYS_ERR_GVRP_MIRROR_TARGET,       "errGvrpMirror",    "Mirror destination port can not enable gvrp"},
    { RT_ERR_END,                   SYS_ERR_GVRP_PORT_MODE,           "errGvrpPortmode",       "GVRP Port enable must be in trunk mode"},
    { RT_ERR_END,                   SYS_ERR_GVRP_TIMER_DIV,           "errGvrpTimerDIV",      "GVRP Timer value must device by 5" },
    { RT_ERR_END,                   SYS_ERR_GVRP_TIMER_JOIN,           "errGvrpTimerJoin",   "GVRP join timer must less than half of leave timer"},
    { RT_ERR_END,                   SYS_ERR_GVRP_TIMER_LEAVE,        "errGvrpTimerLeave",   "GVRP leave timer must larger than tiwce of join time and less than leaveall time"},
    { RT_ERR_END,                   SYS_ERR_GVRP_TIMER_LEAVEALL,   "errGvrpTimerLeaveall", "GVRP leaveall timer must larger than leavetime"},
#endif

#ifdef CONFIG_SYS_MGMT_ACL
    
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NODORM,         "errMGMTACLNodorm",   "Cannot modify/delete active management access list" },
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NODOADD,        "errMGMTACLNodoadd",   "Cannot modify active management access list" },
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NOSUCHLIST,     "errMGMTACLNoSuchlish",   "No such management access list"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NOALLLIST,      "errMGMTACLNoALLlist",   "No such management access list"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACE_PRIEXCEED,       "errMGMTACLPriexceed",  "Value is out of range"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_EXISTSUCHACE,    "errMGMTACLexists",  "Entry already exists"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACE_NOPRi_PRIEXCEED, "errMGMTACLNopri",  "With no explicit priority input ,value is out of range,you can continue input with sequence command"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NOSUCHACE,       "errMGMTACLNoSuchace",  "Cannot remove unexisting management access entry" },
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_ENTRY_FULL,      "errMGMTACLentry",  "Cannot create new entry in mgmt acl table since the table is full"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_PORT_WRONG,      "errMGMTACLportWrong",  "Wrong or no port set"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_SOURCEIP_WRONG,  "errMGMTACLsourceIP",  "Source ip should be unicast"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_SOURCVID_WRONG,  "errMGMTACLsourceVid",  "Wrong vid of source set"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_CONSOLE_REPEAT, "errMGMTACLconsole",   "Input profile rule name mustn't  be Console-only,console-only,Console only or console only"},
    { RT_ERR_END,                   SYS_ERR_MGMT_ACL_NAME_EMPTY,      "errMGMTACLname",  "Please specify a list name" },
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	{ RT_ERR_END,                   SYS_ERR_DHCP_LEASE_MAX_TIME,    	"errLMT",              "Lease time can not exceed 999 days!"},
    { RT_ERR_END,                   SYS_ERR_DHCP_LEASE_TIME_EMPTY,      "errLTE",               "Lease time can not be 0 !"},
    { RT_ERR_END,                   SYS_ERR_DHCP_LEASE_TIME_ERR,        "errLTER",               "Error date input!"},
    { RT_ERR_END,                   SYS_ERR_DHCP_SECTION_STARTIP_INVALID,   "errDSSI",          "Start ip is invalid"},
	{ RT_ERR_END,					SYS_ERR_DHCP_SECTION_ENDIP_INVALID,		 "errDEI",         "End ip is invalid"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_STARTIP_HIGHER,   "errDSSH",           "Start ip can not be larger than end ip"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_LENGTH_TOOBIG,    "errDSLT",           "There are only 1024 ip addresses in a section."},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_OVERLAP,        "errDSO",             "IP addresses conflict with another section"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_NUM_FULL,        "errDSNF",            "Section index is out of range,you can configure no more than 256 sections"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_IPGATE_NOSAMESUBNET,  "errDSIN",        "The ip address and gateway ip are not in the same subnet."},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_NO_GATEWAY,        "errDSNG",            "No gateway ip configured"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW,      "errDSNCG",            "There should not be any gateway ip inside a section"},	
	{ RT_ERR_END,                   SYS_ERR_DHCP_STATIC_IP_INVALID,        "errDSII",            "This ip does not exist in the ip address pool"},
	{ RT_ERR_END,                   SYS_ERR_DHCP_SECTION_IP_INUSED,        "errDSIM",            "This ip is in use,current pool can not be modified"},
	{ RT_ERR_END, 					SYS_ERR_DHCP_IP_POOL_FULL, 			   "errDSEntryFull",	 "The ip pool database is full (maximum 128)" },
#endif

	{ RT_ERR_END,					SYS_ERR_OSPF_AREA_NETWORK_EXIST,		"errOSPFNWexist",			   "Error: Please delete the network command(s) of the area first."},
	{ RT_ERR_END,					SYS_ERR_OSPF_IS_CLOSE,		       "errOSPFIsClose",			   "Error: Please enable OSPF function first."},
	{ RT_ERR_END,					SYS_ERR_OSPF_INTERFACE_NOT_EXIST,    "errOspfif",		   "The ospf interface does not exist."},
	{ RT_ERR_END,					SYS_ERR_OSPF_AREA_IS_BACKBONE,    "errOspfvf",		   "The Can't configure it to backbone area."},
	{ RT_ERR_END,					SYS_ERR_OSPF_AREA_NOT_DEFAULT,    "errOspfvf",		   "Area is a stub or nssa so virtual links are not allowed"},
	{ RT_ERR_END,					SYS_ERR_RIP_IS_CLOSE,			   "errRipIsClose",			   "Error: Please enable RIP function first."},

#ifdef CONFIG_SYS_L3_VRRP
	{ RT_ERR_END,                   SYS_ERR_VRRP_CFGERR_SUBNETDIFF,    "errVrrpSubif",         "The virtual IP is not in the same subnet."},
    { RT_ERR_END,                   SYS_ERR_VRRP_CFGERR_MAXVSRV,       "errVrrpMacsrv",        "The amount of virtual router has arrived the maximum."},
    { RT_ERR_END,                   SYS_ERR_VRRP_CFGERR_MAXVIP,        "errVrrpMacvip",        "The amount of virtual IP has arrived the maximum."},
#endif
    
    { RT_ERR_UNIT_ID,               SYS_ERR_DEV_ID,                "",     "Invalid dev id" },
    { RT_ERR_END,                   SYS_ERR_DEV_OPEN,               "",    "Device open error" },
    { RT_ERR_END,                   SYS_ERR_DEV_CLOSE,              "",    "Device close error" },
    { RT_ERR_END,                   SYS_ERR_DEV_READ,               "",    "Device read error" },
    { RT_ERR_END,                   SYS_ERR_DEV_WRITE,             "",     "Device write error" },
    { RT_ERR_END,                   SYS_ERR_FILE_CREATE,            "",    "File create error" },
    { RT_ERR_END,                   SYS_ERR_FILE_OPEN,             "",     "File open error" },
    { RT_ERR_END,                   SYS_ERR_FILE_READ,               "",   "File read error" },
    { RT_ERR_END,                   SYS_ERR_FILE_SEEK,              "",    "File seek error" },
    { RT_ERR_END,                   SYS_ERR_FILE_SIZE,             "",     "File size error" },
    { RT_ERR_END,                   SYS_ERR_FILE_WRITE,            "",     "File write error" },
    { RT_ERR_END,                   SYS_ERR_FILE_NOT_EXIST,        "",     "File not exist" },
    { RT_ERR_END,                   SYS_ERR_INFO_BADCRC,           "",     "INFO partition CRC error" },
    { RT_ERR_END,                   SYS_ERR_INFO_NOT_EXIST,        "",     "Variable does not exist in INFO partition" },
    { RT_ERR_END,                   SYS_ERR_IOCTL,                 "",     "IOCTL error" },
    { RT_ERR_END,                   SYS_ERR_KSI_CMD_NOT_FOUND,     "",     "Execute an empty ksi command" },
    { RT_ERR_END,                   SYS_ERR_KSI_DATA_EMPTY,       "",      "No data in ksi message" },
    { RT_ERR_END,                   SYS_ERR_MODULE_NOT_INIT,       "",     "The module is not initial" },
    { RT_ERR_NULL_POINTER,          SYS_ERR_NULL_POINTER,          "",     "NULL Pointer" },
    { RT_ERR_SEM_LOCK_FAILED,       SYS_ERR_SEM_LOCK,              "",     "Sem lock failed" },
    { RT_ERR_SEM_UNLOCK_FAILED,     SYS_ERR_SEM_UNLOCK,            "",     "Sem locked failed" },
    { RT_ERR_END,                   SYS_ERR_SOCKET,                "",     "Socket error" },	
	{ RT_ERR_END,					SYS_ERR_MAC_VLAN_GROUP_ID, 	    "errMacVlanId",  		"Group idx is exist" },
	{ RT_ERR_END,					SYS_ERR_MAC_ADDRESS_INVALID,    "errInvalidMacAddress",  "Mac address is invalid" },
    
    { RT_ERR_END, SYS_ERR_END, NULL}
};

#ifdef CONFIG_SYS_BUILD_RELEASE
static char unspecifiedErrStr[CAPA_NAME_STR_LEN];
#endif
char *sys_errStr_ret(int32 sysErr)
{
    ski_sysErr_t *ptr = NULL;

    SYS_PARAM_CHK(IS_SYS_ERR_INVALID(sysErr), (char *)"Unspecified error(0xFFFF)");

#ifdef CONFIG_SYS_BUILD_RELEASE
    if ((sysErr > SYS_ERR_INTERNAL_START) && (sysErr < SYS_ERR_INTERNAL_END))
    {
        osal_sprintf(unspecifiedErrStr, "Unspecified error(0x%X)", sysErr);
        return unspecifiedErrStr;
    }
#endif

    for (ptr = sysError; ptr->errStr; ptr++)
    {
        if (ptr->sys_err == sysErr)
            return (ptr->errStr);
    }

    return ((char*) "Unspecified error(0xFFFE)");
}
int32 sys_errXlate_ret(int32 rtErr)
{
    ski_sysErr_t *ptr = NULL;
    
    SYS_PARAM_CHK(IS_RT_ERR_INVALID(rtErr), SYS_ERR_OUT_OF_RANGE);
    for (ptr = sysError; ptr->errStr; ptr++)
    {
        if (ptr->rt_err == rtErr)
            return (ptr->sys_err);
    }
    SYS_PRINTF("unknown rt error code %x\n", rtErr);

    return SYS_ERR_UNKNOWN;
}
char *sys_errDes_ret(int32 sysErr)
{
    ski_sysErr_t *ptr = NULL;

    SYS_PARAM_CHK(IS_SYS_ERR_INVALID(sysErr), (char *)"Unspecified error(0xFFFF)");

#ifdef CONFIG_SYS_BUILD_RELEASE
    if ((sysErr > SYS_ERR_INTERNAL_START) && (sysErr < SYS_ERR_INTERNAL_END))
    {
        osal_sprintf(unspecifiedErrStr, "Unspecified error(0x%X)", sysErr);
        return unspecifiedErrStr;
    }
#endif

    for (ptr = sysError; ptr->codeDes; ptr++)
    {
        if (ptr->sys_err == sysErr)
            return (ptr->codeDes);
    }

    return ((char*) "Unspecified error(0xFFFE)");
}

