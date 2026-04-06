/*******************************************************************************
VRRP模块的实现文件
*******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <libipc/vosMemory.h>
#include <libipc/vosPrint.h>
#include <libsal/sal_socket.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_l3_intf.h>
#include <libsal/sal_l3_vrrp.h>
#include <libsal/sal_l3_route_zebra.h>

#include "vrrpd.h"
#include "vrrpFuncParse.h"
#include "rfc2787.h"

#define RN "\r\n"

/* 备份组(虚拟路由器)数据结构，每个备份组占用数组的一项 */
vrrp_rt gVrrp_vsrv[VRRP_VSRV_SIZE_MAX] = {{0}};
static ulong_t g_ulVrrpVsrvNum = 0;

/* 接收VRRP报文的原始IP插口 */
int gVrrp_sock = 0;
int gVrrp_tx_sock = 0;

/* 是否发送VRRP通告(TRAP) */
ulong_t gVrrp_NotificationCntl = EvrrpNotificationCntl_enabled;

/* 收到的校验和错误的VRRP报文数统计 */
ulong_t gVrrp_ChecksumErrors = 0;

/* 收到的版本号错误的VRRP报文数统计 */
ulong_t gVrrp_VersionErrors = 0;

/* 收到的VRID错误的VRRP报文数统计 */
ulong_t gVrrp_VrIdErrors = 0;

/* 认证失败的报文包含的源地址 */
ulong_t gVrrp_TrapPktSrc = 0;

/* 认证失败的类型 */
ulong_t gVrrp_TrapAuthErrType = EvrrpTrapAuthErrorType_authTypeMismatch;

/*路由协议向VRRP注册结构数组*/
typedef struct {
    NODE        lstNode;
    char        szProcessName[GLN_PROC_NAME_MAX_LEN + 1];
    ulong_t     ulFunCallBackId;
}VRRP_PROTO_REG_STC;

LIST g_lstVrrpProtoReg;

/*是否完成初始化*/
static int gVrrp_initialized = FALSE;
VRRP_VIP_STC *g_stcVrrpVip = NULL;

static ushort_t vrrp_in_csum( ushort_t *addr, int len, ushort_t csum );
static int vrrp_sock_join( int sockfd, int vidIndex, ulong_t ipaddr );
static int vrrp_sock_leave( int sockfd, int vidIndex, ulong_t ipaddr );
static int vrrp_vsrv_add_virtual_if(vrrp_rt *pVsrv);
static int vrrp_vsr_del_virtual_if(vrrp_rt *pVsrv);
static int vrrp_vsrv_add_ip_to_system(vrrp_rt *pVsrv, ulong_t ulIp);
static int vrrp_vsrv_del_ip_from_system(vrrp_rt *pVsrv, ulong_t ulIp);
static int vrrp_vsrv_add_vmac_to_hw(vrrp_rt *pVsrv);
static int vrrp_vsrv_del_vmac_from_hw(vrrp_rt *pVsrv);
static int vrrp_in_chk( struct ip *iph );
static int vrrp_in_chk2( vrrp_rt *vsrv, struct ip *iph );
static int vrrp_hd_len( vrrp_pkt *pkt );
static int vrrp_hd_len2( vrrp_rt *rt );
static int vrrp_dlt_len( vrrp_rt *rt );
static int vrrp_iphdr_len( vrrp_rt *vsrv );
static void vrrp_vsrv_init( vrrp_rt *vsrv );
static vrrp_rt * vrrp_vsrv_get( void );
static void vrrp_vsrv_free( vrrp_rt * vsrv );
static vrrp_rt * vrrp_vsrv_find_by_pkt(struct ip * iph);
static void vrrp_add_ipaddr( vrrp_rt *vsrv, uint_t ipaddr );
static void state_init( vrrp_rt *vsrv );
static void state_back( vrrp_rt *vsrv, char *pPkt );
static void state_mast( vrrp_rt *vsrv, char *pPkt );
static void state_goto_master( vrrp_rt *vsrv );
static void state_leave_master( vrrp_rt *vsrv, int advF );
static int vrrp_send_adv( vrrp_rt *vsrv, int prio );
static void vrrp_build_pkt( vrrp_rt *vsrv, int prio, char *buffer, int buflen );
static void vrrp_build_dlt( vrrp_rt *vsrv, char *buffer, int buflen );
static void vrrp_build_ip( vrrp_rt *vsrv, char *buffer, int buflen );
static void vrrp_build_ip( vrrp_rt *vsrv, char *buffer, int buflen );
static int vrrp_build_vrrp( vrrp_rt *vsrv, int prio, char *buffer, int buflen );
static int vrrp_send_pkt( vrrp_rt *vsrv, char *buffer, int buflen );
static int send_gratuitous_arp( vrrp_rt *vsrv, int addr, int vAddrF );
static int Vrrp_TrapNewMasterSend(ulong_t ifindex, ulong_t vrid);
static int Vrrp_TrapAuthFailureSend(void);

extern int BldRun_vrrp_cmd(char *pszBuf, int *pbufLen, ulong_t ulFlags, ulong_t ulPort);

/*******************************************************************************
  函数: Vrrp_Debug
  描述: VRRP调试信息输出函数
  输入: fmtE:英文信息 fmtC:中文信息
  输出: 无
  返回: OK: 成功；ERROR: 失败
  其它: 
*******************************************************************************/
#if 0
int Vrrp_Debug( const char *fmt, ... )
{
    va_list ap;
    char szString[512];

    szString[0] = 0;

    //if ( !syslogModuleSwitch(MID_VRRP, DEBUGGING) )
    {
      // return OK;
    }
    
    va_start(ap, fmt);    
    vsprintf( szString, fmt, ap);
    va_end(ap);
	printf("%s", szString);
    //return syslogSendInfo(MID_VRRP, DEBUGGING, "DEBUG", szString, szString, FALSE, FALSE, NO_WAIT); 
    return 0;
}
#endif
/*******************************************************************************
  函数: VrrpInit
  描述: VRRP初始化函数
  输入: 无
  输出: 无
  返回: OK: 初始化成功；ERROR: 初始化失败
  其它: 
*******************************************************************************/
int VrrpInit(void)
{
    int                      i = 0;
    char on = 1;
	sal_sockaddr_in sa = {0};

    /*共享内存初始化*/
    g_stcVrrpVip = vosMemoryShareGet(VRRP_PROCESS_NAME, VRRP_SHARED_ID_VIP, 
                                sizeof(VRRP_VIP_STC)*(VRRP_VSRV_SIZE_MAX), VOS_MEM_SHARE_FLAG_CREATE);
    if (NULL == g_stcVrrpVip)
    {
        printf("\r\nFailed to alloc space for vip info");
        return VOS_ERR_FAIL;
    }

    memset(g_stcVrrpVip, 0, sizeof(VRRP_VIP_STC)*(VRRP_VSRV_SIZE_MAX));

    /* 初始化glob_vsrv数组 */
    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        gVrrp_vsrv[i].no = i;
        vrrp_vsrv_init(&gVrrp_vsrv[i]);
    }

    /* 创建接收VRRP报文的原始IP插口 */
    gVrrp_sock = sal_socket( AF_INET, SOCK_RAW, IPPROTO_VRRP );
    if( gVrrp_sock < 0 )
    {
        VRRP_LOG("cant open raw socket."RN);
        return VOS_ERR_FAIL;
    }

#if 1	
	gVrrp_tx_sock = sal_socket(SAL_AF_INET, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_VRRP );
    if( gVrrp_tx_sock < 0 )
    {
        VRRP_LOG("cant open dgraw socket."RN);
        return VOS_ERR_FAIL;
    }

	on = 0;
    if( sal_setsockopt (gVrrp_tx_sock, IPPROTO_IP, IP_MULTICAST_ALL, (char *)&on, sizeof(on)) < 0 )
	 {
        VRRP_LOG("cant set opt IP_MULTICAST_ALL socket."RN);
       // return VOS_ERR_FAIL;
    }
	
	on = 1;
	if( sal_setsockopt (gVrrp_tx_sock, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0 )
	{
       // return VOS_ERR_FAIL;
    }
	
    if( sal_setsockopt (gVrrp_tx_sock, SAL_SOL_SOCKET, SAL_SO_REUSEADDR, (char *)&on, sizeof(on)) < 0 )
    {
       // return VOS_ERR_FAIL;
    }
#endif
    /*路由注册初始化*/
    lstInit(&g_lstVrrpProtoReg);

    /*初始化结束*/
    gVrrp_initialized = 1;

    return VOS_ERR_NONE;
}

int VrrpTimer(ulong_t ulTimerOver, void *pArg)
{
    vrrp_rt *pVsrv;
    int      i, j;
    
    /* 查找匹配的备份组 */
    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        pVsrv = &gVrrp_vsrv[i];

        if (!pVsrv || !pVsrv->used )
        {
            continue;
        }

        /* 检查监视接口是否down */
        if (pVsrv->priority < VRRP_PRIO_OWNER)
        {
            pVsrv->priority = pVsrv->oldpriority;
            for (j = 0; j < pVsrv->niftrack; j++)
            {
            #ifdef GLN_INCLUDE_TRACK
                if ( TRACK_STATE_NEGATIVE == pVsrv->trackState[j] )
            #else
                if (IfIsDown(pVsrv->iftrack[j]))
            #endif
                {
                    pVsrv->priority -= pVsrv->pritrack[j];
                    if (pVsrv->priority < 1)
                    {
                        pVsrv->priority = 1;
                        break;
                    }
                }
            }
        }
    
		if ((pVsrv->rowStatus == EvrrpOperRowStatus_active)
			&& (pVsrv->vif.sockJoinState == FALSE))
		{
			if (vrrp_sock_join(gVrrp_sock, pVsrv->vif.ifindex, pVsrv->vif.ipaddrM) == 0)
			{
				pVsrv->vif.sockJoinState = TRUE;
			}
			else
			{
				pVsrv->vif.sockJoinState = FALSE;
			}
		}
		
        switch( pVsrv->state )
        {
            case EvrrpOperState_initialize:
                state_init( pVsrv );
                break;
                
            case EvrrpOperState_backup:
                if( VRRP_TIMER_EXPIRED(pVsrv->ms_down_timer) )
                {
                    state_goto_master( pVsrv );
                    Vrrp_Debug("Virtual Router %d goto master.", pVsrv->vrid);
                }
                break;
                
            case EvrrpOperState_master:
                if( VRRP_TIMER_EXPIRED(pVsrv->adver_timer) )
                {
                    vrrp_send_adv( pVsrv, pVsrv->priority );
                    pVsrv->adver_timer = pVsrv->adver_int;
                }
                break;
        }
    }  

    return 0;
}

