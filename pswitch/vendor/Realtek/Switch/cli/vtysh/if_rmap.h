
#ifndef _ZEBRA_IF_RMAP_H
#define _ZEBRA_IF_RMAP_H

enum if_rmap_type
{
  IF_RMAP_IN,
  IF_RMAP_OUT,
  IF_RMAP_MAX
};

struct if_rmap
{
  
  char *ifname;

  char *routemap[IF_RMAP_MAX];
};

extern void if_rmap_init (int);
extern void if_rmap_reset (void);
extern void if_rmap_hook_add (void (*) (struct if_rmap *));
extern void if_rmap_hook_delete (void (*) (struct if_rmap *));
extern struct if_rmap *if_rmap_lookup (const char *);
extern int config_write_if_rmap (struct vty *);

#endif 
