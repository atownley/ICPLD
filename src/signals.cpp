/*
   This file handles all the signals that
   may be sent to icpld during runtime.

   It catches SIGUSR1, INTERRUPT, SIGTERM and SIGQUIT
   and removes the pid file as well as terminate
   itself (unless SIGUSR1 is caught).
*/
#include "cuc.h"
#include <pwd.h>
#include <csignal>

void
signal_handler (int sig)
{

  /* Find and remove the pid-file */
  string remFile;	
  char *getHome;
  if ((getHome = getenv ("HOME")) != NULL)
    remFile = getHome;
	
  else
    {
      struct passwd *getHome = getpwuid (getuid ());
      remFile = getHome->pw_dir;
      cout << "Could not read environment variable $HOME.. It's very recomended to set this." << endl;
      cout << "Reverted to passwd struct.." << endl;
    }
	
  remFile += "/.icpld/icpld.pid";
  if (remove (remFile.c_str ()) == -1)
    cout << "Could not remove " << remFile << ": " << strerror(errno) << endl;

  if (sig == SIGINT) /* 2 */
    cout << "Caught INTERRUPT (signal " << sig << "), aborting..."  << endl;
  else if (sig == SIGTERM) /* 15 */
    cout << "Caught SIGTERM (signal " << sig << "), aborting..."  << endl;
  else if (sig == SIGQUIT) /* 3 */
    cout << "Caught SIGQUIT (signal " << sig << "), aborting..." << endl;
  else if (sig == SIGUSR1) { /* 10 */
    cout << "Caught SIGUSR1 (signal " << sig << "), detaching..." << endl;
    daemonize(1, 0, 1);
    /* The PID file has already been removed, so it's safe to write
       the new PID
    */
    std::ofstream writePid (remFile.c_str (), ios::app);
    writePid << getpid () << endl;
    writePid.close ();
    return;
  }
  else if (sig == SIGPIPE){ /* 13 This one is hopefully rare */
    cout << "Caught SIGPIPE (signal " << sig << "), aborting..." << endl;
    cout << "This signal is caught when a program tries to write to something";
    cout << "\nthat isn't there. Do you have a shell?" << endl;
  }

 
  if (sig != SIGPIPE)
    exit (EXIT_SUCCESS);
  else
    exit (EXIT_FAILURE);
}