int VrrpRecv(int iSocketFd, void *pArg)
{
    int len = 0;
    char buff[1024];
    vrrp_rt * pVsrv = NULL;
    int ihl = 0;
    vrrp_pkt * hd = NULL;
    struct ip * iph = NULL;
    
    /* 读取VRRP报文 */
    len = read(gVrrp_sock, buff, sizeof(buff));
    if( len > 0 && vrrp_in_chk( (struct ip *)buff ) )
    {
        VRRP_LOG("bogus packet!"RN);
        len = 0;
    }
    if (len <= 0)
    {
        return -1;
    }

    /* 调试信息输出报文内容 */
    iph = (struct ip *)buff;
    ihl = iph->ip_hl << 2;
    hd  = (vrrp_pkt *)((char *)iph + ihl);
    Vrrp_Debug("received vrrp packet: vers_type:0x%02x,vrid:0x%02x,priority:0x%02x,naddr:0x%02x,adver_int:0x%02x,...", hd->vers_type, hd->vrid,
        hd->priority, hd->naddr, hd->adver_int);

    /* 确定报文接收者 */
    pVsrv = vrrp_vsrv_find_by_pkt((struct ip *)buff);
    if (!pVsrv || pVsrv->adminState != EvrrpOperAdminState_up)
    {
        return 0;
    }
    pVsrv->staAdverRcvd++;

    /* 继续检查报文的合法性 */
    if (vrrp_in_chk2(pVsrv, (struct ip *)buff))
    {
        return 0;
    }

    switch( pVsrv->state )
    {
        case EvrrpOperState_initialize:
            state_init( pVsrv );
            break;
        case EvrrpOperState_backup:
            state_back( pVsrv, buff );
            break;
        case EvrrpOperState_master:
            state_mast( pVsrv, buff );
            break;
        default:
            break;
    }

    return 0;
}
/*******************************************************************************
  函数: Vrrp_VsrvCreate
  描述: 为某个备份组创建处理任务
  输入: vsrv: 备份组数据结构指针
  输出: 无
  返回: OK: 成功；ERROR: 失败
  其它: 
*******************************************************************************/
int Vrrp_VsrvCreate( vrrp_rt * vsrv )
{
    int rc = VOS_ERR_NONE;

    CHECK_VALID(vsrv, VOS_ERR_FAIL);
    
    return rc;
}

/*******************************************************************************
  函数: Vrrp_VsrvDelete
  描述: 为某个备份组删除处理任务
  输入: vsrv: 备份组数据结构指针
  输出: 无
  返回: OK: 成功；ERROR: 失败
  其它: 
*******************************************************************************/
int Vrrp_VsrvDelete( vrrp_rt * vsrv )
{
    CHECK_VALID(vsrv, VOS_ERR_FAIL);

    /* 如果处于主交换机状态 */
    if (vsrv->state == EvrrpOperState_master)
    {
        state_leave_master(vsrv, 1);
        vsrv->adminState = EvrrpOperAdminState_down;
        vsrv->state = EvrrpOperState_initialize;
    }
    else if (vsrv->state == EvrrpOperState_backup)
    {
        vsrv->adminState = EvrrpOperAdminState_down;
        vsrv->state = EvrrpOperState_initialize;
    }

    return VOS_ERR_NONE;
}

/****************************************************************
 NAME    : vrrp_in_csum                00/05/10 20:12:20
 AIM    : compute a IP checksum
 REMARK    : from kuznet's iputils
****************************************************************/
static ushort_t vrrp_in_csum( ushort_t *addr, int len, ushort_t csum)
{
    register int nleft = len;
    const ushort_t *w = addr;
    register ushort_t answer;
    register int sum = csum;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while (nleft > 1)  {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if (nleft == 1)
        sum += htons(*(u_char *)w << 8);

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0xffff);    /* add hi 16 to low 16 */
    sum += (sum >> 16);            /* add carry */
    answer = ~sum;                /* truncate to 16 bits */
    return (answer);
}

/****************************************************************
 NAME    : vrrp_hd_len                00/02/02 15:16:23
 AIM    : return the vrrp header size in byte
 REMARK    :
****************************************************************/
static int vrrp_hd_len( vrrp_pkt *pkt )
{
    return sizeof( vrrp_pkt ) + pkt->naddr*sizeof(uint_t)
                    + VRRP_AUTH_LEN;
}

/****************************************************************
 NAME    : vrrp_hd_len2                00/02/02 15:16:23
 AIM    : return the vrrp header size in byte
 REMARK    :
****************************************************************/
static int vrrp_hd_len2( vrrp_rt *rt )
{
    return sizeof( vrrp_pkt ) + rt->naddr*sizeof(uint_t)
                    + VRRP_AUTH_LEN;
}

/****************************************************************
 NAME    : vrrp_dlthd_len            00/02/02 15:16:23
 AIM    : return the vrrp header size in byte
 REMARK    :
****************************************************************/
static int vrrp_dlt_len( vrrp_rt *rt )
{
    return sizeof(struct ether_header);    /* hardcoded for ethernet */
}

/****************************************************************
 NAME    : vrrp_iphdr_len            00/02/02 15:16:23
 AIM    : return the ip  header size in byte
 REMARK    :
****************************************************************/
static int vrrp_iphdr_len( vrrp_rt *vsrv )
{
    return sizeof( struct ip );
}

/****************************************************************
 NAME    : vrrp_in_chk                00/02/02 12:54:54
 AIM    : check a incoming packet. return 0 if the pkt is valid, != 0 else
 REMARK    : rfc2338.7.1
****************************************************************/
static int vrrp_in_chk( struct ip *iph )
{
    int        ihl = iph->ip_hl << 2;
    vrrp_pkt * hd  = (vrrp_pkt *)((char *)iph + ihl);
    
    /* WORK: MUST verify the VRRP checksum */
    if( vrrp_in_csum( (ushort_t*)hd, vrrp_hd_len(hd), 0) )
    {
        VRRP_LOG("Invalid vrrp checksum"RN );
        gVrrp_ChecksumErrors++;
        return 1;
    }
    
    /* MUST verify the VRRP version */
    if( (hd->vers_type >> 4) != VRRP_VERSION )
    {
        Vrrp_Debug("invalid version. %d and expect %d"RN, 
                        (hd->vers_type >> 4), VRRP_VERSION);
        gVrrp_VersionErrors++;
        return 1;
    }

    /* MUST verify that the VRID is valid on the receiving interface */
    /* 在后续处理中进一步检查 */
    if (0 == hd->vrid)
    {
        Vrrp_Debug("invalid vrid %d."RN, hd->vrid);
        gVrrp_VrIdErrors++;
        return 1;
    }

    return 0;
}

/****************************************************************
 NAME    : vrrp_in_chk2                00/02/02 12:54:54
 AIM    : check a incoming packet. return 0 if the pkt is valid, != 0 else
 REMARK    : rfc2338.7.1
****************************************************************/
static int vrrp_in_chk2( vrrp_rt *vsrv, struct ip *iph )
{
    int        ihl = iph->ip_hl << 2;
    vrrp_pkt * hd  = (vrrp_pkt *)((char *)iph + ihl);
    
    /* MUST verify that the IP TTL is 255 */
    if( iph->ip_ttl != VRRP_IP_TTL ) 
    {
        Vrrp_Debug("invalid ttl. %d and expect %d"RN, iph->ip_ttl, VRRP_IP_TTL);
        vsrv->staIpTtlErrors++;
        return 1;
    }
    
    /* MUST verify that the received packet length is greater than or
    ** equal to the VRRP header */
    /* iph->ip_len记录的IP报文的长度在IP层时已被减去IP首部的长度 */
    if( ntohs(iph->ip_len) <= sizeof(vrrp_pkt) )
    {
        Vrrp_Debug("ip payload too short. %d and expect at least %d"RN,
                        ntohs(iph->ip_len)-ihl, sizeof(vrrp_pkt));
        vsrv->staPktsLenErrors++;
        return 1;
    }
    
    /* MUST verify the VRRP type */
    if( (hd->vers_type & 0x0F) != VRRP_PKT_ADVERT )
    {
        Vrrp_Debug("invalid vrrp type. %d and expect %d"RN, 
                        (hd->vers_type & 0x0F), VRRP_PKT_ADVERT);
        vsrv->staInvTypePktsRcvd++;
        return 1;
    }
    
    /* Auth Type must be 0 */
    if( 0 != hd->auth_type )
    {        
        Vrrp_Debug("receive a %d auth, expecting 0!"RN, hd->auth_type);
        if (VRRP_IS_BAD_AUTH_TYPE(hd->auth_type))
        {
            vsrv->staInvAuthType++;
            gVrrp_TrapAuthErrType = EvrrpTrapAuthErrorType_invalidAuthType;
        }
        else
        {
            vsrv->staAuthTypeMismatch++; 
            gVrrp_TrapAuthErrType = EvrrpTrapAuthErrorType_authTypeMismatch;
        }
        /* 发送TRAP */
        Vrrp_TrapAuthFailureSend();
        return 1;
    }
    
    /* MUST verify that the VRID is valid on the receiving interface */
    if( vsrv->vrid != hd->vrid || VRRP_PRIO_OWNER == vsrv->priority )
    {
        return 1;
    }

    /* MAY verify that the IP address(es) associated with the VRID are
    ** valid */
    /* WORK: currently we don't */

    /* MUST verify that the Adver Interval in the packet is the same as
    ** the locally configured for this virtual router */
    if( vsrv->adver_int/VRRP_TIMER_HZ != hd->adver_int )
    {
        Vrrp_Debug("advertissement interval mismatch mine=%d rcved=%d"RN, 
                        vsrv->adver_int/VRRP_TIMER_HZ, hd->adver_int);
        vsrv->staAdverIntErrors++;
        return 1;
    }

    return 0;
}

/****************************************************************
 NAME    : vrrp_sock_join                00/02/07 12:40:00
 AIM    : socket join the multicast group.
 REMARK    :
****************************************************************/
static int vrrp_sock_join( int sockfd, int vidIndex, ulong_t ipaddr )
{
    struct ip_mreq req;
    int    ret;
    
    /* join the multicast group */
    memset( &req, 0, sizeof(req) );
    req.imr_multiaddr.s_addr = htonl(INADDR_VRRP_GROUP);
    req.imr_interface.s_addr = htonl(ipaddr);
    ret = sal_setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                    (char *) &req, sizeof (struct ip_mreq));
    if( ret < 0 )
    {
        VRRP_LOG("cant do IP_ADD_MEMBERSHIP"RN);
        return -1;
    }
    return 0;
}

