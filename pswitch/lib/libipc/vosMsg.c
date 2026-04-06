
#include <sys/types.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <linux/netlink.h>

#include "vos.h"
#include <libipc/vosMsg.h>
#include <libipc/vosPrint.h>
#include <libipc/vosSystem.h>
#include <common/sys_error.h>
extern ulong_t g_ulVosDebugFlags;

int vosMsgSendStream(int iSocketFd, char *pStream, ulong_t ulSize, 
    void *peerAddr, socklen_t iSocketLen);
int vosMsgRecvStream(int iSocketFd, char *pStream, ulong_t ulSize, 
    void *peerAddr, socklen_t *pSockLen);

#define VOS_FD_INVALID -1

int g_vosFdProc = VOS_FD_INVALID;     
#ifdef VOS_MSG_PRIORITY_SUPPORT
int g_vosFdProcHigh = VOS_FD_INVALID; 
#endif

#ifdef VOS_NETLINK_INCLUDE
int g_vosFdNLUni = VOS_FD_INVALID;    

#ifdef VOS_NETLINK_MULTI_INCLUDE
int g_vosFdNLMulti = VOS_FD_INVALID;  
#endif

#endif

#define VOS_MSG_BUFFER_LEN  2148
#define VOS_MSG_SEND_MAX_NUM 128
#define VOS_MSG_RECV_MAX_NUM 1024

char g_vosMsgBufferRecv[VOS_MSG_BUFFER_LEN]; 
char g_vosMsgBufferSend[VOS_MSG_BUFFER_LEN]; 

vosMsgBldrunProcess g_vosBldrunProc = NULL;

vosMsgPortLinkProcess g_vosApLinktrapProc = NULL;
vosMsgPortLinkProcess g_vosAgLinktrapProc = NULL;

vosMsgVlanintfLinkProcess g_vosVlanintfLinktrapProc = NULL;
vosMsgCcdProcess g_vosCcdProc = NULL;

vosMsgCcdObjInfoGetProcess g_vosCcdObjInfoGetProc = NULL;

typedef struct VOS_TIMER_tag{
    struct VOS_TIMER_tag *pNext;
    ulong_t             ulMagic;        
    VOS_TIMER_TYPE      type;          
    vosTimerProcess     func;            
    ulong_t             ulTime;          
    void               *pArg;           
    struct timeval     start;           
    ulong_t                bStart;         
}VOS_TIMER;

VOS_TIMER *g_pVosMsgTimerFirst = NULL; 
ulong_t g_ulVosMsgMagic = 0; 

#define VOS_TIME_INVALID VOS_TIMEOUT_FOREVER
#define VOS_LOCAL_FILE_MAX 9
#define VOS_LOCAL_FUILE_MINI 1

static ulong_t g_vosulClientSocketNum = VOS_LOCAL_FUILE_MINI;

ulong_t g_vosUserSocketNum = 0;

VOS_SOCK_USER *g_pUserSocketList = NULL;

VosToSysErr_t vosToSysErr[] = 
{

	{ VOS_OSPF_AREA_NETWORK_EXSIT,         SYS_ERR_OSPF_AREA_NETWORK_EXIST },
	{ VOS_OSPF_IS_CLOSE,                   SYS_ERR_OSPF_IS_CLOSE           },
	{ VOS_RIP_IS_CLOSE,                    SYS_ERR_RIP_IS_CLOSE            },

	{ VOS_ERR_MAX,                         SYS_ERR_END },
};

int vos_to_sys_errId_ret(int vosErr)
{
    VosToSysErr_t *ptr = NULL;

    if( -1 > vosErr && vosErr > VOS_ERR_MAX)
    	return -1;

    for (ptr = vosToSysErr;  ptr->vos_err < VOS_ERR_MAX ; ptr++)
    {
        if (ptr->vos_err == vosErr)
            return (ptr->sys_err);
    }

    return vosErr;
}

static int vosMsgTimerAdd(VOS_TIMER *pTimer)
{
    VOS_TIMER *pTimerTmp = NULL;
    
    if ( NULL == g_pVosMsgTimerFirst )
    {
        g_pVosMsgTimerFirst = pTimer;
    }
    else
    {
        pTimerTmp = g_pVosMsgTimerFirst;
        
        while ( NULL != pTimerTmp->pNext )
        {
            pTimerTmp = pTimerTmp->pNext;
        }

        pTimerTmp->pNext = pTimer;
    }

    pTimer->pNext = NULL;

    return VOS_ERR_NONE;
}

static int vosMsgTimerDelete(VOS_TIMER *pTimer)
{
    VOS_TIMER *pTimerTmp = NULL, *pTimerPre;
    
    pTimerTmp = g_pVosMsgTimerFirst;
    pTimerPre = NULL;
    
    while ( NULL != pTimerTmp )
    {
        if ( pTimer == pTimerTmp )
        {
            if ( pTimer->ulMagic != pTimerTmp->ulMagic )
            {
                return VOS_ERR_FAIL;
            }
            
            if ( NULL == pTimerPre )
            {
                g_pVosMsgTimerFirst = pTimer->pNext;
            }
            else
            {
                pTimerPre->pNext = pTimerTmp->pNext;
            }

            return VOS_ERR_NONE;
        }

        pTimerPre = pTimerTmp;
        pTimerTmp = pTimerTmp->pNext;
    }

    return VOS_ERR_FAIL;
}

static ulong_t vosMsgTimerCheck(VOS_TIMER *pTimer)
{
    VOS_TIMER               *pTimerTmp;

    pTimerTmp = g_pVosMsgTimerFirst;
    while ( NULL != pTimerTmp )
    {
        if ( pTimer == pTimerTmp )
        {
            if ( pTimer->ulMagic != pTimerTmp->ulMagic )
            {
                return FALSE;
            }
        
            return TRUE;
        }

        pTimerTmp = pTimerTmp->pNext;
    }

    return FALSE;
}

static int vosMsgTimerProcess(VOS_TIMER *pTimer, ulong_t *pulTimeLess)
{
    int iRet;
    ulong_t ulTimeCal;
    struct timeval tv;

    iRet = gettimeofday(&tv, NULL);
    if ( -1 == iRet )
    {
        
        return VOS_ERR_FAIL;
    }

    vosTvsub(&tv, &pTimer->start);
    ulTimeCal = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    if ( ulTimeCal >= pTimer->ulTime )
    {
        if ( NULL != pTimer->func )
        {
            
            pTimer->func(ulTimeCal - pTimer->ulTime, pTimer->pArg);
        }

        if ( VOS_TIMER_TYPE_ONE == pTimer->type )
        {
            iRet = vosMsgTimerDelete(pTimer);
            if ( VOS_ERR_NONE == iRet )
            {
                free(pTimer);
            }

            *pulTimeLess = VOS_TIME_INVALID;
        }
        else
        {
            gettimeofday(&pTimer->start, NULL);
            *pulTimeLess = pTimer->ulTime;
        }
    }
    else
    {
        *pulTimeLess = (pTimer->ulTime - ulTimeCal);
    }

    return VOS_ERR_NONE;
}

static VOS_TIMER *vosMsgTimerGetNext(VOS_TIMER *pTimer)
{
    return pTimer->pNext;
}

static int vosMsgProcessInternel(int iFd, VOS_MSG *pMsg, 
    struct sockaddr *sock, socklen_t iSockLen)
{
    int iRet= VOS_ERR_NONE, iBufLen, iSendLen;
    VOS_PORT_TYPE portType;
    VOS_LINK_STATUS linkStatus;
    ulong_t bSuperVlan;
    ulong_t ulPort, ulID;
    char *pBuf = NULL;
    if ( NULL == pMsg )
    {
        
        return VOS_ERR_PARAM;
    }

