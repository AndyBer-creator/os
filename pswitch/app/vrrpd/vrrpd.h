/*******************************************************************************
  Copyright (C), 2001-2004, GreenNet Tech. Co., Ltd.
  文件名:  vrrpd.h
  作者:    吴开健 (W0039)
  版本:    1.0.0          
  日期:    2005-08-30
  描述:    VRRP模块的头文件
  函数列表:  
    1. -------
  修改历史:         
    1.日期:
      作者:
      版本:
      修改:
    2.-------
*******************************************************************************/

#ifndef __VRRPD_H__
#define __VRRPD_H__
#include <libipc/vos.h>
#include <libipc/lstLib.h>
#include <libipc/if_arp.h>


#define GLN_PROC_NAME_MAX_LEN 32 /*进程名称最大长度，包括路径*/
/*进程名称*/
#define VRRP_PROCESS_NAME  "/sbin/vrrpd"
/*共享内存id*/
#define VRRP_SHARED_ID_VIP               1

/* local include */

typedef struct {    /* rfc2338.5.1 */
    uchar_t        vers_type;    /* 0-3=type, 4-7=version */
    uchar_t        vrid;         /* virtual router id */
    uchar_t        priority;     /* router priority */
    uchar_t        naddr;        /* address counter */
    uchar_t        auth_type;    /* authentification type */
    uchar_t        adver_int;    /* advertissement interval(in sec) */
    ushort_t       chksum;       /* checksum (ip-like one) */
/* here <naddr> ip addresses */
/* here authentification infos */
} vrrp_pkt;

/* protocol constants */
#define INADDR_VRRP_GROUP 0xe0000012    /* multicast addr - rfc2338.5.2.2 */
#define INADDR_VRRP_ANY 0x0
#define VRRP_IP_TTL      255     /* in and out pkt ttl -- rfc2338.5.2.3 */
#define IPPROTO_VRRP     112     /* IP protocol number -- rfc2338.5.2.4*/
#define VRRP_VERSION     2       /* current version -- rfc2338.5.3.1 */
#define VRRP_PKT_ADVERT  1       /* packet type -- rfc2338.5.3.2 */
#define VRRP_PRIO_OWNER  255     /* priority of the ip owner -- rfc2338.5.3.4 */
#define VRRP_PRIO_DFL    100     /* default priority -- rfc2338.5.3.4 */
#define VRRP_PRIO_STOP   0       /* priority to stop -- rfc2338.5.3.4 */
#define VRRP_AUTH_NONE   0       /* no authentification -- rfc2338.5.3.6 */
#define VRRP_AUTH_PASS   1       /* password authentification -- rfc2338.5.3.6 */
#define VRRP_AUTH_AH     2       /* AH(IPSec) authentification - rfc2338.5.3.6 */
#define VRRP_ADVER_DFL   1       /* advert. interval (in sec) -- rfc2338.5.3.7 */
#define VRRP_PREEMPT_DFL 1       /* rfc2338.6.1.2.Preempt_Mode */

/* 每个备份组允许配置的监视接口的最大个数 */
//#define VRRP_IF_TRACK_MAX     8

/* 监视接口down时缺省降低的优先级值 */
#define VRRP_PRI_TRACK        10
#if 0
typedef struct {    /* parameters per interface -- rfc2338.6.1.1 */
    char        hwaddr[6];    /* WORK: lame hardcoded for ethernet !!!! */
    uint_t      ipaddrM;      /* the master address of the interface */
    uint_t      ipaddrP;      /* the primary address of the interface */
    int         ifindex;      /* 接口索引，对应MIB变量rfc2233:ifindex */
    char        ifname[CAPA_NAME_STR_LEN + 1];       /* the device name for this ipaddr */
#if 0
    int         auth_type;    /* authentification type. VRRP_AUTH_* */
    uchar_t     auth_data[8]; /* authentification data *//
    #endif
} vrrp_if;

typedef struct {
    uint_t      addr;         /* the ip address */
    #if 0
    int         deletable;    /* TRUE if one of my primary addr */
    #endif
} vip_addr;

