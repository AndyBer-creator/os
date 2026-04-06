
#include <zebra.h>
#include <log.h>

#ifndef HAVE_DAEMON

int
daemon (int nochdir, int noclose)
{
  pid_t pid;

  pid = fork ();

  if (pid < 0)
    {
      zlog_err ("fork failed: %s", safe_strerror(errno));
      return -1;
    }

  if (pid != 0)
    exit (0);

  pid = setsid();

  if (pid == -1)
    {
      zlog_err ("setsid failed: %s", safe_strerror(errno));
      return -1;
    }

  if (! nochdir)
    chdir ("/");

  if (! noclose)
    {
      int fd;

      fd = open ("/dev/null", O_RDWR, 0);
      if (fd != -1)
    {
      dup2 (fd, STDIN_FILENO);
      dup2 (fd, STDOUT_FILENO);
      dup2 (fd, STDERR_FILENO);
      if (fd > 2)
        close (fd);
    }
    }

  umask (0027);

  return 0;
}

#endif 
