
#ifndef __RSD_L2_LIMIT_H__
#define __RSD_L2_LIMIT_H__

#include <rtk/l2.h>
#include <common/sys_def.h>
#include <rsd/rsd_nic.h>

typedef enum sys_l2_limit_module_e
{
    
    SYS_L2_LIMIT_MOD_AUTHMGR = 0,
    SYS_L2_LIMIT_MOD_PSECURE,
    SYS_L2_LIMIT_MOD_END
} sys_l2_limit_module_t;

typedef struct sys_l2_limit_s
{
    sys_enable_t enable;
    uint32       limitNum;
    int32        exceedAction;
    int32        moveAction;
    int32        staticMoveAction;
} sys_l2_limit_t;

extern int32 rsd_l2_limit_init(void);

extern int32 rsd_l2_limit_handler_register(sys_l2_limit_module_t module, rsd_nic_handler_t *pHandler);

extern int32 rsd_l2_limit_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_l2_limit_portCtrl_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    sys_enable_t enable);

extern int32 rsd_l2_limit_portEnable_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    sys_enable_t enable);

extern int32 rsd_l2_limit_portLimitNum_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    uint32 limitNum);

extern int32 rsd_l2_limit_portExceedAction_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    int32 exceedAction);

extern int32 rsd_l2_limit_portMoveAction_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    int32 moveAction);

extern int32 rsd_l2_limit_staticPortMoveAction_set(
    sys_l2_limit_module_t module,
    sys_logic_port_t port,
    int32 staticMoveAction);

#endif 

