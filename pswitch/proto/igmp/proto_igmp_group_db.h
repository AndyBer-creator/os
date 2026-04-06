
#ifndef __PROTO_IGMP_GROUP_DB_H__
#define __PROTO_IGMP_GROUP_DB_H__
#include <igmp/proto_igmp_rx.h>
#include <rsd/rsd_ipmcast.h>

#define MAX_ROUTER_VLAN      CAPA_VLAN_NUM

#define MAX_SRC_NUM_PER_GRP  32

typedef enum igmp_fwdFlag_e
{
    IGMPV3_FWD_NORMAL,
    IGMPV3_FWD_CONFLICT,
    IGMPV3_FWD_NOT
} igmp_fwdFlag_t;

typedef enum mcast_group_type_e
{
    IGMP_GROUP_ENTRY_DYNAMIC = (1 << 0),
    IGMP_GROUP_ENTRY_STATIC =  (1 << 1),
    IGMP_GROUP_ENTRY_STATIC_MAC = (1 << 2),
    IGMP_GROUP_ENTRY_ALL = (0x7),
    IGMP_GROUP_ENTRY_END
}mcast_group_type_t;

typedef enum mcast_app_type_e
{
    MCAST_ENTRY_SNP,
    MCAST_ENTRY_MVR,
    MCAST_ENTRY_ALL,
}mcast_app_type_t;

typedef enum igmp_router_port_type_e
{
    IGMP_ROUTER_PORT_DYNAMIC,
    IGMP_ROUTER_PORT_STATIC,
    IGMP_ROUTER_PORT_FORBID
}igmp_router_port_type_t;

typedef enum multicast_ipType_e
{
    MULTICAST_TYPE_IPV4,
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    MULTICAST_TYPE_IPV6,
#endif
	MULTICAST_TYPE_MCAST,
    MULTICAST_TYPE_END
} multicast_ipType_t;

typedef struct sys_igmp_db_s
{
    uint32  indx;
    uint32  dip;
    uint32  sip;
    uint8    dipv6[IPV6_ADDR_LEN];
    uint8    sipv6[IPV6_ADDR_LEN];
    uint32  life;
    uint8   mac[ETHER_ADDR_LEN];
    uint8   l2TableWriteFlag;
    sys_vid_t  vid;
    multicast_ipType_t ipType;
    sys_logic_portmask_t portmask; 

    struct
    {
        igmp_fwdFlag_t          fwdFlag[SYS_PORT_NUM_MAX];
        sys_logic_portmask_t    fliterFlag;
        uint8                   fmode;  
    }v3;

    mcast_group_type_t       form;
    sys_logic_portmask_t    staticMbr;
    sys_logic_portmask_t    staticInMbr;
} sys_igmp_db_t;

typedef struct sys_igmp_groupHead_db_s
{
    multicast_ipType_t    ipType;
    uint32                      indx;
    uint32                      dip;
    uint32                      sip;
    uint8                       mac[CAPA_MAC_BYTE_NUM];
    uint16                     mbrTimer[SYS_PORT_NUM_MAX];
    sys_vid_t                   vid;
    sys_logic_portmask_t        fmode;  
}sys_igmp_groupHead_db_t;

typedef struct igmp_router_entry_s
{
    multicast_ipType_t      ipType;
    sys_vid_t                   vid;
    uint8                          rtVersion;
    uint16                          portTimer[SYS_PORT_NUM_MAX];
    sys_logic_portmask_t    router_pmsk;

    sys_logic_portmask_t    router_static_pmsk;
    sys_logic_portmask_t    router_forbid_pmsk;
} igmp_router_entry_t;

typedef struct igmp_router_db_s
{
    igmp_router_entry_t     rt_entry[MAX_ROUTER_VLAN];
    uint16                  entry_num;
} igmp_router_db_t;

