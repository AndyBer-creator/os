
#include <zebra.h>

#include "linklist.h"
#include "memory.h"
#include "vector.h"
#include "prefix.h"
#include "routemap.h"
#include "command.h"
#include "vty.h"
#include "log.h"

static vector route_match_vec;

static vector route_set_vec;

struct route_map_rule
{
  
  struct route_map_rule_cmd *cmd;

  char *rule_str;

  void *value;

  struct route_map_rule *next;
  struct route_map_rule *prev;
};

struct route_map_list
{
  struct route_map *head;
  struct route_map *tail;

  void (*add_hook) (const char *);
  void (*delete_hook) (const char *);
  void (*event_hook) (route_map_event_t, const char *);
};

static struct route_map_list route_map_master = { NULL, NULL, NULL, NULL };

static void
route_map_rule_delete (struct route_map_rule_list *,
               struct route_map_rule *);

static void
route_map_index_delete (struct route_map_index *, int);

static struct route_map *
route_map_new (const char *name)
{
  struct route_map *new;

  new =  XCALLOC (MTYPE_ROUTE_MAP, sizeof (struct route_map));
  new->name = XSTRDUP (MTYPE_ROUTE_MAP_NAME, name);
  return new;
}

static struct route_map *
route_map_add (const char *name)
{
  struct route_map *map;
  struct route_map_list *list;

  map = route_map_new (name);
  list = &route_map_master;

  map->next = NULL;
  map->prev = list->tail;
  if (list->tail)
    list->tail->next = map;
  else
    list->head = map;
  list->tail = map;

  if (route_map_master.add_hook)
    (*route_map_master.add_hook) (name);

  return map;
}

static void
route_map_delete (struct route_map *map)
{
  struct route_map_list *list;
  struct route_map_index *index;
  char *name;

  while ((index = map->head) != NULL)
    route_map_index_delete (index, 0);

  name = map->name;

  list = &route_map_master;

  if (map->next)
    map->next->prev = map->prev;
  else
    list->tail = map->prev;

  if (map->prev)
    map->prev->next = map->next;
  else
    list->head = map->next;

  XFREE (MTYPE_ROUTE_MAP, map);

  if (route_map_master.delete_hook)
    (*route_map_master.delete_hook) (name);

  if (name)
    XFREE (MTYPE_ROUTE_MAP_NAME, name);

}

struct route_map *
route_map_lookup_by_name (const char *name)
{
  struct route_map *map;

  for (map = route_map_master.head; map; map = map->next)
    if (strcmp (map->name, name) == 0)
      return map;
  return NULL;
}

static struct route_map *
route_map_get (const char *name)
{
  struct route_map *map;

  map = route_map_lookup_by_name (name);
  if (map == NULL)
    map = route_map_add (name);
  return map;
}

static const char *
route_map_type_str (enum route_map_type type)
{
  switch (type)
    {
    case RMAP_PERMIT:
      return "permit";
      break;
    case RMAP_DENY:
      return "deny";
      break;
    default:
      return "";
      break;
    }
}

static int
route_map_empty (struct route_map *map)
{
  if (map->head == NULL && map->tail == NULL)
    return 1;
  else
    return 0;
}

static void
vty_show_route_map_entry (struct vty *vty, struct route_map *map)
{
  struct route_map_index *index;
  struct route_map_rule *rule;

  if (zlog_default)
    vty_out (vty, "%s:%s", zlog_proto_names[zlog_default->protocol],
             VTY_NEWLINE);

  for (index = map->head; index; index = index->next)
    {
      vty_out (vty, "route-map %s, %s, sequence %d%s",
               map->name, route_map_type_str (index->type),
               index->pref, VTY_NEWLINE);

      if (index->description)
    vty_out (vty, "  Description:%s    %s%s", VTY_NEWLINE,
         index->description, VTY_NEWLINE);

      vty_out (vty, "  Match clauses:%s", VTY_NEWLINE);
      for (rule = index->match_list.head; rule; rule = rule->next)
        vty_out (vty, "    %s %s%s",
                 rule->cmd->str, rule->rule_str, VTY_NEWLINE);

      vty_out (vty, "  Set clauses:%s", VTY_NEWLINE);
      for (rule = index->set_list.head; rule; rule = rule->next)
        vty_out (vty, "    %s %s%s",
                 rule->cmd->str, rule->rule_str, VTY_NEWLINE);

      vty_out (vty, "  Call clause:%s", VTY_NEWLINE);
      if (index->nextrm)
        vty_out (vty, "    Call %s%s", index->nextrm, VTY_NEWLINE);

      vty_out (vty, "  Action:%s", VTY_NEWLINE);
      if (index->exitpolicy == RMAP_GOTO)
        vty_out (vty, "    Goto %d%s", index->nextpref, VTY_NEWLINE);
      else if (index->exitpolicy == RMAP_NEXT)
        vty_out (vty, "    Continue to next entry%s", VTY_NEWLINE);
      else if (index->exitpolicy == RMAP_EXIT)
        vty_out (vty, "    Exit routemap%s", VTY_NEWLINE);
    }
}

