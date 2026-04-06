
#ifndef __SAL_L2_H__
#define __SAL_L2_H__

#include <rsd/rsd_l2.h>

#define IS_SYS_MAC(pEntry) sal_l2_isSysMac_ret(pEntry)
#define IS_MCAST_MAC(pEntry) sal_l2_isMcastMac_ret(pEntry)
#define IS_ALLZERO_MAC(pEntry) sal_l2_isAllZeroMac_ret(pEntry)

extern int32 sal_l2_db_gen(void);

extern int32 sal_l2_sort_start(sys_l2_sortMeth_t meth, uint32 *pNum);

extern int32 sal_l2_mac_search(sys_l2_entry_t *pEntry);

extern sys_l2_entry_t *sal_l2_macNext_ret(sys_l2_entry_t *pPrevEntry);

extern uint32 sal_l2_isSysMac_ret(sys_l2_entry_t *pChkEntry);

extern uint32 sal_l2_isMcastMac_ret(sys_l2_entry_t *pChkEntry);

extern uint32 sal_l2_isAllZeroMac_ret(sys_l2_entry_t *pChkEntry);

extern int32 sal_l2_dynamicMac_del(sys_l2_entry_t *pEntry);

extern int32 sal_l2_dynamicMac_clear(void);

extern int32 sal_l2_dynamicMac_clearByPort(sys_logic_port_t port);

extern int32 sal_l2_dynamicMac_clearByVlan(sys_vid_t vid);

extern int32 sal_l2_staticMac_add(sys_l2_entry_t *pEntry);

extern int32 sal_l2_staticMac_del(sys_l2_entry_t *pEntry);

extern uint32 sal_l2_staticMacExistOnPort_ret(sys_logic_port_t port);

extern sys_l2_entry_t *sal_l2_staticMacNext_ret(sys_l2_entry_t *pPrevEntry);

extern sys_l2_entry_t *sal_l2_staticMacByIdx_ret(uint32 idx);

extern uint32 sal_l2_staticMacExist_ret(sys_l2_entry_t *pChkEntry);

#ifdef CONFIG_SYS_BLACK_HOLE_MAC

extern int32 sal_l2_blacklistMac_add(sys_l2_entry_t *pEntry);

extern int32 sal_l2_blacklistMac_del(sys_l2_entry_t *pEntry);

extern sys_l2_entry_t *sal_l2_blacklistMacNext_ret(sys_l2_entry_t *pPrevEntry);

extern sys_l2_entry_t *sal_l2_blacklistMacByIdx_ret(uint32 idx);

extern uint32 sal_l2_blacklistMacExist_ret(sys_l2_entry_t *pChkEntry);

#endif

extern int32 sal_l2_aging_get(uint32 *pAging_time);

extern int32 sal_l2_aging_set(uint32 aging_time);

#endif 

