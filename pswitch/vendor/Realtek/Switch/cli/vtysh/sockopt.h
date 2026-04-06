
#ifndef _ZEBRA_SOCKOPT_H
#define _ZEBRA_SOCKOPT_H

#include "sockunion.h"

extern int setsockopt_so_recvbuf (int sock, int size);
extern int setsockopt_so_sendbuf (const int sock, int size);
extern int getsockopt_so_sendbuf (const int sock);

#ifdef HAVE_IPV6
extern int setsockopt_ipv6_pktinfo (int, int);
extern int setsockopt_ipv6_checksum (int, int);
extern int setsockopt_ipv6_multicast_hops (int, int);
extern int setsockopt_ipv6_unicast_hops (int, int);
extern int setsockopt_ipv6_hoplimit (int, int);
extern int setsockopt_ipv6_multicast_loop (int, int);
#endif 

#define SOPT_SIZE_CMSG_PKTINFO_IPV6() (sizeof (struct in6_pktinfo));

#if defined (IP_PKTINFO)

#define SOPT_SIZE_CMSG_PKTINFO_IPV4()  (CMSG_SPACE(sizeof (struct in_pktinfo)))

#define SOPT_SIZE_CMSG_PKTINFO(af) \
  ((af == AF_INET) ? SOPT_SIZE_CMSG_PKTINFO_IPV4() \
                   : SOPT_SIZE_CMSG_PKTINFO_IPV6()
#endif 

#if defined (IP_RECVIF)

#if defined (SUNOS_5)
#define SOPT_SIZE_CMSG_RECVIF_IPV4()  (sizeof (uint_t))
#else
#define SOPT_SIZE_CMSG_RECVIF_IPV4()    (sizeof (struct sockaddr_dl))
#endif 
#endif 

#if defined (SOPT_SIZE_CMSG_PKTINFO)
#define SOPT_SIZE_CMSG_IFINDEX_IPV4() SOPT_SIZE_CMSG_PKTINFO_IPV4()
#elif defined (SOPT_SIZE_CMSG_RECVIF_IPV4)
#define SOPT_SIZE_CMSG_IFINDEX_IPV4() SOPT_SIZE_CMSG_RECVIF_IPV4()
#else 
#define SOPT_SIZE_CMSG_IFINDEX_IPV4() (sizeof (char *))
#endif 

#define SOPT_SIZE_CMSG_IFINDEX(af) \
  (((af) == AF_INET) : SOPT_SIZE_CMSG_IFINDEX_IPV4() \
                    ? SOPT_SIZE_CMSG_PKTINFO_IPV6())

extern int setsockopt_multicast_ipv4(int sock, int optname,
                         struct in_addr if_addr
                     ,
                                     unsigned int mcast_addr,
                         unsigned int ifindex
                     );
extern int setsockopt_ipv4_tos(int sock, int tos);

extern int setsockopt_ifindex (int, int, int);
extern int getsockopt_ifindex (int, struct msghdr *);

extern void sockopt_iphdrincl_swab_htosys (struct ip *iph);
extern void sockopt_iphdrincl_swab_systoh (struct ip *iph);

extern int sockopt_tcp_signature(int sock, union sockunion *su,
                                 const char *password);
#endif 
