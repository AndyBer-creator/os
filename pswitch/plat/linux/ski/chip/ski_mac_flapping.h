
#ifndef __SKI_MAC_FLAPPING_H__
#define __SKI_MAC_FLAPPING_H__

#include <ski/ski_core.h>
#include <rsd/rsd_mac_flapping.h>
#if 0

typedef enum ski_psecure_cmd_e
{
    
    SKI_PSECURE_SET_BASE = (SKI_CORE_MOD_PSECURE * SKI_CORE_MODCMD_TOTAL),
    SKI_PSECURE_SET_ENABLE,
    SKI_PSECURE_SET_RATELIMIT,
    SKI_PSECURE_SET_PORT,
    SKI_PSECURE_SET_MAC_ADD,
    SKI_PSECURE_SET_MAC_DEL,

    SKI_PSECURE_GET_BASE = (SKI_PSECURE_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_PSECURE_GET_PORT_INFO,

    SKI_PSECURE_CMD_END
} ski_psecure_cmd_t;

typedef union ski_psecure_ioctl_u
{
    sys_enable_t       enable;
    uint32             rate;
    sys_l2_entry_t     l2Entry;

    struct {
        sys_logic_port_t   port;
        sys_psecure_port_t cfg;
    } portCfg;

    struct {
        sys_logic_port_t       port;
        sys_psecure_portInfo_t info;
    } portInfo;
} ski_psecure_ioctl_t;
#endif

extern int32 ski_mac_flapping_init(void);

#endif  

