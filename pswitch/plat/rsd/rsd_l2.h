
#ifndef __RSD_L2_H__
#define __RSD_L2_H__

#include <rtk/l2.h>
#include <common/sys_def.h>
#include <rsd/rsd_l2_db.h>

typedef rtk_l2_vlanMode_t sys_l2_vlanMode_t;
typedef rtk_l2_portMoveAct_t sys_portMoveAct_t;      
typedef rtk_l2_portMoveLrn_t sys_portMoveLrn_t;

extern osal_mutex_t g_l2_sem;

#define IS_L2_DBG() IS_DBG_SET(DBG_L2)
#define L2_TMP_DBG(fmt, args...) SYS_PRINTF("%s(%u): "fmt"\n", __FUNCTION__, __LINE__, ##args)
#define L2_DBG(fmt, args...) SYS_DBG(DBG_L2, fmt, ##args)
#define L2_DBG_MON(fmt, args...) L2_DBG(fmt, ##args)

#define L2_SEM_LOCK()   \
do { } while (osal_sem_mutex_take(g_l2_sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define L2_SEM_UNLOCK() \
do { } while (osal_sem_mutex_give(g_l2_sem) != SYS_ERR_OK)

#define L2_TABLE_ACCESS(op, ret)        \
do {                                    \
    L2_SEM_LOCK();                      \
    ret = op;                           \
    L2_SEM_UNLOCK();                    \
    ret = sys_errXlate_ret(ret);        \
} while(0)

#define L2_TABLE_ACCESS_XLATE_CHK(op)       \
do {                                        \
    int32 _sysErr = 0;                      \
    L2_TABLE_ACCESS(op, _sysErr);           \
    if ((_sysErr != SYS_ERR_OK) &&          \
        (_sysErr != SYS_ERR_PORT_ID))       \
        return _sysErr;                     \
} while(0)

typedef rtk_l2_ucastAddr_t rsd_l2_ucastAddr_t;

typedef struct sys_l2_event_s
{
    uint32 mcastEntryNum;
} sys_l2_event_t;

extern int32 rsd_l2_init(void);

extern int32 rsd_l2_aging_set(uint32 aging_time);

extern int32 rsd_l2_aging_get(uint32 *pAging_time);

extern int32 rsd_l2_mac_add(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_mac_del(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_mac_search(sys_l2_entry_t *pEntry);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 rsd_l2_mac4RouteFlood_add(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_mac4RouteFloodHwOnly_add(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_mac4Route_set(sys_l2_entry_t *pEntry);

extern int32 rsd_l2_mac4Route_search(sys_l2_entry_t *pEntry);
#endif

extern int32 rsd_l2_mac_clear(sys_l2_clearCfg_t *pCfg);

extern int32 rsd_l2_mac_clearByPort(sys_logic_port_t port);

extern int32 rsd_l2_dynamicMacNum_get(uint32 *pNum);

extern int32 rsd_l2_portDynamicMacNum_ret(sys_logic_port_t port);

#endif 

