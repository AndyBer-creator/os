#ifndef __L2G_AUTHMGR_H__
#define __L2G_AUTHMGR_H__
#include <common/sys_error.h>
#include <common/sys_type.h>
#include <common/sys_portview.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_auth.h>
#include <rsd/rsd_vlan.h>

#include <authmgr/proto_authmgr_common.h>
#ifdef CONFIG_SYS_PROTO_DOT1X
#include <authmgr/proto_dot1x.h>
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
#include <authmgr/proto_macauth.h>
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
#include <authmgr/proto_webauth.h>
#endif
typedef enum authmgr_mode_e
{
    AUTHMGR_MODE_MULTI_AUTH = 0,
    AUTHMGR_MODE_MULTI_HOST,
    AUTHMGR_MODE_SINGLE_HOST,
    AUTHMGR_MODE_END
} authmgr_mode_t;

typedef enum authmgr_type_e
{
    AUTHMGR_TYPE_NONE = 0,
#ifdef CONFIG_SYS_PROTO_DOT1X
    AUTHMGR_TYPE_DOT1X,
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    AUTHMGR_TYPE_MAC,
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    AUTHMGR_TYPE_WEB,
#endif
    AUTHMGR_TYPE_END
} authmgr_type_t;

typedef enum authmgr_method_e
{
    AUTHMGR_METHOD_NONE = 0,
    AUTHMGR_METHOD_RADIUS,
#if defined(CONFIG_SYS_PROTO_MACAUTH) || defined(CONFIG_SYS_PROTO_WEBAUTH)
    AUTHMGR_METHOD_LOCAL,
#endif
    AUTHMGR_METHOD_END
} authmgr_method_t;

typedef enum authmgr_vlanAssign_e
{
    AUTHMGR_VLAN_ASSIGN_DISABLE,
    AUTHMGR_VLAN_ASSIGN_REJECT,
    AUTHMGR_VLAN_ASSIGN_STATIC,
    AUTHMGR_VLAN_ASSIGN_END
} authmgr_vlanAssign_t;

typedef enum authmgr_sessClearType_e
{
    AUTHMGR_SESS_CLEAR_ALL = 0,
    AUTHMGR_SESS_CLEAR_INTF,
    AUTHMGR_SESS_CLEAR_MAC,
    AUTHMGR_SESS_CLEAR_ID,
    AUTHMGR_SESS_CLEAR_TYPE,
    AUTHMGR_SESS_CLEAR_END
} authmgr_sessClearType_t;

typedef struct authmgr_order_s
{
    authmgr_type_t order[AUTHMGR_TYPE_END - 1];
} authmgr_order_t;

typedef struct authmgr_methodOrder_s
{
    authmgr_method_t order[AUTHMGR_METHOD_END - 1];
} authmgr_methodOrder_t;

typedef struct authmgr_cfg_port_s
{
    authmgr_portCtrl_t    ctrlType;
    authmgr_mode_t        mode;
    authmgr_order_t       typeOrder;
    uint8                 typeEnbl[AUTHMGR_TYPE_END];
    authmgr_methodOrder_t methodOrder;
    uint8                 reauthEnbl;
    uint8                 guestVlanEnbl;
    uint16                maxHosts;
    authmgr_vlanAssign_t  vlanAssignMode;

    uint32                reauthPeriod;
    uint16                inactTimeout;
    uint16                quietPeriod;
    
#ifdef CONFIG_SYS_PROTO_DOT1X
  
    uint8                 maxReq;
    uint16                txPeriod;
    uint16                suppTimeout;
    uint16                serverTimeout;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
  
    uint8                 loginAttempt;
#endif
} authmgr_cfg_port_t;

typedef struct authmgr_cfg_s
{
    uint8                   typeEnbl[AUTHMGR_TYPE_END];
    sys_vid_t               guestVlan;
#ifdef CONFIG_SYS_PROTO_MACAUTH
    authmgr_radiusUserFmt_t radiusUserFmt;
#endif
    authmgr_cfg_port_t      portCfg[SYS_NORMALPORT_NUM_MAX];
} authmgr_cfg_t;

typedef struct authmgr_sess_status_s
{
    authmgr_status_t    authStatus;
    sys_vid_t           currVid;
    authmgr_author_t    authorInfo;
    authmgr_authorSrc_t authorSrc;

    uint32           authedWhile;
    uint32           unauthedWhile;
    uint32           inactWhile;
    uint16           quietWhile;

    uint32           pktCnt;
    uint32           aclKey;
} authmgr_sess_status_t;

