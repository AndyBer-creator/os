#ifndef __L2G_WEBAUTH_H__
#define __L2G_WEBAUTH_H__
#define WEBAUTH_NAT_TCPPORT_HTTP  40001
#define WEBAUTH_NAT_TCPPORT_HTTPS 40002

#define WEBAUTH_HTTP_PID_FILE        "/var/run/boa_http_wa.pid"
#define WEBAUTH_HTTPS_PID_FILE       "/var/run/boa_https_wa.pid"
typedef struct webauth_arp_s
{
    sys_ipv4_t ip;
    sys_mac_t  mac;
} webauth_arp_t;

typedef struct webauth_tcpSessInfo_s
{
    uint8              ipVer;
    union {
        sys_ipv4_t     srcIP;
        sys_ipv6_t     srcIPv6;
    };
    union {
        sys_ipv4_t     dstIP;
        sys_ipv6_t     dstIPv6;
    };
    authmgr_suppInfo_t suppInfo;
    authmgr_sessId_t   sessId;
    uint16             tcpPort;
} webauth_tcpSessInfo_t;

typedef struct webauth_tcpSess_s
{
    webauth_tcpSessInfo_t sessInfo;
    uint16                timer;

    struct webauth_tcpSess_s *next;
} webauth_tcpSess_t;

typedef struct authmgr_webauth_local_s
{
    char             username[CAPA_SYS_USERNAME_LEN + 1];
    char             password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    authmgr_author_t authorInfo;
} authmgr_webauth_local_t;

typedef struct webauth_authInfo_s
{
    uint16              tcpPort;
    char                username[CAPA_SYS_USERNAME_LEN + 1];
    authmgr_author_t    author;
    authmgr_authorSrc_t authorSrc;
    sys_auth_status_t   authSts;
} webauth_authInfo_t;

typedef struct webauth_s
{
    authmgr_status_t currState;
    uint8            initialize;

    uint8            reauthEnbl;

    uint8            authSuccess;
    uint8            authFail;

    uint8            reauthTimeout;
    uint8            inactTimeout;
    uint8            quietTimeout;

    uint8            loginFailNum;

    uint8            guestChk;
    uint8            inactWhile;
    void             *pParentSess; 
} webauth_t;
#define WEBAUTH_SEM_LOCK(sem)   \
do { } while (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define WEBAUTH_SEM_UNLOCK(sem) \
do { } while (osal_sem_mutex_give(sem) != SYS_ERR_OK)

#define WEBAUTH_DBG(_pWebauth, fmt, args...) \
do { \
    char __str[SYS_BUF256_LEN]; \
    SYS_MEM_CLEAR(__str);\
    authmgr_dbgStr_get(AUTHMGR_TYPE_WEB, _pWebauth, __str, fmt, ##args); \
    SYS_DBG(DBG_AUTHMGR_WEBAUTH, __str); \
} while(0)

#define WEBAUTH_DBG2(fmt, args...) \
do { \
    SYS_DBG(DBG_AUTHMGR_WEBAUTH, fmt, ##args); \
} while(0)
extern l2g_act_t webauth_pkt_chk(sys_nic_pkt_t *pPkt, void *pCookie);
extern uint32 webauth_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
extern int32 webauth_init(void);
extern int32 webauth_exit(void);
extern webauth_t *webauth_create(sys_logic_port_t port);
extern void webauth_timer_routine(webauth_t *pWebauth);
extern void webauth_timer_chk(webauth_t *pWebauth);
extern int32 authmgr_webauthStatus_set(webauth_authInfo_t *pAuthInfo);
extern int32 authmgr_webauthStatusByKey_get(uint32 key, authmgr_status_t *pStatus);
extern int32 authmgr_webauthSuppInfoByKey_get(uint32 key, authmgr_suppInfo_t *pSupp);

#endif 
