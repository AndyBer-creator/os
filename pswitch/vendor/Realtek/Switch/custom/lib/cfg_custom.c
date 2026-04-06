
#include <libcfg/cfg.h>

int32 cfg_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_custom_t *pCustom = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_CUSTOM, (cfg_addr)&pCustom);

    pCustom->portEnableExample[port] = enable;

    CFG_CLOSE(CFG_ID_CUSTOM);
}

int32 cfg_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_custom_t *pCustom = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_CUSTOM, (cfg_addr)&pCustom);

    *pEnable = pCustom->portEnableExample[port];

    CFG_CLOSE(CFG_ID_CUSTOM);
}

int32 cfg_custom_enableExample_set(sys_enable_t enable)
{
    cfg_custom_t *pCustom = NULL;

    CFG_OPEN(CFG_ID_CUSTOM, (cfg_addr)&pCustom);

    pCustom->enableExample = enable;

    CFG_CLOSE(CFG_ID_CUSTOM);
}

int32 cfg_custom_enableExample_get(sys_enable_t *pEnable)
{
    cfg_custom_t *pCustom = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_CUSTOM, (cfg_addr)&pCustom);

    *pEnable = pCustom->enableExample;

    CFG_CLOSE(CFG_ID_CUSTOM);
}

int32 cfg_custom_init(void)
{
    return SYS_ERR_OK;
}