    switch ( pMsg->ulType )
    {
        case VOS_MSG_TYPE_BLDRUN:
            if ( NULL != g_vosBldrunProc )
            {
                iBufLen = (pMsg->ulBufferLen + sizeof(VOS_MSG));
                pBuf = malloc(iBufLen);
                if ( NULL == pBuf )
                {
                    
                    iRet = VOS_ERR_NOMEMORY;
                }
                else
                {
                    iBufLen = pMsg->ulBufferLen;
                    
                    iRet = g_vosBldrunProc((pBuf + sizeof(VOS_MSG)), &iBufLen, pMsg->ulValue1, pMsg->ulValue2);
                    if ( VOS_ERR_NONE == iRet )
                    {
                        pMsg->ulRet = (ulong_t)iRet;
                        pMsg->ulValue3 = iBufLen;
                        pMsg->ulBufferLen -= iBufLen;
                        memcpy(pBuf, pMsg, sizeof(VOS_MSG));
                    }
                }
            }
            else
            {
                
                iRet = VOS_ERR_NOTSUPPORT;
            }

            if ( VOS_ERR_NONE != iRet )
            {
                pMsg->ulRet = (ulong_t)iRet;
                iSendLen = sizeof(VOS_MSG);
                iRet = vosMsgSendStream(iFd, (char *)pMsg, iSendLen, (void *)sock, iSockLen);
            }
            else
            {
                iSendLen = (pMsg->ulBufferLen + 1 + sizeof(VOS_MSG));
                iRet = vosMsgSendStream(iFd, (char *)pBuf, iSendLen, (void *)sock, iSockLen);
            }

            if ( iRet < 0 )
            {
                
                iRet = VOS_ERR_FAIL;
            }
            else
            {
                iRet = VOS_ERR_NONE;
            }

            if ( NULL != pBuf )
            {
                free(pBuf);
                pBuf = NULL;
            }
            break;

        case VOS_MSG_TYPE_PACKET:
        #if 0
            if ( NULL != g_vosPktProc )
            {
            #define IP_ALLIGN 2

                iBufLen = pMsg->ulValue1;

                pBuf = (char *)(pMsg + 1);
                pBuf += IP_ALLIGN;
                pPkt = pBuf;

                pBuf += iBufLen;
            }
          #endif
            break;

        case VOS_MSG_TYPE_LINK:
            portType = pMsg->ulValue1;

            if ( (VOS_PORT_TYPE_AP == portType) && (NULL != g_vosApLinktrapProc) )
            {
                ulPort = pMsg->ulValue2;
                linkStatus = pMsg->ulValue3;
                
                g_vosApLinktrapProc(ulPort, linkStatus);
            }
            else if ( (VOS_PORT_TYPE_AG == portType) && (NULL != g_vosAgLinktrapProc) )
            {
                ulPort = pMsg->ulValue2;
                linkStatus = pMsg->ulValue3;
                
                g_vosAgLinktrapProc(ulPort, linkStatus);
            }
            else if ( (VOS_PORT_TYPE_IP == portType) && (NULL != g_vosVlanintfLinktrapProc) )
            {
                bSuperVlan = pMsg->ulValue2;
                ulID = pMsg->ulValue3;
                linkStatus = pMsg->ulValue4;
                
                g_vosVlanintfLinktrapProc(bSuperVlan, ulID, linkStatus);
            }
            break;

        case VOS_MSG_VOS_DEBUG:
            break;

        case VOS_MSG_SMD_DEBUG:
            break;

        case VOS_MSG_TYPE_CCD:
            
#if 0
            if ( NULL != g_vosCcdProc )
            {
                STATUS ccdRet;
                int iTmpLen, iRecvLen, iObjLen;
                ulong_t request;
                char *pObjName, *pValueBuf;
                ulong_t *pIndex, indexSubIdCount, *pValueLen;
                ulong_t index[MAX_SUBID_INDEX_COUNT];
                ushort_t flags;
                VOS_MSG *pMsgSend;

                request = pMsg->ulValue1;
                indexSubIdCount = pMsg->ulValue2;
                flags = pMsg->ulValue3;

                iRecvLen = sizeof(VOS_MSG);
                pBuf = (char *)pMsg;
                pIndex = (ulong_t *)(pBuf + iRecvLen);
                iTmpLen = (indexSubIdCount * sizeof(ulong_t));
                memcpy(index, pIndex, iTmpLen);
                iRecvLen += iTmpLen;

                memcpy(&iTmpLen, pBuf + iRecvLen, 4);
                pObjName = (pBuf + iRecvLen + 4);
                iRecvLen += (iTmpLen + 4);
                iObjLen = iTmpLen;
                pValueLen = (ulong_t *)(pBuf + iRecvLen);
                pValueBuf = (pBuf + iRecvLen + 4);
                iRecvLen += 4;

                assert(iRecvLen < VOS_MSG_BUFFER_LEN);
                iTmpLen = (VOS_MSG_BUFFER_LEN - iRecvLen);

                ccdRet = g_vosCcdProc(request, pObjName, index, &indexSubIdCount, pValueBuf, pValueLen, &flags);
                if ( OK == ccdRet )
                {
                    assert(*pValueLen <= (ulong_t)iTmpLen);
                }

                pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;
                memcpy(pMsgSend, pMsg, sizeof(VOS_MSG));
                pMsgSend->ulRet = ccdRet;
                pMsgSend->ulValue2 = indexSubIdCount;
                pMsgSend->ulValue3 = flags;
                pMsgSend->ulValue4 = errnoGet();

                iRecvLen = sizeof(VOS_MSG);
                pBuf = (char *)pMsgSend;
                iTmpLen = (indexSubIdCount * sizeof(ulong_t));
                memcpy(pBuf + iRecvLen, index, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &iObjLen, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = iObjLen;
                memcpy(pBuf + iRecvLen, pObjName, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, pValueLen, iTmpLen);
                iRecvLen += iTmpLen;

                if ( OK == ccdRet )
                {
                    iTmpLen = *pValueLen;
                    memcpy(pBuf + iRecvLen, pValueBuf, iTmpLen);
                    iRecvLen += iTmpLen;
                }

                iRet = vosMsgSendStream(iFd, pBuf, iRecvLen, (void *)sock, iSockLen);
            }
 #endif           
            break;
        case VOS_MSG_TYPE_CCD_OBJ_INFO_GET:
            
#if 0
            if ( NULL != g_vosCcdObjInfoGetProc )
            {
                STATUS       ccdRet;
                int          iTmpLen, iRecvLen, iObjLen;
                ulong_t      ulValueMask = 0;
                ulong_t      ulLocalId = 0;               
                ulong_t      ulLastOidPrefixSubId = 0;    
                ulong_t      ulSize = 0;                  
                ulong_t      ulRangeMin = 0;              
                ulong_t      ulRangeMax = 0;              
                int          nType = 0;                   
                ulong_t      OidPrefixWoLastSubId[32] = {0};
                ulong_t      ulOidPrefixLen = 0;          
                ulong_t      ulIndexObjCount = 0;         
                ulong_t      ulIndexSubIdCount = 0;       
                ulong_t      ulIndexSubIdCountMax = 0;    
                char         *pObjName = NULL;
                VOS_MSG      *pMsgSend;

                iRecvLen = sizeof(VOS_MSG);
                ulValueMask =  pMsg->ulValue1;
                pBuf = (char *)pMsg;

                iObjLen = pMsg->ulValue2;
                pObjName = (pBuf + iRecvLen);
                iRecvLen += iObjLen;

                assert(iRecvLen < VOS_MSG_BUFFER_LEN);

                ccdRet = g_vosCcdObjInfoGetProc(pObjName, ulValueMask, 
                                &ulLocalId, &ulLastOidPrefixSubId, &ulSize, &ulRangeMin,
                                &ulRangeMax, &nType, OidPrefixWoLastSubId, &ulOidPrefixLen, 
                                &ulIndexObjCount, &ulIndexSubIdCount, &ulIndexSubIdCountMax);

                pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

                memcpy(pMsgSend, pMsg, sizeof(VOS_MSG));
                pMsgSend->ulRet = ccdRet;
                pMsgSend->ulValue1 = ulValueMask;
                pMsgSend->ulValue2 = ulLocalId;
                pMsgSend->ulValue3 = ulLastOidPrefixSubId;
                pMsgSend->ulValue4 = errnoGet();

                iRecvLen = sizeof(VOS_MSG);
                pBuf = (char *)pMsgSend;
                
                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &iObjLen, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = iObjLen;
                memcpy(pBuf + iRecvLen, pObjName, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulSize, iTmpLen);
                iRecvLen += iTmpLen;                

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulRangeMin, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulRangeMax, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &nType, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulOidPrefixLen, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4 * 32;
                memcpy(pBuf + iRecvLen, OidPrefixWoLastSubId, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulIndexObjCount, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulIndexSubIdCount, iTmpLen);
                iRecvLen += iTmpLen;

                iTmpLen = 4;
                memcpy(pBuf + iRecvLen, &ulIndexSubIdCountMax, iTmpLen);
                iRecvLen += iTmpLen;

                iRet = vosMsgSendStream(iFd, (uchar_t *)pBuf, iRecvLen, (void *)sock, iSockLen);
            }
#endif
            break;
            
        default:
            
            return VOS_ERR_FAIL;
    }

    return iRet;
}

static int vosMsgNLChangePath(char *name, char *newname)
{
#if 0
    char *pBaseName;

    pBaseName = strrchr(name, '/');
    if ( NULL == pBaseName )
    {
        pBaseName = name;
    }
    else
    {
        pBaseName += 1;
    }
#endif    
    sprintf(newname, VOS_MSG_SOCKET_DIR"/%s%s", name, "server");

    return VOS_ERR_NONE;
}

int vosMsgclientChangePath(char *name, char *newname)
{
#if 0
    char *pBaseName;

    pBaseName = strrchr(name, '/');
    if ( NULL == pBaseName )
    {
        pBaseName = name;
    }
    else
    {
        pBaseName += 1;
    }
#endif    
    sprintf(newname, VOS_MSG_SOCKET_DIR"/%s%s", name, "client");

    return VOS_ERR_NONE;
}

static int vosMsgSocketOptmize(int iSocketFd, ulong_t bCheck)
{
    int                      iRet, iBufLenRet;
    socklen_t                       iOptLen, iSocketBuffLen;
    
    iSocketBuffLen = (VOS_MSG_BUFFER_LEN * VOS_MSG_SEND_MAX_NUM);
    iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_SNDBUF,
        (char *)&iSocketBuffLen, sizeof(iSocketBuffLen));
    if ( iRet < 0 )
    {
        
    }

