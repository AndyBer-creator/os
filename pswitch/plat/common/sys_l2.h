
#ifndef __COMMON_SYS_L2_H__
#define __COMMON_SYS_L2_H__
#include <common/sys_type.h>
#define L2DB_MAJOR 127
#define L2DB_NAME  "l2db"
#define L2DB_DEV   "/dev/"L2DB_NAME

#define L2_DB_ENTRY_EMPTY -1
#define L2_DB_ENTRY_FULL  -1
#define CAPA_L2_MAC_HASH_ENTRY_NUM     256
#define L2_DB_HANDLER_QUEUE_LENGTH     512
#define L2_LOGIC_PORT_DONT_CARE        0xFFFFFF
#define CAPA_L2_MAC_DY_PORT_MOVE_COUNT     10

typedef enum sys_l2_type_e {
    SYS_L2_TYPE_DYNAMIC = 0,
    SYS_L2_TYPE_SUSPEND,
    SYS_L2_TYPE_MGMT,
    SYS_L2_TYPE_STATIC,
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    SYS_L2_TYPE_BLACKLIST,
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_L2_TYPE_AUTH,
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    SYS_L2_TYPE_PSECURE_DYNAMIC,
    SYS_L2_TYPE_PSECURE_STICKY,
    SYS_L2_TYPE_PSECURE_STATIC,
#endif
    SYS_L2_TYPE_NEXTHOP_FLOOD,
    SYS_L2_TYPE_L3_MGMT,
    SYS_L2_TYPE_END
} sys_l2_type_t;

#if 0
typedef enum sys_l2_staticMode_e {
    SYS_L2_STATIC_MODE_PERMANENT = 0,
    SYS_L2_STATIC_MODE_END
} sys_l2_staticMode_t;
#endif

typedef enum sys_l2_clearType_e
{
    SYS_L2_CLEAR_ALL = 0,
    SYS_L2_CLEAR_BY_PORT,
    SYS_L2_CLEAR_BY_VLAN,
    SYS_L2_CLEAR_END,
} sys_l2_clearType_t;

typedef enum sys_l2_sortData_e
{
    SYS_L2_SORTDATA_DBENTRY,
    SYS_L2_SORTDATA_ENTRY,
    SYS_L2_SORT_END
} sys_l2_sortData_t;

typedef enum sys_l2_sortMeth_e
{
    SYS_L2_SORTMETH_MACVLAN = 0,
    SYS_L2_SORTMETH_VLANMAC,
    SYS_L2_SORTMETH_END
} sys_l2_sortMeth_t;

typedef enum sys_move_action_e
{
    SYS_L2_MOVE_ACT_FORWARD= 0,
    SYS_L2_MOVE_ACT_DROP,
    SYS_L2_MOVE_ACT_COPY_CPU,
    SYS_L2_MOVE_ACT_TRAP_CPU,
    SYS_L2_MOVE_ACT_END
} sys_move_action_t;

typedef struct sys_move_entry_s {
    sys_vid_t               vid;
    sys_mac_t               mac;
    sys_logic_portmask_t    portMask;
	sys_move_action_t       triggerAct;
	sys_move_action_t       recoverAct;
	uint16                  moveCount;
	uint16                  recoverTimes;
	uint16                  active;
} sys_move_entry_t;

typedef struct sys_l2_entry_s {
    sys_vid_t           vid;
    sys_mac_t           mac;
    sys_logic_port_t    port;
    sys_l2_type_t       type;
#ifdef CONFIG_SYS_L3_ROUTE
    uint32              nexthop;
    uint32              hwIdx;
    uint32              routeIdx;
#endif
} sys_l2_entry_t;

typedef struct sys_l2_clearCfg_s
{
    sys_l2_clearType_t   type;

    union
    {
        sys_logic_port_t port;
        sys_vid_t        vid;
        sys_mac_t        mac;
    };
} sys_l2_clearCfg_t;

typedef struct sys_l2_hashHead_s {
    uint16            num;
    uint32            entryIdx;
} sys_l2_hashHead_t;

typedef struct sys_l2_dbEntry_s {
    sys_l2_entry_t l2Entry;
    uint8          active;
    int32          nextEntryIdx;
} sys_l2_dbEntry_t;

typedef struct sys_l2_dbLink_s {
    int32          dbIdx;
} sys_l2_dbLink_t;

typedef struct sys_l2_dbSort_s {
    int32          entryIdx;
} sys_l2_dbSort_t;

typedef struct sys_l2_dbNum_s {
    uint32 hwTotal;
    uint32 hwDisable;
    uint32 dynamics;
    uint32 portDynamics[SYS_PORT_NUM_MAX];
    uint32 multicasts;
    uint32 statics;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    uint32 blacklist;
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    uint32 nexthops;
    uint32 nexthopsAct;
    uint32 nexthopsDyn;
    uint32 portNexthopsDyn[SYS_PORT_NUM_MAX];
#endif
} sys_l2_dbNum_t;

typedef struct sys_l2_dbOffset_s {
    uint32 hash;
    uint32 entries;
    uint32 statics;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
    uint32 blacklist;
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    uint32 route;
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
    uint32 psecure;
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
	uint32 move;
#endif
    uint32 sort;
} sys_l2_dbOffset_s;

typedef struct sys_l2_dbHead_s {
    uint32            dbSize;
    uint32            swDbMode;
    uint32            useNotify;
    sys_l2_dbNum_t    entryNum;
    sys_l2_dbOffset_s offset;
} sys_l2_dbHead_t;

