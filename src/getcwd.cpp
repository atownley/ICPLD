#include "cuc.h"
#include <cassert>


#ifdef DARWIN
#include <sys/param.h>
#endif

std::string getCwdir()
{

#if !defined(DARWIN) && !defined(NETBSD) && !defined(OPENBSD)
  std::string returnMe;
  long tempsize = pathconf (".", _PC_PATH_MAX);
  char *tempbuf;
  tempbuf = new char[tempsize];
  assert(tempbuf != NULL);
  if ((getcwd (tempbuf, (size_t) tempsize)) == NULL)
    {
      cout << "Could not obtain Current Working Directory..." << endl;
      perror ("getcwd()");
      exit (EXIT_FAILURE);
    }
  returnMe = tempbuf;
  delete [] tempbuf;
  return returnMe;
#else
  char buf[BUFSIZ];
  string savepwd2=getcwd(buf, MAXPATHLEN);
  return savepwd2;
#endif


}