    if ( TRUE == bCheck )
    {
        iOptLen = sizeof(iBufLenRet);
        iRet = getsockopt(iSocketFd, SOL_SOCKET, SO_SNDBUF,
            (char *)&iBufLenRet, &iOptLen);
        if ( iRet < 0 )
        {
            
        }
        else if ( iBufLenRet != (iSocketBuffLen * 2) )
        {
        }
    }

    iSocketBuffLen = (VOS_MSG_BUFFER_LEN * VOS_MSG_RECV_MAX_NUM);
    iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_RCVBUF,
        (char *)&iSocketBuffLen, sizeof(iSocketBuffLen));
    if ( iRet < 0 )
    {
        
    }

    if ( TRUE == bCheck )
    {
        iOptLen = sizeof(iBufLenRet);
        iRet = getsockopt(iSocketFd, SOL_SOCKET, SO_RCVBUF,
            (char *)&iBufLenRet, &iOptLen);
        if ( iRet < 0 )
        {
            
        }
        else if ( iBufLenRet != (iSocketBuffLen * 2) )
        {
        }
    }

    return VOS_ERR_NONE;
}

static int vosMsgGetLocalFile(char *szLocalFile)
{
    int pid;
    
    pid = getpid();
    sprintf(szLocalFile, VOS_MSG_SOCKET_DIR"/client%dv%d", pid, g_vosulClientSocketNum++);
    if ( g_vosulClientSocketNum >= VOS_LOCAL_FILE_MAX )
    {
        g_vosulClientSocketNum = VOS_LOCAL_FUILE_MINI;
    }

    return VOS_ERR_NONE;
}

#ifdef VOS_NETLINK_INCLUDE

static int vosMsgNetlinkSocketOptmize(int iSocketFd, BOOL bCheck)
{
#define MAX_PKT_SIZE 1522
    int                      iRet,  iBufLenRet;
    socklen_t                      iOptLen, iSocketBuffLen;
    
    iSocketBuffLen = (MAX_PKT_SIZE * VOS_MSG_SEND_MAX_NUM);
    iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_SNDBUF,
        (char *)&iSocketBuffLen, sizeof(iSocketBuffLen));
    if ( iRet < 0 )
    {
        
    }

    if ( TRUE == bCheck )
    {
        iOptLen = sizeof(iBufLenRet);
        iRet = getsockopt(iSocketFd, SOL_SOCKET, SO_SNDBUF,
            (char *)&iBufLenRet, &iOptLen);
        if ( iRet < 0 )
        {
            
        }
        else if ( iBufLenRet != (iSocketBuffLen * 2) )
        {
        }
    }

    iSocketBuffLen = (MAX_PKT_SIZE * VOS_MSG_RECV_MAX_NUM);
    iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_RCVBUF,
        (char *)&iSocketBuffLen, sizeof(iSocketBuffLen));
    if ( iRet < 0 )
    {
        
    }

    if ( TRUE == bCheck )
    {
        iOptLen = sizeof(iBufLenRet);
        iRet = getsockopt(iSocketFd, SOL_SOCKET, SO_RCVBUF,
            (char *)&iBufLenRet, &iOptLen);
        if ( iRet < 0 )
        {
            
        }
        else if ( iBufLenRet != (iSocketBuffLen * 2) )
        {
        }
    }

    return VOS_ERR_NONE;
}

static int vosMsgNetlinkUniProcess(int iSockFd)
{
    int                       iRet;
    socklen_t                 iSocklen;
    VOS_MSG                  *pMsgRecv;
    struct nlmsghdr         *pNetlinkHdr;
    struct sockaddr_nl       clientAddr;
    iSocklen = sizeof(struct sockaddr_nl);
    
    iRet = vosMsgRecvStream(iSockFd, g_vosMsgBufferRecv, VOS_MSG_BUFFER_LEN, 
                    (void *)&clientAddr, &iSocklen);
    if ( iRet < 0 )
    {
        
        return VOS_ERR_FAIL;
    }
    else
    {
        
        pMsgRecv = (VOS_MSG *)(NLMSG_DATA(g_vosMsgBufferRecv));
        switch ( pMsgRecv->ulType )
        {
            case VOS_MSG_TYPE_PACKET:
            case VOS_MSG_TYPE_LINK:
                iRet = vosMsgProcessInternel(iSockFd, pMsgRecv, (struct sockaddr *)&clientAddr, iSocklen);
                break;

            default:
                
                    __func__, pMsgRecv->ulType, iRet, iRet - NLMSG_LENGTH(0));

                pNetlinkHdr = (struct nlmsghdr *)g_vosMsgBufferRecv;
                
                    __func__, pNetlinkHdr->nlmsg_len, pNetlinkHdr->nlmsg_flags, 
                    pNetlinkHdr->nlmsg_type, pNetlinkHdr->nlmsg_pid);
                    __func__, pMsgRecv->ulType, pMsgRecv->ulBufferLen, pMsgRecv->ulValue1);

                iRet = VOS_ERR_FAIL;
                break;
        }
    }
    return iRet;
}

#ifdef VOS_NETLINK_MULTI_INCLUDE

static int vosMsgNetlinkMultiProcess(int iSockFd)
{
    int                       iRet;
    socklen_t                 iSocklen;
    VOS_MSG                  *pMsgRecv;
    struct nlmsghdr         *pNetlinkHdr;
    struct sockaddr_nl       clientAddr;
    iSocklen = sizeof(struct sockaddr_nl);
    
    iRet = vosMsgRecvStream(iSockFd, g_vosMsgBufferRecv, VOS_MSG_BUFFER_LEN, 
                    (void *)&clientAddr, &iSocklen);
    if ( iRet < 0 )
    {
        
        return VOS_ERR_FAIL;
    }
    else
    {
        
        pMsgRecv = (VOS_MSG *)(NLMSG_DATA(g_vosMsgBufferRecv));

        switch ( pMsgRecv->ulType )
        {
            case VOS_MSG_TYPE_PACKET:
            case VOS_MSG_TYPE_LINK:
                iRet = vosMsgProcessInternel(iSockFd, pMsgRecv, (struct sockaddr *)&clientAddr, iSocklen);
                break;

            default:
                pNetlinkHdr = (struct nlmsghdr *)g_vosMsgBufferRecv;
                iRet = VOS_ERR_FAIL;
                break;
        }
    }
    return iRet;
}
#endif
#endif

int vosMsgCheck(void)
{
    vosAssert(0 == ((ulong_t)g_vosMsgBufferRecv)%sizeof(long));
    vosAssert(0 == ((ulong_t)g_vosMsgBufferSend)%sizeof(long));
    vosAssert(0 == sizeof(VOS_MSG)%sizeof(long));
    
    return VOS_ERR_NONE;
}

