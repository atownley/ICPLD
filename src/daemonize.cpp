/*
   This file consists of the daemonizing code
   which forks icpld into the background

   This is perhaps more complex way of doing it
   though solaris doesn't support the daemon() 
   call. And we do want to be portable, right?
*/

#include "cuc.h"
#include <fcntl.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <sys/wait.h>

void closeallfd(int fd)
{
  int fdlimit = sysconf(_SC_OPEN_MAX);

  while (fd < fdlimit)
    close(fd++);
}

/* believed to work on all Posix systems */

int daemonize(int nochdir, int noclose, int silent)
{
  int childPid;
  if (silent == 1){
    cout << "Everything seems allright..." << endl;
  }
 
  switch (fork())
    {
    case 0:  break;
    case -1: return -1;
    default: {
       wait(NULL); /* We need to wait for the other process, otherwise the */
      _exit(0);   /* ... PID is written AFTER the parent has exited, which is ugly */      
    }

    }
  if (setsid() < 0)               /* shoudn't fail */
    return -1;
  childPid=fork();
  switch (childPid)
    {
    case 0:  break;
    case -1: return -1;
    default: {
      if (silent == 1)
	cout << "Forking into background as " << childPid << endl;
      _exit(0);
    }
    }

  if (!nochdir){
    if(chdir("/var/tmp") == -1 && chdir("/tmp") == -1){
      cout << "Could not find a suitable location to chdir() to." << endl;
      perror("chdir():");
      closeallfd(0);
      exit(EXIT_FAILURE);
    }
  }
			

  if (!noclose)
    {
      closeallfd(0);
      if (open("/dev/null",O_RDWR) == -1) /* Just shouldn't happen */
	cout << "Could not open device /dev/null for writing. Output is now located 'elsewhere'" << endl;
      dup(0); dup(0);
    }
  return EXIT_SUCCESS;
}
