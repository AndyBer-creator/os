
#ifndef _ZEBRA_DISTRIBUTE_H
#define _ZEBRA_DISTRIBUTE_H

enum frr_distribute_type
{
  DISTRIBUTE_IN,
  DISTRIBUTE_OUT,
  DISTRIBUTE_MAX
};

enum distribute_type
{
  DISTRIBUTE_V4_IN,
  DISTRIBUTE_V6_IN,
  DISTRIBUTE_V4_OUT,
  DISTRIBUTE_V6_OUT,
  DISTRIBUTE_MAX_ALL
};

struct distribute
{
  
  char *ifname;

  char *list[DISTRIBUTE_MAX];

  char *prefix[DISTRIBUTE_MAX];
};

extern void distribute_list_init (int);
extern void distribute_list_reset (void);
extern void distribute_list_add_hook (void (*) (struct distribute *));
extern void distribute_list_delete_hook (void (*) (struct distribute *));
extern struct distribute *distribute_lookup (const char *);
extern int config_write_distribute (struct vty *);
extern int config_show_distribute (struct vty *);

extern enum filter_type distribute_apply_in (struct interface *, struct prefix *);
extern enum filter_type distribute_apply_out (struct interface *, struct prefix *);

#endif 