int vosMsgInit(char *name, VOS_MSG_SOURCE msgSrc, int *iHandle)
{
    int iRet, iSocketFd;
#ifdef VOS_MSG_PRIORITY_SUPPORT
    int iSocketFdHigh;
#endif
    int iSocketLen;
    struct sockaddr_un local;
#ifdef VOS_NETLINK_INCLUDE  
    struct sockaddr_nl netlinkUi;
#endif
    char szLocalFile[VOS_LOCAL_FILE_MAX_LEN];
    if ( (NULL == name) || (NULL ==iHandle) )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( VOS_MSG_SOURCE_PROCESS & msgSrc )
    {
        if ( VOS_FD_INVALID != g_vosFdProc )
        {
            return VOS_ERR_EXISTS;
        }
    #ifdef VOS_MSG_PRIORITY_SUPPORT
        if ( VOS_FD_INVALID != g_vosFdProcHigh )
        {
            
            return VOS_ERR_EXISTS;
        }
    #endif

        iSocketFd = vosMsgLocalSocket(VOS_TIMEOUT_DEFAULT);
        
        if ( iSocketFd < 0 )
        {
            return VOS_ERR_FAIL;
        }
    #ifdef VOS_MSG_PRIORITY_SUPPORT
        iSocketFdHigh = vosMsgLocalSocket(VOS_TIMEOUT_DEFAULT);
        if ( iSocketFdHigh < 0 )
        {
            
            return VOS_ERR_FAIL;
        }
    #endif

        vosMsgNLChangePath(name, szLocalFile);  
        
        vosMsgLocalAddrInit(szLocalFile, &local, (void *)&iSocketLen);
        unlink(szLocalFile);
        iRet = bind(iSocketFd, (struct sockaddr*)&local, iSocketLen);
        if ( iRet < 0 )
        {
            close(iSocketFd);
            return VOS_ERR_FAIL;
        }
        vosMsgSocketOptmize(iSocketFd, TRUE);
        *iHandle = iSocketFd;
    #ifdef VOS_MSG_PRIORITY_SUPPORT
        strcat(szLocalFile,VOS_MSG_HIGH_SUFFIX);
        vosMsgLocalAddrInit(szLocalFile, (void *)&local, &iSocketLen);

        unlink(szLocalFile);
        iRet = bind(iSocketFdHigh, (struct sockaddr*)&local, iSocketLen);
        if ( iRet < 0 )
        {
            close(iSocketFdHigh);
            return VOS_ERR_FAIL;
        }

        vosMsgSocketOptmize(iSocketFdHigh, TRUE);

        g_vosFdProcHigh = iSocketFdHigh;
        
    #endif
    }

#ifdef VOS_NETLINK_INCLUDE
    if ( VOS_MSG_SOURCE_KERNEL_UNICAST & msgSrc )
    {
        
        if ( VOS_FD_INVALID != g_vosFdNLUni )
        {
            
            return VOS_ERR_EXISTS;
        }

        iSocketFd = socket(PF_NETLINK, SOCK_RAW, VOS_NL_UI_IMP);
        if ( iSocketFd < 0 )
        {
            
            return VOS_ERR_FAIL;
        }
        memset(&netlinkUi, 0, sizeof(netlinkUi));
        netlinkUi.nl_family = AF_NETLINK;
        netlinkUi.nl_pid = getpid();
        netlinkUi.nl_groups = 0;

        iRet = bind(iSocketFd, (struct sockaddr*)&netlinkUi, sizeof(netlinkUi));
        if ( iRet < 0 )
        {
            close(iSocketFd);
            return VOS_ERR_FAIL;
        }

        vosMsgNetlinkSocketOptmize(iSocketFd, TRUE);

        g_vosFdNLUni = iSocketFd;
        
    }

    #ifdef VOS_NETLINK_MULTI_INCLUDE
    if ( VOS_MSG_SOURCE_KERNEL_MULTICAST & msgSrc )
    {
        
        if ( VOS_FD_INVALID != g_vosFdNLMulti )
        {
            
            return VOS_ERR_EXISTS;
        }

        iSocketFd = socket(PF_NETLINK, SOCK_RAW, VOS_NL_MU_IMP);
        if ( iSocketFd < 0 )
        {
            
            return VOS_ERR_FAIL;
        }
        memset(&netlinkUi, 0, sizeof(netlinkUi));
        netlinkUi.nl_family = AF_NETLINK;
        netlinkUi.nl_pid = getpid();
        netlinkUi.nl_groups = VOS_ML_GROUP;

        iRet = bind(iSocketFd, (struct sockaddr*)&netlinkUi, sizeof(netlinkUi));
        if ( iRet < 0 )
        {
            close(iSocketFd);
            return VOS_ERR_FAIL;
        }

        vosMsgNetlinkSocketOptmize(iSocketFd, TRUE);

        g_vosFdNLMulti = iSocketFd;
        
    }
    #endif    
#endif
    return VOS_ERR_NONE;
}

int vosMsgRecv(int iHandle, void *pMsg, ulong_t ulMsgSize, 
    ulong_t ulTimeOut, VOS_MSG_ADDR *pClientAddr)
{
    int iMaxsock, iRet, iRecvSize;
    socklen_t iSocklen;
    ulong_t ulTimeOutSelect;
    ulong_t ulTimeLess = 0, ulTimeLessMini = 0;
    ulong_t bNeedTimer;
    VOS_MSG *pMsgRecv;
    VOS_TIMER *pTimer, *pTimerNext;
    VOS_SOCK_USER *pUserSocket, *pSocketPre;
    fd_set fdSelect;
    struct timeval tv, *pTv;
    struct sockaddr_un clientaddr;
#ifdef VOS_MSG_PRIORITY_SUPPORT
    fd_set fdSelectHigh;
    struct timeval tvHigh;
#endif
    if ( NULL == pClientAddr )
    {
        
        printf("\r %s %d fail \n", __func__, __LINE__);
        return VOS_ERR_PARAM;
    }

    if ( VOS_CLIENT_ADDR_LEN < sizeof(struct sockaddr_un) )
    {
        
        printf("\r %s %d fail \n", __func__, __LINE__);
        return VOS_ERR_SYSTEM;
    }
#if 0
    if ( VOS_FD_INVALID == g_vosFdProc )
    {
        
        return VOS_ERR_INIT;
    }
#endif
#ifdef VOS_MSG_PRIORITY_SUPPORT    
    if ( VOS_FD_INVALID == g_vosFdProcHigh )
    {
        
        return VOS_ERR_INIT;
    }
#endif

    if ( ulMsgSize > (VOS_MSG_BUFFER_LEN - sizeof(VOS_MSG)) )
    {
        
        printf("\r %s %d fail\n", __func__, __LINE__);
        return VOS_ERR_PARAM;
    }

    while ( 1 )
    {
        
        ulTimeLessMini = VOS_TIME_INVALID;
        pTimer = g_pVosMsgTimerFirst;
        
        while ( NULL != pTimer )
        {
            pTimerNext = vosMsgTimerGetNext(pTimer);

            if ( TRUE == pTimer->bStart )
            {
                vosMsgTimerProcess(pTimer, &ulTimeLess);
                if ( ulTimeLess < ulTimeLessMini )
                {
                    ulTimeLessMini = ulTimeLess;
                }
            }

            pTimer = pTimerNext;
        }

        ulTimeOutSelect = ulTimeOut;        
        bNeedTimer = FALSE;
        
        if ( (VOS_TIME_INVALID != ulTimeLessMini) && (ulTimeLessMini < ulTimeOut) )
        {
            ulTimeOutSelect = ulTimeLessMini;
            bNeedTimer = TRUE;
        }

        FD_ZERO(&fdSelect);
        
        FD_SET(iHandle, &fdSelect);
        iMaxsock = iHandle;

    #ifdef VOS_MSG_PRIORITY_SUPPORT
        FD_SET(g_vosFdProcHigh, &fdSelect);
        if ( iMaxsock < g_vosFdProcHigh )
        {
            iMaxsock = g_vosFdProcHigh;
        }
    #endif
        
    #ifdef VOS_NETLINK_INCLUDE
        if ( VOS_FD_INVALID != g_vosFdNLUni )
        {
            FD_SET(g_vosFdNLUni, &fdSelect);
            if ( iMaxsock < g_vosFdNLUni )
            {
                iMaxsock = g_vosFdNLUni;
            }
        }

        #ifdef VOS_NETLINK_MULTI_INCLUDE
        if ( VOS_FD_INVALID != g_vosFdNLMulti )
        {
            FD_SET(g_vosFdNLMulti, &fdSelect);
            if ( iMaxsock < g_vosFdNLMulti )
            {
                iMaxsock = g_vosFdNLMulti;
            }
        }
        #endif
    #endif

        pUserSocket = g_pUserSocketList;
    
        while ( NULL != pUserSocket )
        {
            FD_SET(pUserSocket->iSocketFd, &fdSelect);
            if ( iMaxsock < pUserSocket->iSocketFd )
            {
                iMaxsock = pUserSocket->iSocketFd;
            }

            pUserSocket = pUserSocket->pNext;
        }

        if ( VOS_TIMEOUT_FOREVER == ulTimeOutSelect )
        {
            pTv = NULL;
        }
        else
        {
            pTv = &tv;
            
            tv.tv_sec = (ulTimeOutSelect/1000);
            tv.tv_usec = (ulTimeOutSelect%1000)*1000;
        }

        iRet = select(iMaxsock + 1, &fdSelect, NULL, NULL, pTv);
        if ( iRet < 0 ) 
        {
            printf("\r %s %d fail\n", __func__, __LINE__);
            return VOS_ERR_FAIL;
        }
        else if ( 0 == iRet ) 
        {
            
            if ( TRUE == bNeedTimer )
            {
                continue;
            }
            else
            {
                return VOS_ERR_TIMEOUT;
            }
        }

    #ifdef VOS_NETLINK_INCLUDE
        if ( (VOS_FD_INVALID != g_vosFdNLUni) && (FD_ISSET(g_vosFdNLUni, &fdSelect)) )
        {
            vosMsgNetlinkUniProcess(g_vosFdNLUni);
        }

        #ifdef VOS_NETLINK_MULTI_INCLUDE
        if ( (VOS_FD_INVALID != g_vosFdNLMulti) && (FD_ISSET(g_vosFdNLMulti, &fdSelect)) )
        {
            vosMsgNetlinkMultiProcess(g_vosFdNLMulti);
        }
        #endif
    #endif

    #ifdef VOS_MSG_PRIORITY_SUPPORT
        
        fdSelectHigh = fdSelect;
        
        while ( 1 )
        {
            if ( !FD_ISSET(g_vosFdProcHigh, &fdSelectHigh) )
            {
                break;
            }

            iSocklen = sizeof(struct sockaddr_un);
            
            iRet = vosMsgRecvStream(g_vosFdProcHigh, g_vosMsgBufferRecv, VOS_MSG_BUFFER_LEN, 
                            (void *)&clientaddr, &iSocklen);
            if ( iRet < sizeof(VOS_MSG) )
            {
                
                return VOS_ERR_FAIL;
            }
            else
            {
                
                pMsgRecv = (VOS_MSG *)g_vosMsgBufferRecv;
                if ( VOS_MSG_TYPE_SELF == pMsgRecv->ulType )
                {
                    iRecvSize = iRet - sizeof(VOS_MSG);
                    iRecvSize = ((ulong_t)iRecvSize < ulMsgSize)?(ulong_t)iRecvSize:ulMsgSize;
                    memcpy(pMsg, (pMsgRecv + 1), iRecvSize);
                    memcpy(pClientAddr, &clientaddr, sizeof(struct sockaddr_un));
                    pClientAddr->iSocketLen = iSocklen;
                    pClientAddr->ulNeedRet = pMsgRecv->ulNeedRet;
                    pClientAddr->iVosFd = g_vosFdProcHigh;
                    
                    return iRecvSize;
                }
                else
                {
                    vosMsgProcessInternel(g_vosFdProcHigh, pMsgRecv, (struct sockaddr *)&clientaddr, iSocklen);
                }
            }

            FD_ZERO(&fdSelectHigh);
            
            FD_SET(g_vosFdProcHigh, &fdSelectHigh);
            iMaxsock = g_vosFdProcHigh;

            tvHigh.tv_sec = 0; 
            tvHigh.tv_usec = 0;

            iRet = select(iMaxsock + 1, &fdSelectHigh, NULL, NULL, &tvHigh);
            if ( iRet <= 0 ) 
            {
                break;
            }
        }
    #endif
    
        if ( FD_ISSET(iHandle, &fdSelect) )
        {
            
            iSocklen = sizeof(struct sockaddr_un);
            
            iRet = vosMsgRecvStream(iHandle, g_vosMsgBufferRecv, VOS_MSG_BUFFER_LEN, 
                            (void *)&clientaddr, &iSocklen);
            
            if ( iRet < 0 )
            {
                
                printf("\r %s %d fail\n", __func__, __LINE__);
                return VOS_ERR_FAIL;
            }
            else
            {
                
                pMsgRecv = (VOS_MSG *)g_vosMsgBufferRecv;
                if ( VOS_MSG_TYPE_SELF == pMsgRecv->ulType )
                {
                    iRecvSize = ((ulong_t)iRet < ulMsgSize)?(ulong_t)iRet:ulMsgSize;
                    memcpy(pMsg, (pMsgRecv + 1), iRecvSize);
                    memcpy(pClientAddr, &clientaddr, sizeof(struct sockaddr_un));
                    pClientAddr->iSocketLen = iSocklen;
                    pClientAddr->ulNeedRet = pMsgRecv->ulNeedRet;
                    pClientAddr->iVosFd = iHandle;
                    
                    return iRecvSize;
                }
                else
                {
                    vosMsgProcessInternel(iHandle, pMsgRecv, (struct sockaddr *)&clientaddr, iSocklen);
                }
            }
        }

        pUserSocket = g_pUserSocketList;
    
        while ( NULL != pUserSocket )
        {
            pSocketPre = pUserSocket;
            pUserSocket = pUserSocket->pNext;

            if ( FD_ISSET(pSocketPre->iSocketFd, &fdSelect) )
            {
                pSocketPre->process(pSocketPre->iSocketFd, pSocketPre->pArg);
            }
        }

    }
    
    return VOS_ERR_NONE;
}

