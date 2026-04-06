#ifndef __COMMON_SYS_ERROR_H__
#define __COMMON_SYS_ERROR_H__
#include <common/rt_error.h>
#include <common/sys_autoconf.h>
#define SYS_ERR_STR(errCode)        sys_errStr_ret(errCode)
#define SYS_ERR_DES(errCode)        sys_errDes_ret(errCode)
typedef struct ski_sysErr_s
{
    int32   rt_err;
    int32   sys_err;
    char	*codeDes;
    char    *errStr;
} ski_sysErr_t;
typedef enum sys_err_code_e
{
    SYS_ERR_FAILED = -1,                             
    SYS_ERR_OK = 0,                                  
    SYS_ERR_COMMON_START,                            
    SYS_ERR_ACT_NOT_ALLOWED,                         
    SYS_ERR_ACT_NOT_FINISH,                          
    SYS_ERR_BUF_SIZE,
    SYS_ERR_CHIP_NOT_SUPPORTED,                      
    SYS_ERR_CUSTOM_CODE,                             
    SYS_ERR_DROP_PRECEDENCE,                         
    SYS_ERR_DSCP_ID,                                 
    SYS_ERR_DUPLICATE_ENTRY,
    SYS_ERR_DUPLICATE_NAME,
    SYS_ERR_ENTRY_INDEX,                             
    SYS_ERR_ENTRY_NOT_FOUND,                         
    SYS_ERR_ENTRY_IS_EXIST,                          
    SYS_ERR_ENTRY_FULL,                              
    SYS_ERR_ETHER_TYPE,                              
    SYS_ERR_FIBER_CONFIG_PHY,
    SYS_ERR_FIBER_NOT_SUPPORT_10_100,
    SYS_ERR_GATEWAY_NOT_FOUND,
    SYS_ERR_INPUT,                                   
    SYS_ERR_IPV4_ADDR,                               
    SYS_ERR_IPV4_SUBNET,                             
    SYS_ERR_IPV4_GATEWAY_MISMATCH,                   
    SYS_ERR_IPV6_ADDR,                               
    SYS_ERR_IPV6_GATEWAY_MISMATCH,                   
    SYS_ERR_HOSTNAME,                                
    SYS_ERR_MAC,                                     
    SYS_ERR_OUT_OF_MEMORY,                           
    SYS_ERR_OUT_OF_RANGE,                            
    SYS_ERR_RANGE,                                   
    SYS_ERR_PORT_ID,                                 
    SYS_ERR_PRIORITY,                                
    SYS_ERR_QUEUE_ID,                                
    SYS_ERR_SNTP_UPDATE_FAIL,
    SYS_ERR_STRING,
    SYS_ERR_TIME_DLS_RANGE_ERR,
    SYS_ERR_TIME_DLS_RANGE_OFFSET_ERR,
    SYS_ERR_STR_LEN_EMPTY,
    SYS_ERR_STR_LEN_EXCEED,                          
    SYS_ERR_TCPUDP_PORT,                             
    SYS_ERR_TIMEOUT,                                 
    SYS_ERR_UNKNOWN_HOST,
    SYS_ERR_CFG_SYNC_OK,                             
    SYS_ERR_CFG_SYNC_FAILED,                         
    SYS_ERR_CFG_EXTNAME,                             
    SYS_ERR_CFG_INVALID,                             
    SYS_ERR_IMAGE_INVALID,                           
    SYS_ERR_IMAGE_MAGIC_INVALID,                     
    SYS_ERR_IMAGE_ERASE_ACTIVE,                      
    SYS_ERR_IMAGE_OUT_OF_MEMORY,                     
    SYS_ERR_IMAGE_EXTNAME,                           
    SYS_ERR_IMAGE_PROCESSING,                        
    SYS_ERR_LOCALUSER_FULL,
    SYS_ERR_LOCALUSER_NOT_EXIST,
    SYS_ERR_LOCALUSER_EMPTY,
    SYS_ERR_LOCALUSER_DEFAULT_DEL,                   
    SYS_ERR_LOCALUSER_CURRENT_DEL,					 
    SYS_ERR_USERNAME_INVALID,
    SYS_ERR_PASSWORD_INVALID,
    SYS_ERR_USERNAME_LENGTH_INVALID,
    SYS_ERR_PASSWORD_LENGTH_INVALID,
    SYS_ERR_USERNAME_ACCESSMETH,
    SYS_ERR_LAST_LOCALUSER,
    SYS_ERR_LOCALUSER_PASSWORD_LEN,                  
    SYS_ERR_DNS_RESOLVE,                             
	SYS_ERR_DNS_ENTRY_FULL,
    SYS_ERR_COMMON_END,                              
#ifdef CONFIG_SYS_AAA
    SYS_ERR_AAA_START,                               
    SYS_ERR_AAA_AUTHLIST_FULL,                       
    SYS_ERR_AAA_AUTHLIST_NOT_EXIST,                  
    SYS_ERR_AAA_AUTHLIST_EMPTY,
    SYS_ERR_AAA_HTTP_NOT_SUPPORTED_ENABLE,
    SYS_ERR_AAA_END,                                 
#endif
    SYS_ERR_ACL_START,                               
    SYS_ERR_ACL_PHASE_INVALID,                       
    SYS_ERR_ACL_BLK_AGGTYPE_INVALID,                 
    SYS_ERR_ACL_BLK_USING,                           
    SYS_ERR_ACL_BLK_ID_INVALID,                      
    SYS_ERR_ACL_BLK_FULL,                            
    SYS_ERR_ACL_TMPLTE_FULL,                         
    SYS_ERR_ACL_TMPLTE_ID_INVALID,                   
    SYS_ERR_ACL_FIELD_NOT_EXIST,                     
    SYS_ERR_ACL_RATE_ENTRY_ID_INVALID,               
    SYS_ERR_ACL_RATE_ENTRY_FULL,                     
    SYS_ERR_ACL_VLAN_RANGE_ENTRY_FULL,               
    SYS_ERR_ACL_L4_PORT_RANGE_CHK_FULL,              
    SYS_ERR_ACL_LOG_ENTRY_FULL,                      
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    SYS_ERR_ACL_ENTRY_EMPTY,                         
    SYS_ERR_ACL_ENTRY_EXISTS,                        
    SYS_ERR_ACL_ENTRY_PRIORITY,                      
    SYS_ERR_ACL_LIST_EMPTY,                          
    SYS_ERR_ACL_LIST_EXISTS,                         
    SYS_ERR_ACL_LIST_BINDING,                        
    SYS_ERR_ACL_EMPTY_BINDING,                       
    SYS_ERR_ACL_ILLEGAL_BINDING,                     
    SYS_ERR_ACL_INSUFF_PIEENTRY,                     
    SYS_ERR_ACL_DSCP_INVALID,                        
    SYS_ERR_ACL_TCPFLAG_INVALID,                     
    SYS_ERR_ACL_PRECEDENCE_INVALID,                  
    SYS_ERR_ACL_PORT_RANGE_INVALID,                  
    SYS_ERR_ACL_PORT_HAS_BINDED_TYPE,                
    SYS_ERR_ACL_PORT_HAS_BINDED_ACL,                 
    SYS_ERR_ACL_PORT_TRKMBR_BIND_ACL,                
    SYS_ERR_ACL_ENTRY_FULL,                          
    SYS_ERR_ACE_ENTRY_FULL,                          
    SYS_ERR_ACE_ENTRY_POOL_FULL,                     
#endif
    SYS_ERR_ACL_END,                                 
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_AUTH_START,                              
    SYS_ERR_AUTH_PORT_TRUNKMBR,                      
    SYS_ERR_DOT1X_VLAN_MODE,                         
    SYS_ERR_DOT1X_TRUNK_MODE,                        
    SYS_ERR_VLAN_VID_GUESTVLAN,                      
    SYS_ERR_DOT1X_GUEST_VLAN_DEL,                    
    SYS_ERR_DOT1X_GUEST_VLAN_RSPAN_VLAN,             
    SYS_ERR_DOT1X_GUEST_VLAN_VOICE_VLAN,             
    SYS_ERR_DOT1X_GUEST_VLAN_SURVEILLANCE_VLAN,      
    SYS_ERR_DOT1X_GUEST_VLAN_MGMT_VLAN,              
    SYS_ERR_DOT1X_GUEST_VLAN_PORT_VLAN_CONFIG,       
    SYS_ERR_DOT1X_GUEST_VLAN_PORT_PVID_CONFIG,       
    SYS_ERR_DOT1X_MGMT_VLAN_GUEST_VLAN,              
  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    SYS_ERR_WEBAUTH_WEBAUTH_MULTIHOST,               
    SYS_ERR_WEBAUTH_MULTIHOST_WEBAUTH,               
  #endif
    SYS_ERR_AUTH_END,                                
#endif
    SYS_ERR_CMD_START,                               
    SYS_ERR_CMD_AMBIGUOUS,
    SYS_ERR_CMD_COMPLETE_FULL_MATCH,
    SYS_ERR_CMD_COMPLETE_LIST_MATCH,
    SYS_ERR_CMD_COMPLETE_MATCH,
    SYS_ERR_CMD_EXEED_ARGC_MAX,
    SYS_ERR_CMD_INCOMPLETE,
    SYS_ERR_CMD_NOT_MATCH,
    SYS_ERR_CMD_BAD_PARAM,
    SYS_ERR_CMD_NOTHING_TODO,
    SYS_ERR_CMD_SLAVE_PORT,
    SYS_ERR_CMD_PORTLIST,
    SYS_ERR_CMD_SUCCESS_DAEMON,
    SYS_ERR_CMD_TRUNK_MBR,
    SYS_ERR_CMD_WARNING,
    SYS_ERR_CMD_NOT_ALLOW,
    SYS_ERR_CMD_VALUE,                               
    SYS_ERR_CMD_SEQUENCE_TOKEN,                      
    SYS_ERR_CMD_RANGE_FORMAT,                        
    SYS_ERR_CMD_RANGE,                               
    SYS_ERR_CMD_IPMASK,                              
    SYS_ERR_CMD_MACMASK,                              
    SYS_ERR_CMD_END,                                 
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    SYS_ERR_IGMP_START,                              
    SYS_ERR_IGMP_REMOVE_PORT_OK,
    SYS_ERR_IGMP_RSV_ADDR,
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    SYS_ERR_IGMP_MAX_GROUP_ACT,
#endif
    SYS_ERR_IGMP_QUERY_INTVL,                        

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    SYS_ERR_IGMP_QUERIER_CHANGE,                     
    SYS_ERR_IGMP_QUERIER_VERSION,
#endif
    SYS_ERR_IGMP_RESPONSE_TIME,                      
    SYS_ERR_IGMP_ROUTER_TIMEOUT,                     
    SYS_ERR_IGMP_LSTMBRQUERY_INTVL,                  
    SYS_ERR_IGMP_ROBUSTNESS_VAR,                     
    SYS_ERR_IGMP_STATIC_ENTRY_EXIST,
    SYS_ERR_IGMP_GROUP_ENTRY_NOT_EXIST,
    SYS_ERR_IGMP_UNKNOWN_ROUTERPORT_ACT,
    SYS_ERR_MCAST_GROUP_MAX,
    SYS_ERR_MCAST_GROUP_TYPE,
    SYS_ERR_MCAST_ADDR,
    SYS_ERR_MCAST_IPTYPE,
    SYS_ERR_MCAST_VLAN_NOT_EXIST,
    SYS_ERR_MCAST_DATABASE_FULL,

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    SYS_ERR_IGMP_PROFILE_ID,
    SYS_ERR_IGMP_PROFILE_TYPE,
    SYS_ERR_IGMP_PROFILE_USED,
    SYS_ERR_IGMP_PROFILE_NOT_EXIST,
    SYS_ERR_IGMP_PROFILE_EMPTY,
    SYS_ERR_MCAST_PROFILE_IPTYPE_NOT_MATCH,
#endif

    #ifdef CONFIG_SYS_PROTO_MVR
    SYS_ERR_MVR_ENABLE,
    SYS_ERR_MVR_TYPE,
    SYS_ERR_MVR_PORT_MODE,
    SYS_ERR_MVR_VLAN_PORT_TYPE,
    SYS_ERR_MVR_VLAN_PORT_SOURCE,
    SYS_ERR_MVR_VLAN_PORT_RECEIVER,
    SYS_ERR_MVR_STATIC_GROUP_CONFLICT,
    SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE,
    SYS_ERR_MVR_VLAN_STATIC_GROUP_SET,
    SYS_ERR_MVR_IGMP_STATIC_GROUP_SET,
    SYS_ERR_STATIC_GROUP_SET,
    #endif

    SYS_ERR_IGMP_END,                                
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    SYS_ERR_ISG_START,                               
    SYS_ERR_ISG_MAX_ENTRY_REACH,                     
    SYS_ERR_ISG_MAX_ENTRY_INVALID,                   
    SYS_ERR_ISG_DB_FULL,                             
    SYS_ERR_ISG_SAME_ENTRY,                          
    SYS_ERR_ISG_MULTI_INVALID,                       
    SYS_ERR_ISG_END,                                 
#endif
    SYS_ERR_L2_START,                                
    SYS_ERR_L2_MAC_CLEARTYPE_INVALID,                
    SYS_ERR_L2_MAC_CONFIT_WITH_STATIC,               
    SYS_ERR_L2_MAC_FULL,                             
    SYS_ERR_L2_MAC_NOT_EXIST,                        
    SYS_ERR_L2_MAC_IS_EXIST,                         
    SYS_ERR_L2_MAC_UCAST_PORT_INVALID,               
    SYS_ERR_L2_MAC_UCAST_ADDR,                       
    SYS_ERR_L2_STATIC_AZERO_INVALID,                 
    SYS_ERR_L2_STATIC_EQ_SYSMAC,                     
    SYS_ERR_L2_STATIC_BE_MCAST,                      
    SYS_ERR_L2_STATIC_PORT_TRUNKMBR_INVALID,         
    SYS_ERR_L2_STATIC_PORT_EMPTYTRUNK_INVALID,       
    SYS_ERR_L2_DB_BUSY,                              
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    SYS_ERR_L2_MAC_CONFIT_WITH_FILTER,               
    SYS_ERR_L2_FILTER_AZERO_INVALID,                 
    SYS_ERR_L2_FILTER_EQ_SYSMAC,                     
    SYS_ERR_L2_FILTER_BE_MCAST,                      
#endif
    SYS_ERR_L2_END,                                  
    SYS_ERR_L3_START,                                
    SYS_ERR_L3_INTFTYPE_INVALID,                     
    SYS_ERR_L3_DB_FULL,                              
    SYS_ERR_L3_DB_NOT_FOUND,                         
    SYS_ERR_L3_INTF_LO_ID_INVALID,                   
    SYS_ERR_L3_INTF_NUM_EXCEED,                      
    SYS_ERR_L3_INTF_DISABLE_IPV6_ADDR_EXIST,         
    SYS_ERR_L3_INTF_IP_NOT_EXIST,                    
    SYS_ERR_L3_INTF_PRIMARY_IP_NOT_EXIST,            
    SYS_ERR_L3_INTF_IP_EXIST,                        
	SYS_ERR_L3_INTF_PRIMARY_IP_EXIST,				 
    SYS_ERR_L3_INTF_IP_SUB_EXIST,                    
    SYS_ERR_L3_INTF_IP_DEL_PRIMARY,                  
    SYS_ERR_L3_INTF_IP_DEL_SUB,                      
    SYS_ERR_L3_MGMT_IP_CONFLICT,                     
    SYS_ERR_L3_INTF_IP_NUM_EXCEED,                   
    SYS_ERR_L3_INTF_NOT_ROUTING,                     
    SYS_ERR_L3_IP_DUPLICATE,                         
    SYS_ERR_L3_IP_OVERLAP,                           
    SYS_ERR_L3_IP_ADDR_INVALID,                      
    SYS_ERR_L3_IP_MASK_INVALID,                      
    SYS_ERR_L3_VLAN_IS_MGMT_VLAN, 					 
    SYS_ERR_MGMT_VLAN_IS_L3_IP, 					 
    SYS_ERR_L3_IPV6_PREFIX_INVALID,                  
    SYS_ERR_L3_ROUTE_GW_ADDR_INVALID,                
    SYS_ERR_L3_ROUTE_GW_INTF_IP,                     
    SYS_ERR_L3_ROUTE_GW_NOT_DIRECT,                  
    SYS_ERR_L3_ROUTE_GW_DHCP_ENBL,                   
    SYS_ERR_L3_ROUTE_GW_MGMT_SUBNET,                 
    SYS_ERR_L3_ROUTE_ON_LO_INTF,                     
    SYS_ERR_L3_ROUTE_MAX_NH_NUM_EXCEED,              
    SYS_ERR_L3_STATIC_ROUTE_FULL,                    
    SYS_ERR_L3_STATIC_ROUTE_NOT_EXIST,               
    SYS_ERR_L3_STATIC_NEIGH_MAC,                     
    SYS_ERR_L3_STATIC_NEIGH_MAC_SAMEDUT,             
    SYS_ERR_L3_STATIC_NEIGHV4_NOT_MATCH,             
    SYS_ERR_L3_STATIC_NEIGHV4_NOT_EXIST,             
    SYS_ERR_L3_STATIC_NEIGHV4_SAME_DUT,              
    SYS_ERR_L3_STATIC_NEIGHV4_INTF_NOT_EXIST,        
    SYS_ERR_L3_STATIC_NEIGHV4_INTF_DHCP,             
    SYS_ERR_L3_STATIC_NEIGHV6_INTF_NOTON,            
    SYS_ERR_L3_STATIC_NEIGHV6_NOT_MATCH,             
    SYS_ERR_L3_INTF_EXIST,                           
    SYS_ERR_L3_END,                                  
#ifdef CONFIG_SYS_PROTO_LLDP
    SYS_ERR_LLDP_START,                              
  #ifdef CONFIG_SYS_PROTO_STP_MSTP
    SYS_ERR_LLDP_DISABLE_ACTION_FLOODING_MSTP,       
  #endif
    SYS_ERR_LLDP_TX_INTERVAL,                        
    SYS_ERR_LLDP_HOLDTIME_MUL,                       
    SYS_ERR_LLDP_REINIT_DELAY,                       
    SYS_ERR_LLDP_HOLDTIME_MUL_TX_INTERVAL,           
    SYS_ERR_LLDP_TX_DELAY_TX_INTERVAL,               
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_LLDP_NETWORK_POLICY_NOT_EXIST,           
    SYS_ERR_LLDP_NETWORK_POLICY_DUPLICATE,           
    SYS_ERR_LLDP_NETWORK_POLICY_BINDED,              
    SYS_ERR_LLDP_NETWORK_POLICY_VOICE_WHEN_AUTO,     
    SYS_ERR_LLDP_NETWORK_POLICY_AUTO_WHEN_VOICE,     
    SYS_ERR_LLDP_LOCATION_FORMAT,                    
  #endif
    SYS_ERR_LLDP_END,                                
#endif
    SYS_ERR_LOG_START,                               
    SYS_ERR_LOG_SERVER_IP,
    SYS_ERR_LOG_SERVER_FULL,
    SYS_ERR_LOG_SERVER_EXIST,
    SYS_ERR_LOG_SERVER_NOT_EXIST,
    SYS_ERR_LOG_SEVERITY_ERROR,
    SYS_ERR_LOG_CATEGORY_ERROR,
    SYS_ERR_LOG_END,                                 
#ifdef CONFIG_SYS_MIRROR
    SYS_ERR_MIRROR_START,                            
    SYS_ERR_MIRROR_GROUP_INVALID,                    
    SYS_ERR_MIRROR_TARGET_EXIST,                     
    SYS_ERR_MIRROR_TARGET_NOT_EXIST,                 
    SYS_ERR_MIRROR_TARGET,                           
    SYS_ERR_MIRROR_SELF_MIRROR,                      
    SYS_ERR_MIRROR_MONITOR_PORT_MIRRORED,            
    SYS_ERR_MIRROR_TRUNK,                            
    SYS_ERR_MIRROR_TRUNK_MEMBER,                     
    #ifdef CONFIG_SYS_PROTO_GVRP
    SYS_ERR_MIRROR_GVRP_ENABLE,                      
    #endif
    SYS_ERR_MIRROR_NOT_PORTBASED,                    
    SYS_ERR_MIRROR_END,                              
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SYS_ERR_MLD_START,                              
    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    SYS_ERR_MLD_MAX_GROUP_ACT,
    #endif
    SYS_ERR_MLD_PROFILE_ID,
    SYS_ERR_MLD_PROFILE_USED,
    SYS_ERR_MLD_PROFILE_NOT_EXIST,
    SYS_ERR_MLD_PROFILE_EMPTY,
    SYS_ERR_MLD_END,                              
#endif
#ifdef CONFIG_SYS_PROTO_LACP
    SYS_ERR_LACP_START,                              
    SYS_ERR_LACP_SYSTEM_PRIORITY,                    
    SYS_ERR_LACP_ACTIVE_PORTS,                       
    SYS_ERR_LACP_NOT_MEMBER_PORTS,                   
    SYS_ERR_LACP_STATIC_TRUNK_GROUP,                 
    SYS_ERR_LACP_END,                                
#endif
    SYS_ERR_PORT_START,                              
    SYS_ERR_PORT_LINKDOWN,
    SYS_ERR_PORT_RANGE,
    SYS_ERR_PORT_AUTO_NEGO,
    SYS_ERR_PORT_AUTO_NEGO_NOT_ALLOW,
    SYS_ERR_PORT_AUTO_FLOW_NOT_ALLOW_COMBO_FIBER,   
    SYS_ERR_PORT_SPEED,
    SYS_ERR_PORT_DUPLEX,
    SYS_ERR_PORT_1000M_NOT_SUPPORT,                  
    SYS_ERR_PORT_1000M_HALF_NOT_SUPPORT,             
    SYS_ERR_PORT_FIBER_SPEED,
    SYS_ERR_PORT_FIBER_DUPLEX,
    SYS_ERR_PORT_NOTJOINIVLAN,
    SYS_ERR_PORT_SET_FIBER_TO_COPPER,                
    SYS_ERR_PORT_RTCT_NOT_FINISH,                   
    SYS_ERR_PORT_RTCT_NOT_COPPER,                    
    SYS_ERR_PORT_EEE_NOT_FIBER,                      
    SYS_ERR_PORT_NOT_FIBER,                         
    SYS_ERR_PORT_END,                                
#ifdef CONFIG_SYS_PORT_SECURITY
    SYS_ERR_PSECURE_START,                           
    SYS_ERR_PSECURE_MAX_LESS_CURRENT,                
    SYS_ERR_PSECURE_MAC_NOT_EXIST,                   
    SYS_ERR_PSECURE_MAC_FULL,                        
    SYS_ERR_PSECURE_TRUNK,                           
    SYS_ERR_PSECURE_TYPE_INVALID,                    
    SYS_ERR_PSECURE_AZERO_INVALID,                   
    SYS_ERR_PSECURE_EQ_SYSMAC,                       
    SYS_ERR_PSECURE_BE_MCAST,                        
    SYS_ERR_PSECURE_MAC_CONFIT_WITH_PSECURE,         
    SYS_ERR_PSECURE_FORBID_STATIC,                   
    SYS_ERR_PSECURE_PORT_STATIC_MAC_EXIST,           
    SYS_ERR_PSECURE_END,                             
#endif
    SYS_ERR_QOS_START,                               
    SYS_ERR_QOS_MODE,                                
    SYS_ERR_QOS_MODE_BASIC,                          
    SYS_ERR_QOS_PRI_INVALID,                         
    SYS_ERR_QOS_WEIGHT_INVALID,                      
    SYS_ERR_QOS_COS_INVALID,                         
    SYS_ERR_QOS_DSCP_INVALID,                        
    SYS_ERR_QOS_IPP_INVALID,                         
    SYS_ERR_QOS_QUEUE_INVALID,                       
    SYS_ERR_QOS_REMARK_CONFLICT,                     
    SYS_ERR_QOS_END,                                 
#ifdef CONFIG_SYS_LIB_RADIUS
    SYS_ERR_RADIUS_START,                            
    SYS_ERR_RADIUS_SRV_ERR,
    SYS_ERR_RADIUS_SRV_EMPTY,
    SYS_ERR_RADIUS_SRV_FULL,
    SYS_ERR_RADIUS_AUTH_ERR,
    SYS_ERR_RADIUS_AUTH_REJECT,
    SYS_ERR_RADIUS_SRV_EXIST,
    SYS_ERR_RADIUS_SRV_PRIORITY_EXIST,
    SYS_ERR_RADIUS_END,                              
#endif
    SYS_ERR_RATE_START,                              
    SYS_ERR_RATE_OUT_OF_RANGE,
    SYS_ERR_RATE_NOT_MULTIPLE_OF_16,
    SYS_ERR_RATE_BURSTSIZE_NOT_MULTIPLE_OF_128,
    SYS_ERR_RATE_BURSTSIZE_NOT_MULTIPLE_OF_1024,
    SYS_ERR_RATE_END,                                

    SYS_ERR_SNMP_START,                              
    SYS_ERR_SNMP_STR,
    SYS_ERR_SNMP_COMMUNITY_FULL,
    SYS_ERR_SNMP_TARGET_FULL,
    SYS_ERR_SECURITY_LEVEL,
    SYS_ERR_TRAP_TYPE,
    SYS_ERR_SNMP_INFORM,
    SYS_ERR_SNMP_COMMUNITY_NAME,
    #ifdef CONFIG_SYS_SNMPV3
    SYS_ERR_SNMP_VIEW_FULL,
    SYS_ERR_SNMP_VIEW_DEFAULT,
    SYS_ERR_SNMP_VIEW_NAME,
    SYS_ERR_SNMP_GROUP_FULL,
    SYS_ERR_SNMP_ACCESS_FULL,
    SYS_ERR_SNMP_ACCESS_GROUP_NAME,
    SYS_ERR_SNMP_USER_FULL,
    SYS_ERR_SNMP_REMOTE_FULL,
    SYS_ERR_OID,
    SYS_ERR_OID_DFLT,
    SYS_ERR_OID_LEN,
    SYS_ERR_OID_MASK,
    SYS_ERR_OID_MASK_LEN,
    SYS_ERR_COMNUNITY_NOT_FOUND,
    SYS_ERR_GROUP_EXIST,
    SYS_ERR_GROUP_NOT_EXIST,
    SYS_ERR_GROUP_VIEW_CONFLICT,
    SYS_ERR_READ_VIEW_NOT_EXIST,
    SYS_ERR_READ_VIEW_MUST_EXIST,
    SYS_ERR_WRITE_VIEW_NOT_EXIST,
    SYS_ERR_NOTIFY_VIEW_NOT_EXIST,
    SYS_ERR_ACCESS_GROUP_USED,
    SYS_ERR_ENGINEID_ERR,
    SYS_ERR_ENGINEID_LEN_ERR,
    SYS_ERR_USER_NOT_FOUND,
    SYS_ERR_SNMPv3_USERNAME_ERR,
    SYS_ERR_SNMPv3_PASSWORD_ERR,
    SYS_ERR_SNMPv3_AUTH_ERR,
    SYS_ERR_SNMPv3_ENCRYPT_ERR,
    #endif
    SYS_ERR_SNMP_END,                                

    SYS_ERR_STP_START,                               
    SYS_ERR_STP_HELLO_TIME,                          
    SYS_ERR_STP_FORWARD_DELAY,                       
    SYS_ERR_STP_MAXIMUM_AGE,                         
    SYS_ERR_STP_TIMER_RELATIONSHIPS,                 
    SYS_ERR_STP_BRIDGE_PRIORITY,                     
    SYS_ERR_STP_PORT_PRIORITY,                       
    SYS_ERR_STP_CST_DEL,                             
    SYS_ERR_STP_CST_VLAN_DEL,                        
#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_STP_MSTP)
    SYS_ERR_STP_MSTP_LLDP_FLOODING,                  
#endif
    SYS_ERR_STP_COST_SHORT_RANGE,
    SYS_ERR_STP_END,                                 
#ifdef CONFIG_SYS_LIB_TACPLUS
    SYS_ERR_TACPLUS_START,                           
    SYS_ERR_TACPLUS_SRV_ERR,
    SYS_ERR_TACPLUS_SRV_EMPTY,
    SYS_ERR_TACPLUS_SRV_FULL,
    SYS_ERR_TACPLUS_SRV_EXIST,
    SYS_ERR_TACPLUS_SRV_PRIORITY_EXIST,
    SYS_ERR_TACPLUS_END,                             
#endif
    SYS_ERR_TFTP_START,                              
    SYS_ERR_TFTP_DOWNLOAD_IMAGE,
    SYS_ERR_TFTP_DOWNLOAD_IMAGE_MAGIC,
    SYS_ERR_TFTP_DOWNLOAD_CONFIG,
    SYS_ERR_TFTP_UPLOAD_IMAGE,
    SYS_ERR_TFTP_UPLOAD_CONFIG,
    SYS_ERR_TFTP_FILE_NAME,
    SYS_ERR_TFTP_FILE_NOT_FOUND,                     
    SYS_ERR_TFTP_SERVER_IP,
    SYS_ERR_TFTP_TIMEOUT,
    SYS_ERR_TFTP_UDP,
    SYS_ERR_TFTP_BCAST_NORESP,                       
    SYS_ERR_TFTP_END,                                
    SYS_ERR_TRUNK_START,                             
    SYS_ERR_TRUNK_INVALID,                           
    SYS_ERR_TRUNK_MBR,                               
    SYS_ERR_TRUNK_HASHMASK,                          
    SYS_ERR_TRUNK_LACP_GROUP,                        
    SYS_ERR_TRUNK_LESS_THAN_2_PORTS,                 
    SYS_ERR_TRUNK_MORE_THAN_4_PORTS,                 
    SYS_ERR_TRUNK_MORE_THAN_8_PORTS,                 
    SYS_ERR_TRUNK_MORE_THAN_16_PORTS,                
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_TRUNK_PORT_HAS_AUTH,                     
#endif
    SYS_ERR_TRUNK_PORT_USED_BY_TRK,                  
    SYS_ERR_TRUNK_PORT_VLAN_CONFLICT,                
    SYS_ERR_TRUNK_PORT_MIRRORING,                    
    SYS_ERR_TRUNK_PORT_RATE_LIMITED,                 
    SYS_ERR_TRUNK_PORT_NEGO,                         
    SYS_ERR_TRUNK_PORT_SPEED,                        
    SYS_ERR_TRUNK_PORT_DUPLEX,                       
    SYS_ERR_TRUNK_TRK_NO_EXIST,                      
    SYS_ERR_TRUNK_BACKUPPORT_USED_BY_TRK,            
#ifdef CONFIG_SYS_PORT_SECURITY
    SYS_ERR_TRUNK_PORT_HAS_PORT_SECURITY,           
#endif
    SYS_ERR_TRUNK_END,                               
#ifdef CONFIG_SYS_PROTO_UDLD
    SYS_ERR_UDLD_START,                              
    SYS_ERR_UDLD_AGSV,                               
    SYS_ERR_UDLD_NOT_AGSV,                           
    SYS_ERR_UDLD_PORT_NML_NOT,                       
    SYS_ERR_UDLD_PORT_NML_BUT_AGSV,                  
    SYS_ERR_UDLD_PORT_NML_BUT_DBL,                   
    SYS_ERR_UDLD_PORT_AGSV_NOT,                      
    SYS_ERR_UDLD_PORT_AGSV_BUT_NML,                  
    SYS_ERR_UDLD_PORT_AGSV_BUT_DBL,                  
    SYS_ERR_UDLD_PORT_DBL_NOT,                       
    SYS_ERR_UDLD_PORT_DBL_BUT_NML,                   
    SYS_ERR_UDLD_PORT_DBL_BUT_AGSV,                  
    SYS_ERR_UDLD_PORT_NOT_COMBO_FIBER,               
    SYS_ERR_UDLD_ONLY_FIBER_SUPPORT,                 
    SYS_ERR_UDLD_END,                                
#endif
    SYS_ERR_VLAN_START,                              
    SYS_ERR_VLAN_DFLTVLAN_DEL,                       
    SYS_ERR_VLAN_EXIST,                              
    SYS_ERR_VLAN_FRAMETYPE,                          
    SYS_ERR_VLAN_ID,                                 
    SYS_ERR_VLAN_VID_MGMTVLAN,                       
    SYS_ERR_VLAN_MEMBERSHIP,                         
    SYS_ERR_VLAN_MGMTVLAN_DEL,                       
    SYS_ERR_VLAN_MGMTVLAN_EMPTY,                     
    SYS_ERR_VLAN_NAME_NULL,                          
    SYS_ERR_VLAN_NAME_EXIST,                         
    SYS_ERR_VLAN_NOT_EXIST,                          
    SYS_ERR_VLAN_NOT_STATIC,                         
    SYS_ERR_VLAN_WAS_NOT_CREATED_BY_USER,            
    SYS_ERR_VLAN_TRKMBR_PORT,                        
#ifdef CONFIG_SYS_VLAN_QINQ
    SYS_ERR_VLAN_PORTMODE_NOT_UPLINK,                
#endif
    SYS_ERR_VLAN_MVR_VID_DEL,                        
    SYS_ERR_VLAN_NUM_NOT_SUPPORT,                    
#ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_VLAN_VID_VOICEVLAN,                      
    SYS_ERR_VLAN_VOICEVLAN_OUI_EXIST,                
    SYS_ERR_VLAN_VOICEVLAN_OUI_MCAST,                
    SYS_ERR_VLAN_VOICEVLAN_OUI_ZERO,                 
    SYS_ERR_VLAN_VOICEVLAN_AUTO_PORT,                
    SYS_ERR_VLAN_VOICEVLAN_OUI_PORT,                 
    SYS_ERR_VLAN_VOICEVLAN_PORT_MODE,                
    SYS_ERR_VLAN_VOICEVLAN_PORT_STATIC,              
    SYS_ERR_VLAN_VOICEVLAN_OUI_ENABLED_PORT,         
    SYS_ERR_VLAN_VOICEVLAN_OUI_DFLT_VID,             
    SYS_ERR_VLAN_VOICEVLAN_VID_DEL,                  
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_VLAN_VOICEVLAN_GUEST_VLAN,               
  #endif
  #ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_ERR_VLAN_VOICEVLAN_SURVEILLANCE_VLAN,        
  #endif
    SYS_ERR_VLAN_VOICEVLAN_VID_NOT_EXIST,            
    SYS_ERR_VLAN_VOICEVLAN_VID_ENABLE,               
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    SYS_ERR_VLAN_PROVLAN_PORT_REF,                   
    SYS_ERR_VLAN_PROVLAN_PORT_MODE,                  
    SYS_ERR_VLAN_PROVLAN_ENABLED_PORT,               
#endif
#ifdef CONFIG_SYS_MACVLAN
    SYS_ERR_VLAN_MAC_MAX,                            
    SYS_ERR_VLAN_MAC_NOT_UNI,                        
    SYS_ERR_VLAN_MAC_OVERLAP,                        
    SYS_ERR_VLAN_MAC_LAST_DEL,                       
    SYS_ERR_VLAN_MAC_NO_RESOURCE,                    
    SYS_ERR_VLAN_MAC_NO_GID,                         
    SYS_ERR_VLAN_MAC_GENERAL_MODE,                   
    SYS_ERR_VLAN_MAC_PORT_MODE,                      
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SYS_ERR_VLAN_SV_OUI_MCAST,                       
    SYS_ERR_VLAN_SV_OUI_EXIST,                       
    SYS_ERR_VLAN_SV_OUI_ZERO,                        
  #ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_VLAN_SV_EQ_GUEST,                        
  #endif
  #ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_VLAN_SV_EQ_VOICE,                        
  #endif
    SYS_ERR_VLAN_SV_EQ_DEFAULT,                      
    SYS_ERR_VLAN_SV_NOT_EXIST,                       
    SYS_ERR_VLAN_SV_VID_ENABLE,                      
    SYS_ERR_VLAN_SV_ENABLED_PORT,                    
    SYS_ERR_VLAN_SV_PORT_MODE,                       
    SYS_ERR_VLAN_SV_PORT_STATIC,                     
    SYS_ERR_VLAN_SV_GLOBAL_DISABLE,                  
    SYS_ERR_VLAN_VID_SURVEILLANCEVLAN,               
    SYS_ERR_VLAN_SV_VID_DEL,                         
    SYS_ERR_VLAN_SV_USED_PORT,                       
#endif
    SYS_ERR_VLAN_END,                                
#ifdef CONFIG_SYS_PROTO_GVRP
    SYS_ERR_GVRP_START,                              
    SYS_ERR_GVRP_NOT_ENABLE,
    SYS_ERR_GVRP_DB_FULL,
    SYS_ERR_GVRP_PORT_NOT_TRUNK,
    SYS_ERR_GVRP_MIRROR_TARGET,
    SYS_ERR_GVRP_PORT_MODE,
    SYS_ERR_GVRP_TIMER_DIV,
    SYS_ERR_GVRP_TIMER_JOIN,
    SYS_ERR_GVRP_TIMER_LEAVE,
    SYS_ERR_GVRP_TIMER_LEAVEALL,
    SYS_ERR_GVRP_END,                                
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    SYS_ERR_MGMT_ACL_START,                          
    SYS_ERR_MGMT_ACL_NODORM,                         
    SYS_ERR_MGMT_ACL_NODOADD,
    SYS_ERR_MGMT_ACL_NOSUCHLIST,
    SYS_ERR_MGMT_ACL_NOALLLIST,
    SYS_ERR_MGMT_ACE_PRIEXCEED,
    SYS_ERR_MGMT_ACL_EXISTSUCHACE,
    SYS_ERR_MGMT_ACL_NOSUCHACE,
    SYS_ERR_MGMT_ACE_NOPRi_PRIEXCEED,
    SYS_ERR_MGMT_ACL_ENTRY_FULL,
    SYS_ERR_MGMT_ACL_PORT_WRONG,
    SYS_ERR_MGMT_ACL_SOURCEIP_WRONG,
    SYS_ERR_MGMT_ACL_SOURCVID_WRONG,
    SYS_ERR_MGMT_ACL_CONSOLE_REPEAT,
    SYS_ERR_MGMT_ACL_NAME_EMPTY,
    SYS_ERR_MGMT_ACL_END,                            
#endif
    
    SYS_ERR_INTERNAL_START,
    SYS_ERR_DEV_ID,                                  
    SYS_ERR_DEV_OPEN,                                
    SYS_ERR_DEV_CLOSE,                               
    SYS_ERR_DEV_READ,                                
    SYS_ERR_DEV_WRITE,                               
    SYS_ERR_FILE_CREATE,
    SYS_ERR_FILE_OPEN,
    SYS_ERR_FILE_READ,
    SYS_ERR_FILE_SEEK,
    SYS_ERR_FILE_SIZE,
    SYS_ERR_FILE_WRITE,
    SYS_ERR_FILE_NOT_EXIST,                          
    SYS_ERR_INFO_BADCRC,                             
    SYS_ERR_INFO_NOT_EXIST,                          
    SYS_ERR_IOCTL,
    SYS_ERR_KSI_CMD_NOT_FOUND,                       
    SYS_ERR_KSI_DATA_EMPTY,                          
    SYS_ERR_MODULE_NOT_INIT,                         
    SYS_ERR_NULL_POINTER,                            
    SYS_ERR_SEM_LOCK,                                
    SYS_ERR_SEM_UNLOCK,                              
    SYS_ERR_SOCKET,	
    SYS_ERR_INTERNAL_END,
    SYS_ERR_UNKNOWN,                                 
    
    SYS_ERR_INFO_START,
    SYS_ERR_ACL_PIE_FULL,                            
    SYS_ERR_ACL_DUMP_SKIP,                           
    SYS_ERR_TRUNK_EMPTY,                             
    SYS_ERR_TRUNK_MEMBER_SKIP,                       
    SYS_ERR_PORT_AUTO_SPEED,                         
    SYS_ERR_PORT_STR_FMT,                            
#ifdef CONFIG_SYS_VLAN_QINQ
    SYS_ERR_QINQ_ENTRY_NOT_EXIST,                    
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    SYS_ERR_ISG_ENTRY_NOT_EXIST,                     
#endif
#ifdef CONFIG_SYS_MGMT_ACL
    SYS_ERR_MGMT_ACL_EXIST,
    SYS_ERR_MGMT_ACL_IP_FRAG,
    SYS_ERR_MGMT_ACL_TCP_NONE,
    SYS_ERR_MGMT_ACL_UDP_NONE,
    SYS_ERR_MGMT_ACL_ACTION_PERMIT,
    SYS_ERR_MGMT_ACL_ACTION_DENY,
    SYS_ERR_MGMT_ACL_CLASS_DISABLE,
#endif
#ifdef CONFIG_SYS_PROTO_STP
  #ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    SYS_ERR_STP_BPDU_SELFLOOP,
  #endif
#endif

#ifdef CONFIG_SYS_DHCP_SERVER
	SYS_ERR_DHCP_LEASE_MAX_TIME,
	SYS_ERR_DHCP_LEASE_TIME_EMPTY,
	SYS_ERR_DHCP_LEASE_TIME_ERR,
	SYS_ERR_DHCP_SECTION_STARTIP_INVALID,
	SYS_ERR_DHCP_SECTION_ENDIP_INVALID,
	SYS_ERR_DHCP_SECTION_STARTIP_HIGHER,
	SYS_ERR_DHCP_SECTION_LENGTH_TOOBIG,
	SYS_ERR_DHCP_SECTION_OVERLAP,
	SYS_ERR_DHCP_SECTION_NUM_FULL,
	SYS_ERR_DHCP_SECTION_IPGATE_NOSAMESUBNET,
	SYS_ERR_DHCP_SECTION_NO_GATEWAY,
	SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW,
	SYS_ERR_DHCP_STATIC_IP_INVALID,
	SYS_ERR_DHCP_SECTION_IP_INUSED,
	SYS_ERR_DHCP_IP_POOL_FULL,
#endif

	SYS_ERR_OSPF_AREA_NETWORK_EXIST,
	SYS_ERR_OSPF_IS_CLOSE,
	SYS_ERR_OSPF_INTERFACE_NOT_EXIST,
	SYS_ERR_OSPF_AREA_IS_BACKBONE,
    SYS_ERR_OSPF_AREA_NOT_DEFAULT,
	
	SYS_ERR_RIP_IS_CLOSE,
	
#ifdef CONFIG_SYS_L3_VRRP
	SYS_ERR_VRRP_CFGERR_SUBNETDIFF,
	SYS_ERR_VRRP_CFGERR_MAXVSRV,
	SYS_ERR_VRRP_CFGERR_MAXVIP,
#endif
	SYS_ERR_MAC_VLAN_GROUP_ID,
	SYS_ERR_MAC_ADDRESS_INVALID,
    SYS_ERR_INFO_END,
    SYS_ERR_MAC_PORT_MOVE_DROP,
    SYS_ERR_END                                      
} sys_err_code_t;
#define IS_RT_ERR_INVALID(rtErr)      \
    ((((rtErr) >= RT_ERR_END) || ((rtErr) < RT_ERR_FAILED)) ? 1: 0)

