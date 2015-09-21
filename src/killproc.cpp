/* 
   This function kills a running icpld process
   by reading its pid from ~/.icpld/icpld.pid .
   
   icpld -quit and icpld -detach goes here for instance
*/
#include "cuc.h"
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <csignal>


bool
killProc (std::string fileName, int silent, int signr, string statFile)
{
	string buf, pidNum;
	if (fileCheck (fileName) == "NE" || fileCheck(fileName) == "ENP")
	{
		if(silent == 1){
			cerr << "There doesn't seem to be an ICPLD process running." << endl;
			cerr << "Or perhaps the PID file is missing." << endl;
		}
		return false;
	}
	ifstream readPid (fileName.c_str (), ios::in);
	while (getline (readPid, buf))
		pidNum = buf;
	readPid.close();
	buf="";
	if (signr == 10){
      	  ifstream readStat (statFile.c_str(), ios::binary);
	  while (getline(readStat, buf)){
	      if (buf == "Daemonized") {
		cout << "The currently running instance of icpld is already detached." << endl;
		exit(EXIT_FAILURE);
	      }
	  } 	       
	} 
	  
	       
	if(silent == 1 && signr == SIGTERM)
	  cout << "Sending process " << pidNum << " the SIGTERM (signal " << SIGTERM << ")..." << endl;
	else if(silent == 1 && signr == SIGUSR1)
	  cout << "Sending process " << pidNum << " the SIGUSR1 (signal " << SIGUSR1 << ")..." << endl;
	if (kill (atoi (pidNum.c_str ()), signr) != 0){
	  if (errno != ESRCH){ /* No such process */
	    perror("kill");
	    cout << "Removing possibly stale PID-file.." << endl;
	  }
	  remove (fileName.c_str());
	  return false;
	}
	if (silent == 1 && signr == SIGTERM)
	  cout << "Process terminated successfully..." << endl;
	else if (silent == 1 && signr == SIGUSR1)
	  cout << "Process detached successfully... " << endl; 
	
	return true;
}
