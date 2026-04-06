
#ifndef _ZEBRA_HASH_H
#define _ZEBRA_HASH_H

#define HASHTABSIZE     1024

struct hash_backet
{
  
  struct hash_backet *next;

  unsigned int key;

  void *data;
};

struct hash
{
  
  struct hash_backet **index;

  unsigned int size;

  unsigned int (*hash_key) (void *);

  int (*hash_cmp) (const void *, const void *);

  unsigned long count;
};

extern struct hash *hash_create (unsigned int (*) (void *),
                 int (*) (const void *, const void *));
extern struct hash *hash_create_size (unsigned int, unsigned int (*) (void *),
                                             int (*) (const void *, const void *));

extern void *hash_get (struct hash *, void *, void * (*) (void *));
extern void *hash_alloc_intern (void *);
extern void *hash_lookup (struct hash *, void *);
extern void *hash_release (struct hash *, void *);

extern void hash_iterate (struct hash *,
           void (*) (struct hash_backet *, void *), void *);

extern void hash_clean (struct hash *, void (*) (void *));
extern void hash_free (struct hash *);

#endif 
