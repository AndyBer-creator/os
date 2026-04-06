
#ifndef __SKI_LLDP_MED_H__
#define __SKI_LLDP_MED_H__

#include <ski/ski_core.h>
#include <lldp/proto_lldp.h>

typedef enum ski_lldp_med_cmd_e
{
    
    SKI_LLDP_MED_SET_BASE = (SKI_CORE_MOD_LLDP_MED * SKI_CORE_MODCMD_TOTAL),
    SKI_LLDP_MED_SET_MED_FAST_REPEAT_COUNT,
    SKI_LLDP_MED_SET_MED_NETWORK_POLICY,
    SKI_LLDP_MED_SET_MED_NETWORK_POLICY_AUTO,
    SKI_LLDP_MED_SET_PORT_MED_TLV_SEL,
    SKI_LLDP_MED_SET_PORT_MED_ENABLE,
    SKI_LLDP_MED_SET_PORT_MED_NETWORK_POLICY_BIND,
    SKI_LLDP_MED_SET_PORT_MED_LOCATION,
    SKI_LLDP_MED_SET_MED_INVENTORY,

    SKI_LLDP_MED_GET_BASE = (SKI_LLDP_MED_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_LLDP_MED_GET_PORT_MED_LOCAL_ENTRYINFO,
    SKI_LLDP_MED_GET_PORT_MED_REMOTE_ENTRYINFO,
    SKI_LLDP_MED_GET_MED_FAST_REPEAT_COUNT,
    SKI_LLDP_MED_GET_MED_NETWORK_POLICY,
    SKI_LLDP_MED_GET_MED_NETWORK_POLICY_AUTO,
    SKI_LLDP_MED_GET_PORT_MED_ENABLE,
    SKI_LLDP_MED_GET_PORT_MED_TLV_SEL,
    SKI_LLDP_MED_GET_PORT_MED_NETWORK_POLICY_BIND,
    SKI_LLDP_MED_GET_PORT_MED_LOCATION,

    SKI_LLDP_MED_CMD_END
} ski_lldp_med_cmd_t;

typedef union ski_lldp_med_ioctl_u
{
    struct {
        sys_logic_port_t    port;
        display_med_entry_t entryInfo;
    } localMedEntryInfo;
    struct {
        sys_logic_port_t    port;
        uint8               index;
        display_med_entry_t entryInfo;
    } remoteMedEntryInfo;
    struct {
        uint32              idx;
        lldp_med_networkPolicy_t    policy;
    } lldp_medNetworkPolicy;
    struct {
        sys_logic_port_t    port;
        lldp_med_tlv_sel_t  lldpMedTlvSel;
    } lldp_portMedTlvSel;
    struct {
        sys_logic_port_t    port;
        sys_enable_t        enable;
    } lldp_portMedEnable;
    struct {
        sys_logic_port_t    port;
        lldp_med_networkPolicy_bind_t   policyBind;
    } lldp_portMedNetworkPolicyBind;
    struct {
        sys_logic_port_t    port;
        lldp_med_location_t location;
    } lldp_portMedLocation;
    uint8 fastRepeatCount;
    sys_enable_t networkPolicyAuto;
    struct {
        lldp_inventory_type_t inventoryType;
        char strData[SYS_BUF64_LEN];
    } lldp_medInventory;
} ski_lldp_med_ioctl_t;

extern int32 ski_lldp_med_init(void);

#endif  