/****************************************************************
 NAME    : vrrp_sock_leave                00/02/07 12:40:00
 AIM    : socket leave the multicast group.
 REMARK    :
****************************************************************/
static int vrrp_sock_leave( int sockfd, int vidIndex, ulong_t ipaddr )
{
    struct ip_mreq req;
    int    ret;
    
    /* join the multicast group */
    memset( &req, 0, sizeof(req) );
    req.imr_multiaddr.s_addr = htonl(INADDR_VRRP_GROUP);
    req.imr_interface.s_addr = htonl(ipaddr);
    ret = sal_setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                    (char *) &req, sizeof (struct ip_mreq));
    if( ret < 0 )
    {
        //VRRP_LOG(("cant do IP_ADD_MEMBERSHIP"RN));
        return -1;
    }
    return 0;
}

static int vrrp_vsrv_add_virtual_if(vrrp_rt *pVsrv)
{
    int                      iRet, iSw;
    char                     szRealIfName[32];
    char                     szBuf[256];
#if 0
    iSw = (pVsrv->vif.ifindex & SW_IFACE_IFMASK);
    sprintf(szRealIfName, "sw%d", iSw - 1);
    iRet = smdVDVrrpIntfCreate(iSw, pVsrv->vrid, szRealIfName);
    if ( VOS_ERR_NONE != iRet )
    {
        sprintf(szBuf, "add virtual ip interface for sw %d vrid %d failed.", iSw, pVsrv->vrid);
        //syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        VRRP_LOG(szBuf);
        return -1;
    }
#endif   
    return 0;
}

static int vrrp_vsr_del_virtual_if(vrrp_rt *pVsrv)
{
    int                      iRet, iSw;
    char                     szBuf[256];
#if 0
    iSw = (pVsrv->vif.ifindex & SW_IFACE_IFMASK);
    iRet = smdVDVrrpIntfDestroy(iSw, pVsrv->vrid);
    if ( VD_E_NONE != iRet )
    {
        sprintf(szBuf, "del virtual ip interface for sw %d vrid %d failed.", iSw, pVsrv->vrid);
        syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        return -1;
    }
#endif    
    return 0;
}

static int vrrp_vsrv_add_ip_to_system(vrrp_rt *pVsrv, ulong_t ulIp)
{
    int                      iRet;
    char                     szVrrpIfName[32];
    char                     szAddrStr[32];
    char                     szBuf[256];

    //smdVDVrrpIntfVrip(TRUE, pVsrv->vif.ifindex & SW_IFACE_IFMASK, ulIp, pVsrv->vrid);

    sprintf(szVrrpIfName, "vrrp%d", pVsrv->vrid-1);
    //networkToStrPLen(ulIp, 0xffffffff, szAddrStr, sizeof(szAddrStr));
    sal_l3_vrrp_intf_IPv4_add(pVsrv->vif.ifindex, pVsrv->vrid, ulIp);
    iRet = sal_l3_interface_ip_add(szVrrpIfName, ulIp, 0xffffffff);
    if ( VOS_ERR_NONE != iRet )
    {
        sprintf(szBuf, "add virtual ip %s failed.", szAddrStr);
        //syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        return -1;
    }
   
    return 0;
}

static int vrrp_vsrv_del_ip_from_system(vrrp_rt *pVsrv, ulong_t ulIp)
{
    int                      iRet;
    char                     szVrrpIfName[32];
    char                     szAddrStr[32];
    char                     szBuf[256];
    

    sprintf(szVrrpIfName, "vrrp%d", pVsrv->vrid-1);
    //networkToStrPLen(ulIp, 0xffffffff, szAddrStr, sizeof(szAddrStr));
    iRet = sal_l3_interface_ip_del(szVrrpIfName, ulIp, 0xffffffff);
    if ( VOS_ERR_NONE != iRet )
    {
        sprintf(szBuf, "del virtual ip %s failed.", szAddrStr);
        //syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        return -1;
    }
    
	//sal_l3_vrrp_intf_IPv4_del(pVsrv->vif.ifindex, pVsrv->vrid, ulIp);

    //smdVDVrrpIntfVrip(FALSE, pVsrv->vif.ifindex & SW_IFACE_IFMASK, ulIp, pVsrv->vrid);
    
    return 0;
}

static int vrrp_vsrv_add_vmac_to_hw(vrrp_rt *pVsrv)
{
#if 0
    int                      iRet, i;
    vd_l2_addr_t             l2addr;
    ushort_t                 usNum, usVids[MAX_SUPERVLAN_INFO];
    ushort_t                 usVid = 0;
    char                     szBuf[256];

    memset(&l2addr, 0, sizeof(l2addr));
    memcpy(l2addr.mac, pVsrv->vhwaddr, 6);
    l2addr.port = GBN_CPU_PORT;
    l2addr.flags = VD_L2_L3LOOKUP | VD_L2_STATIC;
    
    l3ifSwVidGet(pVsrv->vif.ifindex & SW_IFACE_IFMASK, &usVid);
    if ( (usVid > 0) && (usVid < 4095) )
    {
        l2addr.vid = usVid;
        if ( VD_E_NONE != smdVDMacAdd(l2addr) )
        {
            sprintf(szBuf, "add virtual mac failed for vid %d.", usVid);
            syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        }
    }
    else
    {
        usNum = 0;
        iRet = superVlanGetAllVlanWitchSw(pVsrv->vif.ifindex & SW_IFACE_IFMASK,
            &usNum, usVids);
        if ( (OK != iRet) || (0 == usNum) )
        {
            sprintf(szBuf, "find no vid for ifindex 0x%x supper vid %d when add.", 
                pVsrv->vif.ifindex, usVid);
            syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        }
        else
        {
            for ( i = 0; i < usNum; i++ )
            {
                l2addr.vid = usVids[i];
                if ( VD_E_NONE != smdVDMacAdd(l2addr) )
                {
                    sprintf(szBuf, "add virtual mac failed for vid %d.", l2addr.vid);
                    syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
                }
            }
        }
    }
#endif
    return 0;
}

static int vrrp_vsrv_del_vmac_from_hw(vrrp_rt *pVsrv)
{
#if 0
    int                      iRet, i;
    vd_l2_addr_t             l2addr;
    ushort_t                 usNum, usVids[MAX_SUPERVLAN_INFO];
    ushort_t                 usVid = 0;
    char                     szBuf[256];

    memset(&l2addr, 0, sizeof(l2addr));
    memcpy(l2addr.mac, pVsrv->vhwaddr, 6);
    l2addr.port = GBN_CPU_PORT;
    l2addr.flags = VD_L2_L3LOOKUP | VD_L2_STATIC;
    
    l3ifSwVidGet(pVsrv->vif.ifindex & SW_IFACE_IFMASK, &usVid);
    if ( (usVid > 0) && (usVid < 4095) )
    {
        l2addr.vid = usVid;
        if ( VD_E_NONE != smdVDMacDelete(l2addr) )
        {
            sprintf(szBuf, "del virtual mac failed for vid %d.", usVid);
            syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        }
    }
    else
    {
        usNum = 0;
        iRet = superVlanGetAllVlanWitchSw(pVsrv->vif.ifindex & SW_IFACE_IFMASK,
            &usNum, usVids);
        if ( (OK != iRet) || (0 == usNum) )
        {
            sprintf(szBuf, "find no vid for ifindex 0x%x supper vid %d when del.", 
                pVsrv->vif.ifindex, usVid);
            syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
        }
        else
        {
            for ( i = 0; i < usNum; i++ )
            {
                l2addr.vid = usVids[i];
                if ( VD_E_NONE != smdVDMacDelete(l2addr) )
                {
                    sprintf(szBuf, "del virtual mac failed for vid %d.", l2addr.vid);
                    syslogSendInfo(MID_VRRP, ERRORS, "errors", szBuf, szBuf, FALSE, FALSE, NO_WAIT);
                }
            }
        }
    }
#endif
    return 0;
}

/****************************************************************
 NAME    : vrrp_vsrv_init            00/02/06 09:18:02
 AIM    :
 REMARK    :
****************************************************************/
static void vrrp_vsrv_init( vrrp_rt *vsrv )
{
    int no = 0;
	sys_mac_t mac;

    if (!vsrv)
    {
        return;
    }
    no = vsrv->no;
    
    memset( vsrv, 0, sizeof(*vsrv) );

    vsrv->no          = no;
    vsrv->state       = EvrrpOperState_initialize;
    vsrv->priority    = VRRP_PRIO_DFL;
    vsrv->oldpriority = VRRP_PRIO_DFL;
    vsrv->adver_int   = VRRP_ADVER_DFL*VRRP_TIMER_HZ;
    vsrv->preempt     = VRRP_PREEMPT_DFL;
    vsrv->adminState  = EvrrpOperAdminState_down;
    vsrv->rowStatus   = EvrrpOperRowStatus_notReady;

    //getSysMacAdress(vsrv->vif.hwaddr); 
    sal_sys_macAddr_get(&mac);
	memcpy(vsrv->vif.hwaddr, mac.octet, sizeof(sys_mac_t));
	
    memset(&g_stcVrrpVip[vsrv->no], 0, sizeof(VRRP_VIP_STC));
}

/*******************************************************************************
  函数: vrrp_vsrv_get
  描述: 从全局备份组数组中获取未用的空闲表项
  输入: gVrrp_vsrv: 全局备份组数组
  输出: 无
  返回: 未用的空闲表项，无可用项时返回NULL
  其它: 
*******************************************************************************/
static vrrp_rt * vrrp_vsrv_get( void )
{
    vrrp_rt * vsrv = NULL;
    int i = 0;

    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        if (!gVrrp_vsrv[i].used)
        {
            vsrv = &gVrrp_vsrv[i];
            vrrp_vsrv_init(vsrv);
            vsrv->used = 1;
            break;
        }
    }

    return vsrv;
}

