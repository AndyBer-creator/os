
#ifndef __SKI_DAI_H__
#define __SKI_DAI_H__

#include <ski/ski_core.h>
#include <dai/proto_dai.h>

typedef enum ski_dai_cmd_e
{
    
    SKI_DAI_SET_BASE = (SKI_CORE_MOD_DAI * SKI_CORE_MODCMD_TOTAL),
    SKI_DAI_ENABLE_SET,
    SKI_DAI_VLANENABLE_SET,
    SKI_DAI_PORT_TRUST_SET,
    SKI_DAI_PORT_SMACCHK_SET,
    SKI_DAI_PORT_DMACCHK_SET,
    SKI_DAI_PORT_IPCHK_SET,
    SKI_DAI_PORT_IPZEROALLOW_SET,
    SKI_DAI_PORT_MIB_CLEAR,
    SKI_DAI_VLAN_MIB_CLEAR,
    SKI_DAI_ARP_RL_SET,

    SKI_DAI_GET_BASE = (SKI_DAI_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_DAI_ENABLE_GET,
    SKI_DAI_VLANENABLE_GET,
    SKI_DAI_PORT_TRUST_GET,
    SKI_DAI_PORT_SMACCHK_GET,
    SKI_DAI_PORT_DMACCHK_GET,
    SKI_DAI_PORT_IPCHK_GET,
    SKI_DAI_PORT_IPZEROALLOW_GET,
    SKI_DAI_PORT_MIB_GET,
    SKI_DAI_VLAN_MIB_GET,

    SKI_DAI_CMD_END
} ski_dai_cmd_t;

typedef struct ski_dai_ioctl_u
{
    uint32 rate;
    sys_logic_port_t port;
    sys_vid_t vid;
    sys_enable_t enable;
    sys_arp_mib_t counter;
} ski_dai_ioctl_t;

extern int32 ski_dai_init(void);

#endif

