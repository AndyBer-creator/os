
#ifndef __SKI_SURVEILLANCEVLAN_H__
#define __SKI_SURVEILLANCEVLAN_H__

#include <ski/ski_core.h>
#include <ski/proto/surveillancevlan/ski_surveillancevlan.h>
#include <surveillancevlan/proto_surveillancevlan.h>

typedef enum ski_surveillance_vlan_cmd_e
{
    
    SKI_SV_SET_BASE = (SKI_CORE_MOD_SURVEILLANCEVLAN * SKI_CORE_MODCMD_TOTAL),
    SKI_SV_VID_SET,
    SKI_SV_PRI_SET,
    SKI_SV_STATE_SET,
    SKI_SV_AGING_SET,
    SKI_SV_OUI_SET,
    SKI_SV_REMARK_ENABLE_SET,
    SKI_SV_PORT_ENABLE_SET,
    SKI_SV_PORT_MODE_SET,
    SKI_SV_PORT_COS_MODE_SET,

    SKI_SV_GET_BASE = (SKI_SV_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_SV_CMD_END
} ski_surveillance_vlan_cmd_t;

typedef struct ski_surveillance_vlan_ioctl_s
{
    uint32 idx;
    uint32 aging;
    sys_logic_port_t lp;
    sys_enable_t state;
    sys_vid_t vid;
    sys_pri_t pri;
    surveillance_vlan_oui_t oui;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t portCosMode;
} ski_surveillance_vlan_ioctl_t;

extern int32 ski_surveillance_vlan_init(void);
#endif