typedef struct authmgr_sess_info_s
{
    authmgr_sessId_t        sessId;         
    authmgr_type_t          currType;
    authmgr_suppInfo_t      suppInfo;
    authmgr_sess_status_t   status;
} authmgr_sess_info_t;

typedef struct authmgr_sess_clear_s
{
    authmgr_sessClearType_t  clearType;

    union
    {
        sys_logic_portmask_t lpm;
        sys_mac_t            mac;
        authmgr_sessId_t     sessId;
        authmgr_type_t       type;
    } data;
} authmgr_sess_clear_t;

typedef struct authmgr_sess_s
{
    authmgr_sessId_t        sessId;         
    authmgr_order_t         typeOrder;      
    uint8                   currTypeIdx;    
    uint8                   gotoNextType;   
    authmgr_suppInfo_t      suppInfo;
    authmgr_sess_status_t   status;
    authmgr_mode_t          currMode;

#ifdef CONFIG_SYS_PROTO_DOT1X
    dot1x_pacp_t            *pDot1x;
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    macauth_t               *pMacauth;
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    webauth_t               *pWebauth;
#endif

    authmgr_cfg_port_t      *pPortCfg;      

    uint8                   delSess;
    struct authmgr_sess_s   *next;
} authmgr_sess_t;

typedef struct authmgr_sess_ptr_s
{
    authmgr_sess_t *pSess;
} authmgr_sess_ptr_t;

extern osal_mutex_t authmgr_sem;
extern authmgr_cfg_t g_authCfg;
extern sys_logic_portmask_t g_portAuthLpm;
#define AUTHMGR_SEM_LOCK(sem)   \
do { } while (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define AUTHMGR_SEM_UNLOCK(sem) \
do { } while (osal_sem_mutex_give(sem) != SYS_ERR_OK)

#define AUTHMGR_DBG(fmt, args...) \
do { \
    SYS_DBG(DBG_AUTHMGR, fmt, ##args); \
} while(0)
#define AUTHMGR_SESS_DBG(_pSess, fmt, args...) \
do { \
    char __str[SYS_BUF256_LEN]; \
    SYS_MEM_CLEAR(__str);\
    authmgr_dbgStr_get(AUTHMGR_TYPE_END, _pSess, __str, fmt, ##args); \
    SYS_DBG(DBG_AUTHMGR, __str); \
} while(0)
#define IS_PORT_GUEST_VLAN_ENABLE(port) ((g_authCfg.guestVlan != 0) && g_authCfg.portCfg[port].guestVlanEnbl)
extern authmgr_sess_t *authmgr_sess_getById_ret(authmgr_sessId_t *pSessId);
extern authmgr_sess_t *authmgr_sess_getBySuppInfo_ret(authmgr_suppInfo_t *pSuppInfo);
extern void authmgr_dbgStr_get(authmgr_type_t type, void *pInfo, char *str, char* fmt,...);
extern int32 authmgr_action_authStatus_set(authmgr_sess_t *pSess, authmgr_status_t status);
extern authmgr_status_t authmgr_action_authStatus_ret(authmgr_sess_t *pSess);
extern int32 authmgr_action_authStatus_clear(authmgr_sess_t *pSess);
extern uint32 authmgr_action_nextType_ret(authmgr_sess_t *pSess);
extern uint32 authmgr_action_nextTypeIdx_ret(authmgr_sess_t *pSess);
extern uint32 authmgr_action_activeChk_ret(authmgr_sess_t *pSess);
extern uint32 authmgr_action_guestActiveChk_ret(authmgr_sess_t *pSess);
extern int32 authmgr_action_authorInfo_set(authmgr_sess_t *pSess, authmgr_author_t *pAuthor, authmgr_authorSrc_t *pAuthorSrc);
extern int32 authmgr_init(void);
extern int32 authmgr_exit(void);
extern int32 authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable);
extern int32 authmgr_guestVlan_set(sys_vid_t vid);
extern int32 authmgr_portCfg_set(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg);
extern int32 authmgr_sessCnt_get(uint32 *pCnt);
extern int32 authmgr_sessInfo_getByIdx(uint32 idx, authmgr_sess_info_t *pSessInfo);
extern int32 authmgr_sess_clear(authmgr_sess_clear_t *pSessClear);

#endif 
