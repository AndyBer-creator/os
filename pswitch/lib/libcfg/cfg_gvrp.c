
#include <libcfg/cfg.h>
#include <libsal/sal_gvrp.h>

int32 cfg_gvrp_enable_set(sys_enable_t enable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    pGvrp->enable = enable;

    CFG_CLOSE(CFG_ID_GVRP);

}

int32 cfg_gvrp_enable_get(sys_enable_t *pEnable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    *pEnable = pGvrp->enable;

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_timer_set(gvrp_timer_type_t  timer_type, uint32 timeout)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    pGvrp->timer[timer_type] = timeout;

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_timer_get(gvrp_timer_type_t  timer_type, uint32 *pTimeout)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    *pTimeout = pGvrp->timer[timer_type];

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    pGvrp->portEnable[port] = enable;

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    *pEnable = pGvrp->portEnable[port];

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    pGvrp->regMode[port] = regmode;

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_regmode_get(sys_logic_port_t port, register_mode_t *pRegMode)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    *pRegMode = pGvrp->regMode[port];

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    pGvrp->forbidVlan[port] = enable;

    CFG_CLOSE(CFG_ID_GVRP);
}

int32 cfg_gvrp_port_forbidVlan_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_gvrp_t *pGvrp = NULL;

    CFG_OPEN(CFG_ID_GVRP, (cfg_addr)&pGvrp);

    *pEnable = pGvrp->forbidVlan[port];

    CFG_CLOSE(CFG_ID_GVRP);
}

