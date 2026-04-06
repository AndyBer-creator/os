
#ifndef __SAL_UTIL_LOCK_H__
#define __SAL_UTIL_LOCK_H__

#include <sys/mman.h>
#include <sys/sem.h>

#include <common/sys_util.h>
#include <libcfg/cfg.h>

#define USER_LOCK_ID_BASE 65536

typedef enum sys_userlock_type_e
{
    SYS_USER_LOCK_FLSHINFO = 0,
    SYS_USER_LOCK_L3,
    SYS_USER_LOCK_L3DB,
    SYS_USER_LOCK_L2DB,
    SYS_USER_LOCK_END
} sys_userlock_type_t;

#define USER_LOCK(type) sal_util_sem_lock(type)
#define USER_UNLOCK(type) sal_util_sem_unlock(type)

extern int32 sal_util_lock_init(void);

extern int32 sal_util_sem_lock(sys_userlock_type_t type);

extern int32 sal_util_sem_unlock(sys_userlock_type_t type);

#endif 

