
#include <libcfg/cfg.h>
#include <libsal/sal_garp.h>

int32 cfg_gmrp_enable_set(sys_enable_t enable)
{
    cfg_gmrp_t *pGmrp = NULL;

    CFG_OPEN(CFG_ID_GARP, (cfg_addr)&pGmrp);

    pGmrp->enable = enable;

    CFG_CLOSE(CFG_ID_GARP);

}

int32 cfg_gmrp_enable_get(sys_enable_t *pEnable)
{
    cfg_gmrp_t *pGmrp = NULL;

    CFG_OPEN(CFG_ID_GARP, (cfg_addr)&pGmrp);

    *pEnable = pGmrp->enable;

    CFG_CLOSE(CFG_ID_GARP);
}

int32 cfg_gmrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_gmrp_t *pGmrp = NULL;

    CFG_OPEN(CFG_ID_GARP, (cfg_addr)&pGmrp);

    pGmrp->portEnable[port] = enable;

    CFG_CLOSE(CFG_ID_GARP);
}

int32 cfg_gmrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_gmrp_t *pGmrp = NULL;

    CFG_OPEN(CFG_ID_GARP, (cfg_addr)&pGmrp);

    *pEnable = pGmrp->portEnable[port];

    CFG_CLOSE(CFG_ID_GARP);
}

