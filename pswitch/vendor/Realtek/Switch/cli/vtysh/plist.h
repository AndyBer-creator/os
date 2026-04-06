
#ifndef _QUAGGA_PLIST_H
#define _QUAGGA_PLIST_H

#define AFI_ORF_PREFIX 65535

enum prefix_list_type
{
  PREFIX_DENY,
  PREFIX_PERMIT,
};

enum prefix_name_type
{
  PREFIX_TYPE_STRING,
  PREFIX_TYPE_NUMBER
};

struct prefix_list
{
  char *name;
  char *desc;

  struct prefix_master *master;

  enum prefix_name_type type;

  int count;
  int rangecount;

  struct prefix_list_entry *head;
  struct prefix_list_entry *tail;

  struct prefix_list *next;
  struct prefix_list *prev;
};

struct orf_prefix
{
  u_int32_t seq;
  u_char ge;
  u_char le;
  struct prefix p;
};

extern void prefix_list_init (void);
extern void prefix_list_reset (void);
extern void prefix_list_add_hook (void (*func) (struct prefix_list *));
extern void prefix_list_delete_hook (void (*func) (struct prefix_list *));

extern struct prefix_list *prefix_list_lookup (afi_t, const char *);
extern enum prefix_list_type prefix_list_apply (struct prefix_list *, void *);

extern struct stream * prefix_bgp_orf_entry (struct stream *,
                                             struct prefix_list *,
                                             u_char, u_char, u_char);
extern int prefix_bgp_orf_set (char *, afi_t, struct orf_prefix *, int, int);
extern void prefix_bgp_orf_remove_all (char *);
extern int prefix_bgp_show_prefix_list (struct vty *, afi_t, char *);

#endif 
