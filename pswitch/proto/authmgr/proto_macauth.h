#ifndef __L2G_MACAUTH_H__
#define __L2G_MACAUTH_H__
typedef enum authmgr_delimiter_s
{
    AUTHMGR_DELIMITER_NONE = 0,
    AUTHMGR_DELIMITER_COLON,
    AUTHMGR_DELIMITER_HYPHEN,
    AUTHMGR_DELIMITER_DOT,
    AUTHMGR_DELIMITER_END
} authmgr_delimiter_t;

typedef enum authmgr_macCase_s
{
    AUTHMGR_MACCASE_UPPER = 0,
    AUTHMGR_MACCASE_LOWER,
    AUTHMGR_MACCASE_END
} authmgr_macCase_t;

typedef struct authmgr_radiusUserFmt_s
{
    authmgr_delimiter_t delimiter;
    authmgr_macCase_t   macCase;
    uint32              gap;
} authmgr_radiusUserFmt_t;

typedef struct authmgr_macauth_local_s
{
    sys_mac_t          mac;
    authmgr_author_t   authorInfo;
    uint8              state;
} authmgr_macauth_local_t;

typedef struct macauth_authInfo_s
{
    sys_mac_t           mac;
    sys_logic_port_t    port;
    authmgr_sessId_t    sessId;
    authmgr_author_t    author;
    authmgr_authorSrc_t authorSrc;
    sys_auth_status_t   authSts;
    authmgr_portCtrl_t  ctrl;
} macauth_authInfo_t;

typedef struct macauth_s
{
    authmgr_status_t currState;
    uint8            initialize;

    uint8            reauthEnbl;

    uint8            authing;
    uint8            authSuccess;
    uint8            authFail;
    uint8            forceAuthFail;

    uint8            reauthTimeout;
    uint8            inactTimeout;
    uint8            quietTimeout;

    uint8            guestChk;
    void             *pParentSess; 
} macauth_t;
#define MACAUTH_DBG(_pMacauth, fmt, args...) \
do { \
    char __str[SYS_BUF256_LEN]; \
    SYS_MEM_CLEAR(__str);\
    authmgr_dbgStr_get(AUTHMGR_TYPE_MAC, _pMacauth, __str, fmt, ##args); \
    SYS_DBG(DBG_AUTHMGR_MACAUTH, __str); \
} while(0)
extern macauth_t *macauth_create(sys_logic_port_t port);
extern void macauth_timer_routine(macauth_t *pMacauth);
extern void macauth_timer_chk(macauth_t *pMacauth);
extern int32 authmgr_macauthStatus_set(macauth_authInfo_t *pAuthInfo);

#endif 
