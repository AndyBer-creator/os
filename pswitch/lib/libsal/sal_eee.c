
#include <libsal/sal_util.h>
#include <libsal/sal_eee.h>

int32 sal_eee_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_eee_portEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_eee_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_eee_portEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_eee_portState_get(sys_logic_port_t port, sys_enable_t *pState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_eee_portState_get(port, pState));

    return SYS_ERR_OK;
}

int32 sal_eee_enablePortmask_set(sys_enable_t enable, sys_logic_portmask_t *pPortmask)
{
    char portstr[CAPA_PORTMASK_STR_LEN];

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    osal_memset(portstr, 0, sizeof(portstr));

    SYS_ERR_CHK(rsd_eee_enablePortmask_set(enable, pPortmask));

    LPM2STR(pPortmask, portstr);

    return SYS_ERR_OK;
}

int32 sal_eee_enablePortmask_get(sys_enable_t enable, sys_logic_portmask_t *pPortmask)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_eee_enablePortmask_get(enable, pPortmask));

    return SYS_ERR_OK;
}