int vosMsgResp(int iHandle, int iRet, void *pMsg, ulong_t ulMsgSize, 
    VOS_MSG_ADDR *pClientAddr)
{
    int iSocketLen;
    int iRetTmp = VOS_ERR_NONE;
    int iSendLen;
    VOS_MSG *pMsgSend;
    struct sockaddr_un clientaddr;
    if ( (NULL == pMsg) || (NULL == pClientAddr) )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( ulMsgSize > (VOS_MSG_BUFFER_LEN - sizeof(VOS_MSG)) )
    {
        
        return VOS_ERR_TOOLONG;
    }

    if ( VOS_MSG_RET_NOT == pClientAddr->ulNeedRet )
    {
        
        return VOS_ERR_NONE;
    }
    
    pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

    memset(pMsgSend, 0, sizeof(VOS_MSG));
    
    pMsgSend->ulType = VOS_MSG_TYPE_SELF;
    pMsgSend->ulBufferLen = ulMsgSize;
    pMsgSend->ulRet = iRet;
    
    iSendLen = (ulMsgSize + sizeof(VOS_MSG));
    memcpy(g_vosMsgBufferSend + sizeof(VOS_MSG), pMsg, ulMsgSize);

    memcpy(&clientaddr, pClientAddr, sizeof(struct sockaddr_un));
    iSocketLen = pClientAddr->iSocketLen;

    iRetTmp = sendto(pClientAddr->iVosFd, (void *)pMsgSend, iSendLen,0, (struct sockaddr *)&clientaddr, iSocketLen);
    if ( iRetTmp < 0 )
    {
        return VOS_ERR_FAIL;
    }
    else if ( iRetTmp < iSendLen )
    {
        
    }
    return VOS_ERR_NONE;
}

int vosMsgSend(char *name, void *pMsg, ulong_t ulMsgSize, VOS_MSG_RET needRet, 
    int *piRet, ulong_t ulTimeOut, ulong_t ulFlags)
{
    int iRet = VOS_ERR_NONE;
    int iSocketFd, iSendLen;
    int iSocketLen;
    VOS_MSG *pMsgSend, *pMsgRecv;
    char *pRecvBuf;
    struct sockaddr_un servaddr, clientaddr;
    char szLocalCilentFile[VOS_LOCAL_FILE_MAX_LEN], szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];
    if ( (NULL == name) || (NULL == pMsg) )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( ulMsgSize > (VOS_MSG_BUFFER_LEN - sizeof(VOS_MSG)) )
    {
        
        return VOS_ERR_TOOLONG;
    }

    iSocketFd = vosMsgLocalSocket(ulTimeOut);
    if ( iSocketFd < 0 )
    {
        
        return VOS_ERR_FAIL;
    }   
    vosMsgclientChangePath(name, szLocalCilentFile);
    vosMsgLocalAddrInit(szLocalCilentFile, (void *)&clientaddr, &iSocketLen);
    unlink(szLocalCilentFile);    
    iRet = bind(iSocketFd, (struct sockaddr*)&clientaddr, iSocketLen);
    if ( iRet < 0 )
    {
		printf("\r\n %s %d bind fail\n", __func__, __LINE__);

        close(iSocketFd);
        return VOS_ERR_FAIL;
    }
    vosMsgSocketOptmize(iSocketFd, FALSE);

    pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

    memset(pMsgSend, 0, sizeof(VOS_MSG));
    
    pMsgSend->ulType = VOS_MSG_TYPE_SELF;
    pMsgSend->ulNeedRet = needRet;
    pMsgSend->ulBufferLen = ulMsgSize;
    
    iSendLen = (ulMsgSize + sizeof(VOS_MSG));
    memcpy(g_vosMsgBufferSend + sizeof(VOS_MSG), pMsg, ulMsgSize);

    vosMsgNLChangePath(name, szLocalServerFile);
    
#ifdef VOS_MSG_PRIORITY_SUPPORT
    if ( ulFlags & VOS_MSG_FLAG_URGENT )
    {
        strcat(szLocalServerFile, VOS_MSG_HIGH_SUFFIX);
    }
#endif
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    if ( -1 == connect(iSocketFd, (struct sockaddr *)&servaddr, iSocketLen) )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }
    iRet = write(iSocketFd, g_vosMsgBufferSend, iSendLen);
    if ( iRet < 0 )
    {
		printf("\r\n %s %d write msg fail\n", __func__, __LINE__);

        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }
    else if ( iRet < iSendLen )
    {
        
    }

    iRet = VOS_ERR_NONE;
    
    if ( VOS_MSG_RET_NEED == needRet )
    {
        pRecvBuf = g_vosMsgBufferSend;
		
        iRet = read(iSocketFd, pRecvBuf, iSendLen);
        if ( iRet < 0 )
        {
            printf("\r\n %s %d read msg fail\n", __func__, __LINE__);
            iRet = VOS_ERR_FAIL;
        }
        else
        {
            if ( iRet < iSendLen )
            {
                
            }
            
            pMsgRecv = (VOS_MSG *)pRecvBuf;

            *piRet = pMsgRecv->ulRet;
            memcpy(pMsg, (pMsgRecv + 1), ulMsgSize);

            iRet = VOS_ERR_NONE;
        }
    }

    unlink(szLocalCilentFile);
    close(iSocketFd);
    return iRet;
}
#if 0