#define IS_SYS_ERR_INVALID(sysErr)    \
    ((((sysErr) >= SYS_ERR_END) || ((sysErr) < SYS_ERR_FAILED)) ? 1: 0)

#define SYS_PRINTF_FILE_LINE        do {}while (0)
#define SYS_PRINTF_FILE_LINE_STR(_str)   do {}while (0)

#define SYS_INIT_CHK(state)                                 \
do {                                                        \
    if (SYS_INIT_COMPLETED != (state)) {                    \
        return SYS_ERR_MODULE_NOT_INIT;                     \
    }                                                       \
} while (0)

#define SYS_ERR_CONTINUE(op)                                \
    if ((op) != SYS_ERR_OK) continue;

#define SYS_ERR_CHK(op)                                     \
do {                                                        \
    int32 errCode;                                          \
    if ((errCode = (op)) != SYS_ERR_OK)                    \
        return errCode;                                     \
} while(0)

#define SYS_ERR_TRK_CHK(op)                                 \
do {                                                        \
    int32 errCode = (op);                                   \
    if ((errCode != SYS_ERR_OK) &&                          \
        (errCode != SYS_ERR_TRUNK_EMPTY) &&                 \
        (errCode != SYS_ERR_TRUNK_MEMBER_SKIP))             \
        return errCode;                                     \
} while(0)