static int
vty_show_route_map (struct vty *vty, const char *name)
{
  struct route_map *map;

  if (name)
    {
      map = route_map_lookup_by_name (name);

      if (map)
        {
          vty_show_route_map_entry (vty, map);
          return CMD_SUCCESS;
        }
      else
        {
          vty_out (vty, "%%route-map %s not found%s", name, VTY_NEWLINE);
          return CMD_WARNING;
        }
    }
  else
    {
      for (map = route_map_master.head; map; map = map->next)
    vty_show_route_map_entry (vty, map);
    }
  return CMD_SUCCESS;
}

static struct route_map_index *
route_map_index_new (void)
{
  struct route_map_index *new;

  new =  XCALLOC (MTYPE_ROUTE_MAP_INDEX, sizeof (struct route_map_index));
  new->exitpolicy = RMAP_EXIT; 
  return new;
}

static void
route_map_index_delete (struct route_map_index *index, int notify)
{
  struct route_map_rule *rule;

  while ((rule = index->match_list.head) != NULL)
    route_map_rule_delete (&index->match_list, rule);

  while ((rule = index->set_list.head) != NULL)
    route_map_rule_delete (&index->set_list, rule);

  if (index->next)
    index->next->prev = index->prev;
  else
    index->map->tail = index->prev;

  if (index->prev)
    index->prev->next = index->next;
  else
    index->map->head = index->next;

  if (index->nextrm)
    XFREE (MTYPE_ROUTE_MAP_NAME, index->nextrm);

  if (route_map_master.event_hook && notify)
    (*route_map_master.event_hook) (RMAP_EVENT_INDEX_DELETED,
                    index->map->name);

  XFREE (MTYPE_ROUTE_MAP_INDEX, index);
}

static struct route_map_index *
route_map_index_lookup (struct route_map *map, enum route_map_type type,
            int pref)
{
  struct route_map_index *index;

  for (index = map->head; index; index = index->next)
    if ((index->type == type || type == RMAP_ANY)
    && index->pref == pref)
      return index;
  return NULL;
}

static struct route_map_index *
route_map_index_add (struct route_map *map, enum route_map_type type,
             int pref)
{
  struct route_map_index *index;
  struct route_map_index *point;

  index = route_map_index_new ();
  index->map = map;
  index->type = type;
  index->pref = pref;

  for (point = map->head; point; point = point->next)
    if (point->pref >= pref)
      break;

  if (map->head == NULL)
    {
      map->head = map->tail = index;
    }
  else if (point == NULL)
    {
      index->prev = map->tail;
      map->tail->next = index;
      map->tail = index;
    }
  else if (point == map->head)
    {
      index->next = map->head;
      map->head->prev = index;
      map->head = index;
    }
  else
    {
      index->next = point;
      index->prev = point->prev;
      if (point->prev)
    point->prev->next = index;
      point->prev = index;
    }

  if (route_map_master.event_hook)
    (*route_map_master.event_hook) (RMAP_EVENT_INDEX_ADDED,
                    map->name);

  return index;
}

static struct route_map_index *
route_map_index_get (struct route_map *map, enum route_map_type type,
             int pref)
{
  struct route_map_index *index;

  index = route_map_index_lookup (map, RMAP_ANY, pref);
  if (index && index->type != type)
    {
      
      route_map_index_delete (index, 1);
      index = NULL;
    }
  if (index == NULL)
    index = route_map_index_add (map, type, pref);
  return index;
}