/*******************************************************************************
  函数: vrrp_vsrv_free
  描述: 释放已分配的备份组数据结构
  输入: vsrv: 备份组数据结构指针
  输出: 无
  返回: 无
  其它: 
*******************************************************************************/
static void vrrp_vsrv_free( vrrp_rt * vsrv )
{
    if (vsrv)
    {
        vrrp_vsrv_init(vsrv);
    }
}

/*******************************************************************************
  函数: vrrp_vsrv_find_by_pkt
  描述: 查找当前报文的接收备份组
  输入: iph: 当前收到的VRRP报文
  输出: 无
  返回: 合适的接收备份组，找不到时返回NULL
  其它: 
*******************************************************************************/
static vrrp_rt * vrrp_vsrv_find_by_pkt(struct ip * iph)
{
    vrrp_rt * vsrv = NULL;
    int i = 0;
    int ihl = 0;
    vrrp_pkt * hd = NULL;

    CHECK_VALID(iph, NULL);
    ihl = iph->ip_hl << 2;
    hd  = (vrrp_pkt *)((char *)iph + ihl);

    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        /* 判断VRID是否匹配 */
        if (!gVrrp_vsrv[i].used || gVrrp_vsrv[i].vrid != hd->vrid)
        {
            continue;
        }

        /* 判断接口是否匹配 */
        if( TRUE == sal_l3_intf_network_chk(gVrrp_vsrv[i].vif.ifindex, ntohl(iph->ip_src.s_addr)))
        {
            vsrv = &gVrrp_vsrv[i];
            break;
        }
    }

    return vsrv;
}

/*******************************************************************************
  函数: vrrp_vsrv_find_by_vrid
  描述: 根据VRID等查找备份组
  输入: ifindex: 接口索引，对应rfc2233:ifindex
        vrid: 备份组VRID
  输出: 无
  返回: 匹配的备份组，找不到时返回NULL
  其它: 
*******************************************************************************/
vrrp_rt * vrrp_vsrv_find_by_vrid(int ifindex, int vrid)
{
    vrrp_rt * vsrv = NULL;
    int i = 0;

    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        /* 判断VRID是否匹配 */
        if (!gVrrp_vsrv[i].used || gVrrp_vsrv[i].vrid != vrid)
        {
            continue;
        }
        /* 判断接口是否匹配 */
        if (ifindex == gVrrp_vsrv[i].vif.ifindex)
        {
            vsrv = &gVrrp_vsrv[i];
            break;
        }
    }

    return vsrv;
}

/*******************************************************************************
  函数: Vrrp_VsrvConfig
  描述: 配置备份组
  输入: ifindex: 接口索引，对应rfc2233:ifindex
        vrid: 备份组VRID
        vipaddr: 备份组虚拟IP地址
        opcode: 操作码: VRRP_OPCODE_ADDIP: 添加备份组IP 
                        VRRP_OPCODE_DELIP: 删除备份组IP 
                        VRRP_OPCODE_DELVR: 删除备份组
  输出: 无
  返回: 成功时返回0，失败时返回错误码
  其它: 错误码:  ERROR: 参数错误
                 VRRP_CFGERR_IFWRONG: 接口不存在或未配置正确的IP地址
                 VRRP_CFGERR_SUBNETDIFF: 备份组IP与接口IP不属于同一子网
                 VRRP_CFGERR_MAXVSRV: 备份组数目已达到最大值
                 VRRP_CFGERR_MAXVIP: 备份组IP地址数目已达到最大值
                 VRRP_CFGERR_VSRVNOTEXIST: 指定备份组不存在
                 VRRP_CFGERR_VIPNOTEXIST: 指定备份组IP不存在
*******************************************************************************/
int Vrrp_VsrvConfig(int ifindex, int vrid, uint_t vipaddr, int opcode)
{
    int rc = VOS_ERR_NONE;
    int i = 0;
    int pos = -1;
    ulong_t ipaddr = 0;
    vrrp_rt * vsrv = NULL;
	sys_l3_intfId_t l3intf;

    if(VRRP_IS_BAD_VID(vrid))
    	return VOS_ERR_FAIL;

    if (vipaddr != 0 && (sal_IPv4Valid_chk(vipaddr) !=  VOS_ERR_NONE))
    {
       return VOS_ERR_FAIL;
    }

    switch (opcode)
    {
    case VRRP_OPCODE_ADDIP:
        /* 添加备份组IP时需要判断VLAN接口存在且已配置IP地址，备份组IP和接口IP属于同一子网 */
        //if ( FALSE == l3ifSwIsHasValidIp(ifindex & SW_IFACE_IFMASK) )
        if(0) /*在命令行已经进行了检测*/
        {
            return VRRP_CFGERR_IFWRONG;
        }
        
        if (vipaddr)
        {
           	if ( FALSE == sal_l3_intf_network_chk(ifindex, vipaddr) )
            {
                return VRRP_CFGERR_SUBNETDIFF;
            }
        }
        vsrv = vrrp_vsrv_find_by_vrid(ifindex, vrid);
        if (!vsrv)
        {
            /* 获取主IP地址 */
           // if ( OK != l3ifSwPrimIpGet(ifindex & SW_IFACE_IFMASK, &ipaddr) )
           	if(VOS_ERR_NONE != sal_l3_intf_ipv4_by_vid_get(ifindex, &ipaddr))
            {
                return VRRP_CFGERR_IFWRONG;
            }
            /* 新建备份组 */
            vsrv = vrrp_vsrv_get();            
            if (!vsrv)
            {
                return VRRP_CFGERR_MAXVSRV;
            }

            if ( 0 == g_ulVrrpVsrvNum )
            {
                //smdVDVrrpOnOff(VD_ON_OFF_ENABLE);
                g_ulVrrpVsrvNum++;
            }
            vsrv->vrid = vrid;
            vsrv->vif.ifindex = ifindex;
			
			l3intf.type = SYS_L3_INTFTYPE_VLAN;
			l3intf.id = ifindex;
			sys_l3_intfId2Str(&l3intf, vsrv->vif.ifname);
			vsrv->vif.ifpindex = if_nametoindex(vsrv->vif.ifname);
			if (IfIsDown(ifindex))
			{
				vsrv->vif.ifstate = TRUE;
			}
			else
			{
				vsrv->vif.ifstate = FALSE;
			}
			
            g_stcVrrpVip[vsrv->no].ulIfIndex = ifindex;
            g_stcVrrpVip[vsrv->no].ulIpNum = 0;
            
            if (vipaddr)
            {
                vrrp_add_ipaddr(vsrv, vipaddr);
            }

            vsrv->vif.ipaddrM = ipaddr;
            vsrv->vif.ipaddrP = ipaddr;
            vsrv->vhwaddr[0] = 0x00;
            vsrv->vhwaddr[1] = 0x00;
            vsrv->vhwaddr[2] = 0x5E;
            vsrv->vhwaddr[3] = 0x00;
            vsrv->vhwaddr[4] = 0x01;
            vsrv->vhwaddr[5] = vrid;

            /* 判断VRRP交换机是否为IP地址拥有者 */
            //if (vipaddr && (OK == l3ifSwIpCheck(vipaddr)))
            if(vipaddr && (TRUE == sal_l3_intfIPv4_ip_equal(vipaddr)))
            {
                vsrv->priority = VRRP_PRIO_OWNER;
                vsrv->nowner++;
            }

            Vrrp_VsrvCreate(vsrv);
			if (vrrp_sock_join(gVrrp_sock, ifindex, ipaddr) == 0)
			{
				vsrv->vif.sockJoinState = TRUE;
			}
			else
			{
				vsrv->vif.sockJoinState = FALSE;
			}			
            vrrp_sock_join(gVrrp_tx_sock, ifindex, ipaddr);

            /*创建虚拟vrrp接口*/
            vrrp_vsrv_add_virtual_if(vsrv);
			vsrv->adminState = EvrrpOperAdminState_up;
			vsrv->rowStatus = EvrrpOperRowStatus_active;
        }
        else
        {
            /* 为已存在备份组增加IP地址 */
            for (i = 0; i < vsrv->naddr; i++)
            {
                if (vipaddr == vsrv->vaddr[i].addr)
                {
                    return VOS_ERR_NONE;
                }
            }
            if (vsrv->naddr >= VRRP_VIP_NUMS_MAX)
            {
                return VRRP_CFGERR_MAXVIP;
            }
            if (vipaddr)
            {
                vrrp_add_ipaddr(vsrv, vipaddr);

                if ( EvrrpOperState_backup == vsrv->state )
                {
                	//通知路由协议模块 暂未修改
                    //Vrrp_VsrpStateChangeCallback(vsrv, vipaddr, FALSE);
                }
            }

            /* 判断VRRP交换机是否为IP地址拥有者 */
            //if (vipaddr && (OK == l3ifSwIpCheck(vipaddr)))
            if (vipaddr && (TRUE == sal_l3_intfIPv4_ip_equal(vipaddr)))
            {
                vsrv->priority = VRRP_PRIO_OWNER;
                vsrv->nowner++;

                if (vsrv->state == EvrrpOperState_master)
                {
                    /*发送免费arp*/
                    //send_gratuitous_arp( vsrv, ipaddr, 0 );
                    sal_l3_vrrp_send_gratuitous_arp(vsrv->vif.ifindex, vipaddr, 0);
                }
            }
            else
            {
                /* 增加虚拟IP地址在系统的资源*/
                if (vsrv->state == EvrrpOperState_master)
                {
                    vrrp_vsrv_add_ip_to_system(vsrv, vipaddr);

                    /*发送免费arp*/
                    //send_gratuitous_arp( vsrv, ipaddr, 1 );
                    sal_l3_vrrp_send_gratuitous_arp(vsrv->vif.ifindex, vipaddr, 1);
                }
            }
        } 
        break;
    case VRRP_OPCODE_DELIP:
    case VRRP_OPCODE_DELVR:
        vsrv = vrrp_vsrv_find_by_vrid(ifindex, vrid);
        if (!vsrv)
        {
            return VRRP_CFGERR_VSRVNOTEXIST;
        }
        if (VRRP_OPCODE_DELIP == opcode)
        {
            for (i = 0; i < vsrv->naddr; i++)
            {
                if (vsrv->vaddr[i].addr == vipaddr)
                {
                    pos = i;
                    break;
                }
            }
            if (pos < 0)
            {
                return VRRP_CFGERR_VIPNOTEXIST;
            }
            for (i = pos; i < vsrv->naddr-1; i++)
            {
                vsrv->vaddr[i].addr = vsrv->vaddr[i+1].addr;
                g_stcVrrpVip[vsrv->no].ulVip[i] = g_stcVrrpVip[vsrv->no].ulVip[i + 1];
            }
            vsrv->naddr--;
            g_stcVrrpVip[vsrv->no].ulIpNum--;

            if ( EvrrpOperState_backup == vsrv->state )
            {
            	//通知路由协议
                //Vrrp_VsrpStateChangeCallback(vsrv, vipaddr, TRUE);
            }

            /* 判断VRRP交换机是否为IP地址拥有者 */
            //if (vipaddr && (OK == l3ifSwIpCheck(vipaddr)))
            if (vipaddr && (TRUE == sal_l3_intfIPv4_ip_equal(vipaddr)))
            {
                vsrv->nowner--;
                if (!vsrv->nowner)
                    vsrv->priority = vsrv->oldpriority;
            }
            else
            {
                /* 删除虚拟IP地址在系统的资源*/
                if (vsrv->state == EvrrpOperState_master)
                {
                    vrrp_vsrv_del_ip_from_system(vsrv, vipaddr);
                }
            }
            
            if (vsrv->naddr > 0)
            {
                break;
            }
        }
        Vrrp_VsrvDelete(vsrv);
        Vrrp_VsrpStateChangeCallback(vsrv, 0, TRUE);
        vrrp_sock_leave(gVrrp_sock, ifindex, vsrv->vif.ipaddrM);

        /*删除虚拟vrrp接口*/
        vrrp_vsr_del_virtual_if(vsrv);
        vrrp_vsrv_free(vsrv);

        g_ulVrrpVsrvNum--;
        if ( 0 == g_ulVrrpVsrvNum )
        {
            //smdVDVrrpOnOff(VD_ON_OFF_DISABLE);
        }
        
        break;
    default:
        rc = VOS_ERR_FAIL;
        break;
    }

    return rc;
}

