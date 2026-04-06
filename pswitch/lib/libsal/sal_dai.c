
#include <libsal/sal_util.h>
#include <libsal/sal_dai.h>

int32 sal_dai_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_enable_set(enable));
    SYS_ERR_CHK(cfg_dai_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_dai_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_vlan_enable_set (sys_vid_t vid, sys_enable_t enable)
{
    sys_vlanmask_t vm;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_vlan_enable_set(vid, enable));
    SYS_ERR_CHK(cfg_dai_vlanmask_enable_get(&vm));

    if (ENABLED == enable)
        VLANMASK_SET_VID(vm, vid);
    else
        VLANMASK_CLEAR_VID(vm, vid);

    SYS_ERR_CHK(cfg_dai_vlanmask_enable_set(&vm));

    return SYS_ERR_OK;
}

int32 sal_dai_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable)
{
    sys_vlanmask_t vm;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_vlanmask_enable_get(&vm));

    if (VLANMASK_IS_VIDSET(vm, vid))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}

int32 sal_dai_vlanmask_enable_get (sys_vlanmask_t *vlanmask)
{
    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_vlanmask_enable_get(vlanmask));

    return SYS_ERR_OK;
}

int32 sal_dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    uint32 rate = DFLT_DAI_RATELIMIT_RATE;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_dai_rateLimit_get(port, &rate));

    if (ENABLED == enable)
    {
        
        if (DFLT_DAI_RATELIMIT_RATE == rate)
            SYS_ERR_CHK(sal_dai_rateLimit_set(port, BOND_RATE_UNLIMITED));

        SYS_ERR_CHK(dai_port_trust_set(port, ENABLED));
    }
    else
    {
        SYS_ERR_CHK(dai_port_trust_set(port, DISABLED));

        if (BOND_RATE_UNLIMITED == rate)
            SYS_ERR_CHK(sal_dai_rateLimit_set(port, DFLT_DAI_RATELIMIT_RATE));
    }

    SYS_ERR_CHK(cfg_dai_port_trust_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_port_trust_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_port_smacChk_set(port, enable));
    SYS_ERR_CHK(cfg_dai_port_smacChk_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_port_smacChk_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_port_dmacChk_set(port, enable));
    SYS_ERR_CHK(cfg_dai_port_dmacChk_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_port_dmacChk_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_port_ipChk_set(port, enable));
    SYS_ERR_CHK(cfg_dai_port_ipChk_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_port_ipChk_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(dai_port_ipZeroAllow_set(port, enable));
    SYS_ERR_CHK(cfg_dai_port_ipZeroAllow_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_port_ipZeroAllow_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dai_portMib_get(sys_logic_port_t port, sys_arp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(dai_portMib_get(port, pCounter));

    return SYS_ERR_OK;
}

int32 sal_dai_portMib_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(dai_portMib_clear(port));

    return SYS_ERR_OK;
}

int32 sal_dai_vlanMib_get(sys_vid_t vid, sys_arp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(dai_vlanMib_get(vid, pCounter));

    return SYS_ERR_OK;
}

int32 sal_dai_vlanMib_clear(sys_vid_t vid)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_ERR_CHK(dai_vlanMib_clear(vid));

    return SYS_ERR_OK;
}

int32 sal_dai_rateLimit_get(sys_logic_port_t port, uint32 *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dai_rateLimit_get(port, pRate));

    return SYS_ERR_OK;
}

int32 sal_dai_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(dai_arp_rateLimit_set(port, rate));
    SYS_ERR_CHK(cfg_dai_rateLimit_set(port, rate));

    return SYS_ERR_OK;
}

