
#ifndef __RSD_IPMCAST_H__
#define __RSD_IPMCAST_H__

#include <rtk/l2.h>
#include <common/sys_def.h>
#include <rsd/rsd_sys.h>

typedef struct sys_ipmcast_entry_s
{
    int32   indx;
    uint32  dip;
    uint32  sip;
    uint32  vid;
    sys_logic_portmask_t portmask;
    sys_enable_t            crossVlan;
    rtk_mcast_group_t groupId;
} sys_ipMcastEntry_t;

typedef struct sys_ip6mcast_entry_s
{
    int32   indx;
    uint32  vid;
    sys_ipv6_addr_t dip;
    sys_ipv6_addr_t sip;
    sys_logic_portmask_t portmask;
    rtk_mcast_group_t groupId;
} sys_ip6McastEntry_t;

typedef struct sys_ipv46mcast_entry_s
{
    sys_ipMcastEntry_t     ipMcast;
    sys_ip6McastEntry_t   ipv6Mcast;
    sys_logic_portmask_t  portmask;
}sys_ipv46McastEntry_t;

typedef struct sys_l2mcast_entry_s
{
    uint8                   mac[ETHER_ADDR_LEN];
    int32                   indx;
    uint32                  vid;
    sys_logic_portmask_t    portmask;
    sys_enable_t            crossVlan;
} sys_l2McastEntry_t;

typedef struct  l2_mcast_vlan_db_s{
    uint32        flag;
    uint32       rowStatus;
    uint32      vlan_id;            
    uint8       mcast_mac[CAPA_MAC_BYTE_NUM]; 
		sys_logic_portmask_t   staticMbr;
		sys_logic_portmask_t   gmrpMbr;
}l2mcast_vlan_db_t;

typedef struct l2_mcast_group_s {
    uint8     mac[CAPA_MAC_BYTE_NUM];
    uint32    vlanId;     
		sys_logic_portmask_t   staticMbr;
		sys_logic_portmask_t   gmrpMbr;
} l2_mcast_group_t;

typedef struct rsd_ipmcast_mapper_s
{
    int32 (*rsd_ipMcastEntry_set)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntryIncCPU_set)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntry_del)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntry_get)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntry_getFirst)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntry_getNext)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ipMcastEntry_add)(sys_ipMcastEntry_t *pEntry);
    int32 (*rsd_ip6McastEntry_add)(sys_ip6McastEntry_t *pEntry);
    int32 (*rsd_ip6McastEntry_set)(sys_ip6McastEntry_t *pEntry);
    int32 (*rsd_ip6McastEntryIncCPU_set)(sys_ip6McastEntry_t *pEntry);
    int32 (*rsd_ip6McastEntry_del)(sys_ip6McastEntry_t *pEntry);
    int32 (*rsd_ip6McastEntry_get)(sys_ip6McastEntry_t *pEntry);
    int32 (*rsd_ip6_careByte_set)(uint32 dipcareByte, uint32  sipcareByte);
} rsd_ipmcast_mapper_t;

extern int32 rsd_ipmcast_init(void);

#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
    extern rsd_ipmcast_mapper_t _ipmcast_msw_mapper;
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    extern rsd_ipmcast_mapper_t _ipmcast_csw_mapper;
#endif

extern int32 rsd_l2McastEntry_add(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntry_set(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntryIncCPU_set(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntry_del(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntry_get(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntry_getFirst(sys_l2McastEntry_t *pEntry);

extern int32 rsd_l2McastEntry_getNext(sys_l2McastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_add(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_set(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntryIncCPU_set(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_del(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_get(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_getFirst(sys_ipMcastEntry_t *pEntry);

extern int32 rsd_ipMcastEntry_getNext(sys_ipMcastEntry_t *pEntry);

 extern int32 rsd_ip6McastEntry_add(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ip6McastEntry_set(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ip6McastEntryIncCPU_set(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ip6McastEntry_del(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ip6McastEntry_get(sys_ip6McastEntry_t *pEntry);

extern int32 rsd_ip6_careByte_set(uint32 dipcareByte, uint32  sipcareByte);
 
extern int32 rsd_l2_mcast_group_db_init(void);
extern uint32 rsd_l2_mcast_create(l2_mcast_group_t	 * mcast);
extern uint32 rsd_l2_mcast_port_add(l2_mcast_group_t	*mcast);
extern int rsd_vlan_used_by_l2mcast(uint32 vlanId);
extern uint32 rsd_l2_mcast_port_delete(l2_mcast_group_t  *mcast);
extern uint32 rsd_l2_mcast_delete(l2_mcast_group_t	*mcast);
extern int rsd_l2_mcast_entry_used_cnt(void);
extern int rsd_l2_mcast_is_exist_by_mac_vlan(uint32 vlanId, uint8 *mac);
extern int32 rsd_l2_mcast_lookup(uint32 vlanId, uint8 *mac, uint32 *found, uint32 *freeSlot);
extern int rsd_l2_mcast_entry_by_used_cnt(uint32 index, l2mcast_vlan_db_t  *mcast);

#endif 