/****************************************************************
 NAME    : vrrp_add_ipaddr            00/02/06 09:24:08
 AIM    :
 REMARK    :
****************************************************************/
static void vrrp_add_ipaddr( vrrp_rt *vsrv, uint_t ipaddr )
{
    vsrv->naddr++;
	#if 0
    /* alloc the room */
    if( vsrv->vaddr ){
        vsrv->vaddr = realloc( vsrv->vaddr
                    , vsrv->naddr*sizeof(*vsrv->vaddr) );
    } else {
        vsrv->vaddr = malloc( sizeof(*vsrv->vaddr) );
    }

    //vosAssert( vsrv->vaddr );
	#endif
    /* store the data */
    vsrv->vaddr[vsrv->naddr-1].addr = ipaddr;

    g_stcVrrpVip[vsrv->no].ulIpNum = vsrv->naddr;
    g_stcVrrpVip[vsrv->no].ulVip[vsrv->naddr - 1] = ipaddr;
}

/****************************************************************
 NAME    : state_init                00/02/07 00:15:26
 AIM    :
 REMARK    : rfc2338.6.4.1
****************************************************************/
static void state_init( vrrp_rt *vsrv )
{
    int delay = 0;
    ulong_t ulSecs = 0, ulTicks = 0;
    
    if (!vsrv || vsrv->adminState != EvrrpOperAdminState_up)
    {
        return;
    }

    if( vsrv->priority == VRRP_PRIO_OWNER)
    {
        state_goto_master( vsrv );
        Vrrp_Debug("Virtual Router %d goto master.", vsrv->vrid);
    }
    else
    {
        delay = 3*vsrv->adver_int + VRRP_TIMER_SKEW(vsrv) +
                        (vsrv->preempt ? vsrv->delay : 0);
        VRRP_TIMER_SET( vsrv->ms_down_timer, delay );
        vsrv->state = EvrrpOperState_backup;
        Vrrp_Debug("Virtual Router %d goto backup.", vsrv->vrid);

        /*通知路由协议*/
        Vrrp_VsrpStateChangeCallback(vsrv, 0, FALSE);
    }

    //vosUpTime(&ulSecs, &ulTicks);
    vsrv->upTime = ulSecs;
}

/****************************************************************
 NAME    : state_back                00/02/07 00:15:26
 AIM    :
 REMARK    : rfc2338.6.4.2
****************************************************************/
static void state_back( vrrp_rt *vsrv, char * pPkt )
{
    struct ip * iph = NULL;
    vrrp_pkt * hd = NULL;
    int delay = 0;
    
    if (vsrv->adminState != EvrrpOperAdminState_up)
    {
        vsrv->state = EvrrpOperState_initialize;
        Vrrp_Debug("Virtual Router %d goto init.", vsrv->vrid);
        return;
    }

    /* 接口IP地址已被删除 */
    if (!vsrv->vif.ipaddrM)
    {
        VRRP_TIMER_CLR( vsrv->ms_down_timer );
        vsrv->adminState = EvrrpOperAdminState_down;
        vsrv->state = EvrrpOperState_initialize;
        Vrrp_Debug("Virtual Router %d goto init.", vsrv->vrid);
        return;
    }

    /* 收到VRRP报文 */
    iph = (struct ip *)pPkt;
    hd = (vrrp_pkt *)((char *)iph + (iph->ip_hl<<2));
    
    if ( hd->priority == 0 )
    {
        delay = VRRP_TIMER_SKEW(vsrv);
        delay = (delay == 0) ? 1: delay;
        VRRP_TIMER_SET( vsrv->ms_down_timer, delay );
        vsrv->staPriZeroPktsRcvd++;
    }
    else if( !vsrv->preempt || hd->priority > vsrv->priority ||
            (hd->priority == vsrv->priority &&
            ntohl(iph->ip_src.s_addr) > vsrv->vif.ipaddrM))
    {
        delay = 3*vsrv->adver_int + VRRP_TIMER_SKEW(vsrv) +
                        (vsrv->preempt ? vsrv->delay : 0);
        VRRP_TIMER_SET( vsrv->ms_down_timer, delay );
    }

}

/****************************************************************
 NAME    : state_mast                00/02/07 00:15:26
 AIM    :
 REMARK    : rfc2338.6.4.3
****************************************************************/
static void state_mast( vrrp_rt *vsrv, char * pPkt )
{    
    struct ip * iph = NULL;
    vrrp_pkt * hd = NULL;
    int delay = 0;
    
    if (vsrv->adminState != EvrrpOperAdminState_up)
    {
        vsrv->state = EvrrpOperState_initialize;
        Vrrp_Debug("Virtual Router %d goto init.", vsrv->vrid);
        return;
    }
    
    /* 接口IP地址已被删除 */
    if (!vsrv->vif.ipaddrM)
    {
        VRRP_TIMER_CLR( vsrv->adver_timer );
        state_leave_master( vsrv, 0 );
        vsrv->adminState = EvrrpOperAdminState_down;
        vsrv->state = EvrrpOperState_initialize;
        Vrrp_Debug("Virtual Router %d goto init.", vsrv->vrid);
        return;
    }

    /* 收到VRRP报文 */
    iph = (struct ip *)pPkt;
    hd = (vrrp_pkt *)((char *)iph + (iph->ip_hl<<2));
    
    if( hd->priority == 0 )
    {
        vrrp_send_adv( vsrv, vsrv->priority );
        VRRP_TIMER_SET(vsrv->adver_timer,vsrv->adver_int);
        vsrv->staPriZeroPktsRcvd++;
    }
    else if( hd->priority > vsrv->priority ||
            (hd->priority == vsrv->priority &&
            ntohl(iph->ip_src.s_addr) > vsrv->vif.ipaddrM) )
    {
        delay = 3*vsrv->adver_int + VRRP_TIMER_SKEW(vsrv) +
                        (vsrv->preempt ? vsrv->delay : 0);
        VRRP_TIMER_SET( vsrv->ms_down_timer, delay );
        VRRP_TIMER_CLR( vsrv->adver_timer );
        state_leave_master( vsrv, 0 );
        vsrv->state = EvrrpOperState_backup;
        Vrrp_Debug("Virtual Router %d goto backup.", vsrv->vrid);

        /*通知路由协议*/
        Vrrp_VsrpStateChangeCallback(vsrv, 0, FALSE);
    }
}

/****************************************************************
 NAME    : state_gotomaster            00/02/07 00:15:26
 AIM    :
 REMARK    : called when the state is now MASTER
****************************************************************/
static void state_goto_master( vrrp_rt *vsrv )
{
    unsigned int             i;
    
    /* 设置硬件接口接收目的MAC为虚拟MAC的报文 */
    vrrp_vsrv_add_vmac_to_hw(vsrv);        

    /* 为虚拟IP添加路由 */
    for (i = 0; i < vsrv->naddr; i++)
    {
        /*只添加非owner ip*/
        if( FALSE == sal_l3_intfIPv4_ip_equal(vsrv->vaddr[i].addr))
        {
            vrrp_vsrv_add_ip_to_system(vsrv, vsrv->vaddr[i].addr);

            //send_gratuitous_arp( vsrv, vsrv->vaddr[i].addr, 1 );
            sal_l3_vrrp_send_gratuitous_arp(vsrv->vif.ifindex, vsrv->vaddr[i].addr, 1);
        }
        else
        {
            //send_gratuitous_arp( vsrv, vsrv->vaddr[i].addr, 0 );
            sal_l3_vrrp_send_gratuitous_arp(vsrv->vif.ifindex, vsrv->vaddr[i].addr, 1);
        }
    }

    /* send an advertisement */
    vrrp_send_adv( vsrv, vsrv->priority );

    /* init the struct */
    VRRP_TIMER_SET( vsrv->adver_timer, vsrv->adver_int );
    vsrv->state = EvrrpOperState_master;
    vsrv->staBecomeMaster++;

    /* 发送TRAP */
    //Vrrp_TrapNewMasterSend(vsrv->vif.ifindex, vsrv->vrid);

    /*通知路由协议*/
    Vrrp_VsrpStateChangeCallback(vsrv, 0, TRUE);
}

/****************************************************************
 NAME    : state_leavemaster            00/02/07 00:15:26
 AIM    :
 REMARK    : called when the state is no more MASTER
****************************************************************/
static void state_leave_master( vrrp_rt *vsrv, int advF )
{
    unsigned int             i;
    
    /* 设置硬件接口不再接收目的MAC为虚拟MAC的报文*/
    vrrp_vsrv_del_vmac_from_hw(vsrv);

    /* 删除虚拟IP路由 */
    for (i = 0; i < vsrv->naddr; i++)
    {
        /*只删除非owner ip*/
        if( FALSE == sal_l3_intfIPv4_ip_equal(vsrv->vaddr[i].addr))
        {
            vrrp_vsrv_del_ip_from_system(vsrv, vsrv->vaddr[i].addr);
        }
    }
    
    /* if we stop vrrpd, warn the other routers to speed up the recovery */
    if( advF ){
        vrrp_send_adv( vsrv, VRRP_PRIO_STOP );
        vsrv->staPriZeroPktsSent++;
    }
}