typedef struct igmp_aggregate_entry_s
{
    uint64                  sortKey;    
    uint8                   port_ref_cnt[SYS_PORT_NUM_MAX];
    uint16                  group_ref_cnt;
} igmp_aggregate_entry_t;

typedef struct igmp_aggregate_db_s
{
    uint16                  entry_num;
    igmp_aggregate_entry_t  *aggregate_entry;
} igmp_aggregate_db_t;

typedef struct igmp_group_head_entry_s
{
    multicast_ipType_t    ipType;
    uint32                      dip;
    uint32                      sip;
    sys_vid_t                   vid;
    uint8                       mac[CAPA_MAC_BYTE_NUM];
    uint16                     mbrTimer[SYS_PORT_NUM_MAX];
       sys_logic_portmask_t        fmode;  
}igmp_group_head_entry_t;
typedef struct mcast_groupAddr_s
{
    uint32   ipv4; 
    uint32   sipv4; 
    ipv6_addr_t  ipv6; 
    ipv6_addr_t  sipv6; 
}mcast_groupAddr_t;

typedef struct igmp_group_entry_s
{
    multicast_ipType_t     ipType;
    uint32                      dip;
    uint32                      sip;
    ipv6_addr_t             dipv6;
    ipv6_addr_t             sipv6;
    uint32                  groupId;
    sys_vid_t                   vid;
    uint8                       mac[CAPA_MAC_BYTE_NUM];
    uint64                      sortKey;
    sys_logic_portmask_t        mbr;
    sys_logic_portmask_t        fmode;  
    uint16                      mbrTimer[SYS_PORT_NUM_MAX];
    uint16                      groupTimer;
    uint8                       gsQueryCnt[SYS_PORT_NUM_MAX];
    uint8                       lookupTableFlag;
    uint8                       suppreFlag;

    struct
    {
        sys_logic_portmask_t     isGssQueryFlag;  
        igmp_fwdFlag_t              mbrFwdFlag[SYS_PORT_NUM_MAX];
        uint8                             gssQueryCnt[SYS_PORT_NUM_MAX];
        igmp_group_head_entry_t        *pGroupHead;
    }v3;

    mcast_group_type_t              form;
    sys_logic_portmask_t           staticMbr;
    sys_logic_portmask_t           staticInMbr;
    struct igmp_group_entry_s   *next_subgroup;
    struct igmp_group_entry_s   *next_free;

} igmp_group_entry_t;

typedef struct igmp_static_group_set_s
{
    igmp_group_entry_t  groupEntry;
    uint8                 fmode;
    uint8                 isAdd;
}igmp_static_group_set_t;

typedef struct igmp_group_head_s
{
    uint32              freeListNumber;
    igmp_group_entry_t  *item;
} igmp_group_head_t;

typedef struct igmp_static_group_s
{
    igmp_group_entry_t *pGroup;
    uint32   groupCnt;
}igmp_static_group_t;
typedef struct mcast_vlan_entry_s
{
    sys_vid_t                 vid;
    multicast_ipType_t     ipType;
    sys_enable_t            enable;
    sys_enable_t            fastleave_enable;
    sys_enable_t            pimDvmrpLearn;  
    sys_logic_portmask_t   staticMbr;  
    sys_logic_portmask_t   forbidMbr;  

    uint32  router_port_timeout;
    uint32  grpMembershipIntv;

    uint32  robustnessVar;
    uint32  operRobNessVar;
    uint32  responseTime;
    uint32  operRespTime;
    uint32  queryIntv;
    uint32  operQueryIntv;
    uint32  grpSpecificQueryIntv;       
    uint32  operGsqueryIntv;

    uint32  lastMmbrQueryCnt; 
    uint32  operLastMmbrQueryCnt;
    uint32  otherQuerierPresentInterval; 
} mcast_vlan_entry_t;

typedef struct mcast_vlan_db_s
{
    mcast_vlan_entry_t     vlan_entry[MAX_ROUTER_VLAN];
    uint16                  entry_num;
} mcast_vlan_db_t;

