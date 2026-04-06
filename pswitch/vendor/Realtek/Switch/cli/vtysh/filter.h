
#ifndef _ZEBRA_FILTER_H
#define _ZEBRA_FILTER_H

#include "if.h"

enum filter_type
{
  FILTER_DENY,
  FILTER_PERMIT,
  FILTER_DYNAMIC
};

enum access_type
{
  ACCESS_TYPE_STRING,
  ACCESS_TYPE_NUMBER
};

struct access_list
{
  char *name;
  char *remark;

  struct access_master *master;

  enum access_type type;

  struct access_list *next;
  struct access_list *prev;

  struct filter *head;
  struct filter *tail;
};

extern void access_list_init (void);
extern void access_list_reset (void);
extern void access_list_add_hook (void (*func)(struct access_list *));
extern void access_list_delete_hook (void (*func)(struct access_list *));
extern struct access_list *access_list_lookup (afi_t, const char *);
extern enum filter_type access_list_apply (struct access_list *, void *);

#endif 