static struct route_map_rule *
route_map_rule_new (void)
{
  struct route_map_rule *new;

  new = XCALLOC (MTYPE_ROUTE_MAP_RULE, sizeof (struct route_map_rule));
  return new;
}

void
route_map_install_match (struct route_map_rule_cmd *cmd)
{
  vector_set (route_match_vec, cmd);
}

void
route_map_install_set (struct route_map_rule_cmd *cmd)
{
  vector_set (route_set_vec, cmd);
}

static struct route_map_rule_cmd *
route_map_lookup_match (const char *name)
{
  unsigned int i;
  struct route_map_rule_cmd *rule;

  for (i = 0; i < vector_active (route_match_vec); i++)
    if ((rule = vector_slot (route_match_vec, i)) != NULL)
      if (strcmp (rule->str, name) == 0)
    return rule;
  return NULL;
}

static struct route_map_rule_cmd *
route_map_lookup_set (const char *name)
{
  unsigned int i;
  struct route_map_rule_cmd *rule;

  for (i = 0; i < vector_active (route_set_vec); i++)
    if ((rule = vector_slot (route_set_vec, i)) != NULL)
      if (strcmp (rule->str, name) == 0)
    return rule;
  return NULL;
}

static void
route_map_rule_add (struct route_map_rule_list *list,
            struct route_map_rule *rule)
{
  rule->next = NULL;
  rule->prev = list->tail;
  if (list->tail)
    list->tail->next = rule;
  else
    list->head = rule;
  list->tail = rule;
}

static void
route_map_rule_delete (struct route_map_rule_list *list,
               struct route_map_rule *rule)
{
  if (rule->cmd->func_free)
    (*rule->cmd->func_free) (rule->value);

  if (rule->rule_str)
    XFREE (MTYPE_ROUTE_MAP_RULE_STR, rule->rule_str);

  if (rule->next)
    rule->next->prev = rule->prev;
  else
    list->tail = rule->prev;
  if (rule->prev)
    rule->prev->next = rule->next;
  else
    list->head = rule->next;

  XFREE (MTYPE_ROUTE_MAP_RULE, rule);
}

static int
rulecmp (const char *dst, const char *src)
{
  if (dst == NULL)
    {
      if (src ==  NULL)
    return 0;
      else
    return 1;
    }
  else
    {
      if (src == NULL)
    return 1;
      else
    return strcmp (dst, src);
    }
  return 1;
}

int
route_map_add_match (struct route_map_index *index, const char *match_name,
                     const char *match_arg)
{
  struct route_map_rule *rule;
  struct route_map_rule *next;
  struct route_map_rule_cmd *cmd;
  void *compile;
  int replaced = 0;

  cmd = route_map_lookup_match (match_name);
  if (cmd == NULL)
    return RMAP_RULE_MISSING;

  if (cmd->func_compile)
    {
      compile= (*cmd->func_compile)(match_arg);
      if (compile == NULL)
    return RMAP_COMPILE_ERROR;
    }
  else
    compile = NULL;

  for (rule = index->match_list.head; rule; rule = next)
    {
      next = rule->next;
      if (rule->cmd == cmd)
    {
      route_map_rule_delete (&index->match_list, rule);
      replaced = 1;
    }
    }

  rule = route_map_rule_new ();
  rule->cmd = cmd;
  rule->value = compile;
  if (match_arg)
    rule->rule_str = XSTRDUP (MTYPE_ROUTE_MAP_RULE_STR, match_arg);
  else
    rule->rule_str = NULL;

  route_map_rule_add (&index->match_list, rule);

  if (route_map_master.event_hook)
    (*route_map_master.event_hook) (replaced ?
                    RMAP_EVENT_MATCH_REPLACED:
                    RMAP_EVENT_MATCH_ADDED,
                    index->map->name);

  return 0;
}

int
route_map_delete_match (struct route_map_index *index, const char *match_name,
                        const char *match_arg)
{
  struct route_map_rule *rule;
  struct route_map_rule_cmd *cmd;

  cmd = route_map_lookup_match (match_name);
  if (cmd == NULL)
    return 1;

  for (rule = index->match_list.head; rule; rule = rule->next)
    if (rule->cmd == cmd &&
    (rulecmp (rule->rule_str, match_arg) == 0 || match_arg == NULL))
      {
    route_map_rule_delete (&index->match_list, rule);
    
    if (route_map_master.event_hook)
      (*route_map_master.event_hook) (RMAP_EVENT_MATCH_DELETED,
                      index->map->name);
    return 0;
      }
  
  return 1;
}

