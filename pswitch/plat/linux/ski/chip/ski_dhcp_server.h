
#ifndef _SKI_DHCP_SERVER_H_
#define _SKI_DHCP_SERVER_H_

#include <ski/ski_core.h>

typedef enum ski_dhcp_server_cmd_e
{
    
    SKI_DHCP_SERVER_SET_BASE = (SKI_CORE_MOD_DHCP_SERVER * SKI_CORE_MODCMD_TOTAL),
    SKI_DHCP_SERVER_ENABLE_SET,
    SKI_DHCP_SERVER_VLANENABLE_SET,

    SKI_DHCP_SERVER_GET_BASE = (SKI_DHCP_SERVER_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_DHCP_SERVER_ENABLE_GET,
    SKI_DHCP_SERVER_VLANENABLE_GET,
    
    SKI_DHCP_SERVER_CMD_END
} ski_dhcp_server_cmd_t;

typedef struct ski_dhcp_server_ioctl_s
{
    uint32 rate;
    sys_vid_t vid;
    sys_enable_t enable;
    sys_logic_port_t port;
    sys_enable_t type;
} ski_dhcp_server_ioctl_t;

extern int32 ski_dhcp_server_init(void);
#endif  