int vosMsgRegisterPkt(vosMsgPktProcess pProc)
{
    g_vosPktProc = pProc;
    return VOS_ERR_NONE;
}
#endif

int vosMsgRegisterPortLink(VOS_PORT_TYPE portType, vosMsgPortLinkProcess pProc)
{
    switch ( portType )
    {
        case VOS_PORT_TYPE_AP:
            g_vosApLinktrapProc = pProc;
            break;

        case VOS_PORT_TYPE_AG:
            g_vosAgLinktrapProc = pProc;
            break;

        default:
           
            return VOS_ERR_PARAM;
    }
    return VOS_ERR_NONE;
}

int vosMsgRegisterVlanintfLink(vosMsgVlanintfLinkProcess pProc)
{
    g_vosVlanintfLinktrapProc = pProc;
    return VOS_ERR_NONE;
}

int vosMsgRegisterBldrun(vosMsgBldrunProcess pProc)
{
    g_vosBldrunProc = pProc;
    return VOS_ERR_NONE;
}

int vosMsgRegisterCcd(vosMsgCcdProcess pProc)
{
    g_vosCcdProc = pProc;
    return VOS_ERR_NONE;
}

int vosMsgRegisterCcdObjInfoGet(vosMsgCcdObjInfoGetProcess pProc)
{
    g_vosCcdObjInfoGetProc = pProc;
    return VOS_ERR_NONE;
}

int vosMsgBldrunSend(char *name, char *pszBuf, int *pnBufLen, 
    ulong_t ulFlags, ulong_t ulPort)
{
    int iRet = VOS_ERR_NONE;
    int iSocketFd, iSendLen;
    int iSocketLen;
    VOS_MSG *pMsgSend, *pMsgRecv;
    char *pRecvBuf;
    struct sockaddr_un servaddr, clientaddr;
    char szLocalCilentFile[VOS_LOCAL_FILE_MAX_LEN], szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];
    if ( (NULL == name) || (NULL == pszBuf) || (NULL == pnBufLen) )
    {
        
        return VOS_ERR_PARAM;
    }

    if ( (ulong_t)(*pnBufLen) <= sizeof(VOS_MSG) )
    {
        return VOS_ERR_FAIL;
    }

    iSocketFd = vosMsgLocalSocket(VOS_TIMEOUT_DEFAULT);
    if ( iSocketFd < 0 )
    {
        
        return VOS_ERR_FAIL;
    }   
    vosMsgGetLocalFile(szLocalCilentFile);
    vosMsgLocalAddrInit(szLocalCilentFile, (void *)&clientaddr, &iSocketLen);

    unlink(szLocalCilentFile);    
    iRet = bind(iSocketFd, (struct sockaddr*)&clientaddr, iSocketLen);
    if ( iRet < 0 )
    {
        close(iSocketFd);
        return VOS_ERR_FAIL;
    }

    vosMsgSocketOptmize(iSocketFd, FALSE);

    pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

    memset(pMsgSend, 0, sizeof(VOS_MSG));
    
    pMsgSend->ulType = VOS_MSG_TYPE_BLDRUN;
    pMsgSend->ulNeedRet = VOS_MSG_RET_NEED;
    pMsgSend->ulValue1 = ulFlags;
    pMsgSend->ulValue2 = ulPort;
    pMsgSend->ulBufferLen = *pnBufLen;

    iSendLen = sizeof(VOS_MSG);

    vosMsgNLChangePath(name, szLocalServerFile);
#ifdef VOS_MSG_PRIORITY_SUPPORT
    strcat(szLocalServerFile, VOS_MSG_HIGH_SUFFIX);
#endif
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    if ( -1 == connect(iSocketFd, (struct sockaddr *)&servaddr, iSocketLen) )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }

    iRet = write(iSocketFd, g_vosMsgBufferSend, iSendLen);
    if ( iRet < 0 )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }
    else if ( iRet < iSendLen )
    {
        
    }

    iSendLen = *pnBufLen;
    pRecvBuf = pszBuf;
    
    iRet = read(iSocketFd, pRecvBuf, iSendLen);
    if ( iRet < 0 )
    {
        if ( EAGAIN == errno )
        {
        }
        else
        {
        }
        iRet = VOS_ERR_FAIL;
    }
    else
    {
        if ( iRet < iSendLen )
        {
            
        }
        
        pMsgRecv = (VOS_MSG *)pRecvBuf;

        iRet = pMsgRecv->ulRet;
        
        if ( VOS_ERR_NONE == iRet )
        {
            *pnBufLen = pMsgRecv->ulValue3;
            iSendLen = pMsgRecv->ulBufferLen;

            memset(pszBuf, 0, sizeof(VOS_MSG));
            memcpy(pszBuf, (pMsgRecv + 1), iSendLen);
        }
        else
        {
            memset(pszBuf, 0, sizeof(VOS_MSG));
        }
    }

    unlink(szLocalCilentFile);
    close(iSocketFd);
    return iRet;
}

int vosMsgCcdRequestSend(ulong_t *piRet, char *pProcName, 
    ulong_t request, char *pObjName, ulong_t *pIndex, ulong_t *pIndexSubIdCount, 
    void *pValueBuf, ulong_t *pValueLen, ushort_t *flags)
{
#if 0
    STATUS ccdRet;
    int iRet = VOS_ERR_NONE;
    int iSocketFd, iSendLen, iTmpLen, iTmp1;
    int iSocketLen;
    VOS_MSG *pMsgSend, *pMsgRecv;
    char *pRecvBuf, *pSendBuf;
    struct sockaddr_un servaddr, clientaddr;
    char szLocalCilentFile[VOS_LOCAL_FILE_MAX_LEN], szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];
    pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

    memset(pMsgSend, 0, sizeof(VOS_MSG));
    
    pMsgSend->ulType = VOS_MSG_TYPE_CCD;
    pMsgSend->ulNeedRet = VOS_MSG_RET_NEED;
    pMsgSend->ulValue1 = request;
    pMsgSend->ulValue2 = *pIndexSubIdCount;
    pMsgSend->ulValue3 = *flags;

    pSendBuf = g_vosMsgBufferSend;
    iSendLen = sizeof(VOS_MSG);

    iTmpLen = (sizeof(ulong_t)*(pMsgSend->ulValue2));
    memcpy(pSendBuf + iSendLen, pIndex, iTmpLen);
    iSendLen += iTmpLen;
    assert(iSendLen < VOS_MSG_BUFFER_LEN);

    iTmpLen = (strlen(pObjName) + 1);
    iTmp1 = (iTmpLen%4);
    if ( 0 != iTmp1 )
    {
        
        iTmp1 = (4 - iTmp1);
    }
    iTmpLen += iTmp1;
    
    if ( (iSendLen + iTmpLen ) >= (VOS_MSG_BUFFER_LEN - 8) )
    {
        
        return VOS_ERR_FAIL;
    }

    memcpy(pSendBuf + iSendLen, &iTmpLen, 4);
    iSendLen += 4;

    iTmpLen = strlen(pObjName);
    memcpy(pSendBuf + iSendLen, pObjName, iTmpLen);
    iSendLen += iTmpLen;
    *(pSendBuf + iSendLen) = '\0';
    iSendLen += 1;
    iSendLen += iTmp1;

    iTmpLen = 4;
    memcpy(pSendBuf + iSendLen, pValueLen, iTmpLen);
    iSendLen += iTmpLen;
   	if(0)
    {
        iTmpLen = *pValueLen;

        if ( (iSendLen + iTmpLen) >= VOS_MSG_BUFFER_LEN - 4 )
        {
            
            return VOS_ERR_FAIL;
        }

        memcpy(pSendBuf + iSendLen, pValueBuf, iTmpLen);
        iSendLen += iTmpLen;
    }

    iSocketFd = vosMsgLocalSocket(10 * 1000);
    if ( iSocketFd < 0 )
    {
       
        return VOS_ERR_FAIL;
    }   
    vosMsgGetLocalFile(szLocalCilentFile);
    vosMsgLocalAddrInit(szLocalCilentFile, (void *)&clientaddr, &iSocketLen);

    unlink(szLocalCilentFile);    
    iRet = bind(iSocketFd, (struct sockaddr*)&clientaddr, iSocketLen);
    if ( iRet < 0 )
    {
        close(iSocketFd);
        return VOS_ERR_FAIL;
    }
    vosMsgNLChangePath(pProcName, szLocalServerFile);
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    if ( -1 == connect(iSocketFd, (struct sockaddr *)&servaddr, iSocketLen) )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }

    iRet = write(iSocketFd, g_vosMsgBufferSend, iSendLen);
    if ( iRet < 0 )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }
    else if ( iRet < iSendLen )
    {
       
    }

    pRecvBuf = g_vosMsgBufferSend;
    iSendLen = VOS_MSG_BUFFER_LEN;
    
    iRet = read(iSocketFd, pRecvBuf, iSendLen);
    if ( iRet < 0 )
    {
        iRet = VOS_ERR_FAIL;
    }
    else
    {
        if ( iRet < iSendLen )
        {
           
        }

        iRet = VOS_ERR_NONE;
        
        pMsgRecv = (VOS_MSG *)pRecvBuf;

        ccdRet = pMsgRecv->ulRet;
        *piRet = ccdRet;
        
        *flags = pMsgRecv->ulValue3;

        if ( OK == ccdRet )
        {
            if ( (CCD_GET_NEXT == request) || (CCD_GET == request) )
            {
                
                *pIndexSubIdCount = pMsgRecv->ulValue2;

                iSendLen = sizeof(VOS_MSG);

                iTmpLen = (sizeof(ulong_t) * pMsgRecv->ulValue2);
                if ( (iSendLen + iTmpLen) >= VOS_MSG_BUFFER_LEN - 8 )
                {
                   
                    iRet = VOS_ERR_FAIL;
                    goto done;
                }
                
                memcpy(pIndex, pRecvBuf + iSendLen, iTmpLen);
                iSendLen += iTmpLen;

                memcpy(&iTmpLen, pRecvBuf + iSendLen, 4);
                iSendLen += (iTmpLen + 4);

                memcpy(&iTmpLen, pRecvBuf + iSendLen, 4);
                iSendLen += 4;

                if ( (iSendLen + iTmpLen) > VOS_MSG_BUFFER_LEN )
                {
                   
                    iRet = VOS_ERR_FAIL;
                    goto done;
                }
                
                memcpy(pValueBuf, pRecvBuf + iSendLen, iTmpLen);
                *pValueLen = iTmpLen;
            }
        }
    }

