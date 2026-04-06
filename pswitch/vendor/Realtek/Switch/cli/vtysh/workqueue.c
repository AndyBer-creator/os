
#include <lib/zebra.h>
#include "thread.h"
#include "memory.h"
#include "workqueue.h"
#include "linklist.h"
#include "command.h"
#include "log.h"

static struct list work_queues;

#define WORK_QUEUE_MIN_GRANULARITY 1

static struct work_queue_item *
work_queue_item_new (struct work_queue *wq)
{
  struct work_queue_item *item;
  assert (wq);

  item = XCALLOC (MTYPE_WORK_QUEUE_ITEM,
                  sizeof (struct work_queue_item));

  return item;
}

static void
work_queue_item_free (struct work_queue_item *item)
{
  XFREE (MTYPE_WORK_QUEUE_ITEM, item);
  return;
}

struct work_queue *
work_queue_new (struct thread_master *m, const char *queue_name)
{
  struct work_queue *new;

  new = XCALLOC (MTYPE_WORK_QUEUE, sizeof (struct work_queue));

  if (new == NULL)
    return new;

  new->name = XSTRDUP (MTYPE_WORK_QUEUE_NAME, queue_name);
  new->master = m;
  SET_FLAG (new->flags, WQ_UNPLUGGED);

  if ( (new->items = list_new ()) == NULL)
    {
      XFREE (MTYPE_WORK_QUEUE_NAME, new->name);
      XFREE (MTYPE_WORK_QUEUE, new);

      return NULL;
    }

  new->items->del = (void (*)(void *)) work_queue_item_free;

  listnode_add (&work_queues, new);

  new->cycles.granularity = WORK_QUEUE_MIN_GRANULARITY;

  new->spec.hold = WORK_QUEUE_DEFAULT_HOLD;

  return new;
}

void
work_queue_free (struct work_queue *wq)
{
  if (wq->thread != NULL)
    thread_cancel(wq->thread);

  list_delete (wq->items);
  listnode_delete (&work_queues, wq);

  XFREE (MTYPE_WORK_QUEUE_NAME, wq->name);
  XFREE (MTYPE_WORK_QUEUE, wq);
  return;
}

static inline int
work_queue_schedule (struct work_queue *wq, unsigned int delay)
{
  
  if ( CHECK_FLAG (wq->flags, WQ_UNPLUGGED)
       && (wq->thread == NULL)
       && (listcount (wq->items) > 0) )
    {
      wq->thread = thread_add_background (wq->master, work_queue_run,
                                          wq, delay);
      return 1;
    }
  else
    return 0;
}

void
work_queue_add (struct work_queue *wq, void *data)
{
  struct work_queue_item *item;

  assert (wq);

  if (!(item = work_queue_item_new (wq)))
    {
      zlog_err ("%s: unable to get new queue item", __func__);
      return;
    }

  item->data = data;
  listnode_add (wq->items, item);

  work_queue_schedule (wq, wq->spec.hold);

  return;
}

static void
work_queue_item_remove (struct work_queue *wq, struct listnode *ln)
{
  struct work_queue_item *item = listgetdata (ln);

  assert (item && item->data);

  if (wq->spec.del_item_data)
    wq->spec.del_item_data (wq, item->data);

  list_delete_node (wq->items, ln);
  work_queue_item_free (item);

  return;
}

static void
work_queue_item_requeue (struct work_queue *wq, struct listnode *ln)
{
  LISTNODE_DETACH (wq->items, ln);
  LISTNODE_ATTACH (wq->items, ln); 
}

