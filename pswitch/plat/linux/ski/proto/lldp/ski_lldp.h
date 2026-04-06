
#ifndef __SKI_LLDP_H__
#define __SKI_LLDP_H__

#include <ski/ski_core.h>
#include <lldp/proto_lldp.h>

typedef enum ski_lldp_cmd_e
{
    
    SKI_LLDP_SET_BASE = (SKI_CORE_MOD_LLDP * SKI_CORE_MODCMD_TOTAL),
    SKI_LLDP_SET_ENABLE ,
    SKI_LLDP_SET_PORT,
    SKI_LLDP_SET_HOLDTIME ,
    SKI_LLDP_SET_TX_INTERVAL ,
    SKI_LLDP_SET_REINIT_DELAY ,
    SKI_LLDP_GLOBALSTATISTICS_CLEAR,
    SKI_LLDP_PORTSTATISTICS_CLEAR,
    SKI_LLDP_SET_DISABLE_ACTION,
    SKI_LLDP_SET_PORT_TLV_SEL,
    SKI_LLDP_SET_PORT_TLV_PVID_ENABLE,
    SKI_LLDP_SET_PORT_TLV_VLANMASK,
    SKI_LLDP_SET_TX_DELAY,
    SKI_LLDP_SET_ENTRYINFO_CLEAR,

    SKI_LLDP_GET_BASE = (SKI_LLDP_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_LLDP_GET_ENABLE ,
    SKI_LLDP_GET_PORT,
    SKI_LLDP_GET_PORT_LOCAL_ENTRYINFO,
    SKI_LLDP_GET_PORT_REMOTE_ENTRYINFO,
    SKI_LLDP_GET_PORT_VLAN_NAME_TLV,
    SKI_LLDP_GET_PORT_OVERLOAD,
    SKI_LLDP_GET_STATISTIC,
    SKI_LLDP_GET_PORT_STATISTIC,
    SKI_LLDP_GET_DISABLE_ACTION,
    SKI_LLDP_GET_PORT_TLV_SEL,
    SKI_LLDP_GET_PORT_TLV_PVID_ENABLE,
    SKI_LLDP_GET_PORT_TLV_VLANMASK,
    SKI_LLDP_GET_TX_DELAY,
    SKI_LLDP_GET_CHANGE_TIME,

    SKI_LLDP_CMD_END
} ski_lldp_cmd_t;

#define LLDP_NAME_LEN_MAX            (32)
#define LLDP_VLAN_TO_INSTANCE_SIZE   (4096)

typedef struct ski_lldp_enable_s{
    uint8               lldpPortSate;
    sys_logic_port_t    port;
} ski_lldp_enable_t;

typedef union ski_lldp_ioctl_u
{
    ski_lldp_enable_t       lldpContrl;
    sys_enable_t            lldp_enabled;
    int32                   tx_interval;
    int8                    holdtime;
    int8                    reinit_delay;
    uint32                  changeTime;
    struct {
        sys_enable_t        enable;
    } enable;
    struct {
        sys_logic_port_t    port;
        display_entry_t     entryInfo;
    } localEntryInfo;
    struct {
        sys_logic_port_t    port;
        uint8               index;
        display_entry_t     entryInfo;
    } remoteEntryInfo;
    struct {
        sys_logic_port_t    port;
        uint16              index;
        lldp_vlanName_tlv_t vlanTlv;
    } lldp_portRemoteVlanTlv;

    struct {
        sys_logic_port_t    port;
        lldp_overLoading_t  overload;
    } lldp_portOverload;
    struct {
        sys_logic_port_t    portIndex;
        lldp_port_entry_t   portEntry;
    } lldp_portStatistics;
    struct {
        sys_logic_port_t    port;
        lldp_statistic_t    lldp_statistic;
    } lldp_portStatistic;
    lldp_disable_action_t   action;
    struct {
        sys_logic_port_t    port;
        lldp_tlv_sel_t      lldpTlvSel;
    } lldp_portTlvSel;
    struct {
        sys_logic_port_t    port;
        sys_enable_t pvidEnable;
    } lldp_portTlvPvidEbl;
    struct {
        sys_logic_port_t    port;
        sys_vlanmask_t vlanmask;
    } lldp_portTlvVlanmask;
    uint16 txDelay;
} ski_lldp_ioctl_t;

extern int32 ski_lldp_init(void);

#endif  

