
#ifndef _ZEBRA_ROUTEMAP_H
#define _ZEBRA_ROUTEMAP_H

enum route_map_type
{
  RMAP_PERMIT,
  RMAP_DENY,
  RMAP_ANY
};

typedef enum
{
  RMAP_MATCH,
  RMAP_DENYMATCH,
  RMAP_NOMATCH,
  RMAP_ERROR,
  RMAP_OKAY
} route_map_result_t;

typedef enum
{
  RMAP_RIP,
  RMAP_RIPNG,
  RMAP_OSPF,
  RMAP_OSPF6,
  RMAP_BGP,
  RMAP_ZEBRA
} route_map_object_t;

typedef enum
{
  RMAP_EXIT,
  RMAP_GOTO,
  RMAP_NEXT
} route_map_end_t;

typedef enum
{
  RMAP_EVENT_SET_ADDED,
  RMAP_EVENT_SET_DELETED,
  RMAP_EVENT_SET_REPLACED,
  RMAP_EVENT_MATCH_ADDED,
  RMAP_EVENT_MATCH_DELETED,
  RMAP_EVENT_MATCH_REPLACED,
  RMAP_EVENT_INDEX_ADDED,
  RMAP_EVENT_INDEX_DELETED
} route_map_event_t;

#define RMAP_RECURSION_LIMIT      10

struct route_map_rule_cmd
{
  
  const char *str;

  route_map_result_t (*func_apply)(void *, struct prefix *,
                   route_map_object_t, void *);

  void *(*func_compile)(const char *);

  void (*func_free)(void *);
};

enum
{
  
  RMAP_RULE_MISSING = 1,

  RMAP_COMPILE_ERROR
};

struct route_map_rule_list
{
  struct route_map_rule *head;
  struct route_map_rule *tail;
};

struct route_map_index
{
  struct route_map *map;
  char *description;

  int pref;

  enum route_map_type type;

  route_map_end_t exitpolicy;

  int nextpref;

  char *nextrm;

  struct route_map_rule_list match_list;
  struct route_map_rule_list set_list;

  struct route_map_index *next;
  struct route_map_index *prev;
};

struct route_map
{
  
  char *name;

  struct route_map_index *head;
  struct route_map_index *tail;

  struct route_map *next;
  struct route_map *prev;
};

extern void route_map_init (void);
extern void route_map_init_vty (void);
extern void route_map_finish (void);

extern int route_map_add_match (struct route_map_index *index,
                        const char *match_name,
                        const char *match_arg);

extern int route_map_delete_match (struct route_map_index *index,
                       const char *match_name,
                       const char *match_arg);

extern int route_map_add_set (struct route_map_index *index,
                      const char *set_name,
                      const char *set_arg);

extern int route_map_delete_set (struct route_map_index *index,
                                 const char *set_name,
                                 const char *set_arg);

extern void route_map_install_match (struct route_map_rule_cmd *cmd);

extern void route_map_install_set (struct route_map_rule_cmd *cmd);

extern struct route_map * route_map_lookup_by_name (const char *name);

extern route_map_result_t route_map_apply (struct route_map *map,
                                           struct prefix *,
                                           route_map_object_t object_type,
                                           void *object);

extern void route_map_add_hook (void (*func) (const char *));
extern void route_map_delete_hook (void (*func) (const char *));
extern void route_map_event_hook (void (*func) (route_map_event_t, const char *));

#endif 
