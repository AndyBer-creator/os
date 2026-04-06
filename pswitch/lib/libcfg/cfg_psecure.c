
#include <libcfg/cfg.h>

int32 cfg_psecure_enable_set(sys_enable_t enable)
{
    cfg_psecure_t *pPsecure = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    pPsecure->cfg.enable = enable;

    CFG_CLOSE(CFG_ID_PSECURE);
}

int32 cfg_psecure_enable_get(sys_enable_t *pEnable)
{
    cfg_psecure_t *pPsecure = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    *pEnable= pPsecure->cfg.enable;

    CFG_CLOSE(CFG_ID_PSECURE);
}

int32 cfg_psecure_rateLimit_set(uint32 rate)
{
    cfg_psecure_t *pPsecure = NULL;

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    pPsecure->cfg.rateLimit = rate;

    CFG_CLOSE(CFG_ID_PSECURE);
}

int32 cfg_psecure_rateLimit_get(sys_enable_t *pRate)
{
    cfg_psecure_t *pPsecure = NULL;

    SYS_PARAM_CHK(NULL == pRate, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    *pRate= pPsecure->cfg.rateLimit;

    CFG_CLOSE(CFG_ID_PSECURE);
}

int32 cfg_psecure_port_set(sys_logic_port_t port, sys_psecure_port_t *pSecure)
{
    cfg_psecure_t *pPsecure = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSecure), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    osal_memcpy(&pPsecure->cfg.portCfg[port], pSecure, sizeof(sys_psecure_port_t));

    CFG_CLOSE(CFG_ID_PSECURE);
}

int32 cfg_psecure_port_get(sys_logic_port_t port, sys_psecure_port_t *pSecure)
{
    cfg_psecure_t *pPsecure = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pSecure), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_PSECURE, (cfg_addr)&pPsecure);

    osal_memcpy(pSecure, &pPsecure->cfg.portCfg[port], sizeof(sys_psecure_port_t));

    CFG_CLOSE(CFG_ID_PSECURE);
}

