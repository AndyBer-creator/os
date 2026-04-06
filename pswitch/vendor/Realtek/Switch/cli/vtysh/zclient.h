
#ifndef _ZEBRA_ZCLIENT_H
#define _ZEBRA_ZCLIENT_H

#include "if.h"

#define ZEBRA_MAX_PACKET_SIZ          4096

#define ZEBRA_HEADER_SIZE             6

struct zclient
{
  
  int sock;

  int enable;

  int fail;

  struct stream *ibuf;

  struct stream *obuf;

  struct buffer *wb;

  struct thread *t_read;
  struct thread *t_connect;

  struct thread *t_write;

  u_char redist_default;
  u_char redist[ZEBRA_ROUTE_MAX];

  u_char default_information;

  int (*router_id_update) (int, struct zclient *, uint16_t);
  int (*interface_add) (int, struct zclient *, uint16_t);
  int (*interface_delete) (int, struct zclient *, uint16_t);
  int (*interface_up) (int, struct zclient *, uint16_t);
  int (*interface_down) (int, struct zclient *, uint16_t);
  int (*interface_address_add) (int, struct zclient *, uint16_t);
  int (*interface_address_delete) (int, struct zclient *, uint16_t);
  int (*ipv4_route_add) (int, struct zclient *, uint16_t);
  int (*ipv4_route_delete) (int, struct zclient *, uint16_t);
  int (*ipv6_route_add) (int, struct zclient *, uint16_t);
  int (*ipv6_route_delete) (int, struct zclient *, uint16_t);
};

#define ZAPI_MESSAGE_NEXTHOP  0x01
#define ZAPI_MESSAGE_IFINDEX  0x02
#define ZAPI_MESSAGE_DISTANCE 0x04
#define ZAPI_MESSAGE_METRIC   0x08

struct zserv_header
{
  uint16_t length;
  uint8_t marker;    
  uint8_t version;
#define ZSERV_VERSION    1
  uint16_t command;
};

struct zapi_ipv4
{
  u_char type;

  u_char flags;

  u_char message;

  u_char nexthop_num;
  struct in_addr **nexthop;

  u_char ifindex_num;
  unsigned int *ifindex;

  u_char distance;

  u_int32_t metric;
};

extern struct zclient *zclient_new (void);
extern void zclient_init (struct zclient *, int);
extern int zclient_start (struct zclient *);
extern void zclient_stop (struct zclient *);
extern void zclient_reset (struct zclient *);
extern void zclient_free (struct zclient *);

extern int zclient_socket (void);

extern int zclient_socket_un (const char *);

extern int zebra_redistribute_send (int command, struct zclient *, int type);

extern void zclient_redistribute (int command, struct zclient *, int type);

extern void zclient_redistribute_default (int command, struct zclient *);

extern int zclient_send_message(struct zclient *);

extern void zclient_create_header (struct stream *, uint16_t);

extern struct interface *zebra_interface_add_read (struct stream *);
extern struct interface *zebra_interface_state_read (struct stream *s);
extern struct connected *zebra_interface_address_read (int, struct stream *);
extern void zebra_interface_if_set_value (struct stream *, struct interface *);
extern void zebra_router_id_update_read (struct stream *s, struct prefix *rid);
extern int zapi_ipv4_route (u_char, struct zclient *, struct prefix_ipv4 *,
                            struct zapi_ipv4 *);

#ifdef HAVE_IPV6

struct zapi_ipv6
{
  u_char type;

  u_char flags;

  u_char message;

  u_char nexthop_num;
  struct in6_addr **nexthop;

  u_char ifindex_num;
  unsigned int *ifindex;

  u_char distance;

  u_int32_t metric;
};

extern int zapi_ipv6_route (u_char cmd, struct zclient *zclient,
                     struct prefix_ipv6 *p, struct zapi_ipv6 *api);
#endif 

#endif 
