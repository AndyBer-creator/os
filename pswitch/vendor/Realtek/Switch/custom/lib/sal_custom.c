
#include <common/glue_util.h>
#include <libcustom/sal_custom.h>

int32 _ioctl_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable)
{
    ski_custom_ioctl_t data;

    data.port = port;
    data.enable = enable;

    SYS_ERR_CHK(glue_util_ioctl(SKI_CUSTOM_PORT_ENABLE_SET, &data));

    return SYS_ERR_OK;
}

int32 _ioctl_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    ski_custom_ioctl_t data;

    data.port = port;

    SYS_ERR_CHK(glue_util_ioctl(SKI_CUSTOM_PORT_ENABLE_GET, &data));

    *pEnable = data.enable;

    return SYS_ERR_OK;
}

int32 _ioctl_custom_enableExample_set(sys_enable_t enable)
{
    ski_custom_ioctl_t data;

    data.enable = enable;

    SYS_ERR_CHK(glue_util_ioctl(SKI_CUSTOM_ENABLE_SET, &data));

    return SYS_ERR_OK;
}

int32 _ioctl_custom_enableExample_get(sys_enable_t *pEnable)
{
    ski_custom_ioctl_t data;

    SYS_ERR_CHK(glue_util_ioctl(SKI_CUSTOM_ENABLE_GET, &data));

    *pEnable = data.enable;

    return SYS_ERR_OK;
}

int32 sal_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(_ioctl_custom_portEnableExample_set(port, enable));

    SYS_ERR_CHK(cfg_custom_portEnableExample_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_custom_portEnableExample_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_custom_enableExample_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(_ioctl_custom_enableExample_set(enable));

    SYS_ERR_CHK(cfg_custom_enableExample_set(enable));

    return SYS_ERR_OK;
}

int32 sal_custom_enableExample_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_custom_enableExample_get(pEnable));

    return SYS_ERR_OK;
}

