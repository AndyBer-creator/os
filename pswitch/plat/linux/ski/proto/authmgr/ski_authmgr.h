
#ifndef __SKI_AUTHMGR_H__
#define __SKI_AUTHMGR_H__

#include <ski/ski_core.h>
#include <authmgr/proto_authmgr.h>

typedef enum ski_authmgr_cmd_e
{
    
    SKI_AUTHMGR_SET_BASE = (SKI_CORE_MOD_AUTHMGR * SKI_CORE_MODCMD_TOTAL),
    SKI_AUTHMGR_TYPE_ENABLE_SET,
    SKI_AUTHMGR_GUEST_VLAN_SET,
    SKI_AUTHMGR_PORT_CFG_SET,
#ifdef CONFIG_SYS_PROTO_DOT1X
    SKI_AUTHMGR_DOT1X_RAD_RX_SET,
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    SKI_AUTHMGR_MACAUTH_AUTHINFO_SET,
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SKI_AUTHMGR_WEBAUTH_AUTHINFO_SET,
#endif
    SKI_AUTHMGR_SESS_CLEAR_SET,

    SKI_AUTHMGR_GET_BASE = (SKI_AUTHMGR_SET_BASE + SKI_CORE_MODCMD_RANGE),
#ifdef CONFIG_SYS_PROTO_DOT1X
    SKI_AUTHMGR_DOT1X_RAD_SENDID_GET,
#endif
    SKI_AUTHMGR_SESS_CNT_GET,
    SKI_AUTHMGR_SESS_INFO_GET,
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SKI_AUTHMGR_WEBAUTH_STATUS_GET,
    SKI_AUTHMGR_WEBAUTH_SUPPINFO_GET,
#endif

    SKI_AUTHMGR_CMD_END
} ski_authmgr_cmd_t;

typedef union ski_authmgr_ioctl_u
{
    struct
    {
        authmgr_type_t type;
        sys_enable_t   enable;
    } typeEnbl;

    sys_vid_t          guestVlan;

    struct
    {
        sys_logic_port_t   port;
        authmgr_cfg_port_t cfg;
    } portCfg;

    uint32 sessCnt;

    struct
    {
        uint32 idx;
        authmgr_sess_info_t info;
    } sessInfo;

    authmgr_sess_clear_t   sessClear;

#ifdef CONFIG_SYS_PROTO_DOT1X
    dot1x_rad_recv_t   dot1xRadRecv;

    struct
    {
        authmgr_sessId_t sessId;
        uint32           id;
    } radSendId;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
    macauth_authInfo_t macauthAuthInfo;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
    webauth_authInfo_t webauthAuthInfo;

    struct
    {
        uint16           key;
        authmgr_status_t status;
    } statusByKey;

    struct
    {
        uint16             key;
        authmgr_suppInfo_t suppInfo;
    } suppByKey;
#endif
} ski_authmgr_ioctl_t;

extern int32 ski_authmgr_init(void);

#endif  

