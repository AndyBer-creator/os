
#include <libsal/sal_util.h>
#include <libsal/sal_vlan_ip_subnet.h>

int32 sal_vlan_ip_subnet_group_init(uint32 group_idx, sys_ipSubnet_vlan_group_t *pSubnetGroup)
{

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pSubnetGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_ipsubnet_group_set(group_idx, pSubnetGroup));

    return SYS_ERR_OK;
}

int32 sal_vlan_ip_subnet_port_init(sys_logic_port_t port, sys_enable_t enable)
{
	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_vlan_ipsubnet_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_ip_subnet_group_get(uint32 group_idx, sys_ipSubnet_vlan_group_t *pIpSubNetGroup)
{
    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pIpSubNetGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_ipsubnet_group_get(group_idx, pIpSubNetGroup));

    return SYS_ERR_OK;
} 

int32 sal_vlan_ip_subnet_group_set(uint32 group_idx, sys_ipSubnet_vlan_group_t *pSubnetGroup)
{
    uint32 index;
    sys_ipSubnet_vlan_group_t subnetGroup;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pSubnetGroup), SYS_ERR_NULL_POINTER);

    for (index = 0; index < CAPA_VLAN_IP_SUBNET_GROUP_NUM; index++)
    {
        if (index == group_idx)
            continue;

        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(index, &subnetGroup));
        if ((osal_memcmp(&subnetGroup.ipaddr, &pSubnetGroup->ipaddr, sizeof(sys_l3_ipv4_t)) == 0) &&
            (subnetGroup.vid == pSubnetGroup->vid))
        {
            return SYS_ERR_ENTRY_IS_EXIST;
        }
    }
    
   	SYS_ERR_CHK(rsd_vlan_ip_subnet_group_set(group_idx, pSubnetGroup));
    SYS_ERR_CHK(cfg_vlan_ipsubnet_group_set(group_idx, pSubnetGroup));

    return SYS_ERR_OK;
}

int32 sal_vlan_ip_subnet_group_del(uint32 group_idx)
{
    uint32 index;
    sys_ipSubnet_vlan_group_t subnetGroup;

    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);

    for (index = 0; index < CAPA_VLAN_IP_SUBNET_GROUP_NUM; index++)
    {
        if (index != group_idx)
            continue;

		SYS_MEM_CLEAR(subnetGroup);
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(index, &subnetGroup));
        if ((subnetGroup.ipaddr.addr != 0) &&(subnetGroup.vid != 0))
        {
            break;
        }
    }
    
   	SYS_ERR_CHK(rsd_vlan_ip_subnet_group_destroy(group_idx, &subnetGroup));
   	SYS_MEM_CLEAR(subnetGroup);
    SYS_ERR_CHK(cfg_vlan_ipsubnet_group_set(group_idx, &subnetGroup));

    return SYS_ERR_OK;
}

int32 sal_vlan_port_ipsubnet_vlan_group_bind_get(sys_logic_port_t port, uint32 *pGroup_idx)
{
	uint32 index = 0;
	sys_ipSubnet_vlan_group_t subnetGroup;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pGroup_idx), SYS_ERR_NULL_POINTER);
	
    for (index = (*pGroup_idx); index < CAPA_VLAN_IP_SUBNET_GROUP_NUM; index++)
    {
		SYS_MEM_CLEAR(subnetGroup);
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(index, &subnetGroup));
        if ((subnetGroup.valid == 1) &&(subnetGroup.lport == port))
        {
        	*pGroup_idx = index;
            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_ENTRY_NOT_FOUND;
}

int32 sal_vlan_port_ipsubnet_vlan_group_bind_set(sys_logic_port_t port, uint32 group_idx, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((group_idx >= CAPA_VLAN_IP_SUBNET_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
	sys_ipSubnet_vlan_group_t subnetGroup;

    if(ENABLED == enable)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL)))
            return SYS_ERR_VLAN_PROVLAN_PORT_MODE;

		SYS_MEM_CLEAR(subnetGroup);
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(group_idx, &subnetGroup));
        if(subnetGroup.valid == 0 && subnetGroup.ipaddr.addr == 0)
			return SYS_ERR_ENTRY_NOT_FOUND;
			
        SYS_ERR_TRK_CHK(rsd_vlan_port_ipsubnet_vlan_group_bind(port, group_idx));
        SYS_ERR_CHK(cfg_vlan_ipsubnet_port_group_bind_set(port, group_idx));
    }
    else
    {
        SYS_ERR_TRK_CHK(rsd_vlan_port_ipsubnet_vlan_group_unbind(port, group_idx));
        SYS_ERR_CHK(cfg_vlan_ipsubnet_port_group_unbind_set(port, group_idx));        
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_port_ipsubnet_vlan_enable(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if(ENABLED == enable)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL)))
            return SYS_ERR_VLAN_PROVLAN_PORT_MODE;

        SYS_ERR_TRK_CHK(rsd_vlan_port_ipsubnet_vlan_enable(port, enable));
    }
    else
        SYS_ERR_TRK_CHK(rsd_vlan_port_ipsubnet_vlan_enable(port, enable));

   SYS_ERR_CHK(cfg_vlan_ipsubnet_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_port_ipsubnet_vlan_status(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

   	SYS_ERR_CHK(cfg_vlan_ipsubnet_port_enable_get(port, pEnable));

    return SYS_ERR_OK;
}

