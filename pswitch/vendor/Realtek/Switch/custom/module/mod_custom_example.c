
#include "mod_custom_example.h"
#include <rsd/rsd_acl_rsv.h>

sys_enable_t enableExample = DISABLED;
sys_enable_t portEnableExample[SYS_LOGICPORT_NUM_MAX];
rsd_acl_rsvEntry_t  rsvAclExample;

int32 mod_custom_portEnableExample_set(sys_logic_port_t port, sys_enable_t enable)
{
    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(rsvAclExample.field.spm, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(rsvAclExample.field.spm, port);

    SYS_ERR_CHK(rsd_acl_rsv_rule_update(ACL_RSV_CUSTOM1, &rsvAclExample.field,
        &rsvAclExample.act_ebl, &rsvAclExample.act_dbl));

    portEnableExample[port] = enable;

    return SYS_ERR_OK;
}

int32 mod_custom_portEnableExample_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    *pEnable = portEnableExample[port];

    return SYS_ERR_OK;
}

int32 mod_custom_enableExample_set(sys_enable_t enable)
{
    SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_CUSTOM1, enable));

    enableExample = enable;

    return SYS_ERR_OK;
}

int32 mod_custom_enableExample_get(sys_enable_t *pEnable)
{
    *pEnable = enableExample;

    return SYS_ERR_OK;
}

int32 mod_custom_example_init(void)
{
    SYS_MEM_CLEAR(rsvAclExample);

    rsvAclExample.act_ebl.actionMask |= ACL_ACTIONMASK_TRAP;

    rsvAclExample.act_dbl.actionMask |= ACL_ACTIONMASK_PERMIT;

    rsvAclExample.status = DISABLED;

    rsvAclExample.type = ACL_RSV_CUSTOM1;

    rsvAclExample.field.fieldMask |= ACL_RSV_FIELDMASK_ETHERTYPE;
    rsvAclExample.field.fieldMask |= ACL_RSV_FIELDMASK_SPM;
    rsvAclExample.field.ethertype = 0x9999;
    LOGIC_PORTMASK_CLEAR_ALL(rsvAclExample.field.spm);

    SYS_ERR_CHK(rsd_acl_rsv_rule_update(ACL_RSV_CUSTOM1, &rsvAclExample.field,
        &rsvAclExample.act_ebl, &rsvAclExample.act_dbl));

    return SYS_ERR_OK;
}

