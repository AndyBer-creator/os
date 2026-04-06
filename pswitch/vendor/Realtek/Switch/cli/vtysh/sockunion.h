
#ifndef _ZEBRA_SOCKUNION_H
#define _ZEBRA_SOCKUNION_H

#if 0
union sockunion {
  struct sockinet {
    u_char si_len;
    u_char si_family;
    u_short si_port;
  } su_si;
  struct sockaddr_in  su_sin;
  struct sockaddr_in6 su_sin6;
};
#define su_len                su_si.si_len
#define su_family     su_si.si_family
#define su_port               su_si.si_port
#endif 

union sockunion
{
  struct sockaddr sa;
  struct sockaddr_in sin;
#ifdef HAVE_IPV6
  struct sockaddr_in6 sin6;
#endif 
};

enum connect_result
{
  connect_error,
  connect_success,
  connect_in_progress
};

#ifdef HAVE_IPV6
#define AF_INET_UNION AF_INET6
#else
#define AF_INET_UNION AF_INET
#endif

#define SU_ADDRSTRLEN 46

#ifdef KAME
#define    IN6_LINKLOCAL_IFINDEX(a)  ((a).s6_addr[2] << 8 | (a).s6_addr[3])
#define SET_IN6_LINKLOCAL_IFINDEX(a, i) \
  do { \
    (a).s6_addr[2] = ((i) >> 8) & 0xff; \
    (a).s6_addr[3] = (i) & 0xff; \
  } while (0)
#else
#define    IN6_LINKLOCAL_IFINDEX(a)
#define SET_IN6_LINKLOCAL_IFINDEX(a, i)
#endif 

#define sock2ip(X)   (((struct sockaddr_in *)(X))->sin_addr.s_addr)
#ifdef HAVE_IPV6
#define sock2ip6(X)  (((struct sockaddr_in6 *)(X))->sin6_addr.s6_addr)
#endif 

#define sockunion_family(X)  (X)->sa.sa_family

extern int str2sockunion (const char *, union sockunion *);
extern const char *sockunion2str (union sockunion *, char *, size_t);
extern int sockunion_cmp (union sockunion *, union sockunion *);
extern int sockunion_same (union sockunion *, union sockunion *);

extern char *sockunion_su2str (union sockunion *su);
extern union sockunion *sockunion_str2su (const char *str);
extern struct in_addr sockunion_get_in_addr (union sockunion *su);
extern int sockunion_accept (int sock, union sockunion *);
extern int sockunion_stream_socket (union sockunion *);
extern int sockopt_reuseaddr (int);
extern int sockopt_reuseport (int);
extern int sockunion_bind (int sock, union sockunion *,
                           unsigned short, union sockunion *);
extern int sockopt_ttl (int family, int sock, int ttl);
extern int sockunion_socket (union sockunion *su);
extern const char *inet_sutop (union sockunion *su, char *str);
extern enum connect_result sockunion_connect (int fd, union sockunion *su,
                                              unsigned short port,
                                              unsigned int);
extern union sockunion *sockunion_getsockname (int);
extern union sockunion *sockunion_getpeername (int);
extern union sockunion *sockunion_dup (union sockunion *);
extern void sockunion_free (union sockunion *);

#ifndef HAVE_INET_NTOP

#endif 

#ifndef HAVE_INET_PTON
extern int inet_pton (int family, const char *strptr, void *addrptr);
#endif 

#ifndef HAVE_INET_ATON

#endif

#endif 
