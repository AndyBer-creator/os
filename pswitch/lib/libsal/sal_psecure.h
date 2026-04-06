
#ifndef __SAL_PSECURE_H__
#define __SAL_PSECURE_H__

#include <rsd/rsd_psecure.h>

extern const sys_text_t text_psecure_violation[];

extern int32 sal_psecure_enable_set(sys_enable_t enable);

extern int32 sal_psecure_enable_get(sys_enable_t *pEnable);

extern int32 sal_psecure_rateLimit_set(uint32 rate);

extern int32 sal_psecure_rateLimit_get(sys_enable_t *pRate);

extern int32 sal_psecure_port_set(sys_logic_port_t port, sys_psecure_port_t *pPortCfg);

extern int32 sal_psecure_port_get(sys_logic_port_t port, sys_psecure_port_t *pPortCfg);

extern int32 sal_psecure_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_psecure_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_psecure_portLimitNum_set(sys_logic_port_t port, uint32 limitNum);

extern int32 sal_psecure_portLimitNum_get(sys_logic_port_t port, sys_enable_t *pLimitNum);

extern int32 sal_psecure_portViolation_set(sys_logic_port_t port, sys_psecure_violation_t violation);

extern int32 sal_psecure_portViolation_get(sys_logic_port_t port, sys_psecure_violation_t *pViolation);

extern int32 sal_psecure_portSticky_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_psecure_portSticky_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_psecure_portInfo_get(sys_logic_port_t port, sys_psecure_portInfo_t *pInfo);

extern int32 sal_psecure_mac_add(sys_l2_entry_t *pEntry);

extern int32 sal_psecure_mac_del(sys_l2_entry_t *pEntry);

extern int32 sal_psecure_mac_clear(sys_psecure_clearCfg_t *pClear);

extern sys_l2_entry_t *sal_psecure_portMacEntryByIdx_ret(sys_logic_port_t port, uint32 idx);

extern uint32 sal_psecure_macExist_ret(sys_l2_entry_t *pChkEntry);

#endif 

