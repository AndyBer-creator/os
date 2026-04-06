#ifndef __L2G_DOT1X_H__
#define __L2G_DOT1X_H__
#define ETHER_MAXSIZE       1496

#define EAPOL_FRAME         0x888e
#define EAPOL_HDRLEN        4
#define EAP_HDRLEN          4
#define EAP_REQ_RESP_LEN    1

#define EAP_IDENTITY        1
#define EAP_NAK             3
#define EAP_MD5_CHALLENGE   4
#define EAP_TLS             13
#define EAP_TTLS            21
#define EAP_PEAP            25

#define DOT1X_RAD_FRAMED_MTU 1300
typedef struct eap_pkt_s
{
    uint8  *pkt;
    uint16 length;
} eap_pkt_t;

typedef enum eap_code_e
{
    EAP_REQUEST = 1,
    EAP_RESPONSE,
    EAP_SUCCESS,
    EAP_FAILURE
} eap_code_t;

typedef enum eapol_type_s
{
    EAPOL_EAP_PACKET,
    EAPOL_START,
    EAPOL_LOGOFF,
    EAPOL_KEY,
    EAPOL_ASF_ALERT,
} eapol_type_t;

typedef struct eapol_header_s
{
    uint8   eapol_version;
    uint8   eapol_type;
    uint16  eapol_body_length;
} __attribute__ ((aligned(1), packed)) eapol_header_t;

typedef struct eap_header_s
{
    uint8   eap_code;
    uint8   eap_identifier;
    uint16  eap_length;
    uint8   eap_type;
} __attribute__ ((aligned(1), packed)) eap_header_t;
typedef struct dot1x_radius_stateAvp_s
{
    uint8 len;
    char  val[CAPA_AUTHMGR_DOT1X_STATE_AVP_LEN];
} dot1x_radius_stateAvp_t;

typedef struct dot1x_rad_xmitInfo_s
{
    authmgr_sessId_t        sessId;
    uint32                  dataLen;
    sys_mac_t               callingId;
    sys_logic_port_t        nasPort;
    char                    userName[CAPA_AUTHMGR_DOT1X_USERNAME_LEN + 1];

    dot1x_radius_stateAvp_t stateAvp;
    uint32                  radiusSendId;
} dot1x_rad_xmitInfo_t;

typedef struct dot1x_rad_xmit_s
{
    dot1x_rad_xmitInfo_t info;
    uint8                *pEapPkt;
} dot1x_rad_xmit_t;

typedef struct dot1x_rad_recv_s
{
    authmgr_sessId_t        sessId;
    authmgr_author_t        author;
    authmgr_authorSrc_t     authorSrc;
    dot1x_radius_stateAvp_t stateAvp;
    uint16                  eapLen;
    uint8                   *eapData;
} dot1x_rad_recv_t;
typedef enum dot1x_portControl_s
{
    DOT1X_MODE_FORCE_AUTHORIZED = 0,
    DOT1X_MODE_FORCE_UNAUTHORIZED,
    DOT1X_MODE_AUTO,
} dot1x_portControl_t;

typedef enum dot1x_authStatus_s
{
    DOT1X_STATE_UNAUTHORIZED = 0,
    DOT1X_STATE_AUTHORIZED,
} dot1x_authStatus_t;
typedef enum dot1x_pae_sm_state_s
{
    DOT1X_PAE_INITIALIZE = 0,
    DOT1X_PAE_DISCONNECTED,
    DOT1X_PAE_RESTART,
    DOT1X_PAE_CONNECTING,
    DOT1X_PAE_AUTHENTICATING,
    DOT1X_PAE_AUTHENTICATED,
    DOT1X_PAE_ABORTING,
    DOT1X_PAE_HELD,
    DOT1X_PAE_FORCE_AUTH,
    DOT1X_PAE_FORCE_UNAUTH
} dot1x_pae_sm_state_t;

typedef struct dot1x_pae_sm_s
{
    dot1x_pae_sm_state_t curState;
    uint8                chkNow;

    uint16  txPeriod;
    uint8   reqCnt;
    uint16  txWhile;

    uint8   eapolLogoff;
    uint8   eapolStart;
    uint8   eapReq;
    uint8   eapRestart;
    uint8   portMode;
    uint16  reAuthCount;
    
    uint16  quietPeriod;
    uint8   reAuthMax;

    eap_pkt_t fromSupp;
    eap_pkt_t fromSvr;
} dot1x_pae_sm_t;
typedef enum dot1x_reAuth_sm_state_s
{
    DOT1X_REAUTH_INITIALIZE = 0,
    DOT1X_REAUTH_REAUTHENTICATE,
} dot1x_reAuth_sm_state_t;

