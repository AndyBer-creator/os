
#include <libsal/sal_port.h>
#include <libsal/sal_fiber.h>

int32 sal_fiber_oeStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);

    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    ret = rsd_fiber_oeStatus_get(logicPort, pStatus);

    return ret;
}

int32 sal_fiber_losStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    ret = rsd_fiber_losStatus_get(logicPort, pStatus);

    return ret;
}

int32 sal_fiber_detectStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus)
{
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    ret = rsd_fiber_detectStatus_get(logicPort, pStatus);

    return ret;
}

