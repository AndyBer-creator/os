
#ifndef __PROTO_IGMP_H__
#define __PROTO_IGMP_H__

#include <common/sys_def.h>
#include <common/sys_log_desc.h>
#include <igmp/proto_igmp_util.h>
#include <igmp/proto_igmp_querier.h>

#define MASK_BIT_LEN                            32
#define IGMP_TEMP_PERIOD                        3

#define DVMRP_PROTO_ID                2
#define OSPF_PROTO_ID                89
#define PIM_PROTO_ID                  103
#define VALID_OTHER_PACKET                      0
#define SUPPORTED_IGMP_CONTROL_PACKET           1
#define UNSUPPORTED_IGMP_CONTROL_PACKET         2
#define ERRONEOUS_PACKET                        3
#define MULTICAST_DATA_PACKET                   5
#define ROUTER_ROUTING_PACKET                   6
#define MULTICAST_MVR_PACKET                     7
#define MULTICAST_MVR_DATA_PACKET            8

#define IGMP_HOST_MEMBERSHIP_QUERY              0x11    
#define IGMP_HOST_MEMBERSHIP_REPORT             0x12    
#define IGMP_PIMV1_ROUTING_PKT                  0x14    
#define IGMP_HOST_NEW_MEMBERSHIP_REPORT         0x16    
#define IGMP_HOST_LEAVE_MESSAGE                 0x17
#define IGMPv3_HOST_MEMBERSHIP_REPORT           0x22    

#define IGMP_QUERY_V2                           2
#define IGMP_QUERY_V3                           3

#define MODE_IS_INCLUDE                         1
#define MODE_IS_EXCLUDE                         2
#define CHANGE_TO_INCLUDE_MODE                  3
#define CHANGE_TO_EXCLUDE_MODE                  4
#define ALLOW_NEW_SOURCES                       5
#define BLOCK_OLD_SOURCES                       6

#define FILTER_MODE_INCLUDE                     0
#define FILTER_MODE_EXCLUDE                     1

#define  L2TABLE_NOT_WROTE                      0
#define  L2TABLE_WROTE                          1

#define  EXCLUDE_MODE_TIME                      0xff
#define  IGMPV3_FWD_NORMAL              0
#define  IGMPV3_FWD_CONFLICT            1
#define  IGMPV3_FWD_NOT                 2

#define  IGMPV3_GRP_NOT_SRC_SPEC        0
#define  IGMPV3_GRP_SRC_SPEC            1

#define PASS_SECONDS    1

#define INVALID_PROFILE_ID      0x0

#define MCAST_MAX_VLAN_EVENT_NUM 256

