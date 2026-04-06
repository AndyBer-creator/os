
#ifndef __SAL_AUTHMGR_H__
#define __SAL_AUTHMGR_H__

#include <authmgr/proto_authmgr.h>
#ifdef CONFIG_SYS_PROTO_DOT1X
#include <libsal/sal_authmgr_dot1x.h>
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
#include <libsal/sal_authmgr_macauth.h>
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
#include <libsal/sal_authmgr_webauth.h>
#endif

extern const sys_text_t text_authmgr_type[];
extern const sys_text_t text_authmgr_method[];
extern const sys_text_t text_authmgr_mode[];
extern const sys_text_t text_authmgr_ctrlType[];
extern const sys_text_t text_authmgr_vlanAssign[];
extern const sys_text_t text_authmgr_status[];
extern const sys_text_t text_authmgr_authorSrc[];

extern int32 sal_authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable);

extern int32 sal_authmgr_typeEnbl_get(authmgr_type_t type, sys_enable_t *pEnable);

extern int32 sal_authmgr_guestVlan_set(sys_vid_t vid);

extern int32 sal_authmgr_guestVlan_get(sys_vid_t *pVid);

extern int32 sal_authmgr_guestVlanPortmask_get(sys_logic_portmask_t *pLpm);

extern int32 sal_authmgr_portGuestVlanEnbl_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_authmgr_portGuestVlanEnbl_get(sys_logic_port_t port, sys_enable_t *pEable);

extern int32 sal_authmgr_portCfg_set(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg);

extern int32 sal_authmgr_portCfg_get(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg);

extern int32 sal_authmgr_portCtrl_set(sys_logic_port_t port, authmgr_portCtrl_t ctrl);

extern int32 sal_authmgr_portCtrl_get(sys_logic_port_t port, authmgr_portCtrl_t *pCtrl);

extern int32 sal_authmgr_portMode_set(sys_logic_port_t port, authmgr_mode_t mode);

extern int32 sal_authmgr_portMode_get(sys_logic_port_t port, authmgr_mode_t *pMode);

extern int32 sal_authmgr_portOrder_set(sys_logic_port_t port, authmgr_order_t order);

extern int32 sal_authmgr_portOrder_get(sys_logic_port_t port, authmgr_order_t *pOrder);

int32 sal_authmgr_portMethodOrder_set(sys_logic_port_t port, authmgr_methodOrder_t order);

extern int32 sal_authmgr_portMethodOrder_get(sys_logic_port_t port, authmgr_methodOrder_t *pOrder);

extern int32 sal_authmgr_portTypeEnbl_set(sys_logic_port_t port, authmgr_type_t type, sys_enable_t enable);

extern int32 sal_authmgr_portTypeEnbl_get(sys_logic_port_t port, authmgr_type_t type, sys_enable_t *pEnable);

extern int32 sal_authmgr_portReauthEnbl_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_authmgr_portReauthEnbl_get(sys_logic_port_t port, sys_enable_t *pEable);

extern int32 sal_authmgr_portMaxHost_set(sys_logic_port_t port, uint32 maxHost);

extern int32 sal_authmgr_portMaxHost_get(sys_logic_port_t port, uint32 *pMaxHost);

extern int32 sal_authmgr_portVlanAssign_set(sys_logic_port_t port, authmgr_vlanAssign_t mode);

extern int32 sal_authmgr_portVlanAssign_get(sys_logic_port_t port, authmgr_vlanAssign_t *pMode);

extern int32 sal_authmgr_portReauthPeriod_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portReauthPeriod_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_portInactTimeout_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portInactTimeout_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_portQuietPeriod_set(sys_logic_port_t port, uint32 time);

extern int32 sal_authmgr_portQuietPeriod_get(sys_logic_port_t port, uint32 *pTime);

extern int32 sal_authmgr_sessCnt_get(uint32 *pCnt);

extern int32 sal_authmgr_sessInfo_getByIdx(uint32 idx, authmgr_sess_info_t *pSessInfo);

extern int32 sal_authmgr_sess_clear(authmgr_sess_clear_t *pSessClear);

#endif 