#if 0

typedef struct  l2_mcast_vlan_db_s{
    uint32        flag;
    uint32       rowStatus;
    uint32      vlan_id;            
    uint8       mcast_mac[CAPA_MAC_BYTE_NUM]; 
	sys_logic_portmask_t   staticMbr;
}l2mcast_vlan_db_t;

typedef struct l2_mcast_group_s {
    uint8     mac[CAPA_MAC_BYTE_NUM];
    uint32    vlanId;     
	sys_logic_portmask_t   staticMbr;
} l2_mcast_group_t;
#endif

#define PORT_IS_FILTER_EXCLUDE  IS_LOGIC_PORTMASK_PORTSET
#define PORT_SET_FILTER_EXCLUDE  LOGIC_PORTMASK_SET_PORT
#define PORT_SET_FILTER_INCLUDE  LOGIC_PORTMASK_CLEAR_PORT
#define PORT_IS_EXCLUDE_NOT_FWD         IS_LOGIC_PORTMASK_PORTSET
#define PORT_SET_EXCLUDE_NOT_FWD        LOGIC_PORTMASK_SET_PORT
#define PORT_SET_EXCLUDE_FWD                LOGIC_PORTMASK_CLEAR_PORT

#define PORT_IS_SRC_QUERY                       IS_LOGIC_PORTMASK_PORTSET
#define PORT_SET_SRC_NOT_QUERY          LOGIC_PORTMASK_CLEAR_PORT
#define PORT_SET_SRC_QUERY                  LOGIC_PORTMASK_SET_PORT
void mcast_group_rx_timer_stop(void);
void mcast_group_rx_timer_start(void);
void mcast_group_num_cal(multicast_ipType_t  ipType, uint8 isStaticMac, uint8 isMvrGroup, uint8 isInc);
int32 mcast_hw_group_num_cal(uint8 isInc);

int32 mcast_vlan_db_init(void);
int32 mcast_vlan_db_add(uint16 vid, multicast_ipType_t ipType);
int32 mcast_vlan_db_get(uint16 vid, multicast_ipType_t ipType, mcast_vlan_entry_t **ppEntry);
int32 mcast_vlan_db_set(mcast_vlan_entry_t *pEntry);
int32 mcast_vlan_db_del(uint16 vid, multicast_ipType_t ipType);
int32 mcast_group_sortedArray_search(uint64 search, uint16 *idx, igmp_group_entry_t **groupHead);
int32 mcast_db_init(void);
int32 mcast_router_db_init(void);
int32 mcast_router_db_get(uint16 vid,  multicast_ipType_t ipType, igmp_router_entry_t **ppEntry);
int32 mcast_router_db_set(igmp_router_entry_t *pEntry);
int32 mcast_router_db_del(uint16 vid, multicast_ipType_t ipType);
int32 mcast_routerPort_add_wrapper(uint16 vid, multicast_ipType_t ipType, sys_logic_port_t lPort, uint8 isProto);
int32 mcast_routerPort_get(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t *portmask);
int32 mcast_aggregate_db_init(void);
int32 mcast_aggregate_db_get(igmp_group_entry_t *pGroup, igmp_aggregate_entry_t **ppEntry,uint8 isNullErr);
int32 mcast_aggregate_db_del(igmp_group_entry_t *pGroup);
int32 mcast_aggregate_db_add(igmp_group_entry_t *pGroup, sys_logic_port_t port);
int32 mcast_aggregatePort_get(igmp_group_entry_t *pGroup, sys_logic_portmask_t *portmask);
int32 mcast_newGroup_aggPortMsk_add(igmp_group_entry_t *pEntry, sys_logic_portmask_t pmsk, uint8 isMvrPackt);
int32 mcastData_newGroup_add(igmp_group_entry_t *pEntry, uint8 isMvrPackt, uint16 sortedIdx);

