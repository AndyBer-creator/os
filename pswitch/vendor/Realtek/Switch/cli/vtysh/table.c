
#include <zebra.h>

#include "prefix.h"
#include "table.h"
#include "memory.h"
#include "sockunion.h"

void route_node_delete (struct route_node *);
void route_table_free (struct route_table *);

struct route_table *
route_table_init (void)
{
  struct route_table *rt;

  rt = XCALLOC (MTYPE_ROUTE_TABLE, sizeof (struct route_table));
  return rt;
}

void
route_table_finish (struct route_table *rt)
{
  route_table_free (rt);
}

static struct route_node *
route_node_new (void)
{
  struct route_node *node;
  node = XCALLOC (MTYPE_ROUTE_NODE, sizeof (struct route_node));
  return node;
}

static struct route_node *
route_node_set (struct route_table *table, struct prefix *prefix)
{
  struct route_node *node;

  node = route_node_new ();

  prefix_copy (&node->p, prefix);
  node->table = table;

  return node;
}

static void
route_node_free (struct route_node *node)
{
  XFREE (MTYPE_ROUTE_NODE, node);
}

void
route_table_free (struct route_table *rt)
{
  struct route_node *tmp_node;
  struct route_node *node;

  if (rt == NULL)
    return;

  node = rt->top;

  while (node)
    {
      if (node->l_left)
    {
      node = node->l_left;
      continue;
    }

      if (node->l_right)
    {
      node = node->l_right;
      continue;
    }

      tmp_node = node;
      node = node->parent;

      if (node != NULL)
    {
      if (node->l_left == tmp_node)
        node->l_left = NULL;
      else
        node->l_right = NULL;

      route_node_free (tmp_node);
    }
      else
    {
      route_node_free (tmp_node);
      break;
    }
    }

  XFREE (MTYPE_ROUTE_TABLE, rt);
  return;
}

static const u_char maskbit[] =
{
  0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff
};

static void
route_common (struct prefix *n, struct prefix *p, struct prefix *new)
{
  int i;
  u_char diff;
  u_char mask;

  u_char *np = (u_char *)&n->u.prefix;
  u_char *pp = (u_char *)&p->u.prefix;
  u_char *newp = (u_char *)&new->u.prefix;

  for (i = 0; i < p->prefixlen / 8; i++)
    {
      if (np[i] == pp[i])
    newp[i] = np[i];
      else
    break;
    }

  new->prefixlen = i * 8;

  if (new->prefixlen != p->prefixlen)
    {
      diff = np[i] ^ pp[i];
      mask = 0x80;
      while (new->prefixlen < p->prefixlen && !(mask & diff))
    {
      mask >>= 1;
      new->prefixlen++;
    }
      newp[i] = np[i] & maskbit[new->prefixlen % 8];
    }
}

static void
set_link (struct route_node *node, struct route_node *new)
{
  unsigned int bit = prefix_bit (&new->p.u.prefix, node->p.prefixlen);

  node->link[bit] = new;
  new->parent = node;
}

struct route_node *
route_lock_node (struct route_node *node)
{
  node->lock++;
  return node;
}

void
route_unlock_node (struct route_node *node)
{
  node->lock--;

  if (node->lock == 0)
    route_node_delete (node);
}

struct route_node *
route_node_match (const struct route_table *table, const struct prefix *p)
{
  struct route_node *node;
  struct route_node *matched;

  matched = NULL;
  node = table->top;

  while (node && node->p.prefixlen <= p->prefixlen &&
     prefix_match (&node->p, p))
    {
      if (node->info)
    matched = node;
      node = node->link[prefix_bit(&p->u.prefix, node->p.prefixlen)];
    }

  if (matched)
    return route_lock_node (matched);

  return NULL;
}

struct route_node *
route_node_match_ipv4 (const struct route_table *table,
               const struct in_addr *addr)
{
  struct prefix_ipv4 p;

  memset (&p, 0, sizeof (struct prefix_ipv4));
  p.family = AF_INET;
  p.prefixlen = IPV4_MAX_PREFIXLEN;
  p.prefix = *addr;

  return route_node_match (table, (struct prefix *) &p);
}

