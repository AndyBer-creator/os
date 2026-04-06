#ifndef __COMMON_SYS_DBG_H__
#define __COMMON_SYS_DBG_H__
#include <stdarg.h>

#include <common/sys_type.h>
#include <common/sys_log.h>
#include <common/sys_dbg_desc.h>
#define SYS_DBG_MASK_STR            "dbgMaskStr"
#define SYS_DBG(dbg, fmt, args...)  sys_dbg(dbg, __FUNCTION__, __LINE__, fmt , ## args)
#define IS_DBG_SET(dbg)             sys_dbg_isSet_ret(dbg)
#define SYS_PKT_DUMP(dbg, data, len)    sys_dbg_pkt_dump(dbg,__FUNCTION__, __LINE__, data, len)

#define DEV_PRINTK(string, args...) {}
extern int32 sys_dbg(uint32 dbg, const char *func, const int line, char* fmt, ...);
extern int32 sys_dbg_isSet_ret(uint32 dbg);
extern int32 sys_dbg_pkt_dump(uint32 cat, const char *func, const int line, char *pktData, uint32 pktLen);
extern int32 sys_dbg_enable_set(uint32 dbg, sys_enable_t enable);
extern int32 sys_dbg_enable_get(uint32 dbg, sys_enable_t *pEnable);

#ifdef __KERNEL__

#else

extern int32 sys_dbg_init(void);
extern int32 sys_dbg_reset(void);
#endif

#endif