extern sys_l2_dbHead_t   *g_l2Db;
extern sys_l2_hashHead_t *g_l2DbHash;
extern sys_l2_dbEntry_t  *g_l2DbEntries;
extern sys_l2_dbLink_t   *g_l2DbStatic;
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
extern sys_l2_dbLink_t   *g_l2DbBlacklist;
#endif
#ifdef CONFIG_SYS_L3_ROUTE
extern sys_l2_dbLink_t   *g_l2DbRoute;
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
extern sys_l2_entry_t    *g_l2DbPsecure;
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
extern sys_move_entry_t   *g_l2DbMove;
#endif
extern sys_l2_dbSort_t   *g_l2DbSort;

extern const sys_text_t text_l2_type[];
#ifdef __KERNEL__
#define L2_DB_MAP() (TRUE)
#else
#define L2_DB_MAP() (sys_l2_dbMem_map())
#endif
#define L2_HW_ENTRY_NUM (sys_l2_maxEntryNum_ret())
#define L2_HW_DISABLE_LEARN_NUM (sys_l2_disableLearningNum_ret())
#define L2_DB_ENTRY_PTR(idx) (g_l2DbEntries + idx)
#define L2_ENTRY_PTR(idx) ((L2_DB_ENTRY_EMPTY == idx) ? NULL : (&g_l2DbEntries[idx].l2Entry))
#define L2_SORT_ENTRY_PTR(idx) (&g_l2DbEntries[g_l2DbSort[idx].entryIdx].l2Entry)
#define L2_STATIC_ENTRY_DBIDX(idx) (g_l2DbStatic[idx].dbIdx)
#define L2_STATIC_ENTRY_NEXT(pPrev) (sys_l2_dbLinkNext_ret(g_l2DbStatic, CAPA_L2_STATIC_MAC_NUM, pPrev))
#define FOR_EACH_L2_STATIC_ENTRY(_i, _pCurr)                                \
    for (_i = 0; _i < CAPA_L2_STATIC_MAC_NUM; _i++)                         \
        if (L2_DB_MAP())                                                    \
            if (NULL != (_pCurr = L2_ENTRY_PTR(L2_STATIC_ENTRY_DBIDX(_i))))
#ifdef CONFIG_SYS_BLACK_HOLE_MAC
#define L2_BLACKLIST_ENTRY_DBIDX(idx) (g_l2DbBlacklist[idx].dbIdx)
#define L2_BLACKLIST_ENTRY_NEXT(pPrev) (sys_l2_dbLinkNext_ret(g_l2DbBlacklist, CAPA_L2_FILTER_MAC_NUM, pPrev))
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
#define L2_PSECURE_ENTRY_PTR(port, idx) (g_l2DbPsecure + ((port * CAPA_PSECURE_LIMIT_NUM) + idx))
#endif
#ifdef CONFIG_SYS_L3_ROUTE
#define L2_PROP_FLOODPORT_SET(pEntry) ((pEntry)->port = L2_LOGIC_PORT_DONT_CARE)
#define L2_PROP_NEXTHOP_SET(pEntry) ((pEntry)->nexthop = TRUE)
#define L2_PROP_NEXTHOP_CLEAR(pEntry) ((pEntry)->nexthop = FALSE)
#define IS_L2_ENTRY_NEXTHOP(pEntry) (TRUE == (pEntry)->nexthop)
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
#define IS_L2_ENTRY_DYNAMIC(pEntry) ((SYS_L2_TYPE_DYNAMIC == (pEntry)->type) || (SYS_L2_TYPE_PSECURE_DYNAMIC == (pEntry)->type))
#else
#define IS_L2_ENTRY_DYNAMIC(pEntry) (SYS_L2_TYPE_DYNAMIC == (pEntry)->type)
#endif
#ifdef CONFIG_SYS_FLAPPING_MAC
#define L2_MACFLAP_ENTRY_PTR(idx) (g_l2DbMove + idx)
#endif

#define IS_L2_SWDB_ENABLE() (ENABLED == g_l2Db->swDbMode)
#define IS_L2_SWDB_USENOTIFY() (ENABLED == g_l2Db->useNotify)
#define IS_L2_ENTRY_EQUAL(pSrc, pDst) (((pSrc)->vid == (pDst)->vid) && (0 == osal_memcmp(&(pSrc)->mac, &(pDst)->mac, CAPA_MAC_BYTE_NUM)))
#define IS_L2_ENTRY_ALL_EQUAL(pSrc, pDst) (0 == osal_memcmp(pSrc, pDst, sizeof(sys_l2_entry_t)))
#define IS_HASH_ACTIVE(idx) (g_l2DbHash[idx].num > 0)
#define IS_NEXT_ENTRY_EXIST(pEntry) (L2_DB_ENTRY_EMPTY != pEntry->nextEntryIdx)

#define FMT_MAC "%02x:%02x:%02x:%02x:%02x:%02x"
#define SHOW_MAC(addr) \
        addr[0], \
        addr[1], \
        addr[2], \
        addr[3], \
        addr[4], \
        addr[5]
extern int32 sys_l2_init(void);
extern uint32 sys_l2_dbMem_map(void);
extern uint32 sys_l2_maxEntryNum_ret(void);
extern uint32 sys_l2_disableLearningNum_ret(void);
extern int32 sys_l2_dbNum_get(sys_l2_dbNum_t *pNums);
extern sys_l2_entry_t *sys_l2_dbLinkNext_ret(sys_l2_dbLink_t *pHead, uint32 maxEntries, sys_l2_entry_t *pPrevEntry);

extern void sys_l2_qsort(void *base, uint32 num, uint32 width, sys_l2_sortData_t data, sys_l2_sortMeth_t meth);
#endif  