#ifdef HAVE_IPV6
struct route_node *
route_node_match_ipv6 (const struct route_table *table,
               const struct in6_addr *addr)
{
  struct prefix_ipv6 p;

  memset (&p, 0, sizeof (struct prefix_ipv6));
  p.family = AF_INET6;
  p.prefixlen = IPV6_MAX_PREFIXLEN;
  p.prefix = *addr;

  return route_node_match (table, (struct prefix *) &p);
}
#endif 

struct route_node *
route_node_lookup (struct route_table *table, struct prefix *p)
{
  struct route_node *node;

  node = table->top;

  while (node && node->p.prefixlen <= p->prefixlen &&
     prefix_match (&node->p, p))
    {
      if (node->p.prefixlen == p->prefixlen && node->info)
    return route_lock_node (node);

      node = node->link[prefix_bit(&p->u.prefix, node->p.prefixlen)];
    }

  return NULL;
}

struct route_node *
route_node_get (struct route_table *table, struct prefix *p)
{
  struct route_node *new;
  struct route_node *node;
  struct route_node *match;

  match = NULL;
  node = table->top;
  while (node && node->p.prefixlen <= p->prefixlen &&
     prefix_match (&node->p, p))
    {
      if (node->p.prefixlen == p->prefixlen)
    {
      route_lock_node (node);
      return node;
    }
      match = node;
      node = node->link[prefix_bit(&p->u.prefix, node->p.prefixlen)];
    }

  if (node == NULL)
    {
      new = route_node_set (table, p);
      if (match)
    set_link (match, new);
      else
    table->top = new;
    }
  else
    {
      new = route_node_new ();
      route_common (&node->p, p, &new->p);
      new->p.family = p->family;
      new->table = table;
      set_link (new, node);

      if (match)
    set_link (match, new);
      else
    table->top = new;

      if (new->p.prefixlen != p->prefixlen)
    {
      match = new;
      new = route_node_set (table, p);
      set_link (match, new);
    }
    }
  route_lock_node (new);

  return new;
}

void
route_node_delete (struct route_node *node)
{
  struct route_node *child;
  struct route_node *parent;

  assert (node->lock == 0);
  assert (node->info == NULL);

  if (node->l_left && node->l_right)
    return;

  if (node->l_left)
    child = node->l_left;
  else
    child = node->l_right;

  parent = node->parent;

  if (child)
    child->parent = parent;

  if (parent)
    {
      if (parent->l_left == node)
    parent->l_left = child;
      else
    parent->l_right = child;
    }
  else
    node->table->top = child;

  route_node_free (node);

  if (parent && parent->lock == 0)
    route_node_delete (parent);
}

struct route_node *
route_top (struct route_table *table)
{
  
  if (table->top == NULL)
    return NULL;

  route_lock_node (table->top);
  return table->top;
}

struct route_node *
route_next (struct route_node *node)
{
  struct route_node *next;
  struct route_node *start;

  if (node->l_left)
    {
      next = node->l_left;
      route_lock_node (next);
      route_unlock_node (node);
      return next;
    }
  if (node->l_right)
    {
      next = node->l_right;
      route_lock_node (next);
      route_unlock_node (node);
      return next;
    }

  start = node;
  while (node->parent)
    {
      if (node->parent->l_left == node && node->parent->l_right)
    {
      next = node->parent->l_right;
      route_lock_node (next);
      route_unlock_node (start);
      return next;
    }
      node = node->parent;
    }
  route_unlock_node (start);
  return NULL;
}

struct route_node *
route_next_until (struct route_node *node, struct route_node *limit)
{
  struct route_node *next;
  struct route_node *start;

  if (node->l_left)
    {
      next = node->l_left;
      route_lock_node (next);
      route_unlock_node (node);
      return next;
    }
  if (node->l_right)
    {
      next = node->l_right;
      route_lock_node (next);
      route_unlock_node (node);
      return next;
    }

  start = node;
  while (node->parent && node != limit)
    {
      if (node->parent->l_left == node && node->parent->l_right)
    {
      next = node->parent->l_right;
      route_lock_node (next);
      route_unlock_node (start);
      return next;
    }
      node = node->parent;
    }
  route_unlock_node (start);
  return NULL;
}
