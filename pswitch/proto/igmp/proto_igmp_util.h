
#ifndef __PROTO_IGMP_UTIL_H__
#define __PROTO_IGMP_UTIL_H__

#include <rsd/rsd_nic.h>
#include <rsd/rsd_ipmcast.h>
#include <igmp/proto_igmp_group_db.h>

#define IGMP_IN_CLASSD(a)        ((((long int) (a)) & 0xf0000000) == 0xe0000000)
#define IGMP_IN_MULTICAST(a)     IGMP_IN_CLASSD(a)

#define MLD_IN_MULTICAST(a)     ((a[0]) == 0xff)
#define IGMP_PKT_SEM_LOCK()    \
do {\
    if (osal_sem_mutex_take(igmp_sem_pkt, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_MCAST, "IGMP pkt semaphore lock failed");\
    }\
} while(0)

#define IGMP_PKT_SEM_UNLOCK() \
do {\
    if (osal_sem_mutex_give(igmp_sem_pkt) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_MCAST, "IGMP pkt semaphore unlock failed");\
    }\
} while(0)

#define IGMP_DB_SEM_LOCK()    \
do {\
    if (osal_sem_mutex_take(igmp_sem_db, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_MCAST, "IGMP db semaphore lock failed");\
    }\
} while(0)

#define IGMP_DB_SEM_UNLOCK() \
do {\
    if (osal_sem_mutex_give(igmp_sem_db) != SYS_ERR_OK)\
    {\
        SYS_DBG(DBG_MCAST, "IGMP db semaphore unlock failed");\
    }\
} while(0)
#define IS_IPADDR_EQUAL_UINT32(ip_u32, ip2)   \
    ((((ip_u32 >> 24) & 0xff) == ip2[0]) && \
     (((ip_u32 >> 16) & 0xff) == ip2[1]) && \
     (((ip_u32 >> 8) & 0xff) == ip2[2]) && \
     ((ip_u32 & 0xff) == ip2[3]))
#define MCAST_MAC2DIP(mac, dip) \
do { \
    if ((mac[0] == 0x33) && (mac[1] == 0x33)) \
    {   dip =  ((mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | (mac[5])); } \
    else \
    {  dip = ((mac[3] << 16) | (mac[4] << 8) | (mac[5]));  } \
}while(0)

#define IPV6_TO_IPV4(u8ipv6) \
    ((uint32)u8ipv6[12] << 24) | ((uint32)u8ipv6[13] << 16)  |((uint32)u8ipv6[14] << 8) |((uint32)u8ipv6[15] & 0xff)
#define IPV6_SIP_TO_IPV4(u8ipv6) \
    ((uint32)u8ipv6[10] << 24) | ((uint32)u8ipv6[13] << 16)  |((uint32)u8ipv6[14] << 8) |((uint32)u8ipv6[15] & 0xff)
int32 mcast_hwLgMask2swLgMask_get(sys_logic_portmask_t *pHwMask);
int32 mcast_ipMcast_asicKey_get(sys_vid_t vid, uint32 dip, uint32 sip, sys_ipv46McastEntry_t *pEntry);
int32 mcast_ipv6Mcast_asicKey_get(sys_vid_t vid, char *ipv6_dip, char *ipv6_sip, sys_ipv46McastEntry_t *pEntry);
igmp_group_entry_t *mcast_ip6DipvidMcast_MaxLifeEntry_get(igmp_group_entry_t *pGroup, sys_ipv46McastEntry_t *pIp6Mcast);

int32 mcast_max_life_get(igmp_group_entry_t *group_entry);
int32 mcast_hw_mcst_mbr_remove(igmp_group_entry_t *group_entry, sys_logic_portmask_t *delPmsk);
int32 mcast_hw_mcst_entry_del(igmp_group_entry_t *group_entry);
int32 mcast_hw_l2McastEntry_add(multicast_ipType_t ipType, sys_l2McastEntry_t *pEntry, uint8 isFwdAll);
int32 mcast_hw_l2McastEntry_set(multicast_ipType_t ipType,  sys_l2McastEntry_t *pEntry, uint8 isMoveTrunkMbr, uint8 isFwdAll);
int32 mcast_hw_l2McastEntry_get(sys_l2McastEntry_t *pEntry);
int32 mcast_hw_l2McastEntry_del(sys_l2McastEntry_t *pEntry);

int32 mcast_hw_ipMcastEntry_add(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry, uint8 isFwdAll);
int32 mcast_hw_ipMcastEntry_set(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry,uint8 isMoveTrunk, uint8 isFwdAll);
int32 mcast_hw_ipMcastEntry_get(igmp_group_entry_t *pGroup, sys_ipv46McastEntry_t *pEntry);
int32 mcast_hw_ipMcastEntry_del(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry);
int32 mcast_hw_ipMcastEntryIncCPU_set(multicast_ipType_t ipType,sys_ipv46McastEntry_t *pEntry);
int32 mcast_hw_l2Mcast_validMbr_get(sys_logic_portmask_t *pPmsk, sys_logic_portmask_t *pResultPmsk);

#endif 