done:
    
    unlink(szLocalCilentFile);
    close(iSocketFd);
#endif
    return 0;
}

int vosMsgCcdObjInfoGetSend(
    ulong_t       *piRet, 
    char         *pProcName, 
    char         *pObjName, 
    ulong_t      valueMask,
    ulong_t      *pulLocalId,               
    ulong_t      *pulLastOidPrefixSubId,    
    ulong_t      *pulSize,                  
    ulong_t      *pulRangeMin,              
    ulong_t      *pulRangeMax,              
    int          *pnType,                   
    ulong_t      pOidPrefixWoLastSubId[],   
    ulong_t      *pulOidPrefixLen,          
    ulong_t      *pulIndexObjCount,         
    ulong_t      *pulIndexSubIdCount,       
    ulong_t      *pulIndexSubIdCountMax     
    )
{
#if 0
    STATUS ccdRet;
    int iRet = VOS_ERR_NONE;
    int iSocketFd, iSendLen, iTmpLen, iTmp1;
    int iSocketLen;
    VOS_MSG *pMsgSend, *pMsgRecv;
    char *pRecvBuf, *pSendBuf;
    struct sockaddr_un servaddr, clientaddr;
    char szLocalCilentFile[VOS_LOCAL_FILE_MAX_LEN], szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];
    pMsgSend = (VOS_MSG *)g_vosMsgBufferSend;

    memset(pMsgSend, 0, sizeof(VOS_MSG));
    
    pMsgSend->ulType = VOS_MSG_TYPE_CCD_OBJ_INFO_GET;
    pMsgSend->ulNeedRet = VOS_MSG_RET_NEED;
    pMsgSend->ulValue1 = valueMask;

    pSendBuf = g_vosMsgBufferSend;
    iSendLen = sizeof(VOS_MSG);

    iTmpLen = (strlen(pObjName) + 1);
    iTmp1 = (iTmpLen%4);
    if ( 0 != iTmp1 )
    {
        
        iTmp1 = (4 - iTmp1);
    }
    iTmpLen += iTmp1;

    pMsgSend->ulValue2 = iTmpLen;

    iTmpLen = strlen(pObjName);
    memcpy(pSendBuf + iSendLen, pObjName, iTmpLen);
    iSendLen += iTmpLen;
    *(pSendBuf + iSendLen) = '\0';
    iSendLen += 1;
    iSendLen += iTmp1;

    iSocketFd = vosMsgLocalSocket(10 * 1000);
    if ( iSocketFd < 0 )
    {
        
        return VOS_ERR_FAIL;
    }   
    vosMsgGetLocalFile(szLocalCilentFile);
    vosMsgLocalAddrInit(szLocalCilentFile, (void *)&clientaddr, &iSocketLen);

    unlink(szLocalCilentFile);    
    iRet = bind(iSocketFd, (struct sockaddr*)&clientaddr, iSocketLen);
    if ( iRet < 0 )
    {
        close(iSocketFd);
        return VOS_ERR_FAIL;
    }

    vosMsgSocketOptmize(iSocketFd, FALSE);

    vosMsgNLChangePath(pProcName, szLocalServerFile);
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    if ( -1 == connect(iSocketFd, (struct sockaddr *)&servaddr, iSocketLen) )
    {
       
            __func__, errno, strerror(errno));

        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }

    iRet = write(iSocketFd, g_vosMsgBufferSend, iSendLen);
    if ( iRet < 0 )
    {
        unlink(szLocalCilentFile);
        close(iSocketFd);

        return VOS_ERR_FAIL;
    }
    else if ( iRet < iSendLen )
    {
       
    }

    pRecvBuf = g_vosMsgBufferSend;
    iSendLen = VOS_MSG_BUFFER_LEN;
    
    iRet = read(iSocketFd, pRecvBuf, iSendLen);
    if ( iRet < 0 )
    {
        iRet = VOS_ERR_FAIL;
    }
    else
    {
        if ( iRet < iSendLen )
        {
           
        }

        iRet = VOS_ERR_NONE;
        
        pMsgRecv = (VOS_MSG *)pRecvBuf;

        ccdRet = pMsgRecv->ulRet;
        *piRet = ccdRet;
        errnoSet(pMsgRecv->ulValue4);

        if ( OK == ccdRet )
        {
            *pulLocalId = pMsgRecv->ulValue2;
            *pulLastOidPrefixSubId = pMsgRecv->ulValue3;

            iSendLen = sizeof(VOS_MSG);
            
            memcpy(&iTmpLen, pRecvBuf + iSendLen, 4);
            iSendLen += 4;
            iSendLen += iTmpLen;

            memcpy(pulSize, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pulRangeMin, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pulRangeMax, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pnType, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pulOidPrefixLen, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pOidPrefixWoLastSubId, pRecvBuf + iSendLen, (*pulOidPrefixLen) * 4);
            iSendLen += (*pulOidPrefixLen) * 4;

            memcpy(pulIndexObjCount, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pulIndexSubIdCount, pRecvBuf + iSendLen, 4);
            iSendLen += 4;

            memcpy(pulIndexSubIdCountMax, pRecvBuf + iSendLen, 4);
            iSendLen += 4;
        }
    }

    unlink(szLocalCilentFile);
    close(iSocketFd);
#endif
    return 0;
}

int vosMsgLocalAddrInit(char *name, void *localAddr, int *piAddrLen)
{
    int                      iPathLen = strlen(name);
    struct sockaddr_un      *pLocalAddr = (struct sockaddr_un *)localAddr;
    
    bzero(pLocalAddr, sizeof(struct sockaddr_un));

    pLocalAddr->sun_family = AF_LOCAL;
    strncpy(pLocalAddr->sun_path, name, iPathLen);

    *piAddrLen = sizeof(sa_family_t) + iPathLen + 1;

    return VOS_ERR_NONE;
}

int vosMsgLocalSocket(ulong_t ulTimeOut)
{
    int iSocketFd, iLen, iRet;
    struct timeval tv;
    
    iSocketFd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if ( -1 != iSocketFd )
    {
        
        if(1)
        {
            tv.tv_sec = (ulTimeOut/1000);
            tv.tv_usec = (ulTimeOut%1000)*1000;

            iLen = sizeof(struct timeval);

            iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_SNDTIMEO, &tv, iLen);
            if ( -1 == iRet )
            {
            }

            iRet = setsockopt(iSocketFd, SOL_SOCKET, SO_RCVTIMEO, &tv, iLen);
            if ( -1 == iRet )
            {
            }
        }
    }

    return iSocketFd;
}

int vosMsgSendStream(int iSocketFd, char *pStream, ulong_t ulSize, 
    void *peerAddr, socklen_t iSocketLen)
{
    int iRet;
    iRet = sendto(iSocketFd, (void *)pStream, ulSize,0, (struct sockaddr *)peerAddr, iSocketLen);
    if ( iRet < 0 )
    {
        return VOS_ERR_FAIL;
    }
    else if ( (ulong_t)iRet < ulSize )
    {
       
    }
    return iRet;
}

