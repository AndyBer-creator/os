
#include <libsal/sal_port.h>
#include <libsal/sal_cablediag.h>

int32 sal_cablediag_copper_get(sys_logic_port_t logicPort, sys_rtctResult_t *pRtctResult)
{
    sys_enable_t enable;
    sys_port_media_t media;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRtctResult), SYS_ERR_NULL_POINTER);

    memset(pRtctResult, 0, sizeof(sys_rtctResult_t));

    SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &media));

    if (PORT_MEDIA_COPPER != media)
        return SYS_ERR_PORT_RTCT_NOT_COPPER;

    SYS_ERR_CHK(sal_port_enable_get(logicPort, &enable));

    if (ENABLED == enable)
        SYS_ERR_CHK(rsd_cablediag_copper_get(logicPort, pRtctResult));

    return SYS_ERR_OK;
}

int32 sal_cablediag_copper_set(sys_logic_port_t logicPort)
{
    sys_enable_t enable;
    sys_port_media_t media;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(sal_port_linkMedia_get(logicPort, &media));

    if (PORT_MEDIA_COPPER != media)
        return SYS_ERR_PORT_RTCT_NOT_COPPER;

    SYS_ERR_CHK(sal_port_enable_get(logicPort, &enable));

    if (ENABLED == enable)
        SYS_ERR_CHK(rsd_cablediag_copper_set(logicPort));

    return SYS_ERR_OK;
}
