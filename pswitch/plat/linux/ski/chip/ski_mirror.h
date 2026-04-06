
#ifndef __SKI_MIRROR_H__
#define __SKI_MIRROR_H__

#include <ski/ski_core.h>
#include <rsd/rsd_acl.h>
#include <rsd/rsd_mirror.h>

typedef enum ski_mirror_cmd_e
{
    
    SKI_MIRROR_SET_BASE = (SKI_CORE_MOD_MIRROR * SKI_CORE_MODCMD_TOTAL),
    SKI_MIRROR_GROUP_SET,
    SKI_MIRROR_GROUP_DEL,
    SKI_MIRROR_EGR_MODE_SET,

    SKI_MIRROR_GET_BASE = (SKI_MIRROR_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_MIRROR_GROUP_GET,
    SKI_MIRROR_EGR_MODE_GET,

    SKI_MIRROR_CMD_END
} ski_mirror_cmd_t;

typedef struct ski_mirror_entryCfg_s
{
    sys_gid_t gid;
    rsd_mirror_entry_t  mirrorEntry;
}ski_mirror_entryCfg_t;

typedef struct ski_mirror_ioctl_s
{
    sys_logic_port_t port;
    rsd_mirror_egrMode_t egrMode;
    ski_mirror_entryCfg_t   entry;
} ski_mirror_ioctl_t;

extern int32 ski_mirror_init(void);

#endif  

