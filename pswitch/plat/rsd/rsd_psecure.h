
#ifndef __RSD_PSECURE_H__
#define __RSD_PSECURE_H__

#include <rsd/rsd_nic.h>

typedef enum sys_psecure_violation_e
{
    SYS_PSECURE_VIOLATION_PROTECT = 0,
    SYS_PSECURE_VIOLATION_RESTRICT,
    SYS_PSECURE_VIOLATION_SHUTDOWN,
    SYS_PSECURE_VIOLATION_END
} sys_psecure_violation_t;

typedef enum sys_psecure_clearType_e
{
    SYS_PSECURE_CLEAR_ALL_ALL = 0,
    SYS_PSECURE_CLEAR_ALL_BY_PORT,
    SYS_PSECURE_CLEAR_ALL_BY_MAC,
    SYS_PSECURE_CLEAR_DYNAMIC_ALL,
    SYS_PSECURE_CLEAR_DYNAMIC_BY_PORT,
    SYS_PSECURE_CLEAR_DYNAMIC_BY_MAC,
    SYS_PSECURE_CLEAR_STICKY_ALL,
    SYS_PSECURE_CLEAR_STICKY_BY_PORT,
    SYS_PSECURE_CLEAR_STICKY_BY_MAC,
    SYS_PSECURE_CLEAR_STATIC_ALL,
    SYS_PSECURE_CLEAR_STATIC_BY_PORT,
    SYS_PSECURE_CLEAR_STATIC_BY_MAC,
    SYS_PSECURE_CLEAR_END,
} sys_psecure_clearType_t;

typedef struct sys_psecure_port_s
{
    sys_enable_t            enable;
    sys_enable_t            sticky;
    uint32                  limitNum;
    sys_psecure_violation_t violation;
} sys_psecure_port_t;

typedef struct sys_psecure_cfg_s
{
    sys_enable_t       enable;
    uint32             rateLimit;
    sys_psecure_port_t portCfg[SYS_LOGICPORT_NUM_MAX];
} sys_psecure_cfg_t;

typedef struct sys_psecure_portCnt_s
{
    uint32 violateNum;
    uint32 stickyAddr;
    uint32 staticAddr;
    uint32 totalAddr;
    uint32 maxAddr;
} sys_psecure_portInfo_t;

typedef struct sys_psecure_clearCfg_s
{
    sys_psecure_clearType_t   type;

    union
    {
        sys_logic_port_t port;
        sys_l2_entry_t   entry;
    };
} sys_psecure_clearCfg_t;

#define PSECURE_PROC_NAME    "psecure"
#define PSECURE_PROC_SEQNAME psecure
#define PSECURE_PROC_BUFLEN  4096

#define IS_PSECURE_ENTRY_EMPTY(pEntry) ((NULL == pEntry) || (0 == pEntry->vid))

#define PSECURE_DBG(fmt, args...) \
do { \
    SYS_DBG(DBG_PSECURE, fmt, ##args); \
} while(0)
#define PSECURE_TMP_DBG(fmt, args...) SYS_PRINTF("%s(%u): "fmt"\n", __FUNCTION__, __LINE__, ##args)

#define PSECURE_VERBOSE_DEBUG 1
#if PSECURE_VERBOSE_DEBUG
#define _PSECURE_DBG(fmt, args...) SYS_PRINTF(fmt, ##args)
#else
#define _PSECURE_DBG(fmt, args...)
#endif

extern int32 rsd_psecure_init(void);

extern int32 rsd_psecure_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_psecure_dynamicMac_learn(sys_l2_entry_t *pEntry);

extern int32 rsd_psecure_dbMac_add(sys_l2_entry_t *pEntry);

extern int32 rsd_psecure_dbMac_del(sys_l2_entry_t *pEntry);

extern int32 rsd_psecure_mac_add(sys_l2_entry_t *pEntry);

extern int32 rsd_psecure_mac_del(sys_l2_entry_t *pEntry);

extern int32 rsd_psecure_enable_set(sys_enable_t enable);

extern int32 rsd_psecure_rateLimit_set(uint32 rate);

extern int32 rsd_psecure_port_set(sys_logic_port_t port, sys_psecure_port_t *pPortCfg);

extern int32 rsd_psecure_portInfo_get(sys_logic_port_t port, sys_psecure_portInfo_t *pInfo);

extern uint32 rsd_psecure_portAddrExceed_ret(sys_logic_port_t port);
#endif 

