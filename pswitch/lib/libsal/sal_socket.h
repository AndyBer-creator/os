
#ifndef _INCLUDE_SAL_SOCKET_H_
#define _INCLUDE_SAL_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <rsd/rsd_sys.h>
#include <net/if.h>

#define SAL_AF_INET                          AF_INET
#define SAL_AF_INET6                         AF_INET6
#define SAL_SOCK_DGRAM                       SOCK_DGRAM
#define SAL_SOL_SOCKET                       SOL_SOCKET
#define SAL_SO_REUSEADDR                     SO_REUSEADDR 
#define SAL_SO_BROADCAST                     SO_BROADCAST 
#define SAL_FD_ZERO(p)                       FD_ZERO(p)   
#define SAL_FD_SET(n, p)					 FD_SET(n, p) 
#define SAL_FD_ISSET(n, p)                   FD_ISSET(n, p)

typedef enum sock_id_index_e
{
    SOCK_ID_NONE = 0,
    SOCK_ID_DHCPSERVER = SOCK_ID_NONE,
    
    SOCK_ID_END
} sock_id_index_t;

typedef struct ifreq    sal_ifreq;
typedef struct in_addr sal_in_addr;
typedef struct sockaddr_in sal_sockaddr_in;
typedef struct in6_addr sal_in6_addr;
typedef struct sockaddr_in6 sal_sockaddr_in6;
typedef struct sockaddr_nl sal_sockaddr_nl;
typedef struct sockaddr sal_sockaddr;

union sal_sockaddr_union
{
    sal_sockaddr         sa;
    sal_sockaddr_in      sin;
    sal_sockaddr_in6     sin6;
};

typedef struct {
    uint32                  ulSw; 
    uint32                  ulVid;
    uint32                  ulPort; 
    union sal_sockaddr_union destIp; 
}sal_s_ext_stc_t;

typedef struct socket_mapper_s
{
	uint32 index;
    int32 socktId;
    int32 (*fCallback)(int sockid, void *pArg);
} socket_mapper_t;

struct sal_ip {
	u_int	ip_v:4,			
		ip_hl:4,		
		ip_tos:8,		
		ip_len:16;		
	uint16	ip_id;			
	short	ip_off;			
#define	IP_DF 0x4000			
#define	IP_MF 0x2000			
#define	IP_OFFMASK 0x1fff		
	uint8	ip_ttl;			
	uint8	ip_p;			
	uint16	ip_sum;			
	struct	in_addr ip_src,ip_dst;	
};

int sal_inetPton(int iAf, const char *src, void *dst);

const char *sal_inetNtop(int iAf, const void *src, char *dst, int iDstLen);

uint32 sal_inetAddr(char *src);

char *sal_inetNtoA(struct in_addr in);

int sal_socket(int iDomain, int iType, int iProtocol);

int sal_bind(int sockfd, sal_sockaddr *address, int iAddressLen);

int sal_select(int iWidth, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *pTimeOut);

int sal_listen(int sockfd, int iBacklog);

int sal_accept(int sockfd, sal_sockaddr *addr, int *piAddrlen);

int sal_connect(int sockfd, sal_sockaddr *addr, int iAddrlen);

int sal_connectWithTimeout(int sockfd, sal_sockaddr *addr, 
    int iAddrlen, struct timeval *timeVal );

int sal_sendto(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr, int iAddrLen );

int sal_send(int s, char *buf, int iBufLen, int iFlags);

int sal_sendmsg(int s, struct msghdr * mp, int iFlags);

int sal_recvfrom(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr, int *piAddrLen );

int sal_recvfromExt(int s, char *buf, int iBufLen, int iFlags, sal_sockaddr *addr,
    int *piAddrLen, sal_s_ext_stc_t *pSocktExt);

int sal_recv(int s, char *buf, int iBufLen, int iFlags );

int sal_recvmsg(int s, struct msghdr * mp, int iFlags );

int sal_setsockopt(int s, int iLevel, int iOptname, char *pOptval, int iOptlen);

int sal_getsockopt(int s, int iLevel, int iOptname, char *pOptval, int *piOptlen);

int sal_shutdown(int s, int iHow);

int sal_getsockname(int s, sal_sockaddr *addr, int *piAddrLen);

int sal_getpeername(int s, sal_sockaddr *addr, int *piAddrLen);

int sal_setNonblocking(int sockfd, int bNonblocking);

int sal_close(int socktfd);

int32 sal_socket_init_task(void);

void sal_socket_slect_task(void);

int32 sal_socktet_handler_register(uint32 index, int sockt, int (*fCallback)(int sockid, void *pArg), void *pArg1);

#endif 

