
#ifndef __SKI_SCHED_H__
#define __SKI_SCHED_H__

#include <ski/ski_core.h>
#include <rsd/rsd_sys.h>
#include <common/sys_sched.h>

typedef enum ski_sched_cmd_e
{
    
    SKI_SCHED_SET_BASE = (SKI_CORE_MOD_SCHED * SKI_CORE_MODCMD_TOTAL),
    SKI_SCHED_CREATE,
    SKI_SCHED_SET,
    SKI_SCHED_DEL,
    SKI_SCHED_REGISTER,
    SKI_SCHED_UNREGISTER,
    SKI_SCHED_REC_SET,
    SKI_SCHED_REC_DEL,
    SKI_SCHED_TZ_SET,
    SKI_SCHED_DLS_SET,

    SKI_SCHED_GET_BASE = (SKI_SCHED_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_SCHED_GET,
    SKI_SCHED_STAT_GET,
    SKI_SCHED_REC_GET,
    SKI_SCHED_NAMELIST_GET,

    SKI_SCHED_CMD_END
} ski_sched_cmd_t;

typedef union ski_sched_ioctl_u
{
    sys_namestr_t name;
    sys_namestr_t names[CAPA_TIME_SCHED_NUM];
    sys_task_scheduler_t ts;
    sys_task_scheduler_stat_t stat;
    struct
    {
        sys_namestr_t name;
        sys_task_scheduler_rec_t tsRec[CAPA_TIME_SCHED_REC_NUM];
    } rec;

    struct
    {
        uint32 start;
        uint32 end;
        int32 diff;
    } diff;
} ski_sched_ioctl_t;

extern int32 ski_sched_init(void);

#endif  

