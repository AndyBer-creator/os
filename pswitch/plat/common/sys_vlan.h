
#ifndef __COMMON_SYS_VLAN_H__
#define __COMMON_SYS_VLAN_H__
#include <common/sys_def.h>
#define STR2VMSK(str, pVMsk)               (sys_util_str2Vlanmask(str, pVMsk))
#define VMSK2STR(pVMsk, str)               (sys_util_vlanmask2Str(pVMsk, str))
#define VMSK2CNT(pVMsk, cnt)               (sys_util_vlanmask2Cnt(pVMsk, cnt))
extern int32 sys_vlan_init(void);
extern int32 sys_util_str2Vlanmask(const char *str, sys_vlanmask_t *pVlanmask);
extern int32 sys_util_vlanmask2Str(sys_vlanmask_t *pVlanmask, char *str);
extern int32 sys_util_vlanmask2Cnt(sys_vlanmask_t *pVlanmask, uint32 *pCount);

#endif

