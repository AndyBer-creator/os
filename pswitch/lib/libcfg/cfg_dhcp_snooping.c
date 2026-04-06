
#include <libcfg/cfg.h>

int32 cfg_dhcp_snooping_enable_set(sys_enable_t enable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->enable = enable;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_enable_get(sys_enable_t *pEnable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pEnable = dhcp_snooping->enable;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_vlanmask_enable_set (sys_vlanmask_t *vlanmask)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->enabledVLAN = *vlanmask;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_vlanmask_enable_get (sys_vlanmask_t *vlanmask)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *vlanmask = dhcp_snooping->enabledVLAN;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->trust[port] = enable;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pEnable = dhcp_snooping->trust[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_port_chaddrChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->chaddrChk[port] = enable;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_port_chaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pEnable = dhcp_snooping->chaddrChk[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_rateLimit_get(sys_logic_port_t port, uint32 *pRateValue)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pRateValue = dhcp_snooping->dhcpLimitRate[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->dhcpLimitRate[port] = rate;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->dhcpOption82Enable[port] = enable;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pEnable = dhcp_snooping->dhcpOption82Enable[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((action >= DHCP_OPTION82_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->dhcpOption82Action[port] = action;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pAction == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pAction = dhcp_snooping->dhcpOption82Action[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pID)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    osal_memcpy(&dhcp_snooping->dhcpOption82RemoteID, pID, sizeof(sys_dhcp_option82_id_t));

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pID)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    osal_memcpy(pID, &dhcp_snooping->dhcpOption82RemoteID, sizeof(sys_dhcp_option82_id_t));

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

int32 cfg_dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((format >= DHCP_OPTION82_FORMAT_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    dhcp_snooping->dhcpOption82Format[port] = format;

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pFormat == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    *pFormat = dhcp_snooping->dhcpOption82Format[port];

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_circuitID_set(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    osal_memcpy(&dhcp_snooping->dhcpOption82CircuitID[port], pID, sizeof(dhcp_snooping->dhcpOption82CircuitID[port]));

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}

int32 cfg_dhcp_option82_circuitID_get(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID)
{
    cfg_dhcp_snooping_t *dhcp_snooping = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pID == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SNOOPING, (cfg_addr)&dhcp_snooping);

    osal_memcpy(pID, &dhcp_snooping->dhcpOption82CircuitID[port], sizeof(dhcp_snooping->dhcpOption82CircuitID[port]));

    CFG_CLOSE(CFG_ID_DHCP_SNOOPING);
}
#endif

