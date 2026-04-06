
#ifndef _ZEBRA_THREAD_H
#define _ZEBRA_THREAD_H

struct rusage_t
{
#ifdef HAVE_RUSAGE
  struct rusage cpu;
#endif
  struct timeval real;
};
#define RUSAGE_T        struct rusage_t

#define GETRUSAGE(X) thread_getrusage(X)

struct thread_list
{
  struct thread *head;
  struct thread *tail;
  int count;
};

struct thread_master
{
  struct thread_list read;
  struct thread_list write;
  struct thread_list timer;
  struct thread_list event;
  struct thread_list ready;
  struct thread_list unuse;
  struct thread_list background;
  fd_set readfd;
  fd_set writefd;
  fd_set exceptfd;
  unsigned long alloc;
};

typedef unsigned char thread_type;

struct thread
{
  thread_type type;        
  thread_type add_type;        
  struct thread *next;        
  struct thread *prev;        
  struct thread_master *master;    
  int (*func) (struct thread *); 
  void *arg;            
  union {
    int val;            
    int fd;            
    struct timeval sands;    
  } u;
  RUSAGE_T ru;            
  struct cpu_thread_history *hist; 
  char* funcname;
};

struct cpu_thread_history
{
  int (*func)(struct thread *);
  
  char *funcname;
  unsigned int total_calls;
  struct time_stats
  {
    unsigned long total, max;
  } real;
#ifdef HAVE_RUSAGE
  struct time_stats cpu;
#endif
  thread_type types;
};

enum quagga_clkid {
  QUAGGA_CLK_REALTIME = 0,    
  QUAGGA_CLK_MONOTONIC,        
  QUAGGA_CLK_REALTIME_STABILISED, 
};

#define THREAD_READ           0
#define THREAD_WRITE          1
#define THREAD_TIMER          2
#define THREAD_EVENT          3
#define THREAD_READY          4
#define THREAD_BACKGROUND     5
#define THREAD_UNUSED         6
#define THREAD_EXECUTE        7

#define THREAD_YIELD_TIME_SLOT     10 * 1000L 

#define THREAD_ARG(X) ((X)->arg)
#define THREAD_FD(X)  ((X)->u.fd)
#define THREAD_VAL(X) ((X)->u.val)

#define THREAD_READ_ON(master,thread,func,arg,sock) \
  do { \
    if (! thread) \
      thread = thread_add_read (master, func, arg, sock); \
  } while (0)

#define THREAD_WRITE_ON(master,thread,func,arg,sock) \
  do { \
    if (! thread) \
      thread = thread_add_write (master, func, arg, sock); \
  } while (0)

#define THREAD_TIMER_ON(master,thread,func,arg,time) \
  do { \
    if (! thread) \
      thread = thread_add_timer (master, func, arg, time); \
  } while (0)

#define THREAD_OFF(thread) \
  do { \
    if (thread) \
      { \
        thread_cancel (thread); \
        thread = NULL; \
      } \
  } while (0)

#define THREAD_READ_OFF(thread)  THREAD_OFF(thread)
#define THREAD_WRITE_OFF(thread)  THREAD_OFF(thread)
#define THREAD_TIMER_OFF(thread)  THREAD_OFF(thread)

#define thread_add_read(m,f,a,v) funcname_thread_add_read(m,f,a,v,#f)
#define thread_add_write(m,f,a,v) funcname_thread_add_write(m,f,a,v,#f)
#define thread_add_timer(m,f,a,v) funcname_thread_add_timer(m,f,a,v,#f)
#define thread_add_timer_msec(m,f,a,v) funcname_thread_add_timer_msec(m,f,a,v,#f)
#define thread_add_event(m,f,a,v) funcname_thread_add_event(m,f,a,v,#f)
#define thread_execute(m,f,a,v) funcname_thread_execute(m,f,a,v,#f)

#define thread_add_background(m,f,a,v) funcname_thread_add_background(m,f,a,v,#f)

extern struct thread_master *thread_master_create (void);
extern void thread_master_free (struct thread_master *);

extern struct thread *funcname_thread_add_read (struct thread_master *,
                                int (*)(struct thread *),
                                void *, int, const char*);
extern struct thread *funcname_thread_add_write (struct thread_master *,
                                 int (*)(struct thread *),
                                 void *, int, const char*);
extern struct thread *funcname_thread_add_timer (struct thread_master *,
                                 int (*)(struct thread *),
                                 void *, long, const char*);
extern struct thread *funcname_thread_add_timer_msec (struct thread_master *,
                                      int (*)(struct thread *),
                                      void *, long, const char*);
extern struct thread *funcname_thread_add_event (struct thread_master *,
                                 int (*)(struct thread *),
                                 void *, int, const char*);
extern struct thread *funcname_thread_add_background (struct thread_master *,
                                               int (*func)(struct thread *),
                               void *arg,
                               long milliseconds_to_delay,
                           const char *funcname);
extern struct thread *funcname_thread_execute (struct thread_master *,
                                               int (*)(struct thread *),
                                               void *, int, const char *);
extern void thread_cancel (struct thread *);
extern unsigned int thread_cancel_event (struct thread_master *, void *);
extern struct thread *thread_fetch (struct thread_master *, struct thread *);
extern void thread_call (struct thread *);
extern unsigned long thread_timer_remain_second (struct thread *);
extern int thread_should_yield (struct thread *);

extern void thread_getrusage (RUSAGE_T *);
extern struct cmd_element show_thread_cpu_cmd;

extern int quagga_gettime (enum quagga_clkid, struct timeval *);
extern time_t quagga_time (time_t *);

extern unsigned long thread_consumed_time(RUSAGE_T *after, RUSAGE_T *before,
                      unsigned long *cpu_time_elapsed);

extern struct timeval recent_time;

extern struct timeval recent_relative_time (void);
#endif 
