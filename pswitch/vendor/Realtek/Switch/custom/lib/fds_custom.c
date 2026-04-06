
#include <common/glue_util.h>
#include <libcustom/sal_custom.h>
#include <libcustom/fds_custom.h>

int32 fds_custom_default_init(void)
{
    sys_logic_port_t port;

    sal_custom_enableExample_set(DFLT_CUSTOM_GLOBAL_EBL);

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(fds_custom_port_restore(port));
    }

    return SYS_ERR_OK;
}

int32 fds_custom_port_restore(sys_logic_port_t port)
{
    SYS_ERR_CHK(sal_custom_portEnableExample_set(port, DFLT_CUSTOM_PORT_EBL));

    return SYS_ERR_OK;
}

