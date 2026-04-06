
#include <common/sys_def.h>
#include <libcfg/cfg.h>
#include <libcfg/cfg_garp.h>
#include <libsal/sal_util.h>
#include <libsal/sal_garp.h>

int32 sal_gmrp_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_ERR_CHK(gmrp_enable_set(enable));

    SYS_ERR_CHK(cfg_gmrp_enable_set(enable));

    return SYS_ERR_OK;
} 

int32 sal_gmrp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(cfg_gmrp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_gmrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(gmrp_port_enable_set(port, enable));
    SYS_ERR_CHK(cfg_gmrp_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_gmrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_gmrp_port_enable_get(port, pEnable));
    return SYS_ERR_OK;
}
#if 0

int32 sal_gmrp_info_get(sys_gvrp_info_t *pInfo)
{
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(gmrp_info_get(pInfo));

    return SYS_ERR_OK;
}
#endif

