
#ifndef __SAL_L3_H__
#define __SAL_L3_H__

#include <rsd/rsd_l3.h>
#include <libsal/sal_l3_intf.h>
#include <libsal/sal_l3_route.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <libsal/sal_l3_neigh.h>
#endif

#ifdef CONFIG_SYS_L3_ROUTE
#define L3_ENGINE_PID_FILE "/var/run/zebra.pid"
#define L3_FPMD_PID_FILE   "/var/run/fpmd.pid"
#endif

extern const sys_text_t text_ipv6_dadSts[];

#define L3_USER_LOCK() (USER_LOCK(SYS_USER_LOCK_L3))
#define L3_USER_UNLOCK() (USER_UNLOCK(SYS_USER_LOCK_L3))

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 sal_l3_engineCmd_exec(char *cmd);

extern int32 sal_l3_engineCmdNoLock_exec(char *cmd);

extern int32 sal_l3_engineCmd2File_exec(char *cmd, FILE *fp);

extern int32 sal_l3_engineIntfCmd_exec(sys_l3_intfId_t *pId, char *cmd);

extern int32 sal_l3_engine_restart(void);

extern int32 sal_l3_fwdIPv4Enable_set(sys_enable_t enable);

extern int32 sal_l3_fwdIPv4Enable_get(sys_enable_t *pEnable);

extern int32 sal_l3_fwdIPv6Enable_set(sys_enable_t enable);

extern int32 sal_l3_fwdIPv6Enable_get(sys_enable_t *pEnable);

extern int32 sal_l3_hopLimitIPv6_set(uint32 num);

extern int32 sal_l3_hopLimitIPv6_get(uint32 *pNum);

extern int32 sal_l3_icmpv6RateInterval_set(uint32 time);

extern int32 sal_l3_icmpv6RateInterval_get(uint32 *pTime);

extern int32 sal_l3_icmpv6RateBurst_set(uint32 time);

extern int32 sal_l3_icmpv6RateBurst_get(uint32 *pTime);
#endif

#endif 

