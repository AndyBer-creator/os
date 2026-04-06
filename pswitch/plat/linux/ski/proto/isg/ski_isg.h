
#ifndef __SKI_ISG_H__
#define __SKI_ISG_H__

#include <ski/ski_core.h>
#include <isg/proto_isg.h>

typedef enum ski_isg_cmd_e
{
    
    SKI_ISG_SET_BASE = (SKI_CORE_MOD_ISG * SKI_CORE_MODCMD_TOTAL),

    SKI_ISG_PORT_ENABLE_SET,
    SKI_ISG_BINDING_ENTRY_SET,
    SKI_ISG_BINDING_ENTRY_DEL,
    SKI_ISG_PORT_MAX_ENTRY_NUM_SET,
    SKI_ISG_DATABASE_SET,
    SKI_ISG_DATABASE_TIMER_SET,

    SKI_ISG_GET_BASE = (SKI_ISG_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_ISG_BINDING_ENTRY_GET,
    SKI_ISG_PORT_CURRENT_ENTRY_NUM_GET,
    SKI_ISG_MAX_ENTRY_NUM_GET,
    SKI_ISG_DATABASE_TIMER_GET,

    SKI_ISG_CMD_END
} ski_isg_cmd_t;

typedef struct sys_isg_entry_del_s
{
    uint32 val;
    sys_isg_entry_filter_del_t filter;
} sys_isg_entry_del_t;

typedef struct ski_isg_ioctl_s
{
    sys_logic_port_t port;
    sys_enable_t enable;

    union
    {
        int32  counter;
        uint32 entryIdx;
        uint32 timer;
        sys_isg_verify_type_t vt;
        sys_isg_entry_t entry;
        sys_isg_db_t db;
        sys_isg_entry_del_t del;
    };
} ski_isg_ioctl_t;

extern int32 ski_isg_init(void);

#endif

