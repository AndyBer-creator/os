
#ifndef _QUAGGA_WORK_QUEUE_H
#define _QUAGGA_WORK_QUEUE_H

#define WORK_QUEUE_DEFAULT_HOLD  50

typedef enum
{
  WQ_SUCCESS = 0,
  WQ_ERROR,             
  WQ_RETRY_NOW,         
  WQ_RETRY_LATER,       
  WQ_REQUEUE,        
  WQ_QUEUE_BLOCKED,    
} wq_item_status;

struct work_queue_item
{
  void *data;                           
  unsigned short ran;            
};

#define WQ_UNPLUGGED    (1 << 0) 

struct work_queue
{
  
  struct thread_master *master;       
  struct thread *thread;              
  char *name;                         

  struct {
    
    void *data;

    wq_item_status (*workfunc) (struct work_queue *, void *);

    void (*errorfunc) (struct work_queue *, struct work_queue_item *);

    void (*del_item_data) (struct work_queue *, void *);

    void (*completion_func) (struct work_queue *);

    unsigned int max_retries;

    unsigned int hold;    
  } spec;

  struct list *items;                 
  unsigned long runs;                 

  struct {
    unsigned int best;
    unsigned int granularity;
    unsigned long total;
  } cycles;    

  u_int16_t flags;        
};

extern struct work_queue *work_queue_new (struct thread_master *,
                                          const char *);

extern void work_queue_free (struct work_queue *);

extern void work_queue_add (struct work_queue *, void *);

extern void work_queue_plug (struct work_queue *wq);

extern void work_queue_unplug (struct work_queue *wq);

extern int work_queue_run (struct thread *);
extern struct cmd_element show_work_queues_cmd;
#endif 