/****************************************************************
 NAME    : vrrp_send_adv                00/02/06 16:31:24
 AIM    :
 REMARK    :
****************************************************************/
static int vrrp_send_adv( vrrp_rt *vsrv, int prio )
{
    int    buflen, ret;
    char *    buffer;

    /* alloc the memory */
    buflen = vrrp_dlt_len(vsrv) + vrrp_iphdr_len(vsrv) + vrrp_hd_len2(vsrv);
    buffer = memalign( 4, buflen + 4 );
    //vosAssert( buffer );

    /* build the packet  */
    vrrp_build_pkt( vsrv, prio, buffer + 2, buflen );

    /* send it */
    ret = vrrp_send_pkt( vsrv, buffer + 2, buflen );

    /* free the memory */
    free( buffer );
    return ret;
}

/****************************************************************
 NAME    : vrrp_build_pkt                00/02/02 13:33:32
 AIM    : build a advertissement packet
 REMARK    :
****************************************************************/
static void vrrp_build_pkt( vrrp_rt *vsrv, int prio, char *buffer, int buflen )
{
    vrrp_build_dlt( vsrv, buffer, buflen );
    buffer += vrrp_dlt_len(vsrv);
    buflen -= vrrp_dlt_len(vsrv);

    /* build the ip header */
    vrrp_build_ip( vsrv, buffer, buflen );
    buffer += vrrp_iphdr_len(vsrv);
    buflen -= vrrp_iphdr_len(vsrv);

    /* build the vrrp header */
    vrrp_build_vrrp( vsrv, prio, buffer, buflen );
}

/****************************************************************
 NAME    : vrrp_build_dlt            00/02/02 14:39:18
 AIM    :
 REMARK    : rfc2338.7.3
****************************************************************/
static void vrrp_build_dlt( vrrp_rt *vsrv, char *buffer, int buflen )
{
    /* hardcoded for ethernet */
    struct ether_header *    eth = (struct ether_header *)buffer;
    /* destination address --rfc1122.6.4*/
    eth->ether_dhost[0]    = 0x01;
    eth->ether_dhost[1]    = 0x00;
    eth->ether_dhost[2]    = 0x5E;
    eth->ether_dhost[3]    = (INADDR_VRRP_GROUP >> 16) & 0x7F;
    eth->ether_dhost[4]    = (INADDR_VRRP_GROUP >>  8) & 0xFF;
    eth->ether_dhost[5]    =  INADDR_VRRP_GROUP        & 0xFF;
    /* source address --rfc2338.7.3 */
    memcpy( eth->ether_shost, vsrv->vhwaddr, sizeof(vsrv->vhwaddr));
    /* type */
    eth->ether_type        = htons( ETHERTYPE_IP );
}

/****************************************************************
 NAME    : vrrp_build_ip                00/02/02 14:39:18
 AIM    : build a ip packet
 REMARK    :
****************************************************************/
static void vrrp_build_ip( vrrp_rt *vsrv, char *buffer, int buflen )
{
    struct ip * iph = (struct ip *)(buffer);
    iph->ip_hl      = 5;
    iph->ip_v       = 4;
    iph->ip_tos     = 0;
    iph->ip_len     = iph->ip_hl*4 + vrrp_hd_len2( vsrv );
    iph->ip_len     = htons(iph->ip_len);
    //iph->ip_id      = (ushort_t)(tickGet() & 0xffff);
    iph->ip_off     = 0;
    iph->ip_ttl     = VRRP_IP_TTL;
    iph->ip_p       = IPPROTO_VRRP;
    iph->ip_src.s_addr = htonl(vsrv->vif.ipaddrM);
    iph->ip_dst.s_addr = htonl(INADDR_VRRP_GROUP);
    /* checksum must be done last */
    iph->ip_sum = 0;
    iph->ip_sum = vrrp_in_csum( (ushort_t*)iph, iph->ip_hl*4, 0 );
}

/****************************************************************
 NAME    : vrrp_build_vrrp            00/02/02 14:39:18
 AIM    :
 REMARK    :
****************************************************************/
static int vrrp_build_vrrp( vrrp_rt *vsrv, int prio, char *buffer, int buflen )
{
    int    i;
    vrrp_pkt *hd    = (vrrp_pkt *)buffer;
    uint_t *iparr    = (uint_t *)((char *)hd+sizeof(*hd));
    
    hd->vers_type    = (VRRP_VERSION<<4) | VRRP_PKT_ADVERT;
    hd->vrid    = vsrv->vrid;
    hd->priority    = prio;
    hd->naddr    = vsrv->naddr;
    hd->auth_type    = 0;
    hd->adver_int    = vsrv->adver_int/VRRP_TIMER_HZ;
    /* copy the ip addresses */
    for( i = 0; i < vsrv->naddr; i++ ){
        iparr[i] = htonl(vsrv->vaddr[i].addr);
    }
    hd->chksum    = 0;
    hd->chksum    = vrrp_in_csum( (ushort_t *)hd, vrrp_hd_len2(vsrv), 0);
    return(0);
}