typedef struct {    /* parameters per virtual router -- rfc2338.6.1.2 */    
    #if 0                /* dynamically calculated */
    double    skew_time;     /* skew Master_Down_Interval. (256-Prio)/256 */    
    int    mast_down_int;    /* interval for backup to declare master down*/
    int    wantstate;  /* user explicitly wants a state (back/mast) */
    int    sockfd;     /* the socket descriptor */    
    int    initF;      /* true if the struct is init */    
    #endif
    int    vrid;         /* virtual id. from 1(!) to 255 */
    int    priority;     /* priority value */
    int    oldpriority;  /* old priority value */
    int    nowner;       /* 和接口IP相同的虚拟IP地址个数 */
    int    naddr;        /* number of ip addresses */
    vip_addr vaddr[8];     /* point on the ip address array */
    int    adver_int;    /* delay between advertisements(in sec) */
    int    preempt;    /* true if a higher prio preempt a lower one */
    int    delay;      /* preempt delay */
    int    state;      /* internal state (init/backup/master) */
    uint_t      ms_down_timer;
    uint_t      adver_timer;
    vrrp_if     vif;
    char        vhwaddr[6]; /* 虚拟MAC地址 */
    int         no;         /* 在备份组数组中的下标 */
    int         msgDone;    /* 连续处理的消息条数 */
    int         used;       /* 0: 未用；1: 已使用 */
    int         adminState; /* 管理状态 1: up；2: down */
    int         rowStatus;  /* 行状态 */
    ulong_t     upTime;     /* 离开INIT状态时的rfc1907:sysUpTime对象值 */
    int         niftrack;   /* 监视接口的数目 */
    int         iftrack[VRRP_IF_TRACK_MAX]; /* 监视接口索引数组或者track id索引数组 */
    int         pritrack[VRRP_IF_TRACK_MAX]; /* 监视接口down时降低的优先级值 */
    int         trackState[VRRP_IF_TRACK_MAX]; /*track表项的当前状态*/

    ulong_t     staBecomeMaster;     /* 状态转换为Master的次数 */
    ulong_t     staAdverRcvd;        /* 收到的VRRP通告数 */
    ulong_t     staAdverIntErrors;   /* 通告间隔不同于本地配置的报文数 */
    ulong_t     staAuthFailures;     /* 认证失败的报文数 */
    ulong_t     staIpTtlErrors;      /* IP TTL不等于255的报文数 */
    ulong_t     staPriZeroPktsRcvd;  /* 收到的优先级为0的报文数 */
    ulong_t     staPriZeroPktsSent;  /* 发送的优先级为0的报文数 */
    ulong_t     staInvTypePktsRcvd;  /* 收到的非法类型的报文数 */
    ulong_t     staAddrListErrors;   /* 收到的地址列表不同于本地的报文数 */
    ulong_t     staInvAuthType;      /* 收到的未知认证类型的报文数 */
    ulong_t     staAuthTypeMismatch; /* 收到的认证类型不同于本地的报文数 */
    ulong_t     staPktsLenErrors;    /* 收到的长度非法的报文数 */
} vrrp_rt;
#endif
//#define VRRP_LOG( str )    Vrrp_Debug(str)
#define Vrrp_Debug(string, args...) SYS_DBG(DBG_L3_VRRP_COMMON, "[VRRP DBG] "string"", ##args)
#define VRRP_LOG(string, args...) SYS_DBG(DBG_L3_VRRP_COMMON, "[VRRP DBG] "string"", ##args)
#define VRRP_AUTH_LEN      8

#define VRRP_IS_BAD_VID(id)       ((id)<1 || (id)>255)    /* rfc2338.6.1.vrid */
#define VRRP_IS_BAD_PRIORITY(p)   ((p)<1 || (p)>254)      /* rfc2338.6.1.prio */
#define VRRP_IS_BAD_ADVERT_INT(d) ((d)<1 || (d)>255)
#define VRRP_IS_BAD_AUTH_TYPE(d)  ((d)>2)
#define VRRP_IS_BAD_DELAY(t)      ((t)>255)

/* use the 'tcp sequence number arithmetic' to handle the wraparound.
** VRRP_TIMER_SUB: <0 if t1 precedes t2, =0 if t1 equals t2, >0 if t1 follows t2
*/
#define VRRP_TIMER_SET( val, delta )  (val) = (delta)
#define VRRP_TIMER_EXPIRED( val )     ((0 != val) && ((--val) ==0) )
#define VRRP_TIMER_CLR( val )         (val) = 0
#define VRRP_TIMER_IS_RUNNING( val )  (val)
//#define VRRP_TIMER_HZ                 1

#define VRRP_TIMER_SKEW( srv ) ((256-(srv)->priority)*VRRP_TIMER_HZ/256) 

#define VRRP_MIN( a , b )    ( (a) < (b) ? (a) : (b) )
#define VRRP_MAX( a , b )    ( (a) > (b) ? (a) : (b) )

/* 允许配置的备份组的最大个数 */
//#define VRRP_VSRV_SIZE_MAX    255

/* 每个备份组允许配置的虚拟IP的最大个数 */
//#define VRRP_VIP_NUMS_MAX     8

/* 每连续处理一定数量的消息出让CPU */
#define VRRP_MSG_AMOUNT_ONCEDONE 100

