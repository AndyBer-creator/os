
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <common/type.h>
#include "sal_socket.h"
#include <pthread.h>

#ifdef SOCK_PROCESS
socket_mapper_t socket_mapper[SOCK_ID_END];
#endif

int sal_inetPton(int iAf, const char *src, void *dst)
{
    return inet_pton(iAf, src, dst);
}

const char *sal_inetNtop(int iAf, const void *src, char *dst, int iDstLen)
{
    return inet_ntop(iAf, src, dst, iDstLen);
}

uint32 sal_inetAddr(char *src)
{
    char                    *pTmp;
    unsigned int             uiDotNum = 0, i;
    char                     szBuf[32];

    pTmp = src;
    for ( i = 0; i < 3; i++ )
    {
        pTmp = strchr(pTmp, '.');
        if ( NULL == pTmp )
        {
            break;
        }

        uiDotNum++;
        pTmp++;
    }

    strcpy(szBuf, src);
    if ( uiDotNum < 3 )
    {
        for ( i = 0; i < (3 - uiDotNum); i++ )
        {
            strcat(szBuf, ".0");
        }
    }
    
    return inet_addr(szBuf);
}

char *sal_inetNtoA(struct in_addr in)
{
    return inet_ntoa(in);
}

int sal_socket(int iDomain, int iType, int iProtocol)
{
    return socket(iDomain, iType, iProtocol);
}

int sal_bind(int sockfd, sal_sockaddr *address, int iAddressLen)
{
    return bind(sockfd, address, iAddressLen);
}

int sal_select(int iWidth, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *pTimeOut)
{
    return select(iWidth, readfds, writefds, errorfds, pTimeOut);
}

int sal_listen(int sockfd, int iBacklog)
{
    return listen(sockfd, iBacklog);
}

int sal_accept(int sockfd, sal_sockaddr *addr, int *piAddrlen)
{
    int                      iRet;
    socklen_t                sockLen = *piAddrlen;
    
    iRet = accept(sockfd, addr, &sockLen);
    *piAddrlen = sockLen;
    return iRet;
}

int sal_connect(int sockfd, sal_sockaddr *addr, int iAddrlen)
{
    return connect(sockfd, addr, iAddrlen);
}

int sal_connectWithTimeout(int sockfd, sal_sockaddr *addr, 
    int iAddrlen, struct timeval *timeVal )
{
    int                      iFlags;
    int                      iRet, iSockeErr = -1;
    socklen_t                iLen;
    fd_set                   w;

    iFlags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, iFlags | O_NONBLOCK);

    iRet = connect(sockfd, addr, iAddrlen);
    if ( 0 == iRet )
    {
        return 0;
    }

    if ( EINPROGRESS != errno ) 
    {
        return -1;
    }     
     
    FD_ZERO(&w);
    FD_SET(sockfd,&w);

    iRet = select(sockfd + 1, NULL, &w, NULL, timeVal);
    if (iRet <= 0 )
    {
        return -1;
    }

    iLen = sizeof(iSockeErr);
    iRet = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &iSockeErr, &iLen);
    if ( iRet < 0 )
    {
        osal_printf("%s:getsockopt SOL_SOCKET SO_ERROR failed.", __func__);
        return -1;
    }
    
    fcntl(sockfd, F_SETFL, iFlags);

    if ( 0 == iSockeErr )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int sal_sendto(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr, int iAddrLen )
{
    return sendto(s, buf, iBufLen, iFlags, addr, iAddrLen);
}

int sal_send(int s, char *buf, int iBufLen, int iFlags)
{
    return send(s, buf, iBufLen, iFlags);
}

int sal_sendmsg(int s, struct msghdr * mp, int iFlags)
{
    return sendmsg(s, mp, iFlags);
}

int sal_recvfrom(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr, int *piAddrLen )
{
    int                      iRet;
    socklen_t                sockLen = *piAddrLen;

    iRet = recvfrom(s, buf, iBufLen, iFlags, addr, &sockLen);
    *piAddrLen = sockLen;
    return iRet;
}

int sal_recvfromExt(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr,
    int *piAddrLen, sal_s_ext_stc_t *pSocktExt)
{
    unsigned int             uiSw = 1, uiVid = 1, uiPort = 1;
    int                      iRet;
    union sal_sockaddr_union destIp;
    struct msghdr            msg;
    struct iovec             iov[5];

    memset(&msg, 0, sizeof(struct msghdr));
    iov[0].iov_base = buf;
    iov[0].iov_len = iBufLen;
    iov[1].iov_base = &uiSw;
    iov[1].iov_len = sizeof(unsigned int);
    iov[2].iov_base = &uiVid;
    iov[2].iov_len = sizeof(unsigned int);
    iov[3].iov_base = &uiPort;
    iov[3].iov_len = sizeof(unsigned int);
    iov[4].iov_base = &destIp;
    iov[4].iov_len = sizeof(union sal_sockaddr_union);
    msg.msg_iov = iov;
    msg.msg_iovlen = 5;
    msg.msg_name = addr;
    msg.msg_namelen = sizeof(sal_sockaddr);

    iRet = recvmsg(s, &msg, iFlags);
    if ( iRet > 0 )
    {
        pSocktExt->ulSw = uiSw;
        pSocktExt->ulVid = uiVid;
        pSocktExt->ulPort = uiPort;
        memcpy(&pSocktExt->destIp, &destIp, sizeof(union sal_sockaddr_union));
    }
    
    return iRet;
}

