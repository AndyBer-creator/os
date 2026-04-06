
#ifndef _ZEBRA_PREFIX_H
#define _ZEBRA_PREFIX_H

#include "sockunion.h"

struct prefix
{
  u_char family;
  u_char prefixlen;
  union
  {
    u_char prefix;
    struct in_addr prefix4;
#ifdef HAVE_IPV6
    struct in6_addr prefix6;
#endif 
    struct
    {
      struct in_addr id;
      struct in_addr adv_router;
    } lp;
    u_char val[8];
  } u __attribute__ ((aligned (8)));
};

struct prefix_ipv4
{
  u_char family;
  u_char prefixlen;
  struct in_addr prefix __attribute__ ((aligned (8)));
};

#ifdef HAVE_IPV6
struct prefix_ipv6
{
  u_char family;
  u_char prefixlen;
  struct in6_addr prefix __attribute__ ((aligned (8)));
};
#endif 

struct prefix_ls
{
  u_char family;
  u_char prefixlen;
  struct in_addr id __attribute__ ((aligned (8)));
  struct in_addr adv_router;
};

struct prefix_rd
{
  u_char family;
  u_char prefixlen;
  u_char val[8] __attribute__ ((aligned (8)));
};

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif 

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif 

#ifndef INET6_BUFSIZ
#define INET6_BUFSIZ 51
#endif 

#define IPV4_MAX_BYTELEN    4
#define IPV4_MAX_BITLEN    32
#define IPV4_MAX_PREFIXLEN 32
#define IPV4_ADDR_CMP(D,S)   memcmp ((D), (S), IPV4_MAX_BYTELEN)
#define IPV4_ADDR_SAME(D,S)  (memcmp ((D), (S), IPV4_MAX_BYTELEN) == 0)
#define IPV4_ADDR_COPY(D,S)  memcpy ((D), (S), IPV4_MAX_BYTELEN)

#define IPV4_NET0(a)    ((((u_int32_t) (a)) & 0xff000000) == 0x00000000)
#define IPV4_NET127(a)  ((((u_int32_t) (a)) & 0xff000000) == 0x7f000000)
#define IPV4_LINKLOCAL(a) ((((u_int32_t) (a)) & 0xffff0000) == 0xa9fe0000)

#define IPV6_MAX_BYTELEN    16
#define IPV6_MAX_BITLEN    128
#define IPV6_MAX_PREFIXLEN 128
#define IPV6_ADDR_CMP(D,S)   memcmp ((D), (S), IPV6_MAX_BYTELEN)
#define IPV6_ADDR_SAME(D,S)  (memcmp ((D), (S), IPV6_MAX_BYTELEN) == 0)
#define IPV6_ADDR_COPY(D,S)  memcpy ((D), (S), IPV6_MAX_BYTELEN)

#define PSIZE(a) (((a) + 7) / (8))

#define PREFIX_FAMILY(p)  ((p)->family)

static inline unsigned int
prefix_bit (const u_char *prefix, const u_char prefixlen)
{
  unsigned int offset = prefixlen / 8;
  unsigned int shift  = 7 - (prefixlen % 8);

  return (prefix[offset] >> shift) & 1;
}

static inline unsigned int
prefix6_bit (const struct in6_addr *prefix, const u_char prefixlen)
{
  return prefix_bit((const u_char *) &prefix->s6_addr, prefixlen);
}

extern int afi2family (int);
extern int family2afi (int);

extern struct prefix *prefix_new (void);
extern void prefix_free (struct prefix *);
extern const char *prefix_family_str (const struct prefix *);
extern int prefix_blen (const struct prefix *);
extern int str2prefix (const char *, struct prefix *);
extern int prefix2str (const struct prefix *, char *, int);
extern int prefix_match (const struct prefix *, const struct prefix *);
extern int prefix_same (const struct prefix *, const struct prefix *);
extern int prefix_cmp (const struct prefix *, const struct prefix *);
extern void prefix_copy (struct prefix *dest, const struct prefix *src);
extern void apply_mask (struct prefix *);

extern struct prefix *sockunion2prefix (const union sockunion *dest,
                                        const union sockunion *mask);
extern struct prefix *sockunion2hostprefix (const union sockunion *);

extern struct prefix_ipv4 *prefix_ipv4_new (void);
extern void prefix_ipv4_free (struct prefix_ipv4 *);
extern int str2prefix_ipv4 (const char *, struct prefix_ipv4 *);
extern void apply_mask_ipv4 (struct prefix_ipv4 *);

#define PREFIX_COPY_IPV4(DST, SRC)    \
    *((struct prefix_ipv4 *)(DST)) = *((const struct prefix_ipv4 *)(SRC));

extern int prefix_ipv4_any (const struct prefix_ipv4 *);
extern void apply_classful_mask_ipv4 (struct prefix_ipv4 *);

extern u_char ip_masklen (struct in_addr);
extern void masklen2ip (int, struct in_addr *);

extern in_addr_t ipv4_network_addr (in_addr_t hostaddr, int masklen);

extern in_addr_t ipv4_broadcast_addr (in_addr_t hostaddr, int masklen);

extern int netmask_str2prefix_str (const char *, const char *, char *);

#ifdef HAVE_IPV6
extern struct prefix_ipv6 *prefix_ipv6_new (void);
extern void prefix_ipv6_free (struct prefix_ipv6 *);
extern int str2prefix_ipv6 (const char *, struct prefix_ipv6 *);
extern void apply_mask_ipv6 (struct prefix_ipv6 *);

#define PREFIX_COPY_IPV6(DST, SRC)    \
    *((struct prefix_ipv6 *)(DST)) = *((const struct prefix_ipv6 *)(SRC));

extern int ip6_masklen (struct in6_addr);
extern void masklen2ip6 (int, struct in6_addr *);

extern void str2in6_addr (const char *, struct in6_addr *);
extern const char *inet6_ntoa (struct in6_addr);

#endif 

extern int all_digit (const char *);

#endif 