/****************************************************************
 NAME    : vrrp_send_pkt                00/02/06 16:37:10
 AIM    :
 REMARK    :发送以太网帧
****************************************************************/
static int vrrp_send_pkt( vrrp_rt *vsrv, char *buffer, int buflen )
{
	sal_sockaddr_in destAddr;
	sal_sockaddr_in sAddr;
	struct ip_mreqn mreqn;
	struct in_addr addr;
    int iRet;
    char on = 0;
	char ttl = 255;
	int headlen = vrrp_dlt_len(vsrv);

	iRet = sal_setsockopt(gVrrp_tx_sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
	/* Turn off multicast loop on Tx */
	iRet = sal_setsockopt (gVrrp_tx_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&on, sizeof(on));
	
	memset (&mreqn, 0, sizeof(mreqn));
	
	mreqn.imr_ifindex = vsrv->vif.ifpindex;
	iRet =  sal_setsockopt(gVrrp_tx_sock, IPPROTO_IP, IP_MULTICAST_IF, (void *)&mreqn, sizeof(mreqn));

	memset(&destAddr, 0, sizeof(destAddr));
	destAddr.sin_family = SAL_AF_INET;
	destAddr.sin_addr.s_addr = htonl(INADDR_VRRP_GROUP);
#ifdef HAVE_SA_LEN
	destAddr.sin_len = sizeof(destAddr);
#endif

	iRet = sal_sendto(gVrrp_tx_sock, buffer + headlen, buflen - headlen, 0, (sal_sockaddr *)&destAddr, sizeof(destAddr));
    if (iRet <= VOS_ERR_NONE)
    {
        //VRRP_LOG(("send failed iRet %d."RN, iRet));
        return -1;
    }

    return 0;
}

/****************************************************************
 NAME    : send_gratuitous_arp            00/05/11 11:56:30
 AIM    :
 REMARK    : rfc0826
    : WORK: ugly because heavily hardcoded for ethernet
****************************************************************/
static int send_gratuitous_arp( vrrp_rt *vsrv, int addr, int vAddrF )
{
    ulong_t  buf[(sizeof(struct ether_arp)+sizeof(struct ether_header) + 3)/4];
    char   buflen = sizeof(struct ether_arp)+sizeof(struct ether_header);
    
    struct ether_header *eth = (struct ether_header *)buf;
    struct ether_arp *arph = (struct ether_arp *)((char *)buf+vrrp_dlt_len(vsrv));
    char   *hwaddr = vAddrF ? vsrv->vhwaddr : vsrv->vif.hwaddr;
    int    hwlen = 6;
    ulong_t ulTmp;
    ushort_t usTmp = 0;
    u_char uTmp = 0;

    /* hardcoded for ethernet */
    memset( eth->ether_dhost, 0xFF, hwlen );
    memcpy( eth->ether_shost, hwaddr, hwlen );
    usTmp = htons(ETHERTYPE_ARP);
    memcpy( &eth->ether_type, &usTmp, sizeof(usTmp) );
    
    /* build the arp payload */
    bzero( (char *)arph, sizeof( *arph ) );
    usTmp = htons(ARPHRD_ETHER);
    memcpy( &arph->arp_hrd, &usTmp, sizeof(usTmp) );

    usTmp = htons(ETHERTYPE_IP);
    memcpy( &arph->arp_pro, &usTmp, sizeof(usTmp) );

    uTmp = 6;
    memcpy( &arph->arp_hln, &uTmp, sizeof(uTmp) );

    uTmp = 4;
    memcpy( &arph->arp_pln, &uTmp, sizeof(uTmp) );
    
    usTmp = htons(ARPOP_REQUEST);
    memcpy( &arph->arp_op, &usTmp, sizeof(usTmp) );
    
    memcpy( arph->arp_sha, hwaddr, hwlen );
    ulTmp = htonl(addr);
    memcpy( arph->arp_spa, &ulTmp, sizeof(ulTmp) );
    memcpy( arph->arp_tpa, &ulTmp, sizeof(ulTmp) );
    return vrrp_send_pkt( vsrv, (char *)buf, buflen );
}

/*******************************************************************************
  函数: ifindex_to_vlanid
  描述: 根据接口索引获取VLAN ID
  输入: ifindex: 接口索引
  输出: 无
  返回: 成功时返回VLAN ID，否则返回0
  其它: 
*******************************************************************************/
ulong_t ifindex_to_vlanid(ulong_t ifindex)
{
    ushort_t                 usVid = 0;
    
    //if ( OK != l3ifSwVidGet(ifindex & SW_IFACE_IFMASK, &usVid) )
    {
        return 0;
    }

    return usVid;
}

/*******************************************************************************
  函数: Vrrp_GetIpaddrM
  描述: 根据备份组索引获取接口主IP
  输入: vsrvidx: 备份组索引
  输出: 无
  返回: 成功时返回接口主IP，否则返回0
  其它: 
*******************************************************************************/
ulong_t Vrrp_GetIpaddrM(ulong_t vsrvidx)
{
    CHECK_VALID(vsrvidx < VRRP_VSRV_SIZE_MAX, 0);
    
    return gVrrp_vsrv[vsrvidx].vif.ipaddrM;
}

/*******************************************************************************
  函数: Vrrp_CheckVipAndState
  描述: 判断IP地址是否为虚拟IP地址,若是，检查交换机状态
  输入: ipaddr: 待判断的IP地址
  输出:  checkState: 检查交换机状态,0表示backup，1表示master
  返回: 非虚拟IP时返回0，否则返回对应的备份组索引+1
  其它: 
*******************************************************************************/
int Vrrp_CheckVipAndState(ulong_t ipaddr, int *checkState)
{
    int i,j;
    for(i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        if (!gVrrp_vsrv[i].used)
            continue;
        for(j = 0; j < gVrrp_vsrv[i].naddr; j++)
        {
            if (gVrrp_vsrv[i].vaddr[j].addr == ipaddr)
            {
                if(gVrrp_vsrv[i].state == EvrrpOperState_master)
                    *checkState = 1;
                else 
                    *checkState = 0;
                return (i + 1);
            }
        }
    }
    *checkState = 0;
    return 0;
}
/*******************************************************************************
  函数: Vrrp_VridGet
  描述: 得到备份组的vrid
  输入: vsrvidx: 备份组索引
  输出: 无
  返回: 成功时返回备份组的vrid，失败返回0
  其它: 
*******************************************************************************/
int Vrrp_VridGet(ulong_t vsrvidx)
{
    CHECK_VALID(vsrvidx < VRRP_VSRV_SIZE_MAX, 0);
    return gVrrp_vsrv[vsrvidx].vrid;
}
/*******************************************************************************
  函数: Vrrp_TrapNewMasterSend
  描述: 发送TRAP vrrpTrapNewMaster
  输入: ifindex: 备份组所在的VLAN接口索引        
        vrid: 备份组ID
  输出: 无
  返回: 成功返回OK，失败返回ERROR
  其它: 
*******************************************************************************/
static int Vrrp_TrapNewMasterSend(ulong_t ifindex, ulong_t vrid)
{
#if 0
    int rc = VOS_ERR_NONE;
    vbList_t * vbList;
    ulong_t vrrpTrapNewMaster[] = {1,3,6,1,2,1,68,0,1};
    ulong_t * trapOid;
    ulong_t idxBuf[2],oidLen;
    char * trapVars[] = {        
                        "rfc2787:vrrpOperMasterIpAddr",
                        0
                        };

    if (gVrrp_NotificationCntl != EvrrpNotificationCntl_enabled)
    {
        return ERROR;
    }

    vbList = envoyNotifyVBLAlloc(1);
    trapOid = vrrpTrapNewMaster;
    oidLen = sizeof(vrrpTrapNewMaster) / sizeof(ulong_t);
    idxBuf[0] = ifindex;
    idxBuf[1] = vrid;

    if (NULL != vbList)
    {
        envoyNotifyVBLVbAdd(vbList, (uchar_t *)trapVars[0], idxBuf, 2);
        rc = envoyNotifySend((uchar_t *)"gbn", trapOid, oidLen, vbList);
        envoyNotifyVBLClean(&vbList);
    }
    else
    {
        rc = ERROR;
    }
#endif
    return 0;
}

/*******************************************************************************
  函数: Vrrp_TrapNewMasterSend
  描述: 发送TRAP vrrpTrapAuthFailure
  输入: 
  输出: 无
  返回: 成功返回OK，失败返回ERROR
  其它: 
*******************************************************************************/
static int Vrrp_TrapAuthFailureSend(void)
{
#if 0
    STATUS rc = OK;
    vbList_t * vbList;
    ulong_t vrrpTrapAuthFailure[] = {1,3,6,1,2,1,68,0,2};
    ulong_t * trapOid;
    ulong_t idxBuf[2],oidLen;
    int i;
    char * trapVars[] = {        
                        "rfc2787:vrrpTrapPacketSrc",
                        "rfc2787:vrrpTrapAuthErrorType",
                        0
                        };

    if (gVrrp_NotificationCntl != EvrrpNotificationCntl_enabled)
    {
        return ERROR;
    }

    vbList = envoyNotifyVBLAlloc(2);
    trapOid = vrrpTrapAuthFailure;
    oidLen = sizeof(vrrpTrapAuthFailure) / sizeof(ulong_t);
    idxBuf[0] = 0;

    if (NULL != vbList)
    {
        for (i = 0; i < 2; i++)
        {
            envoyNotifyVBLVbAdd(vbList, (uchar_t *)trapVars[i], idxBuf, 1);
        }
        rc = envoyNotifySend((uchar_t *)"gbn", trapOid, oidLen, vbList);
        envoyNotifyVBLClean(&vbList);
    }
    else
    {
        rc = ERROR;
    }
#endif
    return 0;
}

/*******************************************************************************
  函数: Vrrp_networkBackup( ulong_t ulAddr, ulong_t ulMask )
  描述: 查看VLAN接口的IP网段在备份组中是否为备份状态
  输入: ulIfIndex: 接口索引
              ulAddr:接口地址
              ulMask:接口掩码
  输出: 无
  返回: TRUE: 备份状态；FALSE: 没有VRRP配置或Master状态
  其它: 
*******************************************************************************/
BOOL Vrrp_networkBackup( ulong_t ulAddr, ulong_t ulMask )
{
    vrrp_rt *pVsrv;
    int       i, j;
    
    /* 查找匹配的备份组 */
    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        pVsrv = &gVrrp_vsrv[i];
        /* 判断ulIfIndex是否匹配 */
        if (!pVsrv->used )
        {
            continue;
        }
        
        for (j = 0; j < pVsrv->naddr; j++)
        {
            /* 判断网段是否匹配 、状态是否为备份*/
            if( (pVsrv->vaddr[j].addr & ulMask) == (ulAddr & ulMask) )
            {
                /*master状态下判断备份组所在接口下的所有端口的连接状态
                  如果所有端口都down，则将该备份组作为备份状态对待*/
                if ( EvrrpOperState_master == pVsrv->state )
                {
                    if (IfIsDown(pVsrv->vif.ifindex))
                    {
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                }                
                else if( EvrrpOperState_backup == pVsrv->state )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }

    return FALSE;
}

int Vrrp_routeProtoReg(char *szProcessName, ulong_t ulFunCallBackId)
{
    VRRP_PROTO_REG_STC      *pProtoReg;

    if ( strlen(szProcessName) > GLN_PROC_NAME_MAX_LEN )
    {
        return VOS_ERR_FAIL;
    }

    pProtoReg = (VRRP_PROTO_REG_STC *)lstFirst(&g_lstVrrpProtoReg);
    while ( NULL != pProtoReg )
    {
        if ( !strcmp(szProcessName, pProtoReg->szProcessName) )
        {
            pProtoReg->ulFunCallBackId = ulFunCallBackId;
            return VOS_ERR_NONE;
        }
        
        pProtoReg = (VRRP_PROTO_REG_STC *)lstNext((NODE *)pProtoReg);
    }

    pProtoReg = malloc(sizeof(VRRP_PROTO_REG_STC));
    if ( NULL == pProtoReg )
    {
        return VOS_ERR_FAIL;
    }

    strcpy(pProtoReg->szProcessName, szProcessName);
    pProtoReg->ulFunCallBackId = ulFunCallBackId;

    lstAdd(&g_lstVrrpProtoReg, (NODE *)pProtoReg);
    return VOS_ERR_NONE;
}

int Vrrp_routeProtoDeReg(char *szProcessName, ulong_t ulFunCallBackId)
{
    VRRP_PROTO_REG_STC      *pProtoReg;

    pProtoReg = (VRRP_PROTO_REG_STC *)lstFirst(&g_lstVrrpProtoReg);
    while ( NULL != pProtoReg )
    {
        if ( !strcmp(szProcessName, pProtoReg->szProcessName) )
        {
            break;
        }
        pProtoReg = (VRRP_PROTO_REG_STC *)lstNext((NODE *)pProtoReg);
    }

    if ( NULL == pProtoReg )
    {
        return VOS_ERR_NONE;
    }

    lstDelete(&g_lstVrrpProtoReg, (NODE *)pProtoReg);
    vosFree(pProtoReg);
    return VOS_ERR_NONE;
}

static int vrrpNotifyRouteProto(char *szProcessName, ulong_t ulFunCallBackId, 
    char *szIfName, ulong_t ulVrid, ulong_t ulVrips[], ulong_t ulVripNum, BOOL bBackup)
{
    int                      iRet;
    SMD_MSG                  msg;
#if 0
    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_RM, ulFunCallBackId, SMD_GLOBAL, SMD_MSG_INFO);
    msg.ucNeedRet = SMD_MSG_RET_NOT;

    ARG_ADD(0, szIfName, SMD_ARG_POINTER, strlen(szIfName) + 1);
    ARG_ADD(1, &ulVrid, 0, sizeof(ulong_t));
    ARG_ADD(2, ulVrips, SMD_ARG_POINTER, sizeof(ulong_t) * ulVripNum);
    ARG_ADD(3, &ulVripNum, 0, sizeof(ulong_t));
    ARG_ADD(4, &bBackup, 0, sizeof(BOOL));
    
    iRet = smdMsgRequestByName(szProcessName, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);

    return iRet;
#endif
	return 0;

}

void Vrrp_VsrpStateChangeCallback(vrrp_rt *pVsrv, ulong_t ulVrip, 
    BOOL bMaster)
{
    unsigned int             i;
    BOOL                     bBakup = TRUE;
    VRRP_PROTO_REG_STC      *pProtoReg;
    ulong_t                  ulVrips[VRRP_VIP_NUMS_MAX] = { 0 }, ulVripNum = 0;
    char                     szIfName[32];

    //sprintf(szIfName, "sw%d", (pVsrv->vif.ifindex & SW_IFACE_IFMASK) - 1);
    if ( TRUE == bMaster )
    {
        bBakup = FALSE;
    }

    if ( ulVrip )
    {
        ulVrips[0] = ulVrip;
        ulVripNum = 1;
    }
    else
    {
        for (i = 0; i < pVsrv->naddr; i++)
        {
            ulVrips[ulVripNum++] = pVsrv->vaddr[i].addr;
            if ( ulVripNum >= VRRP_VIP_NUMS_MAX )
            {
                break;
            }
        }
    }

    pProtoReg = (VRRP_PROTO_REG_STC *)lstFirst(&g_lstVrrpProtoReg);
    while ( NULL != pProtoReg )
    {
        vrrpNotifyRouteProto(pProtoReg->szProcessName, pProtoReg->ulFunCallBackId, 
            szIfName, pVsrv->vrid, ulVrips, ulVripNum, bBakup);
        
        pProtoReg = (VRRP_PROTO_REG_STC *)lstNext((NODE *)pProtoReg);
    }
}

/* 检查ip地址是否是vlan接口的虚拟地址 */
int Vrrp_check_ip_in_if(ulong_t ulIfIndex, ulong_t ulIfAddr)
{
    vrrp_rt *pVsrv;
    int       i, j;
    
    /* 查找匹配的备份组 */
    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        pVsrv = &gVrrp_vsrv[i];
        /* 判断ulIfIndex是否匹配 */
        if (!pVsrv->used || (pVsrv->vif.ifindex != ulIfIndex))
        {
            continue;
        }
        
        for (j = 0; j < pVsrv->naddr; j++)
        {
            /* 判断地址是否匹配 */
            if (pVsrv->vaddr[j].addr == ulIfAddr)
            {
                return VOS_ERR_NONE;
            }
        }
    }

    return VOS_ERR_FAIL;
}

/*******************************************************************************
  函数: VrrpIntfChk
  描述: 检测备份组所在接口下的所有端口的连接情况
  输入: 无
  输出: 无
  返回: 无
  其它: 
*******************************************************************************/
void VrrpIntfChk(void)
{
    vrrp_rt *pVsrv;
    int       i;

    if( !gVrrp_initialized )
    {
        return ;
    }
    
    /* 查找匹配的备份组 */
    for (i = 0; i < VRRP_VSRV_SIZE_MAX; i++)
    {
        pVsrv = &gVrrp_vsrv[i];

        if (!pVsrv || !pVsrv->used )
        {
            continue;
        }

        switch( pVsrv->state )
        {
            case EvrrpOperState_initialize:
                break;
                
            case EvrrpOperState_backup:
                break;
                
            case EvrrpOperState_master:
                break;
        }
    }
}

/*******************************************************************************
  函数: IfIsDown
  描述: 检测接口是否down
  输入: ifindex: 接口索引
  输出: 无
  返回: TRUE: 接口down
        FALSE: 接口非down
  其它: 
*******************************************************************************/
BOOL IfIsDown(int ifindex)
{
	sys_l3_intfId_t l3intf;
	
	l3intf.type = SYS_L3_INTFTYPE_VLAN;
	l3intf.id = ifindex;

    if (IS_L3_INTF_LINKUP(&l3intf))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int vrrpVsrvDellAllByIfindex(ulong_t ulIfIndex, ulong_t *pulNumDel)
{
    unsigned int             i;
    ulong_t                  ulNum = 0;

    for ( i = 0; i < VRRP_VSRV_SIZE_MAX; i++ )
    {
        if ( !gVrrp_vsrv[i].used || gVrrp_vsrv[i].rowStatus != EvrrpOperRowStatus_active )
        {
            continue;
        }

        if ( ulIfIndex != gVrrp_vsrv[i].vif.ifindex )
        {
            continue;
        }

        Vrrp_VsrvConfig(ulIfIndex, gVrrp_vsrv[i].vrid, 0, VRRP_OPCODE_DELVR);
        ulNum++;
    }

    *pulNumDel = ulNum;
    return VOS_ERR_NONE;
}

int vrrpVsrvPrioritySet(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulPrio, int *piErrNo)
{
    vrrp_rt                 *pVsrv;

    *piErrNo = 0;
    
    pVsrv = vrrp_vsrv_find_by_vrid(ulIfIndex, ulVrid);
    if ( NULL == pVsrv )
    {
        *piErrNo = VRRP_CFGERR_VSRVNOTEXIST;
        return VOS_ERR_FAIL;
    }

    pVsrv->oldpriority = ulPrio;
    if (pVsrv->priority != VRRP_PRIO_OWNER)
    {
        pVsrv->priority = ulPrio;
    }

    return VOS_ERR_NONE;    
}

int vrrpVsrvPreemptSet(ulong_t ulIfIndex, ulong_t ulVrid, ulong_t ulPreempt,
    ulong_t ulTime, int *piErrNo)
{
    vrrp_rt                 *pVsrv;

    *piErrNo = 0;
    
    pVsrv = vrrp_vsrv_find_by_vrid(ulIfIndex, ulVrid);
    if ( NULL == pVsrv )
    {
        *piErrNo = VRRP_CFGERR_VSRVNOTEXIST;
        return VOS_ERR_FAIL;
    }

    pVsrv->preempt = ulPreempt;
    pVsrv->delay = ulTime * VRRP_TIMER_HZ;

    return VOS_ERR_NONE;    
}

int vrrpVsrvAdverTimeSet(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulTime, int *piErrNo)
{
    vrrp_rt                 *pVsrv;

    *piErrNo = 0;
    
    pVsrv = vrrp_vsrv_find_by_vrid(ulIfIndex, ulVrid);
    if ( NULL == pVsrv )
    {
        *piErrNo = VRRP_CFGERR_VSRVNOTEXIST;
        return VOS_ERR_FAIL;
    }

    pVsrv->adver_int = ulTime * VRRP_TIMER_HZ;

    return VOS_ERR_NONE;    
}

int vrrpVsrvGetNext(int iIndex, vrrp_rt *pVsrvInfo)
{
    vrrp_rt                 *pVsrv;
    ulong_t                  ulIndex = (iIndex + 1);

    if ( ulIndex >= VRRP_VSRV_SIZE_MAX )
    {
        return VOS_ERR_FAIL;
    }

    if ( NULL == pVsrvInfo )
    {
        return VOS_ERR_FAIL;
    }

    for ( ; ulIndex < VRRP_VSRV_SIZE_MAX; ulIndex++ )
    {
        pVsrv = &gVrrp_vsrv[ulIndex];

        if ( !pVsrv->used || pVsrv->rowStatus != EvrrpOperRowStatus_active)
        {
            continue;
        }

        memcpy(pVsrvInfo, pVsrv, sizeof(vrrp_rt));
        return VOS_ERR_NONE;
    }

    return VOS_ERR_FAIL;
}

int vrrpVsrvIfTrackAdd(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulIfTracIndex, ulong_t ulPrio, int *piErrNo)
{
    unsigned int             i;
    vrrp_rt                 *pVsrv;

    *piErrNo = 0;
    
    pVsrv = vrrp_vsrv_find_by_vrid(ulIfIndex, ulVrid);
    if ( NULL == pVsrv )
    {
        *piErrNo = VRRP_CFGERR_VSRVNOTEXIST;
        return VOS_ERR_FAIL;
    }

    for ( i = 0; i < pVsrv->niftrack; i++ )
    {
        if ( ulIfTracIndex == pVsrv->iftrack[i] )
        {
            pVsrv->pritrack[i] = ulPrio;
            return VOS_ERR_NONE;
        }
    }

    if ( pVsrv->niftrack >= VRRP_IF_TRACK_MAX )
    {
        *piErrNo = VRRP_CFGERR_MAXTRACK;
        return VOS_ERR_FAIL;
    }

#ifdef GLN_INCLUDE_TRACK
    if ( OK != trackEntryAppReg(ulIfTracIndex, "vrrp", VRRP_PROCESS_NAME, 
        VRRP_FUNC_TRACK_STATE_CHG) )
    {
        *piErrNo = VRRP_CFGERR_TRACK;
        return VOS_ERR_FAIL;
    }
#endif

    pVsrv->iftrack[pVsrv->niftrack] = ulIfTracIndex;
    pVsrv->pritrack[pVsrv->niftrack] = ulPrio;
#ifdef GLN_INCLUDE_TRACK
    pVsrv->trackState[pVsrv->niftrack] = TRACK_STATE_INVALID;
#endif
    pVsrv->niftrack++;

    return VOS_ERR_NONE;    
}

int vrrpVsrvIfTrackDel(ulong_t ulIfIndex, ulong_t ulVrid, 
    ulong_t ulIfTracIndex, int *piErrNo)
{
    unsigned int             i, uiIndex;
    vrrp_rt                 *pVsrv;

    *piErrNo = 0;
    
    pVsrv = vrrp_vsrv_find_by_vrid(ulIfIndex, ulVrid);
    if ( NULL == pVsrv )
    {
        *piErrNo = VRRP_CFGERR_VSRVNOTEXIST;
        return VOS_ERR_FAIL;
    }

    if ( ulIfTracIndex )
    {
        uiIndex = 0;
        for ( i = 0; i < pVsrv->niftrack; i++ )
        {
            if ( ulIfTracIndex == pVsrv->iftrack[i] )
            {
                uiIndex = i;
                break;
            }
        }

        if ( 0 == uiIndex )
        {
            return VOS_ERR_NONE;
        }

    #ifdef GLN_INCLUDE_TRACK
        trackEntryAppDeReg(ulIfTracIndex, "vrrp");
    #endif

        for (i = uiIndex; i < pVsrv->niftrack - 1; i++)
        {
            pVsrv->iftrack[i] = pVsrv->iftrack[i+1];
            pVsrv->pritrack[i] = pVsrv->pritrack[i + 1];
        #ifdef GLN_INCLUDE_TRACK
            pVsrv->trackState[i] = pVsrv->trackState[i + 1];
        #endif
        }
        pVsrv->niftrack--;
    }
    else
    {
    #ifdef GLN_INCLUDE_TRACK
        for ( i = 0; i < pVsrv->niftrack; i++ )
        {
            trackEntryAppDeReg(pVsrv->iftrack[i], "vrrp");
        }
    #endif
    
        pVsrv->niftrack = 0;
    }

    return VOS_ERR_NONE;    
}

#ifdef GLN_INCLUDE_TRACK
int vrrpTrackStateChg(ulong_t ulTrackIndex, int iLastState, int iNewState)
{
    int                      i, j;
    vrrp_rt                 *pVsrv;
    
    for ( i = 0; i < VRRP_VSRV_SIZE_MAX; i++ )
    {
        pVsrv = &gVrrp_vsrv[i];

        if ( !pVsrv || !pVsrv->used )
        {
            continue;
        }

        for ( j = 0; j < pVsrv->niftrack; j++ )
        {
            if ( ulTrackIndex == pVsrv->iftrack[j] )
            {
                pVsrv->trackState[j] = iNewState;
            }
        }
    }

    return VOS_ERR_NONE;
}
#endif


