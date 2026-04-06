
#include <zebra.h>
#include <sigevent.h>
#include <log.h>

#ifdef SA_SIGINFO
#ifdef HAVE_UCONTEXT_H
#ifdef GNU_LINUX

#ifndef __USE_GNU
#define __USE_GNU
#endif 
#endif 
#include <ucontext.h>
#endif 
#endif 

struct quagga_sigevent_master_t
{
  struct thread *t;

  struct quagga_signal_t *signals;
  int sigc;

  volatile sig_atomic_t caught;
} sigmaster;

static void
quagga_signal_handler (int signo)
{
  int i;
  struct quagga_signal_t *sig;

  for (i = 0; i < sigmaster.sigc; i++)
    {
      sig = &(sigmaster.signals[i]);

      if (sig->signal == signo)
        sig->caught = 1;
    }

  sigmaster.caught = 1;
}

int
quagga_sigevent_process (void)
{
  struct quagga_signal_t *sig;
  int i;
#ifdef SIGEVENT_BLOCK_SIGNALS
  
  sigset_t newmask, oldmask;

  sigfillset (&newmask);
  sigdelset (&newmask, SIGTRAP);
  sigdelset (&newmask, SIGKILL);

  if ( (sigprocmask (SIG_BLOCK, &newmask, &oldmask)) < 0)
    {
      zlog_err ("quagga_signal_timer: couldnt block signals!");
      return -1;
    }
#endif 

  if (sigmaster.caught > 0)
    {
      sigmaster.caught = 0;

      for (i = 0; i < sigmaster.sigc; i++)
        {
          sig = &(sigmaster.signals[i]);

          if (sig->caught > 0)
            {
              sig->caught = 0;
              sig->handler ();
            }
        }
    }

#ifdef SIGEVENT_BLOCK_SIGNALS
  if ( sigprocmask (SIG_UNBLOCK, &oldmask, NULL) < 0 );
    return -1;
#endif 

  return 0;
}

#ifdef SIGEVENT_SCHEDULE_THREAD

int
quagga_signal_timer (struct thread *t)
{
  struct quagga_sigevent_master_t *sigm;
  struct quagga_signal_t *sig;
  int i;

  sigm = THREAD_ARG (t);
  sigm->t = thread_add_timer (sigm->t->master, quagga_signal_timer, &sigmaster,
                              QUAGGA_SIGNAL_TIMER_INTERVAL);
  return quagga_sigevent_process ();
}
#endif 

static int
signal_set (int signo)
{
  int ret;
  struct sigaction sig;
  struct sigaction osig;

  sig.sa_handler = &quagga_signal_handler;
  sigfillset (&sig.sa_mask);
  sig.sa_flags = 0;
  if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
      sig.sa_flags |= SA_INTERRUPT; 
#endif
  } else {
#ifdef SA_RESTART
      sig.sa_flags |= SA_RESTART;
#endif 
  }

  ret = sigaction (signo, &sig, &osig);
  if (ret < 0)
    return ret;
  else
    return 0;
}

#ifdef SA_SIGINFO

static void *
program_counter(void *context)
{
#ifdef HAVE_UCONTEXT_H
#ifdef GNU_LINUX
#ifdef REG_EIP
  if (context)
    return (void *)(((ucontext_t *)context)->uc_mcontext.gregs[REG_EIP]);
#endif 
#endif 
#endif 
  return NULL;
}

#endif 

static void __attribute__ ((noreturn))
exit_handler(int signo
#ifdef SA_SIGINFO
         , siginfo_t *siginfo, void *context
#endif
        )
{
  zlog_signal(signo, "exiting..."
#ifdef SA_SIGINFO
          , siginfo, program_counter(context)
#endif
         );
  _exit(128+signo);
}

static void __attribute__ ((noreturn))
core_handler(int signo
#ifdef SA_SIGINFO
         , siginfo_t *siginfo, void *context
#endif
        )
{
  zlog_signal(signo, "aborting..."
#ifdef SA_SIGINFO
          , siginfo, program_counter(context)
#endif
         );
  abort();
}

static void
trap_default_signals(void)
{
  static const int core_signals[] = {
    SIGQUIT,
    SIGILL,
#ifdef SIGEMT
    SIGEMT,
#endif
    SIGFPE,
    SIGBUS,
    SIGSEGV,
#ifdef SIGSYS
    SIGSYS,
#endif
#ifdef SIGXCPU
    SIGXCPU,
#endif
#ifdef SIGXFSZ
    SIGXFSZ,
#endif
  };
  static const int exit_signals[] = {
    SIGHUP,
    SIGINT,
    SIGALRM,
    SIGTERM,
    SIGUSR1,
    SIGUSR2,
#ifdef SIGPOLL
    SIGPOLL,
#endif
#ifdef SIGVTALRM
    SIGVTALRM,
#endif
#ifdef SIGSTKFLT
    SIGSTKFLT,
#endif
  };
  static const int ignore_signals[] = {
    SIGPIPE,
  };
  static const struct {
    const int *sigs;
    u_int nsigs;
    void (*handler)(int signo
#ifdef SA_SIGINFO
            , siginfo_t *info, void *context
#endif
           );
  } sigmap[] = {
    { core_signals, sizeof(core_signals)/sizeof(core_signals[0]), core_handler},
    { exit_signals, sizeof(exit_signals)/sizeof(exit_signals[0]), exit_handler},
    { ignore_signals, sizeof(ignore_signals)/sizeof(ignore_signals[0]), NULL},
  };
  u_int i;

  for (i = 0; i < sizeof(sigmap)/sizeof(sigmap[0]); i++)
    {
      u_int j;

      for (j = 0; j < sigmap[i].nsigs; j++)
        {
      struct sigaction oact;
      if ((sigaction(sigmap[i].sigs[j],NULL,&oact) == 0) &&
          (oact.sa_handler == SIG_DFL))
        {
          struct sigaction act;
          sigfillset (&act.sa_mask);
          if (sigmap[i].handler == NULL)
            {
          act.sa_handler = SIG_IGN;
          act.sa_flags = 0;
            }
          else
            {
#ifdef SA_SIGINFO
          
          act.sa_sigaction = sigmap[i].handler;
          act.sa_flags = SA_SIGINFO;
#else
          act.sa_handler = sigmap[i].handler;
          act.sa_flags = 0;
#endif
            }
          if (sigaction(sigmap[i].sigs[j],&act,NULL) < 0)
            zlog_warn("Unable to set signal handler for signal %d: %s",
              sigmap[i].sigs[j],safe_strerror(errno));

        }
        }
    }
}

void
signal_init (struct thread_master *m, int sigc,
             struct quagga_signal_t signals[])
{

  int i = 0;
  struct quagga_signal_t *sig;

  trap_default_signals();

  while (i < sigc)
    {
      sig = &signals[i];
      if ( signal_set (sig->signal) < 0 )
        exit (-1);
      i++;
    }

  sigmaster.sigc = sigc;
  sigmaster.signals = signals;

#ifdef SIGEVENT_SCHEDULE_THREAD
  sigmaster.t =
    thread_add_timer (m, quagga_signal_timer, &sigmaster,
                      QUAGGA_SIGNAL_TIMER_INTERVAL);
#endif 
}
