
#ifndef __RSD_AUTH_H__
#define __RSD_AUTH_H__

#include <common/sys_def.h>

typedef enum sys_auth_status_e
{
    SYS_AUTH_DISABLE = 0,
    SYS_UNAUTH,
    SYS_AUTH,
    SYS_AUTHING,
    SYS_AUTH_STATUS_END
} sys_auth_status_t;

typedef enum sys_auth_dirction_e
{
    SYS_AUTH_DIRECTION_BOTH = 0,
    SYS_AUTH_DIRECTION_IN,
    SYS_AUTH_DIRECTION_END
} sys_auth_dirction_t;

typedef enum sys_auth_unauth_action_e
{
    SYS_AUTH_ACTION_FORWARD = ACTION_FORWARD,
    SYS_AUTH_ACTION_DROP = ACTION_DROP,
    SYS_AUTH_ACTION_TRAP2CPU = ACTION_TRAP2CPU,
    SYS_AUTH_ACTION_END
} sys_auth_unauth_action_t;

typedef enum sys_auth_eapol_act_e
{
    SYS_AUTH_EAPOL_ACTION_DROP = 0,
    SYS_AUTH_EAPOL_ACTION_TRAP,
    SYS_AUTH_EAPOL_ACTION_FLOOD,
    SYS_AUTH_EAPOL_ACTION_END
} sys_auth_eapol_act_t;

typedef struct sys_auth_event_s
{
    sys_logic_portmask_t unauthLpm;
} sys_auth_event_t;

extern int32 rsd_auth_init(void);

extern int32 rsd_auth_portBasedState_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_auth_portBasedState_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_auth_portBasedAuthStatus_set(sys_logic_port_t port, sys_auth_status_t status);

extern int32 rsd_auth_portBasedAuthStatus_get(sys_logic_port_t port, sys_auth_status_t *pStatus);

extern int32 rsd_auth_direction_set(sys_logic_port_t port, sys_auth_dirction_t dir);

extern int32 rsd_auth_direction_get(sys_logic_port_t port, sys_auth_dirction_t *pDir);

extern int32 rsd_auth_unauthPktAction_set(sys_logic_port_t port, sys_auth_unauth_action_t action);

extern int32 rsd_auth_unauthPktAction_get(sys_logic_port_t port, sys_auth_unauth_action_t *pAction);

extern int32 rsd_auth_macBasedState_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_auth_macBasedState_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_auth_macBasedAuthMac_add(sys_vid_t vid, sys_logic_port_t port, sys_mac_t *pMac);

extern int32 rsd_auth_macBasedAuthMac_del(sys_vid_t vid, sys_logic_port_t port, sys_mac_t *pMac);

extern int32 rsd_auth_unauthPortmask_get(sys_logic_portmask_t *pLpm);

extern int32 rsd_auth_unauthOpenPort_add(sys_logic_port_t port);

extern int32 rsd_auth_unauthOpenPort_del(sys_logic_port_t port);

extern int32 rsd_auth_eapolEnable_set(sys_enable_t enable);

extern int32 rsd_auth_eapolDisableAction_set(sys_auth_eapol_act_t act);

extern int32 rsd_auth_portEapolEnable_set(sys_logic_port_t port, sys_enable_t enable);

#endif 

