
#ifndef _QUAGGA_SIGNAL_H
#define _QUAGGA_SIGNAL_H

#include <thread.h>

#define QUAGGA_SIGNAL_TIMER_INTERVAL 2L
#define Q_SIGC(sig) (sizeof(sig)/sizeof(sig[0]))

struct quagga_signal_t
{
  int signal;                     
  void (*handler) (void);         

  volatile sig_atomic_t caught;   
};

extern void signal_init (struct thread_master *m, int sigc,
                         struct quagga_signal_t *signals);

extern int quagga_sigevent_process (void);

#endif 