int32 mcast_group_db_init(void);
int32 mcast_build_groupEntryKey(sys_vid_t vid, multicast_ipType_t ipType, uint32 dip,  char *ipv6Addr,  igmp_group_entry_t *pGroup);
uint64 mcast_group_sortKey_ret(igmp_group_entry_t *pGroup);
uint64 mcast_aggregate_sortKey_ret(igmp_group_entry_t *pGroup);
int32 mcast_group_add(igmp_group_entry_t *pNewEntry);
int32 mcast_group_del(igmp_group_entry_t *pGroup);
igmp_group_entry_t* mcast_group_get(igmp_group_entry_t *pGroup);
igmp_group_entry_t* mcast_group_get_with_index(uint32 index);
igmp_group_entry_t* mcast_dip_group_get_with_index(uint32 index);
int32 mcast_group_del_all(void);
int32 mcast_groupMbrPort_add_wrapper(multicast_ipType_t ipType, uint16 vid,
        sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr, sys_logic_port_t port, uint8 *pSendFlag, uint8 isMvrPackt);
int32 mcast_groupMbrPort_del_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr,  sys_logic_port_t port, uint8 isMvrPacket);
int32 mcast_groupMbrPort_del_wrapper_dmacvid(multicast_ipType_t ipType, uint16 vlanid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr,  sys_logic_port_t port, uint8 isMvrPackt);
int32 mcast_igmp_group_mcstData_add_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf,
        mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t * pSip,  sys_logic_port_t port, uint8 isMvrPackt);
int32 mcast_group_mcstData_add_wrapper_dmacvid(multicast_ipType_t ipType,uint16 vid, sys_nic_pkt_t *pBuf,
        mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t *pSip,sys_logic_port_t port, uint8 isMvrPackt);

igmp_group_entry_t* mcast_group_freeEntry_get(multicast_ipType_t ipType, uint8 isMvrPackt);
int32 mcast_group_entry_release(igmp_group_entry_t *entry);
int32 mcast_suppreFlag_clear(sys_vid_t vid, multicast_ipType_t ipType);
int32 mcast_group_portTime_update(multicast_ipType_t ipType , sys_vid_t vid, mcast_groupAddr_t *pAddr);
int32 mcast_groupPort_remove(igmp_group_entry_t *pGroup, sys_logic_port_t port);
int32 mcast_routerPortMbr_remove(multicast_ipType_t ipType, sys_vlanmask_t *pVlanMsk, sys_logic_portmask_t *pdelMbr);
int32 mcast_routerPortMbr_add(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t *pAddMbr);
int32 mcast_group_learnByData_del(multicast_ipType_t ipType);

int32 mcast_group_sortedArray_ins(uint16 sortedIdx, igmp_group_entry_t *entry);
int32 mcast_port_entry_get(multicast_ipType_t ipType, sys_logic_port_t port, uint32 *pEntryNum);
int32 mcast_maxGroupAct_process(multicast_ipType_t ipType, sys_vid_t vid, uint32 dip, uint32 sip,sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

int32 mcast_igmp_group_head_db_init(void);
int32 mcast_igmp_group_head_ins(uint32 groupAddr, uint16 vid, uint16 portid, uint16 rcdType, igmp_group_head_entry_t **ppGrpHead);
igmp_group_head_entry_t *mcast_igmp_group_head_get(uint32 groupAddr, uint16 vid, igmp_group_head_entry_t **pGrpHead);
int32 mcast_igmp_group_head_del(uint32 groupAddr,uint16 vid);
igmp_group_head_entry_t *mcast_igmp_free_group_head_get(void);

int32 mcast_igmpv3_grp_src_spec_get(uint32 groupAddr, uint16 vid, uint16 portid, uint32 *pNumSrc, uint32 *pSip);
int32 mcast_igmpv3_fwdPmsk_get(sys_logic_portmask_t *pPortMsk,  igmp_group_entry_t *group);
int32 mcast_igmpv3_groupMbrPort_isIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_groupMbrPort_isEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid, uint16 rcdType);
int32 mcast_igmpv3_groupMbrPort_toIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_groupMbrPort_block_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_ipMcstEntry_update_dipsip(uint16 vid, uint32 dip, uint32 sip, igmp_group_entry_t *pEntry);
#endif
int32 mcast_igmpv3_group_mcstData_add_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf,
        mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t * pSip, sys_logic_port_t port, uint8 isMvrGroup);