int vosMsgRecvStream(int iSocketFd, char *pStream, ulong_t ulSize, 
    void *peerAddr, socklen_t *pSockLen)
{
    int iRet;
    iRet = recvfrom(iSocketFd, pStream, ulSize, 0, (struct sockaddr *)peerAddr, pSockLen);
    if ( 0 == iRet )
    {
       
        iRet = VOS_ERR_FAIL;
    }
    else if ( iRet < 0 )
    {
        iRet = VOS_ERR_FAIL;
    }
    return iRet;
}

int vosMsgDebugVos(char *name, ulong_t ulFlagsBitMask)
{
    int iRet = VOS_ERR_NONE, iSendBytes;
    int iSocketFd, iSendLen;
    int iSocketLen;
    struct sockaddr_un servaddr;
    VOS_MSG msg;
    char szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];

    memset(&msg, 0, sizeof(VOS_MSG));

    msg.ulType = VOS_MSG_VOS_DEBUG;
    msg.ulValue1 = ulFlagsBitMask;

    iSendLen = sizeof(VOS_MSG);

    iSocketFd = vosMsgLocalSocket(VOS_TIMEOUT_DEFAULT);
    if ( iSocketFd < 0 )
    {
        
        return VOS_ERR_FAIL;
    }
    vosMsgNLChangePath(name, szLocalServerFile);
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    iSendBytes = vosMsgSendStream(iSocketFd, (char *)&msg, iSendLen, (void *)&servaddr, iSocketLen);
    if ( iSendBytes < 0 )
    {
       
        iRet = VOS_ERR_FAIL;
    }

    close(iSocketFd);
    return iRet;
}

int vosMsgDebugSmd(char *name, ulong_t ulFlagsBitMask)
{
    int iRet = VOS_ERR_NONE, iSendBytes;
    int iSocketFd, iSendLen;
    int iSocketLen;
    struct sockaddr_un servaddr;
    VOS_MSG msg;
    char szLocalServerFile[VOS_LOCAL_FILE_MAX_LEN];

    memset(&msg, 0, sizeof(VOS_MSG));

    msg.ulType = VOS_MSG_SMD_DEBUG;
    msg.ulValue1 = ulFlagsBitMask;

    iSendLen = sizeof(VOS_MSG);

    iSocketFd = vosMsgLocalSocket(VOS_TIMEOUT_DEFAULT);
    if ( iSocketFd < 0 )
    {
       
        return VOS_ERR_FAIL;
    }
    vosMsgNLChangePath(name, szLocalServerFile);
    vosMsgLocalAddrInit(szLocalServerFile, (void *)&servaddr, &iSocketLen);

    iSendBytes = vosMsgSendStream(iSocketFd, (char *)&msg, iSendLen, (void *)&servaddr, iSocketLen);
    if ( iSendBytes < 0 )
    {
        
        iRet = VOS_ERR_FAIL;
    }

    close(iSocketFd);
    return iRet;
}
#if 0

ulong_t vosMsgDebugSmdGet(void)
{
    return g_vosDebugSmdFlags;
}

int vosMsgDebugSmdSet(ulong_t ulFlags)
{
    g_vosDebugSmdFlags = ulFlags;

    return VOS_ERR_NONE;
}
#endif

int vosTimerRegister(VOS_TIMER_TYPE type, ulong_t ulTime, vosTimerProcess pProc, 
    ulong_t **ppulTimerHande, void *pArg)
{
    int iRet = VOS_ERR_NONE;
    VOS_TIMER *pTimer = NULL;
    if ( NULL ==  ppulTimerHande )
    {
       
        return VOS_ERR_PARAM;
    }

    pTimer = malloc(sizeof(VOS_TIMER));
    if ( NULL == pTimer )
    {
       
        return VOS_ERR_NOMEMORY;
    }

    memset(pTimer, 0, sizeof(VOS_TIMER));

    pTimer->func = pProc;
    pTimer->ulMagic = (g_ulVosMsgMagic++);
    pTimer->ulTime = ulTime;
    pTimer->type = type;
    pTimer->pArg = pArg;
    pTimer->bStart = TRUE;
    gettimeofday(&pTimer->start, NULL);

    iRet = vosMsgTimerAdd(pTimer);

    *ppulTimerHande = (ulong_t *)pTimer;
    return iRet;
}

int vosTimerCancel(ulong_t *pulTimerHande)
{
    int iRet = VOS_ERR_NONE;
    VOS_TIMER *pTimer = NULL;
    if ( NULL ==  pulTimerHande )
    {
       
        return VOS_ERR_PARAM;
    }

    pTimer = (VOS_TIMER *)pulTimerHande;

    iRet = vosMsgTimerDelete(pTimer);
    if ( VOS_ERR_NONE == iRet )
    {
        free(pTimer);
    }
    return iRet;
}

int vosTimerStart(ulong_t *pulTimerHande, ulong_t ulTime)
{
    VOS_TIMER               *pTimer;

    pTimer = (VOS_TIMER *)pulTimerHande;
    if ( TRUE != vosMsgTimerCheck(pTimer) )
    {
        return VOS_ERR_FAIL;
    }

    pTimer->ulTime = ulTime;
    pTimer->bStart = TRUE;
    gettimeofday(&pTimer->start, NULL);

    return VOS_ERR_NONE;
}

int vosTimerStop(ulong_t *pulTimerHande)
{
    VOS_TIMER               *pTimer;

    pTimer = (VOS_TIMER *)pulTimerHande;
    if ( TRUE != vosMsgTimerCheck(pTimer) )
    {
        return VOS_ERR_FAIL;
    }

    pTimer->bStart = FALSE;

    return VOS_ERR_NONE;
}

int vosMsgSockAdd(int iSocketFd, vosMsgUserSocketProcess process, void *pArg)
{
    VOS_SOCK_USER *pUserSocket, *pSocketNew;

    if ( iSocketFd < 0 )
    {
        return VOS_ERR_PARAM;
    }

    pUserSocket = g_pUserSocketList;

    while ( NULL != pUserSocket )
    {
        if ( iSocketFd == pUserSocket->iSocketFd )
        {
            return VOS_ERR_EXISTS;
        }

        pUserSocket = pUserSocket->pNext;
    }

    pSocketNew = malloc(sizeof(VOS_SOCK_USER));
    if ( NULL == pSocketNew )
    {
        
        return VOS_ERR_NOMEMORY;
    }

    memset(pSocketNew, 0, sizeof(VOS_SOCK_USER));
    pSocketNew->iSocketFd = iSocketFd;
    pSocketNew->process = process;
    pSocketNew->pArg = pArg;

    if ( NULL == g_pUserSocketList )
    {
        g_pUserSocketList = pSocketNew;
    }
    else
    {
        pSocketNew->pNext = g_pUserSocketList;
        g_pUserSocketList = pSocketNew;
    }

    return VOS_ERR_NONE;
}

int vosMsgSockDelete(int iSocketFd)
{
    VOS_SOCK_USER *pUserSocket, *pSocetPre;

    pUserSocket = g_pUserSocketList;
    pSocetPre = NULL;

    while ( NULL != pUserSocket )
    {
        if ( iSocketFd == pUserSocket->iSocketFd )
        {
            if ( NULL == pSocetPre )
            {
                g_pUserSocketList = pUserSocket->pNext;
            }
            else
            {
                pSocetPre->pNext = pUserSocket->pNext;
            }

            free(pUserSocket);
            return VOS_ERR_NONE;
        }

        pSocetPre = pUserSocket;
        pUserSocket = pUserSocket->pNext;
    }

    return VOS_ERR_FAIL;
}

int vosMsgSockAddUser(VOS_SOCK_USER *pSocketNew)
{
    VOS_SOCK_USER *pUserSocket;

    pUserSocket = g_pUserSocketList;

    while ( NULL != pUserSocket )
    {
        if ( (pSocketNew == pUserSocket) 
            || (pSocketNew->iSocketFd == pUserSocket->iSocketFd) )
        {
            return VOS_ERR_EXISTS;
        }

        pUserSocket = pUserSocket->pNext;
    }

    if ( NULL == g_pUserSocketList )
    {
        pSocketNew->pNext = NULL;
        g_pUserSocketList = pSocketNew;
    }
    else
    {
        pSocketNew->pNext = g_pUserSocketList;
        g_pUserSocketList = pSocketNew;
    }

    return VOS_ERR_NONE;
}

int vosMsgSockDelUser(VOS_SOCK_USER *pSocketDel)
{
    VOS_SOCK_USER *pUserSocket, *pSocetPre;

    pUserSocket = g_pUserSocketList;
    pSocetPre = NULL;

    while ( NULL != pUserSocket )
    {
        if ( pSocketDel == pUserSocket )
        {
            if ( NULL == pSocetPre )
            {
                g_pUserSocketList = pUserSocket->pNext;
            }
            else
            {
                pSocetPre->pNext = pUserSocket->pNext;
            }

            return VOS_ERR_NONE;
        }

        pSocetPre = pUserSocket;
        pUserSocket = pUserSocket->pNext;
    }

    return VOS_ERR_FAIL;
}

