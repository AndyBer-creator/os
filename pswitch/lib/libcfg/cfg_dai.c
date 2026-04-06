
#include <libcfg/cfg.h>

int32 cfg_dai_enable_set(sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->enable = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_enable_get(sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_vlanmask_enable_set (sys_vlanmask_t *vlanmask)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->vlanEnable = *vlanmask;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_vlanmask_enable_get (sys_vlanmask_t *vlanmask)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK((NULL == vlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *vlanmask = dai->vlanEnable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->trust[port] = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->trust[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->smacChk[port] = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->smacChk[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->dmacChk[port] = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->dmacChk[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->ipChk[port] = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->ipChk[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->ipZeroAllow[port] = enable;

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dai_t *dai = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((pEnable == NULL), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pEnable = dai->ipZeroAllow[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_rateLimit_get(sys_logic_port_t port, uint32 *pRateValue)
{
    cfg_dai_t *dai = NULL;

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    *pRateValue = dai->arpLimitRate[port];

    CFG_CLOSE(CFG_ID_DAI);
}

int32 cfg_dai_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    cfg_dai_t *dai = NULL;

    CFG_OPEN(CFG_ID_DAI, (cfg_addr)&dai);

    dai->arpLimitRate[port] = rate;

    CFG_CLOSE(CFG_ID_DAI);
}
