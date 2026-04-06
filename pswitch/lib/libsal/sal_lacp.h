
#ifndef __SAL_LACP_H__
#define __SAL_LACP_H__

#include <lacp/proto_lacp.h>

extern const sys_text_t text_lacp_timeout[];

#define MUX2STR(state)   (sal_lacp_mux_state_to_str_ret(state))
#define RCV2STR(state)   (sal_lacp_rcv_state_to_str_ret(state))
#define PERIO2STR(state) (sal_lacp_perio_state_to_str_ret(state))
#define SEL2STR(state)   (sal_lacp_select_state_to_str_ret(state))

extern int32 sal_lacp_portPriority_set(sys_logic_port_t port, uint16 pri);

extern int32 sal_lacp_portPriority_get(sys_logic_port_t port, uint16 *pPri);

extern int32 sal_lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout);

extern int32 sal_lacp_portTimeout_get(sys_logic_port_t port, sys_lacp_timeout_t *pTimeout);

extern int32 sal_lacp_activeMode_set(uint32 trunkId, sys_logic_portmask_t *pActPm);

extern int32 sal_lacp_activeMode_get(uint32 trunkId, sys_logic_portmask_t *pActPm);

extern int32 sal_lacp_member_set(uint32 trunkId, sys_logic_portmask_t *pPortmask);

extern int32 sal_lacp_group_del(uint32 trunkId);

extern int32 sal_lacp_enable_set(sys_enable_t enable);

extern int32 sal_lacp_enable_get(sys_enable_t *pEnable);

extern int32 sal_lacp_systemPriority_set(uint32 sysPri);

extern int32 sal_lacp_systemPriority_get(uint32 *pSysPri);

extern int32 sal_lacp_portState_get(sys_logic_port_t port, sys_lacp_port_t *pPortState);

extern int32 sal_lacp_portCounter_get(sys_logic_port_t port, sys_lacp_counter_t *pPortCounter);

extern int32 sal_lacp_portCounter_reset(sys_logic_port_t port);

extern char *sal_lacp_mux_state_to_str_ret(sys_muxm_state_t s);

extern char *sal_lacp_rcv_state_to_str_ret(sys_rcvm_state_t rcv);

extern char *sal_lacp_perio_state_to_str_ret(sys_perio_state_t perio);

extern char *sal_lacp_select_state_to_str_ret(sys_mosm_t mosm_state);

#endif 

