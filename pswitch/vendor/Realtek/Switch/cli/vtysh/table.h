
#ifndef _ZEBRA_TABLE_H
#define _ZEBRA_TABLE_H

struct route_table
{
  struct route_node *top;
};

struct route_node
{
  
  struct prefix p;

  struct route_table *table;
  struct route_node *parent;
  struct route_node *link[2];
#define l_left   link[0]
#define l_right  link[1]

  unsigned int lock;

  void *info;

  void *aggregate;
};

extern struct route_table *route_table_init (void);
extern void route_table_finish (struct route_table *);
extern void route_unlock_node (struct route_node *node);
extern void route_node_delete (struct route_node *node);
extern struct route_node *route_top (struct route_table *);
extern struct route_node *route_next (struct route_node *);
extern struct route_node *route_next_until (struct route_node *,
                                            struct route_node *);
extern struct route_node *route_node_get (struct route_table *,
                                          struct prefix *);
extern struct route_node *route_node_lookup (struct route_table *,
                                             struct prefix *);
extern struct route_node *route_lock_node (struct route_node *node);
extern struct route_node *route_node_match (const struct route_table *,
                                            const struct prefix *);
extern struct route_node *route_node_match_ipv4 (const struct route_table *,
                         const struct in_addr *);
#ifdef HAVE_IPV6
extern struct route_node *route_node_match_ipv6 (const struct route_table *,
                         const struct in6_addr *);
#endif 

#endif 
