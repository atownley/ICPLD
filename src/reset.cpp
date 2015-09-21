
/*
  This function is invoked by -reset on 
  the command line. It removes all files
  which icpld recognizes, and calls
  firstRun() to restore them
*/
  
#include "cuc.h"
#include <sys/stat.h>
#include <dirent.h>
/* #ifndef SOLARIS
#include <sys/dir.h>
#endif
*/
#include <sys/types.h>

void reset(string apa)
{
  string pathName = apa + "/.icpld", cmd = "rm -rf " + pathName + "/*", cmd2 = "cd .. && rmdir " + pathName;
#ifdef SOLARIS /* I'm getting mighty upset with SOLARIS soon */
  system(cmd.c_str());
  system (cmd2.c_str());
#endif
#ifndef SOLARIS
  struct dirent **files;
  int count =  scandir(pathName.c_str(), &files,  0, alphasort);
  /* No files to remove */
  if (count <= 0)
    exit(EXIT_SUCCESS);
  
  for (int i = 1; i < count+1; ++i){
    /*We don't want to remove . and .. do we?*/
    if ((strncmp(files[i-1]->d_name, ".", 1)!=0) && (strncmp(files[i-1]->d_name, "..", 2)!=0 )){
      if(remove(files[i-1]->d_name)==-1 && errno != ENOENT)
	perror("remove()");
    }
   
  }
  errno = 0;
  if (rmdir(pathName.c_str()) == -1 && errno != ENOTEMPTY ){
    perror("rmdir()");
    exit(EXIT_FAILURE);
  }

#endif
  cout << "ICPLD is now in out-of-the-box-state (note that eventual configuration files are left untouched)" << endl;
  exit(EXIT_SUCCESS);
  
}


