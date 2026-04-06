
#include <libcfg/cfg.h>

int32 cfg_vlan_ipsubnet_group_get( uint32 group_idx, sys_ipSubnet_vlan_group_t *pIpsubGroup)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pIpsubGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);

    osal_memcpy(pIpsubGroup, &pVlan->groups[group_idx], sizeof(sys_ipSubnet_vlan_group_t));

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

int32 cfg_vlan_ipsubnet_group_set( uint32 group_idx, sys_ipSubnet_vlan_group_t *pIpsubGroup)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pIpsubGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);

    osal_memcpy(&pVlan->groups[group_idx], pIpsubGroup, sizeof(sys_ipSubnet_vlan_group_t));

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

int32 cfg_vlan_ipsubnet_port_group_bind_set(sys_logic_port_t port, uint32 group_idx)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);
	pVlan->groups[group_idx].valid = TRUE;
	pVlan->groups[group_idx].lport = port;

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

int32 cfg_vlan_ipsubnet_port_group_unbind_set(sys_logic_port_t port, uint32 group_idx)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);
	pVlan->groups[group_idx].valid = FALSE;
	pVlan->groups[group_idx].lport = 0xFFFF;

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

int32 cfg_vlan_ipsubnet_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);
    
	pVlan->pvlanPortCfg[port] = enable;

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

int32 cfg_vlan_ipsubnet_port_enable_get(sys_logic_port_t port, sys_enable_t* pEnable)
{
    cfg_vlan_ipsubnet_t *pVlan = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_VLAN_IP_SUBNET, (cfg_addr)&pVlan);
    
	*pEnable = pVlan->pvlanPortCfg[port];

    CFG_CLOSE(CFG_ID_VLAN_IP_SUBNET);
}

