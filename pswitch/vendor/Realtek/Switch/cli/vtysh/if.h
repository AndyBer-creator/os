
#ifndef _ZEBRA_IF_H
#define _ZEBRA_IF_H

#include "linklist.h"

#define INTERFACE_NAMSIZ      20
#define INTERFACE_HWADDR_MAX  20

#ifdef HAVE_PROC_NET_DEV
struct if_stats
{
  unsigned long rx_packets;   
  unsigned long tx_packets;   
  unsigned long rx_bytes;     
  unsigned long tx_bytes;     
  unsigned long rx_errors;    
  unsigned long tx_errors;    
  unsigned long rx_dropped;   
  unsigned long tx_dropped;   
  unsigned long rx_multicast; 
  unsigned long rx_compressed;
  unsigned long tx_compressed;
  unsigned long collisions;

  unsigned long rx_length_errors;
  unsigned long rx_over_errors;       
  unsigned long rx_crc_errors;        
  unsigned long rx_frame_errors;      
  unsigned long rx_fifo_errors;       
  unsigned long rx_missed_errors;     
  
  unsigned long tx_aborted_errors;
  unsigned long tx_carrier_errors;
  unsigned long tx_fifo_errors;
  unsigned long tx_heartbeat_errors;
  unsigned long tx_window_errors;
};
#endif 

struct interface
{
  
  char name[INTERFACE_NAMSIZ + 1];

  unsigned int ifindex;
#define IFINDEX_INTERNAL    0

  u_char status;
#define ZEBRA_INTERFACE_ACTIVE     (1 << 0)
#define ZEBRA_INTERFACE_SUB        (1 << 1)
#define ZEBRA_INTERFACE_LINKDETECTION (1 << 2)

  uint64_t flags;

  int metric;

  unsigned int mtu;    
  unsigned int mtu6;   

#ifdef HAVE_STRUCT_SOCKADDR_DL
  struct sockaddr_dl sdl;
#else
  unsigned short hw_type;
  u_char hw_addr[INTERFACE_HWADDR_MAX];
  int hw_addr_len;
#endif 

  unsigned int bandwidth;

  char *desc;

  void *distribute_in;
  void *distribute_out;

  struct list *connected;

  void *info;

#ifdef HAVE_PROC_NET_DEV
  struct if_stats stats;
#endif 
#ifdef HAVE_NET_RT_IFLIST
  struct if_data stats;
#endif 
};

struct connected
{
  
  struct interface *ifp;

  u_char conf;
#define ZEBRA_IFC_REAL         (1 << 0)
#define ZEBRA_IFC_CONFIGURED   (1 << 1)

  u_char flags;
#define ZEBRA_IFA_SECONDARY    (1 << 0)
#define ZEBRA_IFA_PEER         (1 << 1)

  struct prefix *address;

  struct prefix *destination;

  char *label;
};

#define CONNECTED_PEER(C) CHECK_FLAG((C)->flags, ZEBRA_IFA_PEER)

#define CONNECTED_PREFIX(C) \
    (CONNECTED_PEER(C) ? (C)->destination : (C)->address)

#define CONNECTED_ID(C)    \
    ((CONNECTED_PEER(C) && !prefix_match((C)->destination, (C)->address)) ?\
     (C)->destination : (C)->address)

#define IF_NEW_HOOK   0
#define IF_DELETE_HOOK 1

#ifndef IFF_NOTRAILERS
#define IFF_NOTRAILERS 0x0
#endif 
#ifndef IFF_OACTIVE
#define IFF_OACTIVE 0x0
#endif 
#ifndef IFF_SIMPLEX
#define IFF_SIMPLEX 0x0
#endif 
#ifndef IFF_LINK0
#define IFF_LINK0 0x0
#endif 
#ifndef IFF_LINK1
#define IFF_LINK1 0x0
#endif 
#ifndef IFF_LINK2
#define IFF_LINK2 0x0
#endif 
#ifndef IFF_NOXMIT
#define IFF_NOXMIT 0x0
#endif 
#ifndef IFF_NORTEXCH
#define IFF_NORTEXCH 0x0
#endif 
#ifndef IFF_IPV4
#define IFF_IPV4 0x0
#endif 
#ifndef IFF_IPV6
#define IFF_IPV6 0x0
#endif 
#ifndef IFF_VIRTUAL
#define IFF_VIRTUAL 0x0
#endif 

extern int if_cmp_func (struct interface *, struct interface *);
extern struct interface *if_create (const char *name, int namelen);
extern struct interface *if_lookup_by_index (unsigned int);
extern struct interface *if_lookup_exact_address (struct in_addr);
extern struct interface *if_lookup_address (struct in_addr);

extern struct interface *if_lookup_by_name (const char *ifname);
extern struct interface *if_get_by_name (const char *ifname);

extern struct interface *if_lookup_by_name_len(const char *ifname,
                           size_t namelen);
extern struct interface *if_get_by_name_len(const char *ifname, size_t namelen);

extern void if_delete_retain (struct interface *);

extern void if_delete (struct interface *);

extern int if_is_up (struct interface *);
extern int if_is_running (struct interface *);
extern int if_is_operative (struct interface *);
extern int if_is_loopback (struct interface *);
extern int if_is_broadcast (struct interface *);
extern int if_is_pointopoint (struct interface *);
extern int if_is_multicast (struct interface *);
extern void if_add_hook (int, int (*)(struct interface *));
extern void if_init (void);
extern void if_dump_all (void);
extern const char *if_flag_dump(unsigned long);

extern const char *ifindex2ifname (unsigned int);

extern unsigned int ifname2ifindex(const char *ifname);

extern struct connected *connected_new (void);
extern void connected_free (struct connected *);
extern void connected_add (struct interface *, struct connected *);
extern struct connected  *connected_add_by_prefix (struct interface *,
                                            struct prefix *,
                                            struct prefix *);
extern struct connected  *connected_delete_by_prefix (struct interface *,
                                               struct prefix *);
extern struct connected  *connected_lookup_address (struct interface *,
                                             struct in_addr);

#ifndef HAVE_IF_NAMETOINDEX
extern unsigned int if_nametoindex (const char *);
#endif
#ifndef HAVE_IF_INDEXTONAME
extern char *if_indextoname (unsigned int, char *);
#endif

extern struct list *iflist;
extern struct cmd_element interface_desc_cmd;
extern struct cmd_element no_interface_desc_cmd;
extern struct cmd_element interface_cmd;
extern struct cmd_element no_interface_cmd;
extern struct cmd_element interface_pseudo_cmd;
extern struct cmd_element no_interface_pseudo_cmd;
extern struct cmd_element show_address_cmd;

#endif 
