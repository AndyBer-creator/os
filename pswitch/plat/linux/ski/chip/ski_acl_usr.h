
#ifndef __SKI_ACL_H__
#define __SKI_ACL_H__

#include <ski/ski_core.h>
#include <rsd/rsd_acl_usr.h>

typedef enum ski_acl_cmd_e
{
    
    SKI_ACL_SET_BASE = (SKI_CORE_MOD_ACL * SKI_CORE_MODCMD_TOTAL),
    SKI_ACL_SET_ACL_CREATE,
    SKI_ACL_SET_ACL_DEL,
    SKI_ACL_SET_ACL_BIND,
    SKI_ACL_SET_ACL_UNBIND,
    SKI_ACL_SET_ACE_CREATE,
    SKI_ACL_SET_ACE_MODIFY,
    SKI_ACL_SET_ACE_DEL,
    SKI_ACL_SET_L4_PORT_RANGE_DEL,

    SKI_ACL_GET_BASE = (SKI_ACL_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_ACL_GET_ACL_CNT,
    SKI_ACL_GET_ACL_NAME,
    SKI_ACL_GET_ACL_TYPE,
    SKI_ACL_GET_ACL_BINDING,
    SKI_ACL_GET_ACE_CNT,
    SKI_ACL_GET_ACE_CONTENT,
    SKI_ACL_GET_L4_PORT_RANGE,
    SKI_ACL_GET_L4_PORT_RANGE_IDX,
    SKI_ACL_GET_PIE_USAGE,
    SKI_ACL_GET_IS_ACL_BIND,

    SKI_ACL_CMD_END
} ski_acl_cmd_t;

typedef struct ski_acl_ioctl_s
{
    sys_namestr_t name;
    sys_acl_bind_names_t names;
    sys_acl_type_t type;
    sys_ace_content_t ace_content;
    sys_l4port_range_t portRange;
    sys_logic_port_t lp;

    union
    {
        sys_logic_block_4show_t pieUsage;
        sys_logic_portmask_t portmask;
        uint32 ace_priority;
        uint32 count;
        uint32 index;
        uint32 flag;
        uint8 portRangeIdx;
    };
} ski_acl_ioctl_t;

extern int32 ski_acl_init(void);

#endif