int
route_map_add_set (struct route_map_index *index, const char *set_name,
                   const char *set_arg)
{
  struct route_map_rule *rule;
  struct route_map_rule *next;
  struct route_map_rule_cmd *cmd;
  void *compile;
  int replaced = 0;

  cmd = route_map_lookup_set (set_name);
  if (cmd == NULL)
    return RMAP_RULE_MISSING;

  if (cmd->func_compile)
    {
      compile= (*cmd->func_compile)(set_arg);
      if (compile == NULL)
    return RMAP_COMPILE_ERROR;
    }
  else
    compile = NULL;

  for (rule = index->set_list.head; rule; rule = next)
    {
      next = rule->next;
      if (rule->cmd == cmd)
    {
      route_map_rule_delete (&index->set_list, rule);
      replaced = 1;
    }
    }

  rule = route_map_rule_new ();
  rule->cmd = cmd;
  rule->value = compile;
  if (set_arg)
    rule->rule_str = XSTRDUP (MTYPE_ROUTE_MAP_RULE_STR, set_arg);
  else
    rule->rule_str = NULL;

  route_map_rule_add (&index->set_list, rule);

  if (route_map_master.event_hook)
    (*route_map_master.event_hook) (replaced ?
                    RMAP_EVENT_SET_REPLACED:
                    RMAP_EVENT_SET_ADDED,
                    index->map->name);
  return 0;
}

int
route_map_delete_set (struct route_map_index *index, const char *set_name,
                      const char *set_arg)
{
  struct route_map_rule *rule;
  struct route_map_rule_cmd *cmd;

  cmd = route_map_lookup_set (set_name);
  if (cmd == NULL)
    return 1;

  for (rule = index->set_list.head; rule; rule = rule->next)
    if ((rule->cmd == cmd) &&
         (rulecmp (rule->rule_str, set_arg) == 0 || set_arg == NULL))
      {
        route_map_rule_delete (&index->set_list, rule);
    
    if (route_map_master.event_hook)
      (*route_map_master.event_hook) (RMAP_EVENT_SET_DELETED,
                      index->map->name);
        return 0;
      }
  
  return 1;
}

static route_map_result_t
route_map_apply_match (struct route_map_rule_list *match_list,
                       struct prefix *prefix, route_map_object_t type,
                       void *object)
{
  route_map_result_t ret = RMAP_NOMATCH;
  struct route_map_rule *match;

  if (!match_list->head)
    ret = RMAP_MATCH;
  else
    {
      for (match = match_list->head; match; match = match->next)
        {
          
          ret = (*match->cmd->func_apply) (match->value, prefix,
                                           type, object);
          if (ret != RMAP_MATCH)
            return ret;
        }
    }
  return ret;
}

route_map_result_t
route_map_apply (struct route_map *map, struct prefix *prefix,
                 route_map_object_t type, void *object)
{
  static int recursion = 0;
  int ret = 0;
  struct route_map_index *index;
  struct route_map_rule *set;

  if (recursion > RMAP_RECURSION_LIMIT)
    {
      zlog (NULL, LOG_WARNING,
            "route-map recursion limit (%d) reached, discarding route",
            RMAP_RECURSION_LIMIT);
      recursion = 0;
      return RMAP_DENYMATCH;
    }

  if (map == NULL)
    return RMAP_DENYMATCH;

  for (index = map->head; index; index = index->next)
    {
      
      ret = route_map_apply_match (&index->match_list, prefix, type, object);

      if (ret == RMAP_NOMATCH)
        
        continue;
      else if (ret == RMAP_MATCH)
        {
          if (index->type == RMAP_PERMIT)
            
            {
              
              for (set = index->set_list.head; set; set = set->next)
                ret = (*set->cmd->func_apply) (set->value, prefix,
                                               type, object);

              if (index->nextrm)
                {
                  struct route_map *nextrm =
                                    route_map_lookup_by_name (index->nextrm);

                  if (nextrm) 
                    {
                      recursion++;
                      ret = route_map_apply (nextrm, prefix, type, object);
                      recursion--;
                    }

                  if (ret == RMAP_DENYMATCH)
                    return ret;
                }

              switch (index->exitpolicy)
                {
                  case RMAP_EXIT:
                    return ret;
                  case RMAP_NEXT:
                    continue;
                  case RMAP_GOTO:
                    {
                      
                      struct route_map_index *next = index->next;
                      int nextpref = index->nextpref;

                      while (next && next->pref < nextpref)
                        {
                          index = next;
                          next = next->next;
                        }
                      if (next == NULL)
                        {
                          
                          return ret;
                        }
                    }
                }
            }
          else if (index->type == RMAP_DENY)
            
            {
                return RMAP_DENYMATCH;
            }
        }
    }
  
  return RMAP_DENYMATCH;
}