int32 mcast_igmpv3_basic_groupMbrPort_isIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_basic_groupMbrPort_isEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_basic_groupMbrPort_toEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_basic_groupMbrPort_toIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_igmpv3_basic_groupMbrPort_block_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid);

 #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
int32 mcast_router_static_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);
int32 mcast_router_forbid_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk);
#endif

int32 mcast_static_group_save(sys_vlanmask_t vlanMsk,multicast_ipType_t ipType);
int32 mcast_static_group_restore(void);

int32 mcast_filter_check(multicast_ipType_t ipType, sys_logic_port_t port,  mcast_groupAddr_t *pGroupAddr);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
int32 mcast_mldv2_basic_groupMbrPort_isIn_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_mldv2_basic_groupMbrPort_isEx_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_mldv2_basic_groupMbrPort_toEx_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_mldv2_basic_groupMbrPort_toIn_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid);
int32 mcast_mldv2_basic_groupMbrPort_block_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid);
#endif

#ifdef CONFIG_SYS_PROTO_MVR
int32 mcast_mvr_group_remove_port(sys_logic_port_t lPort);
int32 mcast_mvr_staticGroup_portType_chk(sys_logic_port_t lPort);
int32 mcast_mvr_staticGroup_dynMode_chk(void);
int32 mcast_mvr_groupSet_chk(multicast_ipType_t ipType, mcast_groupAddr_t *pStar, mcast_groupAddr_t *pEnd);
uint8 is_mvr_group_ret1(multicast_ipType_t ipType, sys_vid_t vlanid, mcast_groupAddr_t *pGroupAddr, uint8 isChkVlan);
uint8 is_mvr_group_ret2(igmp_group_entry_t *pGroup);
uint8 is_mcast_mvr_maxGroup(multicast_ipType_t ipType);
uint8 is_mcast_mvr_portFilter(multicast_ipType_t ipType);
uint8 is_mcast_mvr_forwardAll(multicast_ipType_t ipType);
uint8 mcast_mvr_fwdAll_ret(igmp_group_entry_t *pGroup);
#endif
uint8 is_mcast_group(igmp_group_entry_t *pGroup);

int32 mcast_portStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lport, uint32 statType);
int32 mcast_portMskStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t portMsk, uint32 statType);
int32 mcast_blockPortMskStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t blockPortMsk, uint32 statType);
int32 l2_mcast_group_db_init(void);
uint32 l2_mcast_create(l2_mcast_group_t   * mcast);
uint32 l2_mcast_port_add(l2_mcast_group_t  *mcast);
int vlan_used_by_l2mcast(uint32 vlanId);
uint32 l2_mcast_port_delete(l2_mcast_group_t  *mcast);
uint32 l2_mcast_delete(l2_mcast_group_t  *mcast);
int32 l2_mcast_lookup(uint32 vlanId, uint8 *mac, uint32 *found, uint32 *freeSlot);
int l2_mcast_entry_used_cnt(void);
int l2_mcast_is_exist_by_mac_vlan(uint32 vlanId, uint8 *mac);
int32 l2_mcast_lookup(uint32 vlanId, uint8 *mac, uint32 *found, uint32 *freeSlot);
int l2_mcast_entry_by_used_cnt(uint32 index, l2mcast_vlan_db_t  *mcast);

#endif 