#define SYS_ERR_CHK_RET(op, errCode)                        \
do {                                                        \
    if ((op) != SYS_ERR_OK)                                 \
        return errCode;                                     \
} while(0)

#define SYS_ERR_LOG(op, str)                                \
do {                                                        \
    int32 errCode;                                          \
    if ((errCode = (op)) != SYS_ERR_OK)                     \
    {                                                       \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_LOG_ES(errCode, str);                           \
        return errCode;                                     \
    }                                                       \
} while(0)

#define SYS_ERR_PRINTF(op, str)                             \
do {                                                        \
    int32 errCode;                                          \
    if ((errCode = (op)) != SYS_ERR_OK)                     \
    {                                                       \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_PRINTF("%s [%s]\n", str, SYS_ERR_STR(errCode)); \
        return errCode;                                     \
    }                                                       \
} while(0)

#define SYS_PARAM_CHK(expr, errCode)                        \
do {                                                        \
    if ((int32)(expr)) {                                    \
        return errCode;                                     \
    }                                                       \
} while (0)

#define SYS_PARAM_LOG(expr, errCode, str)                   \
do {                                                        \
    if ((int32)(expr)) {                                    \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_LOG_ES((errCode), str);                         \
        return (errCode);                                   \
    }                                                       \
} while (0)

