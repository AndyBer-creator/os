#ifndef __L2G_AUTHMGR_COMMON_H__
#define __L2G_AUTHMGR_COMMON_H__
typedef enum authmgr_portCtrl_s
{
    AUTHMGR_PORTCTRL_DISABLE = 0,
    AUTHMGR_PORTCTRL_FORCE_AUTHORIZED,
    AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED,
    AUTHMGR_PORTCTRL_AUTO,
} authmgr_portCtrl_t;

typedef enum authmgr_status_e
{
    AUTHMGR_STATUS_DISABLE = 0,
    AUTHMGR_STATUS_INIT,
    AUTHMGR_STATUS_AUTHED,
    AUTHMGR_STATUS_UNAUTHED,
    AUTHMGR_STATUS_LOCKED,
    AUTHMGR_STATUS_GUEST,
    AUTHMGR_STATUS_END
} authmgr_status_t;

typedef enum authmgr_author_srcType_e
{
    AUTHMGR_AUTHOR_SRC_CFG = 0,
    AUTHMGR_AUTHOR_SRC_LOCAL,
    AUTHMGR_AUTHOR_SRC_RADIUS,
    AUTHMGR_AUTHOR_SRC_END
} authmgr_author_srcType_t;

typedef struct authmgr_sessId_s
{
    uint32 cnt;
    uint32 time;
} authmgr_sessId_t;

typedef struct authmgr_suppInfo_s
{
    sys_logic_port_t port;
    sys_mac_t        mac;
    sys_vid_t        vid;
} authmgr_suppInfo_t;

typedef struct authmgr_author_s
{
    sys_vid_t    authVid;
    uint32       reauthTime;
    uint32       inactTime;
} authmgr_author_t;

typedef struct authmgr_authorSrc_s
{
    authmgr_author_srcType_t vidSrc;
    authmgr_author_srcType_t reauthTimeSrc;
    authmgr_author_srcType_t inactTimeSrc;
} authmgr_authorSrc_t;
#endif 
