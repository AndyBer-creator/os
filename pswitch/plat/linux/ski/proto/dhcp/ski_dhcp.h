
#ifndef __SKI_DHCP_H__
#define __SKI_DHCP_H__

#include <ski/ski_core.h>
#include <dhcp/proto_dhcp.h>

typedef enum ski_dhcp_cmd_e
{
    
    SKI_DHCP_SET_BASE = (SKI_CORE_MOD_DHCP * SKI_CORE_MODCMD_TOTAL),

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    SKI_DHCP_SNOOPING_ENABLE_SET,
    SKI_DHCP_SNOOPING_VLANENABLE_SET,
    SKI_DHCP_SNOOPING_PORTTYPE_SET,
    SKI_DHCP_SNOOPING_PORTCHADDR_CHECK_SET,
#endif

    SKI_DHCP_OPTION82_PORT_ENABLE_SET,
    SKI_DHCP_OPTION82_PORT_ACTION_SET,
    SKI_DHCP_OPTION82_PORT_REMOTEID_SET,
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    SKI_DHCP_OPTION82_PORT_FORMAT_SET,
    SKI_DHCP_OPTION82_PORT_CIRCUITID_SET,
#endif
    SKI_DHCP_SNOOPING_MIB_CLEAR,
    SKI_DHCP_SNOOPING_RL_SET,

    SKI_DHCP_GET_BASE = (SKI_DHCP_SET_BASE + SKI_CORE_MODCMD_RANGE),

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    SKI_DHCP_SNOOPING_ENABLE_GET,
    SKI_DHCP_SNOOPING_VLANENABLE_GET,
    SKI_DHCP_SNOOPING_PORTTYPE_GET,
    SKI_DHCP_SNOOPING_PORTCHADDR_CHECK_GET,
#endif

    SKI_DHCP_OPTION82_PORT_ENABLE_GET,
    SKI_DHCP_OPTION82_PORT_ACTION_GET,
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    SKI_DHCP_OPTION82_PORT_FORMAT_GET,
#endif
    SKI_DHCP_OPTION82_PORT_REMOTEID_GET,
    SKI_DHCP_SNOOPING_MIB_GET,

    SKI_DHCP_CMD_END
} ski_dhcp_cmd_t;

typedef struct ski_dhcp_ioctl_s
{
    uint32 rate;
    sys_vid_t vid;
    sys_enable_t enable;
    sys_logic_port_t port;
    sys_enable_t type;
    sys_dhcp_mib_t  counter;
    sys_dhcp_option82_act_t action;
    sys_dhcp_option82_format_t format;
    sys_dhcp_option82_id_t id;
    sys_dhcp_option82_circuit_id_t cid;
} ski_dhcp_ioctl_t;

extern int32 ski_dhcp_init(void);

#endif

