
#ifndef __RSD_L2_DB_H__
#define __RSD_L2_DB_H__

#include <drv/l2ntfy/l2ntfy.h>

typedef struct rsd_l2_notify_s {
    uint32                    unit;
    uint32                    num;
    rtk_l2ntfy_eventEntry_t *pEntries;
} rsd_l2_notify_t;

typedef struct rsd_l2_notifyHandler_s {
    rsd_l2_notify_t *pNotify[L2_DB_HANDLER_QUEUE_LENGTH];
    uint16          enQueueCnt;
    uint16          deQueueCnt;
} rsd_l2_notifyHandler_t;

extern int32 rsd_l2_db_init(void);

extern int32 rsd_l2_db_sync(void);

extern void rsd_l2_db_clear(sys_l2_clearCfg_t *pCfg);

extern int32 rsd_l2_db_gen(void);

extern int32 rsd_l2_dbEntry_add(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_dbEntry_del(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_dbEntry_get(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_dbEntryNotDynamic_get(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_dbMcastNumber_add(void);

extern int32 rsd_l2_dbMcastNumber_del(void);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 rsd_l2_dbL3Entry_update(void);

extern int32 rsd_l2_dbL3Entry_updateByPort(sys_logic_port_t port);
#endif

#endif 

