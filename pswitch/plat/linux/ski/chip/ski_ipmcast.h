
#ifndef __SKI_IPMCAST_H__
#define __SKI_IPMCAST_H__

#include <ski/ski_core.h>
#include <rsd/rsd_ipmcast.h>

typedef enum ski_ipMcast_cmd_e
{
    
    SKI_IPMCAST_SET_BASE = (SKI_CORE_MOD_IPMCAST * SKI_CORE_MODCMD_TOTAL),

    SKI_IPMCAST_GET_BASE = (SKI_IPMCAST_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_L2MCAST_FIRST_ENTRY_GET,
    SKI_L2MCAST_NEXT_ENTRY_GET,
    SKI_IPMCAST_FIRST_ENTRY_GET,
    SKI_IPMCAST_NEXT_ENTRY_GET,

    SKI_IPMCAST_CMD_END
} ski_ipMcast_cmd_t;

typedef union ski_ipMcast_ioctl_u
{
    sys_ipMcastEntry_t entry;
    sys_l2McastEntry_t l2entry;
} ski_ipMcast_ioctl_t;

extern int32 ski_ipMcast_init(void);

#endif  