typedef struct dot1x_reAuth_sm_s
{
    dot1x_reAuth_sm_state_t curState;

    uint16  reAuthPeriod;
    uint8   reAuthEnabled;
} dot1x_reAuth_sm_t;
typedef enum dot1x_backend_sm_state_s
{
    DOT1X_BACKEND_INITIALIZE = 0,
    DOT1X_BACKEND_IDLE,
    DOT1X_BACKEND_REQUEST,
    DOT1X_BACKEND_RESPONSE,
    DOT1X_BACKEND_IGNORE,
    DOT1X_BACKEND_TIMEOUT,
    DOT1X_BACKEND_FAIL,
    DOT1X_BACKEND_SUCCESS,
} dot1x_backend_sm_state_t;

typedef struct dot1x_backend_sm_s
{
    dot1x_backend_sm_state_t curState;
    uint8                    chkNow;
    uint32                   idFromServer;
    dot1x_radius_stateAvp_t  stateAvp;
    uint32                   radiusSendId;

    uint8   reqCnt;
    uint16  suppTimeout;
    uint16  suppWhile;

    uint8   eapNoReq;
    uint8   eapReq;
    uint8   eapResp;
    
    uint16  serverTimeout;
} dot1x_backend_sm_t;
typedef struct dot1x_pacp_s
{
    uint32  port;
    uint8   portMode;
    uint16  currentId;

    char    userName[CAPA_AUTHMGR_DOT1X_USERNAME_LEN + 1];
    uint32  sessionTime;
    uint8   reqMax;

    uint16  aWhile;
    uint16  quietWhile;
    uint16  reAuthWhen;

    uint8   authAbort;
    uint8   authFail;
    uint8   authPortStatus;
    uint8   authStart;
    uint8   authTimeout;
    uint8   authSuccess;
    uint8   eapFail;
    uint8   eapolEap;
    uint8   eapSuccess;
    uint8   eapTimeout;
    uint8   initialize;
    uint8   keyAvailable;
    uint8   keyDone;
    uint8   keyRun;
    uint8   keyTxEnabled;
    uint8   portControl;
    uint8   portEnabled;
    uint8   portStatus;
    uint8   portValid;
    uint8   reAuthenticate;
    uint8   suppAbort;
    uint8   suppFail;
    uint8   suppPortStatus;
    uint8   suppStart;
    uint8   suppSuccess;

    uint8   inactTimeout; 

    dot1x_pae_sm_t     pae_sm;
    
    dot1x_reAuth_sm_t  reAuth_sm;
    
    dot1x_backend_sm_t backend_sm;

    void    *pParentSess; 
    uint8   smChecking;
    uint8   guestChk;
    uint8   delPacp;
} dot1x_pacp_t;
typedef struct dot1x_cfg_port_s
{
    uint16 txPeriod;
    uint16 quietPeriod;
    uint16 suppTimeout;
    uint16 serverTimeout;

} dot1x_cfg_port_t;
#define HTONS(val) (val)
#define HTONL(val) (val)
#define NTOHS(val) (val)
#define INC_EAPID(x) \
do { x = x + 1; if (x > 255){x = 1;} } while(0)

#define DOT1X_SM_PAE_CHK_NOW(_pPacp) \
do { \
    _pPacp->pae_sm.chkNow = TRUE; \
    _pPacp->backend_sm.chkNow = TRUE; \
} while(0)

#define DOT1X_SM_BACKEND_CHK_NOW(_pPacp) \
do { \
    _pPacp->backend_sm.chkNow = TRUE; \
    _pPacp->pae_sm.chkNow = TRUE; \
} while(0)

#define DOT1X_DBG(_pPacp, fmt, args...) \
do { \
    char __str[SYS_BUF256_LEN]; \
    SYS_MEM_CLEAR(__str);\
    authmgr_dbgStr_get(AUTHMGR_TYPE_DOT1X, _pPacp, __str, fmt, ##args); \
    SYS_DBG(DBG_AUTHMGR_DOT1X, __str); \
} while(0)
extern int32 eapol_pkt_passthrough(dot1x_pacp_t *pPacp);
extern int32 eapol_pkt_tx(dot1x_pacp_t *pPacp, eap_code_t code);
extern int32 eapol_pkt_rx(dot1x_pacp_t *pPacp, sys_nic_pkt_t *pPkt);
extern int32 eapol_radiusRx_process(dot1x_pacp_t *pPacp, dot1x_rad_recv_t *pRecv);
extern int32 eapol_pkt_unauthMcast_tx(sys_logic_port_t port, eap_code_t code, uint32 eapid);
extern uint32 eapol_isEapPkt_ret(sys_nic_pkt_t *pPkt);

extern int32 dot1x_init(void);
extern dot1x_pacp_t *dot1x_pacp_create(sys_logic_port_t port);
extern void dot1x_timer_count(dot1x_pacp_t *pPacp);
extern void dot1x_timer_routine(dot1x_pacp_t *pPacp);
extern void dot1x_timer_chk(dot1x_pacp_t *pPacp);
extern void dot1x_sm_check_trigger(dot1x_pacp_t *pPacp);
extern int32 authmgr_dot1xRadius_rx(dot1x_rad_recv_t recv);
extern int32 authmgr_dot1xRadiusSendId_get(authmgr_sessId_t sessId, uint32 *pId);

#endif 
