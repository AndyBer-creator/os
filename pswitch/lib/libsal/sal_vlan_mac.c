
#include <common/sys_def.h>
#include <libsal/sal_util.h>
#include <libsal/sal_vlan.h>
#include <libsal/sal_vlan_mac.h>

int32 sal_macvlan_group_get(uint32 idx, sys_mac_vlan_group_t *pmacGroup)
{
    SYS_PARAM_CHK((idx >= CAPA_VLAN_MAC_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(macvlan_group_get(idx, pmacGroup));

    return SYS_ERR_OK;
}

int32 sal_macvlan_group_set(sys_mac_vlan_group_t *pmacGroup)
{
    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    if (0x1 & pmacGroup->mac.octet[0])
        return SYS_ERR_VLAN_MAC_NOT_UNI;

    SYS_ERR_CHK(macvlan_group_set(pmacGroup));

    return SYS_ERR_OK;
}

int32 sal_macvlan_group_del(sys_mac_vlan_group_t *pmacGroup)
{
    SYS_PARAM_CHK((NULL == pmacGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(macvlan_group_del(pmacGroup));

    return SYS_ERR_OK;
}

int32 sal_macvlan_mapping_get(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(macvlan_mapping_get(port, mapping));

    return SYS_ERR_OK;
}

int32 sal_macvlan_mapping_set(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, 1 << SYS_VLAN_PORT_MODE_GENERAL))
        return SYS_ERR_VLAN_MAC_GENERAL_MODE;

    SYS_ERR_CHK(macvlan_mapping_set(port, mapping));

    return SYS_ERR_OK;
}

int32 sal_macvlan_mapping_del(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == mapping), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(macvlan_mapping_del(port, mapping));

    return SYS_ERR_OK;
}