/* VRRP消息队列的长度 */
#define VRRP_MSG_QUEUE_LEN 128

/* 配置VRRP的操作码 */
#define VRRP_OPCODE_ADDIP          1
#define VRRP_OPCODE_DELIP          2
#define VRRP_OPCODE_DELVR          3

/* 配置VRRP函数返回的错误码 */
//#define VRRP_CFGERR_IFWRONG        1
//#define VRRP_CFGERR_SUBNETDIFF     2
//#define VRRP_CFGERR_MAXVSRV        3
//#define VRRP_CFGERR_MAXVIP         4
//#define VRRP_CFGERR_VSRVNOTEXIST   5
//#define VRRP_CFGERR_VIPNOTEXIST    6
//#define VRRP_CFGERR_MAXTRACK       7
//#define VRRP_CFGERR_TRACK          8

/*VRRP状态发生改变后通知路由协议数的最大值*/
#define VRRP_NOTIFY_ROUTE_PROTO_MAX    8
/* 消息类型枚举 */
typedef enum
{
    VRRP_MSG_PKT,     /* VRRP报文消息 */
    VRRP_MSG_OTHER    /* 其他消息 */
} VRRP_MSG_TYPE_E;

/* 发送消息结构 */
typedef struct tagVrrpMsgRecord
{
    VRRP_MSG_TYPE_E msg;     /* 消息类型 */
    void * pData;            /* 消息内容存放地址 */
} VRRP_MSG_RECORD_T;

typedef struct {
    ulong_t        ulIfIndex;
    ulong_t        ulIpNum;
    ulong_t        ulVip[VRRP_VIP_NUMS_MAX];
}VRRP_VIP_STC;

#define CHECK_VALID(p, retVal)\
if(!(p))\
{\
   return retVal;\
}

extern vrrp_rt gVrrp_vsrv[];
extern int gVrrp_sock;
extern ulong_t gVrrp_NotificationCntl;
extern ulong_t gVrrp_ChecksumErrors;
extern ulong_t gVrrp_VersionErrors;
extern ulong_t gVrrp_VrIdErrors;
extern ulong_t gVrrp_TrapPktSrc;
extern ulong_t gVrrp_TrapAuthErrType;

#if 0
int Vrrp_Debug( const char *fmt, ... )
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;
#endif

int VrrpInit(void);
int VrrpRecv(int iSocketFd, void *pArg);
int Vrrp_VsrvCreate( vrrp_rt * vsrv );
int Vrrp_VsrvDelete( vrrp_rt * vsrv );
int Vrrp_VsrvTask( vrrp_rt * vsrv );
vrrp_rt * vrrp_vsrv_find_by_vrid(int ifindex, int vrid);
int Vrrp_VsrvConfig(int ifindex, int vrid, uint_t vipaddr, int opcode);
ulong_t ifindex_to_vlanid(ulong_t ifindex);
ulong_t Vrrp_GetIpaddrM(ulong_t vsrvidx);
int Vrrp_CheckVipAndState(ulong_t ipaddr, int *checkState);
int Vrrp_VridGet(ulong_t vsrvidx);
BOOL Vrrp_networkBackup( ulong_t ulAddr, ulong_t ulMask );
BOOL IfIsDown(int ifindex);
void Vrrp_VsrpStateChangeCallback(vrrp_rt *pVsrv, ulong_t ulVrip, 
    BOOL bMaster);
int vrrpVsrvDellAllByIfindex(ulong_t ulIfIndex, ulong_t *pulNumDel);
int vrrpVsrvPrioritySet(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulPrio, int *piErrNo);
int vrrpVsrvPreemptSet(ulong_t ulIfIndex, ulong_t ulVrid, ulong_t ulPreempt,
    ulong_t ulTime, int *piErrNo);
int vrrpVsrvAdverTimeSet(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulTime, int *piErrNo);
int vrrpVsrvGetNext(int iIndex, vrrp_rt *pVsrvInfo);
int vrrpVipGet(ulong_t ulServerNo, ulong_t *pulNum, ulong_t ulVip[VRRP_VIP_NUMS_MAX]);
int vrrpVsrvIfTrackAdd(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulIfTracIndex, ulong_t ulPrio, int *piErrNo);
int vrrpVsrvIfTrackDel(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulIfTracIndex, int *piErrNo);
int vrrpTrackStateChg(ulong_t ulTrackIndex, int iLastState, int iNewState);
int Vrrp_routeProtoReg(char *szProcessName, ulong_t ulFunCallBackId);
int Vrrp_routeProtoDeReg(char *szProcessName, ulong_t ulFunCallBackId);
int VrrpTimer(ulong_t ulTimerOver, void *pArg);


#endif    /* __VRRPD_H__ */

