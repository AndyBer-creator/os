
#ifndef __SAL_AUTHMGR_DOT1X_H__
#define __SAL_AUTHMGR_DOT1X_H__

extern int32 sal_authmgr_portMaxEapReq_set(sys_logic_port_t port, uint32 maxReq);

extern int32 sal_authmgr_portMaxEapReq_get(sys_logic_port_t port, uint32 *pMaxReq);

extern int32 sal_authmgr_portEapTxPeriod_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portEapTxPeriod_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_portSuppTimeout_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portSuppTimeout_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_portSrvTimeout_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portSrvTimeout_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_dot1xRadius_tx(dot1x_rad_xmit_t *pXmit, dot1x_rad_recv_t *pRecv);

extern int32 sal_authmgr_dot1xRadius_rx(dot1x_rad_recv_t recv);

#endif 