int sal_recv(int s, char *buf, int iBufLen, int iFlags )
{
    return recv(s, buf, iBufLen, iFlags);
}

int sal_recvmsg(int s, struct msghdr * mp, int iFlags )
{
    return recvmsg(s, mp, iFlags);
}

int sal_setsockopt(int s, int iLevel, int iOptname, char *pOptval, int iOptlen)
{
    return setsockopt(s, iLevel, iOptname, pOptval, iOptlen);
}

int sal_getsockopt(int s, int iLevel, int iOptname, char *pOptval, int *piOptlen)
{
    int                      iRet;
    socklen_t                sockLen = *piOptlen;

    iRet = getsockopt(s, iLevel, iOptname, pOptval, &sockLen);
    *piOptlen = sockLen;
    return iRet;
}

int sal_shutdown(int s, int iHow)
{
    return shutdown(s, iHow);
}

int sal_getsockname(int s, sal_sockaddr *addr, int *piAddrLen)
{
    int                      iRet;
    socklen_t                sockLen = *piAddrLen;

    iRet = getsockname(s, addr, &sockLen);
    *piAddrLen = sockLen;
    return iRet;
}

int sal_getpeername(int s, sal_sockaddr *addr, int *piAddrLen)
{
    int                      iRet;
    socklen_t                sockLen = *piAddrLen;

    iRet = getpeername(s, addr, &sockLen);
    *piAddrLen = sockLen;
    return iRet;
}

int sal_setNonblocking(int sockfd, int bNonblocking)
{
    int                      iFlags;

    iFlags = fcntl(sockfd, F_GETFL, 0);

    if ( TRUE == bNonblocking )
    {
        iFlags |= O_NONBLOCK;
    }
    else
    {
        iFlags &= ~O_NONBLOCK;
    }

    return fcntl(sockfd, F_SETFL, iFlags);
}

int sal_close(int socktfd)
{
	return close(socktfd);
}

#ifdef SOCK_PROCESS
int sal_socket_select_check(void)
{
	int index = 0;
	for(index = SOCK_ID_NONE; index < SOCK_ID_END; index ++)
	{
		if(socket_mapper[index].socktId > 0)
			return TRUE;
	}
	return FALSE;
}
int32 sal_socket_init_task(void)
{
	pthread_t  sock_select_id;
	if(sal_socket_select_check()== FALSE)
		return SYS_ERR_OK;

	if(pthread_create(&sock_select_id, NULL , (void *)sal_socket_slect_task, NULL) == 0)
	{
		osal_printf("create socket select thread fail\n");
		return SYS_ERR_FAILED;
	}
	osal_printf(" create socket pthread ------ok\n");
	return SYS_ERR_OK;
}

void sal_socket_slect_task(void)
{
	int index = 0;
	int sockId;
	while(1)
	{
		for(index = SOCK_ID_NONE; index < SOCK_ID_END; index ++)
		{
			sockId = socket_mapper[index].socktId;
			if(sockId > 0)
			{
				int ret= -1;
			    struct timeval tv;
			    tv.tv_sec = 3;
			    tv.tv_usec = 0;

			    fd_set rd_fd;
			    SAL_FD_ZERO(&rd_fd);                          
			    SAL_FD_SET(sockId, &rd_fd);
			    ret = sal_select(sockId+ 1, &rd_fd, NULL, NULL, &tv);
			    switch(ret)
			    {
			        case -1:
			            osal_printf("select error\n");
			            break;
			        case 0:
			            osal_printf("selcet timeout\n");
			            break;
			        default:
			            if(SAL_FD_ISSET(sockId, &rd_fd))
			            {
			                osal_printf("yeah, some data come on\n");

							socket_mapper[index].fCallback(sockId, NULL);
			            }
			    }
			}
		}
        
    }
	return ;
}

int32 sal_socktet_handler_register(uint32 index, int sockt, int (*fCallback)(int sockid, void *pArg), void *pArg1)
{
    
    SYS_PARAM_LOG((index >= SOCK_ID_END), SYS_ERR_FAILED, "socket handler: register failed\n");
    SYS_PARAM_CHK((NULL == fCallback), SYS_ERR_NULL_POINTER);

    socket_mapper[index].index = index;
    socket_mapper[index].socktId = sockt;
    socket_mapper[index].fCallback = fCallback;

    return SYS_ERR_OK;
}
#endif