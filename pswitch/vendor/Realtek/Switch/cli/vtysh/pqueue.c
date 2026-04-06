
#include <zebra.h>

#include "memory.h"
#include "pqueue.h"

#define DATA_SIZE (sizeof (void *))
#define PARENT_OF(x) ((x - 1) / 2)
#define LEFT_OF(x)  (2 * x + 1)
#define RIGHT_OF(x) (2 * x + 2)
#define HAVE_CHILD(x,q) (x < (q)->size / 2)

void
trickle_up (int index, struct pqueue *queue)
{
  void *tmp;

  tmp = queue->array[index];

  while (index > 0 &&
         (*queue->cmp) (tmp, queue->array[PARENT_OF (index)]) < 0)
    {
      
      queue->array[index] = queue->array[PARENT_OF (index)];
      if (queue->update != NULL)
    (*queue->update) (queue->array[index], index);
      index = PARENT_OF (index);
    }

  queue->array[index] = tmp;
  if (queue->update != NULL)
    (*queue->update) (tmp, index);
}

void
trickle_down (int index, struct pqueue *queue)
{
  void *tmp;
  int which;

  tmp = queue->array[index];

  while (HAVE_CHILD (index, queue))
    {
      
      if (RIGHT_OF (index) < queue->size &&
          (*queue->cmp) (queue->array[LEFT_OF (index)],
                         queue->array[RIGHT_OF (index)]) > 0)
        which = RIGHT_OF (index);
      else
        which = LEFT_OF (index);

      if ((*queue->cmp) (queue->array[which], tmp) > 0)
        break;

      queue->array[index] = queue->array[which];
       if (queue->update != NULL)
     (*queue->update) (queue->array[index], index);
      index = which;
    }

  queue->array[index] = tmp;
  if (queue->update != NULL)
    (*queue->update) (tmp, index);
}

struct pqueue *
pqueue_create (void)
{
  struct pqueue *queue;

  queue = XCALLOC (MTYPE_PQUEUE, sizeof (struct pqueue));

  queue->array = XCALLOC (MTYPE_PQUEUE_DATA,
                          DATA_SIZE * PQUEUE_INIT_ARRAYSIZE);
  queue->array_size = PQUEUE_INIT_ARRAYSIZE;

  queue->update = NULL;
  return queue;
}

void
pqueue_delete (struct pqueue *queue)
{
  XFREE (MTYPE_PQUEUE_DATA, queue->array);
  XFREE (MTYPE_PQUEUE, queue);
}

static int
pqueue_expand (struct pqueue *queue)
{
  void **newarray;

  newarray = XCALLOC (MTYPE_PQUEUE_DATA, queue->array_size * DATA_SIZE * 2);
  if (newarray == NULL)
    return 0;

  memcpy (newarray, queue->array, queue->array_size * DATA_SIZE);

  XFREE (MTYPE_PQUEUE_DATA, queue->array);
  queue->array = newarray;
  queue->array_size *= 2;

  return 1;
}

void
pqueue_enqueue (void *data, struct pqueue *queue)
{
  if (queue->size + 2 >= queue->array_size && ! pqueue_expand (queue))
    return;

  queue->array[queue->size] = data;
  if (queue->update != NULL)
    (*queue->update) (data, queue->size);
  trickle_up (queue->size, queue);
  queue->size ++;
}

void *
pqueue_dequeue (struct pqueue *queue)
{
  void *data = queue->array[0];
  queue->array[0] =  queue->array[--queue->size];
  trickle_down (0, queue);
  return data;
}