void
route_map_add_hook (void (*func) (const char *))
{
  route_map_master.add_hook = func;
}

void
route_map_delete_hook (void (*func) (const char *))
{
  route_map_master.delete_hook = func;
}

void
route_map_event_hook (void (*func) (route_map_event_t, const char *))
{
  route_map_master.event_hook = func;
}

void
route_map_init (void)
{
  
  route_match_vec = vector_init (1);
  route_set_vec = vector_init (1);
}

void
route_map_finish (void)
{
  vector_free (route_match_vec);
  route_match_vec = NULL;
  vector_free (route_set_vec);
  route_set_vec = NULL;
}

DEFUN (route_map,
       route_map_cmd,
       "route-map WORD (deny|permit) <1-65535>",
       "Create route-map or enter route-map command mode\n"
       "Route map tag\n"
       "Route map denies set operations\n"
       "Route map permits set operations\n"
       "Sequence to insert to/delete from existing route-map entry\n")
{
  int permit;
  unsigned long pref;
  struct route_map *map;
  struct route_map_index *index;
  char *endptr = NULL;

  if (strncmp (argv[1], "permit", strlen (argv[1])) == 0)
    permit = RMAP_PERMIT;
  else if (strncmp (argv[1], "deny", strlen (argv[1])) == 0)
    permit = RMAP_DENY;
  else
    {
      vty_out (vty, "the third field must be [permit|deny]%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  pref = strtoul (argv[2], &endptr, 10);
  if (pref == ULONG_MAX || *endptr != '\0')
    {
      vty_out (vty, "the fourth field must be positive integer%s",
           VTY_NEWLINE);
      return CMD_WARNING;
    }
  if (pref == 0 || pref > 65535)
    {
      vty_out (vty, "the fourth field must be <1-65535>%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  map = route_map_get (argv[0]);
  index = route_map_index_get (map, permit, pref);

  vty->index = index;
  vty->node = RMAP_NODE;
  return CMD_SUCCESS;
}

DEFUN (no_route_map_all,
       no_route_map_all_cmd,
       "no route-map WORD",
       NO_STR
       "Create route-map or enter route-map command mode\n"
       "Route map tag\n")
{
  struct route_map *map;

  map = route_map_lookup_by_name (argv[0]);
  if (map == NULL)
    {
      vty_out (vty, "%% Could not find route-map %s%s",
           argv[0], VTY_NEWLINE);
      return CMD_WARNING;
    }

  route_map_delete (map);

  return CMD_SUCCESS;
}

DEFUN (no_route_map,
       no_route_map_cmd,
       "no route-map WORD (deny|permit) <1-65535>",
       NO_STR
       "Create route-map or enter route-map command mode\n"
       "Route map tag\n"
       "Route map denies set operations\n"
       "Route map permits set operations\n"
       "Sequence to insert to/delete from existing route-map entry\n")
{
  int permit;
  unsigned long pref;
  struct route_map *map;
  struct route_map_index *index;
  char *endptr = NULL;

  if (strncmp (argv[1], "permit", strlen (argv[1])) == 0)
    permit = RMAP_PERMIT;
  else if (strncmp (argv[1], "deny", strlen (argv[1])) == 0)
    permit = RMAP_DENY;
  else
    {
      vty_out (vty, "the third field must be [permit|deny]%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  pref = strtoul (argv[2], &endptr, 10);
  if (pref == ULONG_MAX || *endptr != '\0')
    {
      vty_out (vty, "the fourth field must be positive integer%s",
           VTY_NEWLINE);
      return CMD_WARNING;
    }
  if (pref == 0 || pref > 65535)
    {
      vty_out (vty, "the fourth field must be <1-65535>%s", VTY_NEWLINE);
      return CMD_WARNING;
    }

  map = route_map_lookup_by_name (argv[0]);
  if (map == NULL)
    {
      vty_out (vty, "%% Could not find route-map %s%s",
           argv[0], VTY_NEWLINE);
      return CMD_WARNING;
    }

  index = route_map_index_lookup (map, permit, pref);
  if (index == NULL)
    {
      vty_out (vty, "%% Could not find route-map entry %s %s%s",
           argv[0], argv[2], VTY_NEWLINE);
      return CMD_WARNING;
    }

  route_map_index_delete (index, 1);

  if (route_map_empty (map))
    route_map_delete (map);

  return CMD_SUCCESS;
}

DEFUN (rmap_onmatch_next,
       rmap_onmatch_next_cmd,
       "on-match next",
       "Exit policy on matches\n"
       "Next clause\n")
{
  struct route_map_index *index;

  index = vty->index;

  if (index)
    index->exitpolicy = RMAP_NEXT;

  return CMD_SUCCESS;
}

DEFUN (no_rmap_onmatch_next,
       no_rmap_onmatch_next_cmd,
       "no on-match next",
       NO_STR
       "Exit policy on matches\n"
       "Next clause\n")
{
  struct route_map_index *index;

  index = vty->index;

  if (index)
    index->exitpolicy = RMAP_EXIT;

  return CMD_SUCCESS;
}

DEFUN (rmap_onmatch_goto,
       rmap_onmatch_goto_cmd,
       "on-match goto <1-65535>",
       "Exit policy on matches\n"
       "Goto Clause number\n"
       "Number\n")
{
  struct route_map_index *index = vty->index;
  int d = 0;

  if (index)
    {
      if (argc == 1 && argv[0])
        VTY_GET_INTEGER_RANGE("route-map index", d, argv[0], 1, 65536);
      else
        d = index->pref + 1;

      if (d <= index->pref)
    {
      
      vty_out (vty, "can't jump backwards in route-maps%s",
           VTY_NEWLINE);
      return CMD_WARNING;
    }
      else
    {
      index->exitpolicy = RMAP_GOTO;
      index->nextpref = d;
    }
    }
  return CMD_SUCCESS;
}

DEFUN (no_rmap_onmatch_goto,
       no_rmap_onmatch_goto_cmd,
       "no on-match goto",
       NO_STR
       "Exit policy on matches\n"
       "Goto Clause number\n")
{
  struct route_map_index *index;

  index = vty->index;

  if (index)
    index->exitpolicy = RMAP_EXIT;

  return CMD_SUCCESS;
}

ALIAS (rmap_onmatch_goto,
       rmap_continue_cmd,
       "continue",
       "Continue on a different entry within the route-map\n")

ALIAS (no_rmap_onmatch_goto,
       no_rmap_continue_cmd,
       "no continue",
       NO_STR
       "Continue on a different entry within the route-map\n")

ALIAS (rmap_onmatch_goto,
       rmap_continue_seq_cmd,
       "continue <1-65535>",
       "Continue on a different entry within the route-map\n"
       "Route-map entry sequence number\n")

ALIAS (no_rmap_onmatch_goto,
       no_rmap_continue_seq,
       "no continue <1-65535>",
       NO_STR
       "Continue on a different entry within the route-map\n"
       "Route-map entry sequence number\n")

DEFUN (rmap_show_name,
       rmap_show_name_cmd,
       "show route-map [WORD]",
       SHOW_STR
       "route-map information\n"
       "route-map name\n")
{
    const char *name = NULL;
    if (argc)
      name = argv[0];
    return vty_show_route_map (vty, name);
}

ALIAS (rmap_onmatch_goto,
      rmap_continue_index_cmd,
      "continue <1-65536>",
      "Exit policy on matches\n"
      "Goto Clause number\n")

DEFUN (rmap_call,
       rmap_call_cmd,
       "call WORD",
       "Jump to another Route-Map after match+set\n"
       "Target route-map name\n")
{
  struct route_map_index *index;

  index = vty->index;
  if (index)
    {
      if (index->nextrm)
          XFREE (MTYPE_ROUTE_MAP_NAME, index->nextrm);
      index->nextrm = XSTRDUP (MTYPE_ROUTE_MAP_NAME, argv[0]);
    }
  return CMD_SUCCESS;
}

DEFUN (no_rmap_call,
       no_rmap_call_cmd,
       "no call",
       NO_STR
       "Jump to another Route-Map after match+set\n")
{
  struct route_map_index *index;

  index = vty->index;

  if (index->nextrm)
    {
      XFREE (MTYPE_ROUTE_MAP_NAME, index->nextrm);
      index->nextrm = NULL;
    }

  return CMD_SUCCESS;
}

DEFUN (rmap_description,
       rmap_description_cmd,
       "description .LINE",
       "Route-map comment\n"
       "Comment describing this route-map rule\n")
{
  struct route_map_index *index;

  index = vty->index;
  if (index)
    {
      if (index->description)
    XFREE (MTYPE_TMP, index->description);
      index->description = argv_concat (argv, argc, 0);
    }
  return CMD_SUCCESS;
}

DEFUN (no_rmap_description,
       no_rmap_description_cmd,
       "no description",
       NO_STR
       "Route-map comment\n")
{
  struct route_map_index *index;

  index = vty->index;
  if (index)
    {
      if (index->description)
    XFREE (MTYPE_TMP, index->description);
      index->description = NULL;
    }
  return CMD_SUCCESS;
}

static int
route_map_config_write (struct vty *vty)
{
  struct route_map *map;
  struct route_map_index *index;
  struct route_map_rule *rule;
  int first = 1;
  int write = 0;

  for (map = route_map_master.head; map; map = map->next)
    for (index = map->head; index; index = index->next)
      {
    if (!first)
      vty_out (vty, "!%s", VTY_NEWLINE);
    else
      first = 0;

    vty_out (vty, "route-map %s %s %d%s",
         map->name,
         route_map_type_str (index->type),
         index->pref, VTY_NEWLINE);

    if (index->description)
      vty_out (vty, " description %s%s", index->description, VTY_NEWLINE);

    for (rule = index->match_list.head; rule; rule = rule->next)
      vty_out (vty, " match %s %s%s", rule->cmd->str,
           rule->rule_str ? rule->rule_str : "",
           VTY_NEWLINE);

    for (rule = index->set_list.head; rule; rule = rule->next)
      vty_out (vty, " set %s %s%s", rule->cmd->str,
           rule->rule_str ? rule->rule_str : "",
           VTY_NEWLINE);
   if (index->nextrm)
     vty_out (vty, " call %s%s", index->nextrm, VTY_NEWLINE);
    if (index->exitpolicy == RMAP_GOTO)
      vty_out (vty, " on-match goto %d%s", index->nextpref, VTY_NEWLINE);
    if (index->exitpolicy == RMAP_NEXT)
      vty_out (vty," on-match next%s", VTY_NEWLINE);

    write++;
      }
  return write;
}

static struct cmd_node rmap_node =
{
  RMAP_NODE,
  "%s(config-route-map)# ",
  1
};

void
route_map_init_vty (void)
{
  
  install_node (&rmap_node, route_map_config_write);

  install_default (RMAP_NODE);
  install_element (CONFIG_NODE, &route_map_cmd);
  install_element (CONFIG_NODE, &no_route_map_cmd);
  install_element (CONFIG_NODE, &no_route_map_all_cmd);

  install_element (RMAP_NODE, &route_map_cmd);
  install_element (RMAP_NODE, &rmap_onmatch_next_cmd);
  install_element (RMAP_NODE, &no_rmap_onmatch_next_cmd);
  install_element (RMAP_NODE, &rmap_onmatch_goto_cmd);
  install_element (RMAP_NODE, &no_rmap_onmatch_goto_cmd);

  install_element (RMAP_NODE, &rmap_continue_cmd);
  install_element (RMAP_NODE, &no_rmap_continue_cmd);
  install_element (RMAP_NODE, &rmap_continue_index_cmd);

  install_element (RMAP_NODE, &rmap_call_cmd);
  install_element (RMAP_NODE, &no_rmap_call_cmd);

  install_element (RMAP_NODE, &rmap_description_cmd);
  install_element (RMAP_NODE, &no_rmap_description_cmd);

  install_element (ENABLE_NODE, &rmap_show_name_cmd);
}