#define MCAST_LOG(mid,args...)    \
do {                                                    \
    SYS_LOG(LOG_CAT_MCAST, mid, ##args); \
}while(0)

typedef uint16 sys_igmp_profileId_t;
typedef enum igmp_version_e
{
    IGMP_VERSION_V1 = 1,
    IGMP_VERSION_V2,
    IGMP_VERSION_V3_BASIC,
    IGMP_VERSION_V3_FULL,
    IGMP_VERSION_END,
} igmp_version_t;
typedef enum mcast_version_e
{
    MCAST_VERSION_V1,
    MCAST_VERSION_V2,
    MCAST_VERSION_V3
} mcast_version_t;

typedef enum igmp_lookup_mode_e
{
    IGMP_DMAC_VID_MODE,  
    IGMP_DIP_SIP_MODE,   
    IGMP_DIP_VID_MODE,   
    IGMP_LOOKUP_MODE_END
} igmp_lookup_mode_t;

typedef enum igmp_unknMcastAction_e
{
    IGMP_UNKNOWN_MCAST_ACTION_DROP,
    IGMP_UNKNOWN_MCAST_ACTION_FLOOD,
    IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT,
    IGMP_UNKNOWN_MCAST_ACTION_END,
} igmp_unknMcastAction_t;

typedef enum igmp_maxGroupAct_e
{
    IGMP_ACT_DENY,
    IGMP_ACT_REPLACE,
    IGMP_ACT_END,
}igmp_maxGroupAct_t;

typedef enum mcast_profileAct_e
{
    MCAST_PROFILE_ACT_DENY,
    MCAST_PROFILE_ACT_PERMIT,
    MCAST_PROFILE_ACT_END,
}mcast_profileAct_t;

typedef enum mcast_profileType_e
{
    MCAST_PROFILE_TYPE_FILTER,
    MCAST_PROFILE_TYPE_PREVIEW,
    MCAST_PROFILE_TYPE_END,
}mcast_profileType_t;

typedef enum mcast_forward_e
{
    MCAST_FORWARD_NONE,
    MCAST_FORWARD_STATIC,
    MCAST_FORWARD_FORBIDDEN,
    MCAST_FORWARD_END,
}mcast_forward_t;
typedef enum mvr_portType_e
{
    MVR_TYPE_NONE,
    MVR_TYPE_RECEIVER,
    MVR_TYPE_SOURCE,
    MVR_TYPE_END,
}mvr_portType_t;

typedef enum mvr_mode_e
{
    MVR_MODE_COMPATABLE,
    MVR_MODE_DYNAMIC,
    MVR_MODE_END,
}mvr_mode_t;

typedef enum mcast_statType_e
{
    MCAST_STAT_TOTAL_RX,
    MCAST_STAT_VALID_RX,
    MCAST_STAT_INVALID_RX,
    MCAST_STAT_OTHER_RX,
    MCAST_STAT_LEAVE_RX,
    MCAST_STAT_REPORT_RX,
    MCAST_STAT_GENQRY_RX,
    MCAST_STAT_SPEC_GROUP_QRY_RX,
    MCAST_STAT_SPEC_GROUP_SRC_QRY_RX,
    MCAST_STAT_LEAVE_TX,
    MCAST_STAT_REPORT_TX,
    MCAST_STAT_GENQRY_TX,
    MCAST_STAT_SPEC_GROUP_QRY_TX,
    MCAST_STAT_SPEC_GROUP_SRC_QRY_TX,
    MCAST_STAT_END,
}mcast_statType_t;
typedef enum mld_version_e
{
    MLD_VERSION_V1 = 1,
    MLD_VERSION_V2_BASIC,
    MLD_VERSION_V2_FULL,
    MLD_VERSION_END,
} mld_version_t;

typedef struct mcast_mac_s
{
    multicast_ipType_t ipType;
    sys_vid_t vid;
    uint32   dip;
    uint8   dipv6[IPV6_ADDR_LEN];
    uint8  mac[CAPA_MAC_BYTE_NUM];
    sys_logic_portmask_t  portMbr;
}mcast_mac_t;

#ifdef CONFIG_SYS_PROTO_MVR
typedef struct mvr_config_s
{
    sys_enable_t  mvr_enable;
    mvr_mode_t   mode;
    uint32             queryTime;
    sys_vid_t       mvr_vid;
    mcast_groupAddr_t      groupIp_start;
    mcast_groupAddr_t      groupIp_end;
    mvr_portType_t  type[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t      fastLeaveEnable[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t      isMaxGroup;
    sys_enable_t      isFilter;
    sys_enable_t      isForward;
}mvr_config_t;
#endif

typedef struct igmp_stats_s
{
    uint32 igmpStatCnt[MCAST_STAT_END];
} igmp_stats_t;

typedef struct igmp_cfg_s
{
    sys_enable_t igmpsnp_enable;
    sys_enable_t   suppre_enable;
    igmp_version_t  oper_version;

    #ifdef CONFIG_SYS_PROTO_MVR
    mvr_config_t  mvrConfig;
    #endif

    uint32  port_entry_limit[SYS_LOGICPORT_NUM_MAX];
    uint32  max_groups_act[SYS_LOGICPORT_NUM_MAX];
}igmp_cfg_t;
typedef enum igmp_type_e
{
    IGMP_TYPE_MEMBERSHIP_QUERY     = 0x11,  
    IGMPV1_TYPE_MEMBERSHIP_REPORT  = 0x12,  
    IGMPV2_TYPE_MEMBERSHIP_REPORT  = 0x16,  
    IGMPV2_TYPE_MEMBERSHIP_LEAVE   = 0x17,  
    IGMPV3_TYPE_MEMBERSHIP_REPORT  = 0x22   
} igmp_type_t;

typedef enum ip_mcast_type_e
{
    IP_MCAST_IGMP_CONTROL,      
    IP_MCAST_DATA,              
} ip_mcast_type_t;

typedef enum mcast_rt_portType_e
{
    MCAST_RT_LRN_BY_PROTO,
    MCAST_RT_LRN_BY_PIM,
    MCAST_RT_TIMEOUT,
    MCAST_RT_END,
}mcast_rt_portType_t;

typedef enum mcast_operType_e
{
    MCAST_UPDATE,
    MCAST_ADD,
    MCAST_DEL,
    MCAST_OPER_END,
}mcast_operType_t;

typedef struct sys_mcast_event_s
{
    sys_vid_t vid;
    uint8      isAdd;
    sys_logic_portmask_t chgMbr;
}sys_mcast_event_t;

typedef struct sys_igmp_vlan_s
{
    sys_vid_t vid;
    sys_enable_t enable;
    sys_logic_portmask_t pmsk;
} sys_igmp_vlan_t;

typedef struct sys_igmp_portGroup_limit_s
{
    multicast_ipType_t ipType;
    sys_logic_port_t port;
    uint16                maxnum;
}sys_igmp_portGroup_limit_t;

typedef struct sys_igmp_maxGroup_act_s
{
    multicast_ipType_t ipType;
    sys_logic_port_t port;
    igmp_maxGroupAct_t act;
}sys_igmp_maxGroup_act_t;

typedef struct sys_igmp_filter_s
{
    multicast_ipType_t ipType;
    sys_logic_port_t port;
    sys_igmp_profileId_t idx;
}sys_igmp_filter_t;
typedef enum igmp_trunk_port_role_e
{
    IGMP_TRUNK_PORT_ROLE_NONE = 0,
    IGMP_TRUNK_PORT_ROLE_MAJOR,
    IGMP_TRUNK_PORT_ROLE_MINOR,
} igmp_trunk_port_role_t;
typedef struct sys_igmp_router_s
{
    uint8   port;
    uint8   routerIndx;
    sys_vid_t vid;
    uint32  ipAddr;
    uint32  timeout;
} sys_igmp_router_t;

typedef struct sys_mcast_info_s
{
    sys_enable_t enable;
    sys_enable_t suppreEnable;

    igmp_unknMcastAction_t unknMcastActn;
    igmp_version_t  operVersion;
    mld_version_t   oper_version;
    uint32 totStat[MCAST_STAT_END];
} sys_mcast_info_t;

typedef struct sys_mcast_portStat_s
{
    sys_logic_port_t lport;
    uint32 totStat[MCAST_STAT_END];
} sys_mcast_portStat_t;
typedef struct sys_igmp_profile_s
{
    multicast_ipType_t       ipType;
    sys_igmp_profileId_t    profileId;
    mcast_profileType_t     profileType;
    uint32      valid;
    uint32      lowIp;
    uint32      highIp;
    uint8        ipv6Low[IPV6_ADDR_LEN];
    uint8        ipv6High[IPV6_ADDR_LEN];
    mcast_profileAct_t  act;
}sys_igmp_profile_t;
typedef struct sys_igmp_func_s
{
    int32 (*igmp_group_timer)(void);
    int32 (*igmp_groupMbrPort_del_wrapper)(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr, sys_logic_port_t port, uint8 isMvrPackt);
    int32 (*igmp_group_mcstData_add_wrapper)(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t * pSip, sys_logic_port_t port, uint8 isMvrPackt);
    int32 (*igmpv3_group_mcstData_add_wrapper)(multicast_ipType_t ipType,uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t * pSip, sys_logic_port_t port,uint8 isMvrPackt);
} sys_igmp_func_t;
#define MLD_QUERY_V1                           1
#define MLD_QUERY_V2                           2
#define     MLD_TYPE_MEMBERSHIP_QUERY       130
#define     MLD_TYPE_MEMBERSHIP_REPORT      131
#define     MLD_TYPE_MEMBERSHIP_DONE          132

#define     MLD_ROUTER_SOLICITATION              133
#define     MLD_ROUTER_ADVERTISEMENT          134
#define     MLD_NEIGHBOR_SOLICITATION               135
#define     MLD_NEIGHBOR_ADVERTISEMENT              136
#define     MLD_REDIRECT                                      137

#define     MLDV2_TYPE_MEMBERSHIP_REPORT   143
typedef struct mld_stats_s
{
    uint32 mldStatCnt[MCAST_STAT_END];
} mld_stats_t;

typedef struct mld_cfg_s
{
    sys_enable_t   mldsnp_enable;
    sys_enable_t   suppre_enable;
    uint32  oper_version;
    uint32  port_entry_limit[SYS_LOGICPORT_NUM_MAX];
    uint32  max_groups_act[SYS_LOGICPORT_NUM_MAX];

    #ifdef CONFIG_SYS_PROTO_MVR
    mvr_config_t  mvrConfig;
    #endif
}mld_cfg_t;
#if 1
#define MCASTDBG(string, args...)         SYS_DBG(DBG_MCAST, string, ##args)
#else
#define MCASTDBG(string, args...)         printk("%s(%u) "string"\n", __FUNCTION__, __LINE__, ##args)
#endif
extern igmp_unknMcastAction_t mcast_unknownAct_ret(multicast_ipType_t ipType);
extern int32 mcast_table_clear(void);
extern int32 mcast_igmp_unknMcastAction_set(igmp_unknMcastAction_t action);
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

extern int32 mcast_portGroup_limit_set(multicast_ipType_t ipType, sys_logic_port_t port, uint16 maxnum);
extern int32 mcast_portGroup_excessAct_set(multicast_ipType_t ipType, sys_logic_port_t port, igmp_maxGroupAct_t act);
#endif
extern int32 mcast_portGroup_excessAct_get(multicast_ipType_t ipType, sys_logic_port_t port, igmp_maxGroupAct_t *pAct);
extern int32 mcast_portGroup_limit_get(multicast_ipType_t ipType,sys_logic_port_t port, uint16 *pMaxnum);
extern int32 mcast_vlan_set(mcast_vlan_entry_t *pEntry);
extern int32 mcast_vlan_get(mcast_vlan_entry_t *pEntry);
extern int32 mcast_vlan_del(sys_vid_t vid, multicast_ipType_t ipType);
extern int32 mcast_vlan_add(mcast_vlan_entry_t *pEntry);
extern int32 mcast_vlanMskConfig_get(multicast_ipType_t ipType, sys_vlanmask_t *pBitmap);
extern int32 mcast_router_get(igmp_router_entry_t *pRouter);
extern int32 mcast_router_set(igmp_router_entry_t *pRouter);
extern int32 mcast_router_dynPortMbr_del(sys_vlanmask_t vlanMsk, multicast_ipType_t ipType);
extern int32 mcast_database_get(sys_igmp_db_t *pDb);
extern int32 mcast_databaseSize_get(uint32 *pSize);
extern int32 mcast_igmp_databaseSize_get(uint32 *pSize);
extern int32 mcast_dipOnly_databaseSize_get(uint32 *pSize);
#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

extern int32 mcast_router_static_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);
extern int32 mcast_router_forbid_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);
#endif
extern int32 mcast_static_group_clear(igmp_group_entry_t *pGroup);
extern int32 mcast_basic_static_group_clear(igmp_group_entry_t *pGroup);
extern int32 mcast_basic_static_groupEntry_set(igmp_group_entry_t *pGroup, uint8 fmode,  uint8 addDel);
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

extern int32 mcast_profile_set(multicast_ipType_t ipType, sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);
extern int32 mcast_profile_get(multicast_ipType_t ipType, sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile);
extern int32 mcast_port_filter_set(multicast_ipType_t ipType, sys_logic_port_t port,sys_igmp_profileId_t  profileId);
extern int32 mcast_port_filter_get(multicast_ipType_t ipType, sys_logic_port_t port, sys_igmp_profileId_t  *pProfileId);
#endif

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

extern int32 mcast_igmp_groupHead_database_get(sys_igmp_groupHead_db_t *pGrpHeadDb);
extern int32 mcast_igmpv3_static_filter_group_set(igmp_group_entry_t *pGroup, uint8 fmode);
extern int32 mcast_igmpv3_static_groupEntry_set(sys_vid_t vid, uint32 groupAddr,  uint32 sourceAddr,
                                                                  sys_logic_portmask_t *pSpmsk,  uint8 fmode);

#endif
extern int32 mcast_igmp_statistics_clear(void);
extern int32 mcast_igmp_lookupMode_get(igmp_lookup_mode_t *mode);
extern int32 mcast_igmp_operVersion_get(igmp_version_t *pVersion);
extern int32 mcast_igmp_operVersion_set(uint8 operVersion);
extern int32 mcast_igmp_lookupMode_set(igmp_lookup_mode_t mode);
extern int32 mcast_igmp_suppre_enable_set(sys_enable_t enable);
extern int32 mcast_igmp_enable_set(sys_enable_t enable);
extern int32 mcast_igmp_info_get(sys_mcast_info_t *pInfo);
extern int32 mcast_igmp_portStat_get(sys_logic_port_t lport, sys_mcast_portStat_t *pPortStat);
extern int32 mcast_igmp_portStat_clear(sys_logic_port_t lport);
extern int32 mcast_igmp_group_del_byType( mcast_group_type_t type);
extern int32 mcast_ipv4_group_del_byType(mcast_group_type_t type);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
extern int32 mcast_mld_statistics_clear(void);
extern int32 mcast_mld_suppre_enable_set(sys_enable_t enable);
extern int32 mcast_mld_enable_set(sys_enable_t enable);
extern int32 mcast_mld_info_get(sys_mcast_info_t *pInfo);
extern int32 mcast_mld_lookupMode_get(igmp_lookup_mode_t *mode);
extern int32 mcast_mld_group_del_byType(mcast_group_type_t type);
extern int32 mcast_mld_operVersion_set(mld_version_t  operVersion);
extern int32 mcast_mld_databaseSize_get(uint32 *pSize);
 
extern int32 mcast_mld_unknMcastAction_set(uint8 action);

extern int32 mcast_mld_lookupMode_set(igmp_lookup_mode_t mode);
#endif

#ifdef CONFIG_SYS_PROTO_MVR
extern int32 mcast_mvr_pmsk_get(multicast_ipType_t ipType, mvr_portType_t portType, sys_logic_portmask_t *pMsk);
extern int32 mcast_mvr_portFastleave_get(multicast_ipType_t ipType,sys_logic_port_t port, sys_enable_t *pEnable);
extern uint8 is_mcast_mvr_enable(multicast_ipType_t ipType);
extern int32 mcast_mvr_vlanId_get(multicast_ipType_t ipType,sys_vid_t *pVid);
extern int32 mcast_mvr_portType_get(multicast_ipType_t ipType, sys_logic_port_t port , mvr_portType_t *pType);
extern int32 mcast_mvr_queryTime_get(multicast_ipType_t ipType, uint32 *pTime);
extern int32 mcast_igmp_mvr_enable_set(sys_enable_t enable);
extern int32 mcast_igmp_mvr_mode_set(mvr_mode_t mode);
extern int32 mcast_igmp_mvr_querytime_set(uint32 queryTime);
extern int32 mcast_igmp_mvr_vlanId_set(sys_vid_t vid);
extern int32 mcast_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp);
extern int32 mcast_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type);
extern int32 mcast_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable);
extern int32 mcast_igmp_mvrGroupNum_get(uint32 *pSize);
extern mvr_mode_t mcast_mvr_mode_ret(multicast_ipType_t ipType);
extern int32 mcast_mvr_group_del_byType( mcast_group_type_t type);

