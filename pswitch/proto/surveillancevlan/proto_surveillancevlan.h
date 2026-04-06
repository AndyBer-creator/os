
#ifndef __PROTO_SURVEILLANCEVLAN_H__
#define __PROTO_SURVEILLANCEVLAN_H__
#include <common/sys_error.h>
#include <common/sys_type.h>
#include <common/sys_portview.h>
typedef enum sys_surveillance_vlan_port_mode_e
{
    SURVEILLANCE_VLAN_PORT_MODE_AUTO = 0,
    SURVEILLANCE_VLAN_PORT_MODE_MANUAL,
    SURVEILLANCE_VLAN_PORT_MODE_END
} surveillance_vlan_port_mode_t;

typedef enum surveillance_vlan_cos_mode_e
{
    SURVEILLANCE_VLAN_PORT_COS_SRC = 0,
    SURVEILLANCE_VLAN_PORT_COS_ALL,
    SURVEILLANCE_VLAN_PORT_COS_END
}surveillance_vlan_cos_mode_t;

typedef struct surveillance_vlan_oui_s
{
    uint8 oui[SNAPOUI_LEN];
    sys_namestr_t ouiName;
} surveillance_vlan_oui_t;
#define IS_OUI_CLEAR(oui) (((0 == oui[0]) && (0 == oui[1]) && (0 == oui[2])) ? TRUE : FALSE)
#define SV_DBG(fmt, args...) SYS_DBG(DBG_VLAN_SURVEILLANCE, fmt, ##args)
extern int32 surveillance_vlanVid_set(sys_vid_t vid);
extern int32 surveillance_vlanPri_set(sys_pri_t pri);
extern int32 surveillance_vlanState_set(sys_enable_t state);
extern int32 surveillance_vlanAging_set(uint32 aging);
extern int32 surveillance_vlanOuiByIdx_set(uint32 idx, surveillance_vlan_oui_t *pOui);
extern int32 surveillance_vlan1pRemarkEnable_set(sys_enable_t enable);
extern int32 surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode);
extern int32 surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode);
extern int32 surveillance_vlan_init(void);
extern int32 surveillance_vlan_exit(void);
#endif

