
#include "mod_custom.h"

#ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <board/include/lldp.h>
#endif

#ifdef CONFIG_SYS_PROTO_LLDP_MED

int32 mod_custom_lldp_poePortL2Data_get(sys_logic_port_t port, lldp_poe_portL2Data_t *pLayer2)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLayer2), SYS_ERR_NULL_POINTER);

    return SYS_ERR_OK;
}

int32 mod_custom_lldp_poePortL2Data_set(sys_logic_port_t port, lldp_poe_portL2Data_t layer2)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    return SYS_ERR_OK;
}
#endif

int32 mod_custom_lldp_init(void)
{
#ifdef CONFIG_SYS_PROTO_LLDP_MED
    
    static lldp_poe_ops_t lldpPoeOps =
    {
        .lldp_poePortL2Data_get = mod_custom_lldp_poePortL2Data_get,
        .lldp_poePortL2Data_set = mod_custom_lldp_poePortL2Data_set,
    };

    SYS_ERR_CHK(board_lldp_poe_register(&lldpPoeOps));
#endif

    return SYS_ERR_OK;
}