#define SYS_PARAM_PRINTF(expr, errCode, str)                \
do {                                                        \
    if ((int32)(expr)) {                                    \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_PRINTF("%s [%s]\n", str, SYS_ERR_STR(errCode)); \
        return (errCode);                                   \
    }                                                       \
} while (0)

#define SYS_ERR_XLATE(rtErr, sysErr)                        \
do {                                                        \
    (sysErr) = sys_errXlate_ret((rtErr));                   \
} while (0)

#define SYS_ERR_XLATE_CHK(op)                               \
do {                                                        \
    int32 sysErr = sys_errXlate_ret((op));                  \
    if (sysErr == SYS_ERR_CHIP_NOT_SUPPORTED)               \
        SYS_PRINTF_FILE_LINE_STR("SYS_ERR_CHIP_NOT_SUPPORTED"); \
    if ((sysErr != SYS_ERR_OK) &&                           \
        (sysErr != SYS_ERR_PORT_ID))                        \
        return sysErr;                                      \
} while(0)

#define SYS_ERR_XLATE_LOG(op, str)                          \
do {                                                        \
    int32 sysErr;                                           \
    if ((sysErr = sys_errXlate_ret((op))) != SYS_ERR_OK)   \
    {                                                       \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_LOG_ES(sysErr, str);                            \
        return sysErr;                                      \
    }                                                       \
} while(0)

#define SYS_ERR_XLATE_PRINTF(op, str)                       \
do {                                                        \
    int32 sysErr;                                           \
    if ((sysErr = sys_errXlate_ret((op))) != SYS_ERR_OK)   \
    {                                                       \
        SYS_PRINTF_FILE_LINE;                               \
        SYS_PRINTF("%s [%s]\n", str, SYS_ERR_STR(sysErr));  \
        return sysErr;                                      \
    }                                                       \
} while(0)

#define SYS_ERR_HDL(op, errHandle, ret) \
do {                                    \
    if ((ret = (op)) != SYS_ERR_OK){   \
        goto errHandle;}                \
} while(0)
#define SYS_SNMP_ERR_CHK(op, errorCode)     \
do {                                                        \
    if ((op) != SYS_ERR_OK)                    \
        return errorCode;      \
} while(0)

#define SYS_STR_LENGTH_CHK(strLen, threshold)   \
do {                                           \
    if ((strLen) > threshold)                    \
        return SYS_ERR_STR_LEN_EXCEED;      \
} while(0)
extern char *sys_errStr_ret(int32 sysErr);
extern int32 sys_errXlate_ret(int32 rtErr);

extern char *sys_errDes_ret(int32 sysErr);
#endif  

