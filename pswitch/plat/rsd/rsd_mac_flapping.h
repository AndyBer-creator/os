
#ifndef __RSD_MAC_FLAPPING_H__
#define __RSD_MAC_FLAPPING_H__

#include <rsd/rsd_nic.h>

#define DFLT_MAC_FLAPPING_RATELIMIT    100
#define DFLT_MAC_FLAPPING_AGE          (60*5)

typedef enum sys_mac_flapping_violation_e
{
    SYS_MAC_FLAP_VIOLATION_PROTECT = 0,
    SYS_MAC_FLAP_VIOLATION_RESTRICT,
    SYS_MAC_FLAP_VIOLATION_SHUTDOWN,
    SYS_MAC_FLAP_VIOLATION_END
} sys_mac_flapping_violation_t;

typedef enum sys_mac_flapping_clearType_e
{
    SYS_MAC_FLAP_CLEAR_ALL_ALL = 0,
    SYS_MAC_FLAP_CLEAR_ALL_BY_PORT,
    SYS_MAC_FLAP_CLEAR_ALL_BY_MAC,
    SYS_MAC_FLAP_CLEAR_DYNAMIC_ALL,
    SYS_MAC_FLAP_CLEAR_DYNAMIC_BY_PORT,
    SYS_MAC_FLAP_CLEAR_DYNAMIC_BY_MAC,
    SYS_MAC_FLAP_CLEAR_STICKY_ALL,
    SYS_MAC_FLAP_CLEAR_STICKY_BY_PORT,
    SYS_MAC_FLAP_CLEAR_STICKY_BY_MAC,
    SYS_MAC_FLAP_CLEAR_STATIC_ALL,
    SYS_MAC_FLAP_CLEAR_STATIC_BY_PORT,
    SYS_MAC_FLAP_CLEAR_STATIC_BY_MAC,
    SYS_MAC_FLAP_CLEAR_END,
} sys_mac_flapping_clearType_t;

typedef struct sys_mac_flapping_port_s
{
    sys_enable_t            enable;
    sys_enable_t            sticky;
    uint32                  limitNum;
    sys_mac_flapping_violation_t violation;
} sys_mac_flapping_port_t;

typedef struct sys_mac_flapping_cfg_s
{
    sys_enable_t       enable;
    uint32             rateLimit;
    sys_mac_flapping_port_t portCfg[SYS_LOGICPORT_NUM_MAX];
} sys_mac_flapping_cfg_t;

typedef struct sys_mac_flapping_portCnt_s
{
    uint32 violateNum;
    uint32 stickyAddr;
    uint32 staticAddr;
    uint32 totalAddr;
    uint32 maxAddr;
} sys_mac_flapping_portInfo_t;

typedef struct sys_mac_flapping_clearCfg_s
{
    sys_mac_flapping_clearType_t   type;

    union
    {
        sys_logic_port_t port;
        sys_l2_entry_t   entry;
    };
} sys_mac_flapping_clearCfg_t;

#define MAC_FLAP_PROC_NAME    "mac_flapping"
#define MAC_FLAP_PROC_SEQNAME  mac_flapping
#define MAC_FLAP_PROC_BUFLEN  4096

#define IS_MAC_FLAP_ENTRY_EMPTY(pEntry) ((NULL == pEntry) || (0 == pEntry->vid))

#define MAC_FLAP_DBG(fmt, args...) \
do { \
    SYS_DBG(DBG_PSECURE, fmt, ##args); \
} while(0)
#define MAC_FLAP_TMP_DBG(fmt, args...) SYS_PRINTF("%s(%u): "fmt"\n", __FUNCTION__, __LINE__, ##args)

#define MAC_FLAP_VERBOSE_DEBUG 1
#if MAC_FLAP_VERBOSE_DEBUG
#define _MAC_FLAP_DBG(fmt, args...) SYS_PRINTF(fmt, ##args)
#else
#define _MAC_FLAP_DBG(fmt, args...)
#endif

extern int32 rsd_mac_flapping_init(void);

extern int32 rsd_mac_flapping_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_mac_flapping_dynamicMac_learn(sys_l2_entry_t *pEntry);

extern int32 rsd_mac_flapping_dbMac_add(sys_l2_entry_t *pEntry);

extern int32 rsd_mac_flapping_dbMac_del(sys_l2_entry_t *pEntry);

extern int32 rsd_mac_flapping_mac_add(sys_l2_entry_t *pEntry);

extern int32 rsd_mac_flapping_mac_del(sys_l2_entry_t *pEntry);

extern int32 rsd_mac_flapping_enable_set(sys_enable_t enable);

extern int32 rsd_mac_flapping_rateLimit_set(uint32 rate);

extern int32 rsd_mac_flapping_port_set(sys_logic_port_t port, sys_mac_flapping_port_t *pPortCfg);

extern int32 rsd_mac_flapping_portInfo_get(sys_logic_port_t port, sys_mac_flapping_portInfo_t *pInfo);

extern uint32 rsd_mac_flapping_portAddrExceed_ret(sys_logic_port_t port);

extern int32 rsd_mac_flapping_action_recovery(sys_move_entry_t *pEntry);
#endif 