#endif 

int32 mcast_init(void);
int32 mcast_exit(void);

void mcast_log_tablefull(multicast_ipType_t ipType);
void mcast_log_GroupDb_failure(igmp_group_entry_t *pGroup, mcast_operType_t oper);
void mcast_log_asicMac_fail(sys_l2McastEntry_t *pEntry, mcast_operType_t oper);
void mcast_log_asicIP_fail(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry,mcast_operType_t oper);
void mcast_log_portLimit_arv(multicast_ipType_t ipType, sys_logic_port_t lp, uint32 maxNum);
void mcast_log_checksum_err(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp);
void mcast_log_vlan_disable(multicast_ipType_t ipType, sys_vid_t vid,sys_logic_port_t lp);
void mcast_log_rtPort(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp, mcast_rt_portType_t rtType);
void mcast_log_dipCollison(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry);
void mcast_log_dmacCollison(multicast_ipType_t ipType, sys_l2McastEntry_t *pEntry);

void mcast_log_stpblock(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp);
void mcast_log_invalidGroup(multicast_ipType_t ipType);
void mcast_log_ver_flood(multicast_ipType_t ipType, sys_vid_t vid);
void mcast_log_vlanFilter_drop(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp);
void mcast_log_group_notExsit(multicast_ipType_t ipType);
void mcast_dbg_jn_lv_report(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp, mcast_groupAddr_t *pAddr, uint8 isJn);

extern int32 mcast_static_l2_mcast_groupEntry_add(l2_mcast_group_t *pGroup);
extern int32 mcast_static_l2_mcast_groupEntry_del(l2_mcast_group_t *pGroup);
extern int32 mcast_static_l2_mcast_databaseSize_get(uint32 *pSize);
extern int32 mcast_static_l2_mcast_database_get(l2_mcast_group_t *pDb, uint32 index);

#endif

