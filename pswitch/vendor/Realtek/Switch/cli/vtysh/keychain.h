
#ifndef _ZEBRA_KEYCHAIN_H
#define _ZEBRA_KEYCHAIN_H

struct keychain
{
  char *name;

  struct list *key;
};

struct key_range
{
  time_t start;
  time_t end;

  u_char duration;
};

struct key
{
  u_int32_t index;

  char *string;

  struct key_range send;
  struct key_range accept;
};

extern void keychain_init (void);
extern struct keychain *keychain_lookup (const char *);
extern struct key *key_lookup_for_accept (const struct keychain *, u_int32_t);
extern struct key *key_match_for_accept (const struct keychain *, const char *);
extern struct key *key_lookup_for_send (const struct keychain *);

#endif 