DEFUN(show_work_queues,
      show_work_queues_cmd,
      "show work-queues",
      SHOW_STR
      "Work Queue information\n")
{
  struct listnode *node;
  struct work_queue *wq;

  vty_out (vty,
           "%c %8s %5s %8s %21s%s",
           ' ', "List","(ms) ","Q. Runs","Cycle Counts   ",
           VTY_NEWLINE);
  vty_out (vty,
           "%c %8s %5s %8s %7s %6s %6s %s%s",
           'P',
           "Items",
           "Hold",
           "Total",
           "Best","Gran.","Avg.",
           "Name",
           VTY_NEWLINE);

  for (ALL_LIST_ELEMENTS_RO ((&work_queues), node, wq))
    {
      vty_out (vty,"%c %8d %5d %8ld %7d %6d %6u %s%s",
               (CHECK_FLAG (wq->flags, WQ_UNPLUGGED) ? ' ' : 'P'),
               listcount (wq->items),
               wq->spec.hold,
               wq->runs,
               wq->cycles.best, wq->cycles.granularity,
                 (wq->runs) ?
                   (unsigned int) (wq->cycles.total / wq->runs) : 0,
               wq->name,
               VTY_NEWLINE);
    }

  return CMD_SUCCESS;
}

void
work_queue_plug (struct work_queue *wq)
{
  if (wq->thread)
    thread_cancel (wq->thread);

  wq->thread = NULL;

  UNSET_FLAG (wq->flags, WQ_UNPLUGGED);
}

void
work_queue_unplug (struct work_queue *wq)
{
  SET_FLAG (wq->flags, WQ_UNPLUGGED);

  work_queue_schedule (wq, wq->spec.hold);
}

int
work_queue_run (struct thread *thread)
{
  struct work_queue *wq;
  struct work_queue_item *item;
  wq_item_status ret;
  unsigned int cycles = 0;
  struct listnode *node, *nnode;
  char yielded = 0;

  wq = THREAD_ARG (thread);
  wq->thread = NULL;

  assert (wq && wq->items);

   if (wq->cycles.granularity == 0)
     wq->cycles.granularity = WORK_QUEUE_MIN_GRANULARITY;

  for (ALL_LIST_ELEMENTS (wq->items, node, nnode, item))
  {
    assert (item && item->data);

    if (item->ran > wq->spec.max_retries)
      {
        
    if (wq->spec.errorfunc)
      wq->spec.errorfunc (wq, item->data);
    work_queue_item_remove (wq, node);
    continue;
      }

    do
      {
        ret = wq->spec.workfunc (wq, item->data);
        item->ran++;
      }
    while ((ret == WQ_RETRY_NOW)
           && (item->ran < wq->spec.max_retries));

    switch (ret)
      {
      case WQ_QUEUE_BLOCKED:
        {
          
          item->ran--;
        }
      case WQ_RETRY_LATER:
    {
      goto stats;
    }
      case WQ_REQUEUE:
    {
      item->ran--;
      work_queue_item_requeue (wq, node);
      break;
    }
      case WQ_RETRY_NOW:
        
      case WQ_ERROR:
    {
      if (wq->spec.errorfunc)
        wq->spec.errorfunc (wq, item);
    }
    
      case WQ_SUCCESS:
      default:
    {
      work_queue_item_remove (wq, node);
      break;
    }
      }

    cycles++;

    if ( !(cycles % wq->cycles.granularity)
        && thread_should_yield (thread))
      {
        yielded = 1;
        goto stats;
      }
  }

stats:

#define WQ_HYSTERIS_FACTOR 2

  if (yielded && (cycles < wq->cycles.granularity))
    {
      wq->cycles.granularity = ((cycles > 0) ? cycles
                                             : WORK_QUEUE_MIN_GRANULARITY);
    }

  if (cycles >= (wq->cycles.granularity))
    {
      if (cycles > wq->cycles.best)
        wq->cycles.best = cycles;

      if (cycles > (wq->cycles.granularity * WQ_HYSTERIS_FACTOR * 2))
        wq->cycles.granularity *= WQ_HYSTERIS_FACTOR; 
      else if (cycles > (wq->cycles.granularity * WQ_HYSTERIS_FACTOR))
        wq->cycles.granularity += WQ_HYSTERIS_FACTOR;
    }
#undef WQ_HYSTERIS_FACTOR

  wq->runs++;
  wq->cycles.total += cycles;

#if 0
  printf ("%s: cycles %d, new: best %d, worst %d\n",
            __func__, cycles, wq->cycles.best, wq->cycles.granularity);
#endif

  if (listcount (wq->items) > 0)
    work_queue_schedule (wq, 0);
  else if (wq->spec.completion_func)
    wq->spec.completion_func (wq);

  return 0;
}
