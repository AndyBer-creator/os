
#include <zebra.h>
#include <fcntl.h>
#include <log.h>
#include "version.h"

#define PIDFILE_MASK 0644
#ifndef HAVE_FCNTL

pid_t
pid_output (const char *path)
{
  FILE *fp;
  pid_t pid;
  mode_t oldumask;

  pid = getpid();

  oldumask = umask(0777 & ~PIDFILE_MASK);
  fp = fopen (path, "w");
  if (fp != NULL)
    {
      fprintf (fp, "%d\n", (int) pid);
      fclose (fp);
      umask(oldumask);
      return pid;
    }
  
  zlog_warn("Can't fopen pid lock file %s (%s), continuing",
        path, safe_strerror(errno));
  umask(oldumask);
  return -1;
}

#else 

pid_t
pid_output (const char *path)
{
  int tmp;
  int fd;
  pid_t pid;
  char buf[16];
  struct flock lock;
  mode_t oldumask;

  pid = getpid ();

  oldumask = umask(0777 & ~PIDFILE_MASK);
  fd = open (path, O_RDWR | O_CREAT, PIDFILE_MASK);
  if (fd < 0)
    {
      zlog_err("Can't create pid lock file %s (%s), exiting",
           path, safe_strerror(errno));
      umask(oldumask);
      exit(1);
    }
  else
    {
      size_t pidsize;

      umask(oldumask);
      memset (&lock, 0, sizeof(lock));

      lock.l_type = F_WRLCK;
      lock.l_whence = SEEK_SET;

      if (fcntl(fd, F_SETLK, &lock) < 0)
        {
          zlog_err("Could not lock pid_file %s, exiting", path);
          exit(1);
        }

      sprintf (buf, "%d\n", (int) pid);
      pidsize = strlen(buf);
      if ((tmp = write (fd, buf, pidsize)) != (int)pidsize)
        zlog_err("Could not write pid %d to pid_file %s, rc was %d: %s",
             (int)pid,path,tmp,safe_strerror(errno));
      else if (ftruncate(fd, pidsize) < 0)
        zlog_err("Could not truncate pid_file %s to %u bytes: %s",
             path,(u_int)pidsize,safe_strerror(errno));
    }
  return pid;
}

#endif 
