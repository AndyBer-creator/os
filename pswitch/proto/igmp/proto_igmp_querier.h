
#ifndef __PROTO_IGMP_QUERIER_H__
#define __PROTO_IGMP_QUERIER_H__

#define IGMPV3_HEAD_LEN         4
#define IGMPV3_OPTIONS_LEN      4

enum
{
    IGMP_QUERIER_STATE_INIT = 0,
    IGMP_QUERIER_STATE_LEAVING_HOST, 
    IGMP_QUERIER_STATE_OTHER_HOST, 
};

enum
{
    IGMP_NON_QUERIER,
    IGMP_QUERIER,
};

enum
{
    IGMP_GENERAL_QUERY,
    IGMP_SPECIFIC_GROUP_QUERY,
    IGMP_SPECIFIC_GROUP_SRC_QUERY,
};

typedef struct sys_igmp_querier_s
{
    uint16  vid;
    multicast_ipType_t ipType;
    uint8   enabled;
    uint8   status;
    uint8   version;
    uint32  ipAddr;
} sys_igmp_querier_t;

typedef struct igmp_querier_entry_s
{
    multicast_ipType_t ipType;
    uint32 ip; 
    uint8   ipv6[IPV6_ADDR_LEN];
    uint32 vid;
    uint32 status;
    uint32 enabled;
    uint32 version;
    uint32 configured; 
    uint16 startupQueryCnt;
    uint16 timer;
} igmp_querier_entry_t;

typedef struct igmp_querier_db_s
{
	igmp_querier_entry_t    querier_entry[MAX_ROUTER_VLAN];
	uint16                  entry_num;
} igmp_querier_db_t;

typedef struct igmp_querier_ip_s
{
    uint32 ip;
    uint32 vid;
} igmp_querier_ip_t;
#define IGMP_FILL_QUERY(pkt, field, offset, len) \
do {\
    osal_memcpy(&(pkt)->data[offset], (uint8*)(field), len);\
}while(0)

int32 mcast_querier_db_init(void);
int32 mcast_querier_db_add(uint16 vid, multicast_ipType_t ipType);
int32 mcast_querier_db_get(uint16 vid, multicast_ipType_t ipType, igmp_querier_entry_t **ppEntry);
int32 mcast_querier_db_del(uint16 vid, multicast_ipType_t ipType);
int32 mcast_igmp_querier_check(uint16 vid, uint32 sip, uint8 query_version);
void mcast_igmp_querier_timer(void);
void mcast_igmp_send_grp_specific_query(uint16 vid, uint32 dip, sys_logic_port_t port);
void mcast_igmp_send_grp_src_specific_query(uint16 vid, uint32 dip, uint32 *sip, uint16 numSrc, sys_logic_port_t port);
void mcast_igmp_build_general_query (sys_nic_pkt_t * pkt, sys_vid_t vid,uint16 pktLen, uint8 version, uint32 queryEnable,uint32 gdaddr, uint8 query_type);
void mcast_igmp_send_general_query(igmp_querier_entry_t* qryPtr, uint8 igmp_query_version);

#ifdef CONFIG_SYS_PROTO_MVR
void mcast_igmp_mvr_send_specific_query(uint16 vid, uint32 dip, sys_logic_port_t port);
#endif

void mcast_send_general_query(igmp_querier_entry_t *qryPtr, uint8 version);
void mcast_send_gs_query(igmp_group_entry_t *pGroup, sys_logic_port_t lPort);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

extern int32 mcast_igmp_querier_set(sys_igmp_querier_t *pQuerier);
extern int32 mcast_igmp_querier_get(sys_igmp_querier_t *pQuerier);
extern int32 mcast_querier_enable_set(multicast_ipType_t ipType, sys_vlanmask_t *pVlanMsk, sys_enable_t enable);
#endif
extern int32 mcast_querier_init(void);

