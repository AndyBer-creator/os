
#ifndef __RSD_L3_H__
#define __RSD_L3_H__

#include <common/sys_def.h>
#include <common/sys_l3.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <rsd/rsd_l3_route.h>
#endif
#include <rsd/rsd_l3_intf.h>

#define L3_PROC_NAME    "l3"
#define L3_PROC_SEQNAME l3
#define L3_PROC_BUFLEN  (50 * CAPA_L3_ROUTE_IPV4_ENTRY_NUM)

#define L3_NEIGH_V4_PROC_NAME    "l3_neigh_v4"
#define L3_NEIGH_V4_PROC_SEQNAME l3_neigh_v4
#define L3_NEIGH_V6_PROC_NAME    "l3_neigh_v6"
#define L3_NEIGH_V6_PROC_SEQNAME l3_neigh_v6
#define L3_NEIGH_PROC_BUFLEN  (70 * CONFIG_SYS_L3_HOST_ROUTE_NUM)

typedef union sys_l3_event_u
{
    struct
    {
        sys_l3_intfId_t intfId;
    } intfExist;

    struct
    {
        sys_l3_intfId_t intfId;
        sys_l3_intfLinkStatus_t linkStatus;
    } intfLink;

    struct
    {
        sys_l3_intfId_t intfId;
        sys_l3_af_t   af;
        sys_l3_ip_t   ip;
    } intfIP;

    sys_l3_ipv4_t mgmtIPv4;
    sys_l3_ipv6_t mgmtIPv6;
} sys_l3_event_t;

#if 1
#define L3_DBG(cat, fmt, args...) SYS_DBG(cat, fmt, ##args)
#else
#define L3_DBG(cat, fmt, args...) SYS_PRINTF("%s(%u): "fmt, __FUNCTION__, __LINE__, ##args)
#endif
#define L3_SEM_LOCK(sem)   \
    do { } while (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define L3_SEM_UNLOCK(sem) \
    do { } while (osal_sem_mutex_give(sem) != SYS_ERR_OK)

#define DUMP_L3HR(__l3hr, __msg)     \
do{                                 \
    if (SYS_L3_AF_IPV6 == __l3hr->af) \
        L3_DBG(DBG_L3_ROUTE, "[%s] l3hr %u v6 "FMT_IPV6" mac "FMT_MAC" intfv%u age %u static %u\n", \
            __msg, __l3hr->idx, SHOW_IPV6(__l3hr->ip.v6), SHOW_MAC(__l3hr->mac.octet),              \
            __l3hr->vid, __l3hr->ageTime, __l3hr->type);                                            \
    else                                                                                            \
        L3_DBG(DBG_L3_ROUTE, "[%s] l3hr %u v4 "FMT_IPV4" mac "FMT_MAC" intfv%u age %u static %u\n", \
            __msg, __l3hr->idx, SHOW_IPV4(__l3hr->ip.v4), SHOW_MAC(__l3hr->mac.octet),              \
            __l3hr->vid, __l3hr->ageTime, __l3hr->type);                                            \
}while(0);

extern int32 rsd_l3_init(void);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 rsd_l3_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_l3_route_init(void);

extern int32 rsd_l3_routeV4_add(sys_l3_routeV4_t *pRoute);

extern int32 rsd_l3_routeV4_del(sys_l3_routeV4_t *pRoute);

extern int32 rsd_l3_routeV6_add(sys_l3_routeV6_t *pRoute);

extern int32 rsd_l3_routeV6_del(sys_l3_routeV6_t *pRoute);

extern int32 rsd_l3_routeDb_proc_dump(char *buf, int32 buflen, int32 *pLen);

#ifdef CONFIG_SYS_L3_ROUTE_RIP
extern int32 rsd_rip_enable_set(sys_enable_t enable);
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
extern int32 rsd_ospf_enable_set(sys_enable_t enable);
#endif

#endif
#endif 

