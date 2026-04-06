
#ifndef __RSD_TRAP_H__
#define __RSD_TRAP_H__

#include <rtk/trap.h>

#include <common/sys_def.h>

typedef rtk_trap_reason_type_t sys_trap_reason_type_t;
typedef rtk_trap_type_t sys_trap_type_t;
typedef rtk_mgmt_action_t sys_trap_rma_action_t;
typedef rtk_trap_mgmtType_t sys_trap_mgmtType_t;
typedef rtk_trap_userDefinedRma_t sys_trap_userDefinedRma_t;
typedef rtk_trap_userDefinedMgmt_t sys_trap_userDefinedMgmt_t;

typedef struct rmaActionLog_s{
    uint8           user_configured;
    uint8           proto_configured;
    sys_trap_rma_action_t    default_action;
    sys_action_t    user_action;
    sys_action_t    proto_action;
} rmaActionLog_t;

extern rmaActionLog_t portRmaActionLog[SYS_LOGICPORT_NUM_MAX][MGMT_TYPE_END];
extern rmaActionLog_t rmaActionLog[0x30];

extern int32 rsd_trap_portMgmtFrameAction_set(
    sys_logic_port_t port,
    sys_trap_mgmtType_t frameType,
    sys_action_t action,
    sys_callerType_t callerType);

extern int32 rsd_trap_portMgmtFrameAction_release(sys_logic_port_t port, sys_trap_mgmtType_t frameType, sys_callerType_t callerType);

extern int32 rsd_trap_mgmtFrameAction_set(sys_trap_mgmtType_t frameType, sys_action_t action);

extern int32 rsd_trap_mgmtFramePri_set(sys_trap_mgmtType_t frameType, sys_pri_t priority);

extern int32 rsd_trap_rmaAction_set(sys_mac_t *pRma_frame, sys_trap_rma_action_t rma_action, sys_callerType_t callerType);

extern int32 rsd_trap_rmaAction_get(sys_mac_t *pRma_frame, sys_trap_rma_action_t *pRma_action);

extern int32 rsd_trap_rmaAction_release(sys_mac_t *pRma_frame, sys_callerType_t callerType);

extern int32 rsd_trap_userDefineRma_set(uint32 userDefine_idx, sys_trap_userDefinedRma_t *pUserDefinedRma);

extern int32 rsd_trap_userDefineRmaAction_set(uint32 userDefine_idx, sys_trap_rma_action_t action);

extern int32 rsd_trap_userDefineRmaEnable_set(uint32 userDefine_idx, sys_enable_t enable);

extern int32 rsd_trap_bypassStp_set(sys_trap_bypassStpType_t frameType, sys_enable_t enable);

extern int32 rsd_trap_bypassVlan_set(sys_trap_bypassVlanType_t frameType, sys_enable_t enable);

extern int32 rsd_trap_init(void);
#endif 

