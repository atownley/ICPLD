
/*
 * * * * * * * * * * * * * * * * * * * * * *
 *                                         *
 * ICPLD Connection Uptime Checker         *
 * By: Erik Ljungström                     *
 * Mail: erik@ibiblio.org                  *
 * Alt mail: erik@northernmost.org         *
 * WWW: http://northernmost.org/           *
 * Copying: See COPYING file (BSD license) *
 *                                         *
 * * * * * * * * * * * * * * * * * * * * * *
 * -----------------------------------------------------------------------
 * Real Users never know what they want, but they always know when your
 * program doesn't deliver it.
 *                                 --Fortune
 * ------------------------------------------------------------------------
 *
 *  This is where the action is, start reading here.
 */

#include "cuc.h"

#include <iomanip>
#include <pwd.h>
#include <syslog.h>
#include <csignal>
#include <sys/utsname.h>
#include <cassert>

int
main (int argc, char *argv[])
{

  errno = 0;
  string homedir;

  if (getenv ("HOME") != NULL)
    homedir = getenv ("HOME");
  else
    {
      struct passwd *getHome = getpwuid (getuid ());
      homedir = getHome->pw_dir;
    }
  /* Can't go on without a homedir */
  assert(homedir.size()>0);

  string dir = homedir+"/.icpld", logFile, totFile, pidFile, statFile, ip;
  string confFile = "/usr/local/etc", confFile2 = "/etc";
  string logFile6, htmlFile6="", totFile6, ip6, fbip6;
  string htmlFile, statFile6;
  string fbip, savepwd;
  string cmd4up, cmd4dn;
  string cmd6up, cmd6dn;
  string iface ="";
  /* Only default values, may very well be overridden by user configuration  */
  string errors6 = homedir + "/.icpld/errors";
  string errors = homedir + "/.icpld/errors";
  logFile = homedir + "/.icpld/log";
  logFile6 = homedir+ "/.icpld/log";
  totFile = homedir + "/.icpld/tot";
  totFile6 = homedir + "/.icpld/tot6";
  pidFile = homedir + "/.icpld/icpld.pid";
  statFile = homedir + "/.icpld/stat";
  confFile += "/icpld.conf";
  confFile2 += "/icpld.conf";
  /* pinging interval once in the process of pinging (not to be mixed up with -interval) */
  string pint = "1.0";		
  double checkPint = 1.0;
  /* Gets current working directory */
  savepwd = getCwdir();
  /* Checks if it's the first time we run icpld, and if that's the case, files and directories
     will be created
  */
  firstRun (dir, logFile, totFile, statFile, htmlFile, htmlFile6, totFile6);
  int interval = 10, dinterval = 6, silent = 1, type = 0;
  bool noDaemon = false, noSound = false, haveHTML = false, gaveLog =
    false, gaveTurn = false, gaveQuit = false, haveV4 = true, haveV6 = false;
  /* If /usr/local/etc/icpld.conf exists...*/
  if (fileCheck (confFile) == "EX")
      /* Read it and set the variables */
      parseconf (logFile6, htmlFile6, fbip6, ip6, dir, homedir, logFile, totFile,
		 htmlFile, confFile, ip, fbip, pint, dinterval, interval,
		 noDaemon, noSound, cmd4up, cmd4dn, cmd6dn, cmd6up, errors, errors6, iface);
  /* Same procedure, but with /etc/icpld.conf */
  else if (fileCheck (confFile2) == "EX")
    {
      if (silent == 0)
	cout << "Could not find " << confFile << ". Falling back on " << confFile2 << endl;
      parseconf (logFile6, htmlFile6, fbip6, ip6, dir, homedir, logFile, totFile,
		 htmlFile, confFile2, ip, fbip, pint, dinterval, interval,
		 noDaemon, noSound, cmd4up, cmd4dn, cmd6dn, cmd6up, errors, errors6, iface);
    }

  else if (argc <= 1)
    {
      cout << "No " << confFile <<
	" file was found. And no command line arguments is available." <<
	endl;
      cout <<
	"Refer to 'man icpld' or 'icpld --help' for further information." <<
	endl;
      exit (EXIT_FAILURE);
    }
 
 /* Fix paths such as ~ and ./ and set them to the real path*/ 
  htmlFile = fixPath (htmlFile, homedir, savepwd);
  logFile = fixPath (logFile, homedir, savepwd);
  errors = fixPath (errors, homedir, savepwd);
#ifdef HAVE_IPV6
  errors6 = fixPath (errors6, homedir, savepwd);
  htmlFile6 = fixPath (htmlFile6, homedir, savepwd);
  logFile6 = fixPath (logFile6, homedir, savepwd);
#endif

  /* Parsing command line arguments and set variables accordingly */
  parseArgs (argc, argv, logFile6, htmlFile6, fbip6, ip6, dir, homedir, logFile,
	     totFile, htmlFile, confFile, ip, fbip, pint, dinterval, interval,
	     noDaemon, noSound, silent, checkPint, gaveQuit, pidFile,
	     statFile, gaveTurn, type, savepwd, gaveLog, totFile6, cmd4up, cmd4dn, cmd6dn, cmd6up, errors, errors6, iface );

  /* Fix them again (needed if we get a path at command line */ 
  htmlFile = fixPath (htmlFile, homedir, savepwd);
  logFile = fixPath (logFile, homedir, savepwd);
  errors = fixPath (errors, homedir, savepwd);
#ifdef HAVE_IPV6
  errors6 = fixPath (errors6, homedir, savepwd);
  htmlFile6 = fixPath (htmlFile6, homedir, savepwd);
  logFile6 = fixPath (logFile6, homedir, savepwd);
#endif

  /* Signal handlers */
  signal (SIGTERM, signal_handler);
  signal (SIGINT, signal_handler);
  signal (SIGQUIT, signal_handler);
  signal (SIGPIPE, signal_handler);
  signal (SIGUSR1, signal_handler);
  
  /* We already have a process running, or some clumpsy illerate user has force killed it */
  if (fileCheck (pidFile) == "EX" && silent == 1) 

    {
      cout << "Refusing to start, since there appears to be another ICPLD process running.\n";
      cout << "Issue `icpld -quit` and try again" << endl;
      exit(EXIT_FAILURE);
    }
  else if (fileCheck (pidFile) == "EX" && silent == 0)
    exit (EXIT_FAILURE);

  /* Verify the input, IE check for non existant files, broken permissions etc. */
  checkInp (logFile6, htmlFile6, htmlFile, logFile, ip6, ip, haveV6, fbip,
	    fbip6, checkPint, haveHTML, pint, interval, dinterval, haveV4,
	    type, errors, errors6);
  /* Tag the logs with necessary headers */
  wlogs (type, logFile, logFile6, ip6, haveV4, haveV6, htmlFile, htmlFile6,
	 ip, fbip, fbip6, haveHTML, statFile);

  /* Set the commands that we'll popen().. 
     Since the 'ping' command is mighty reliable, widely available and 
     already is setuid, I've decided to use it in order to
     save users from having to run icpld in a suid environment.
   */
  std::string command, command2;
#ifdef HAVE_IPV6
  std::string command6, command62;
#endif
#ifdef SOLARIS
  /* Necssary since it would be silly to use a default value for 'iface'*/
  if (iface.size() > 0){
    command = "ping -i " + iface + " " + ip + " 2";
    command2 = "ping -i " + iface + " " + fbip + " 2";
  }
  else{
    command = "ping " + ip + " 2";
    command2 = "ping " + fbip + " 2";
  }
#ifdef HAVE_IPV6
 if (haveV6)
    {
      /* PING6 is defined in config.h and is either set to ping6 or ping -A inet6 
         depending on what's available
      */
      if (iface.size() > 0) {
	command6 = PING6;
	command6 += " -i " + iface + " "  + ip6 + " 2";  //Solaris equivalent of ping6
	command62 = PING6;
	command62 += " -i" + iface + " " + fbip6 + " 2";
      }
      command6 = PING6;
      command6 += " -i " + iface + " "  + ip6 + " 2";  //Solaris equivalent of ping6
      command62 = PING6;
      command62 += " -i" + iface + " " + fbip6 + " 2";
    }
#endif /* Solaris ping 6 */
#endif

/* Since MacOSX's ping returns 0 even if the ping fails, we'll have to rely
   on the return code of `grep' instead. No bytes, no replies.. *sigh* AND
   there's no sensible -I option *sigh sigh*
*/
#ifdef DARWIN 
 command = "ping -i " + pint + " -n -c 1 " + ip + " |grep \"bytes from\"";
 command2 = "ping " + " -i " + pint + " -n -c 2 " + fbip + " |grep \"bytes from\"";
#ifdef HAVE_IPV6
  command6 = PING6;
  command6 += " -i " +  pint + " -n -c 2 " + ip6 + " |grep \"bytes from\"";
  command62 =  PING6;
  command62 += " -i " + pint + " -n -c 2 " + fbip6 + " |grep \"bytes from\"";
#endif
#endif

  /* As far as I know, it's only Solaris and MacOSX that needs special treatment */
#if !defined(SOLARIS) && !defined(DARWIN)
  if (iface.size() > 0){
    command = "ping -I " + iface + " -i " + pint + " -n -c 2 " + ip + " 2>&1";
    command2 = "ping -I " + iface + " -i " + pint + " -n -c 2 " + fbip + " 2>&1";
  }
  else 
    command = "ping -i " + pint + " -n -c 2 " + ip + " 2>&1";
    command2 = "ping -i " + pint + " -n -c 2 " + fbip + " 2>&1";
#ifdef HAVE_IPV6
  if (haveV6 && iface.size() > 0)
    {
      command6 = PING6;
      command6 += " -I " + iface + " -i " + pint + " -n -c 2 " + ip6 ;
      command62 = PING6;
      command62 += " -I " + iface + " -i " + pint + " -n -c 2 " + fbip6;
    }
  else
    {
      command6 = PING6;
      command6 += " -i " + pint + " -n -c 2 " + ip6 ;
      command62 = PING6;
      command62 += " -i " + pint + " -n -c 2 " + fbip6;
    }
#endif
#endif
  std::string totals;
  bool conDown = false, fbResp = false;
  int total = 0, seconds = 0, minutes = 0, hours = 0, loops = 0;
  int totSec = 0, totMin = 0, totHour = 0, rets =0, totDrop = 0;
  time_t timer, starttime = 0, endtime = 0;
  struct tm *tblock;
  timer = time (NULL);
  tblock = localtime (&timer);
  char bufferSOL[1024], errBuf[1024];
#ifdef HAVE_IPV6
  char bufferSOL6[1024], errBuf6[1024];
#endif

  std::ofstream toStat (statFile.c_str (), std::ios::app);
  toStat << "UP" << endl << "Has not been down" << endl;
  /* Necessary to keep the formatting since asctime() which
     will be written to this file when a connection has come up
     generates a \n
  */
 toStat << "" << endl;
  if (type != 6)
    toStat << ip << endl;
  else /* IPv4 ip won't be shown if icpld is ran in IPv6-only mode */
    toStat << "" << endl;
  toStat << interval << endl;
  toStat << dinterval << endl;
  if (type != 6)
    toStat << logFile << endl << htmlFile << endl;
  else
    toStat << "" << endl << "" << endl;
  if (type !=4)
    toStat << ip6 << endl << logFile6 << endl << htmlFile6 << endl;
  else 
    toStat << "" << endl << "" << endl << "" << endl;
  toStat.close();



  
  /* Reads the total amount of broken networking occasions
     and for how long it's been down in total
  */
  std::ifstream readTot (totFile.c_str (), ios::binary);
  while (getline (readTot, totals))
    {
      loops++;
      if (loops == 1)
	total = atoi (totals.c_str ());
      else if (loops == 2)
	totSec = atoi (totals.c_str ());
      totals = "";
    }
  loops = 0;
  readTot.close ();

  std::ofstream toStat2 (statFile.c_str (), std::ios::app);
  if (!noDaemon){
    toStat2 << "Daemonized" << endl;
    daemonize (1, 0, silent);
  }
  else{
    cout << "Staying in foreground.." << endl;
    toStat2 << "Foreground" << endl;
  }
  toStat2.close();
  if (fileCheck (pidFile) == "ENP")
    {
      openlog ("ICPLD", LOG_PID, LOG_USER);
      syslog (LOG_ERR, "Could not write pid file. Make sure you have permissions to write to ~/.icpld");
      closelog ();
      cerr << "Can not write pid-file. Terminating..." << endl;
      raise(SIGTERM);
    }

  std::ofstream writePid (pidFile.c_str (), ios::app);
  writePid << getpid () << endl;
  writePid.close ();
   


#ifdef HAVE_IPV6
  std::string totals6;
  bool conDown6 = false;
  bool fbResp6 = false;
  int total6 = 0, seconds6 = 0, minutes6 = 0, hours6 = 0, loops6 = 0;
  int totSec6 = 0, totMin6 = 0, totHour6 = 0, rets6, totDrop6=0;
  time_t timer6, starttime6 = 0, endtime6 = 0;
  struct tm *tblock6;
  timer6 = time (NULL);
  tblock6 = localtime (&timer6);
  char buf6[BUFSIZ] = "";

#endif
  
  /* Here's the deal. First we open a pipe to a shell, and executes the command defined above.
     Then we close the pipe and see if the return code is 0. If it is, it indicates that the ping process
     was successful, and we'll run through to the end of the loop where it will sleep for as many seconds as
     interval is set to. If it does NOT return 0, it has failed (obviously). If the initial ping has 
     failed, we want to make sure it's not the peering machine that's gone under, so we check if we have
     a fallback ip. If we do, we will repeat the process, but with the fallback ip. 
     If this fails, or there wasn't a fallback ip 
     specified, conDown will be set to true. It will check if the logfile is writeable and if it is, it will
     stamp the log. It will also check if the user has specified a HTML-output file and write to it if that's
     the case. Additionally, it will spawn a timer process which will keep on ticking until a pinging process
     returns 0. Finally, it will update ~/.icpld/stat. At the end of the loop it will check the value of conDown, and
     sleep for as long as dinterval. Then it will try again. 
     If the pinging process returns 0 this time conDown is set to false and the log-checking process is ran 
     again, only this time it'll stamp it with a message of success.
  */

  FILE *ptr, *ptr2;
#ifdef HAVE_IPV6
  FILE *ptr6, *ptr62;
#endif
	 
  do
    { 
      /* If we have an IPv4 ip and has not specified that we want 
         exclusively check an IPv6 connection.. go for it
       */
      if (haveV4 && type != 6)
	{
	  cout << flush;
	  errno = 0;
	  cout << "Sending ICMP request to " << ip << "..." << endl;
       	  ptr = popen (command.c_str(), "r");

	  if (ptr == NULL)
	    {
	      perror ("popen()");
	      cout << "Cleaning up and aborting..." << endl;
	      openlog ("ICPLD", LOG_PID, LOG_USER);
	      syslog (LOG_ERR,
		      "popen() failed. Could open pipe to shell. Terminating..");
	      closelog ();
	      raise(SIGTERM);
	      exit(EXIT_FAILURE);
	    }

	  if (feof(ptr) != 0 || ferror(ptr) != 0)
	    clearerr(ptr);
	  /* Empty the buffer */
	  strncpy(bufferSOL, "", 1);
	  strncpy(errBuf, "",1);
	  while (fgets(bufferSOL, sizeof(bufferSOL), ptr) != NULL){
	    /* And write the ping output to errBuf so we can put it in
	       the log later on if the connection has been broken */
	    strncat(errBuf, bufferSOL, sizeof(bufferSOL)); 
	  }

 

/* OBSOLETE?

	 #ifdef DARWIN
	 char bufferMAC[80];int sooooWierd=fread(bufferMAC,1,79,ptr);
	 sooooWierd=0;	 
	 #endif 
*/
	  /* Get the actual return value from the command issued in popen() */
	  rets = pclose (ptr);

	if (rets == -1){
	    perror ("pclose()");
	    cout << "Cleaning up and aborting..." << endl;
	    openlog ("ICPLD", LOG_PID, LOG_USER);
	    syslog (LOG_ERR,
		    "popen() failed. Could not close the pipe to the shell. Terminating..");
	    closelog ();
	    raise(SIGTERM);
	    exit(EXIT_FAILURE);
	  } 
	  else if (rets == 32256 || rets == 127 || rets == 126)	/* Gutty guesses. */
	    {
	      cout <<
		"You do not seem to have the permissions to send ICMP requests on this system. Use\n";
	      cout << "a more privilegied user or alter permissions" << endl;
	      openlog ("ICPLD", LOG_PID, LOG_USER);
	      syslog (LOG_ERR,
		      "Could not execute 'ping'. Place the ping binary with proper permissions somewhere in your $PATH");
	      closelog ();
	      /* Suicide! */
	      raise(SIGTERM);

	    }


	  if (rets != 0)	/* Ping returns 0 on success */
	    {
	      /* The ping failed, but if we have a fallback ip, try it */
	      if (fbip.size () > 0)
		{
		  cout << ip <<
		    " isn't responding. Trying fallback ip: " << fbip << endl;
		  ptr2 = popen (command2.c_str (), "r");
		  if (ptr2 == NULL)
		    {
		      perror ("popen()");
		      cout << "Cleaning up and aborting..." << endl;
		      openlog ("ICPLD", LOG_PID, LOG_USER);
		      syslog (LOG_ERR,
			      "popen() failed. Could open pipe to shell, reason unknown. Terminating..");
		      closelog ();
		      raise (SIGTERM);
		    }
          while (fgets(bufferSOL, sizeof(bufferSOL), ptr2) != NULL)
            strncat(errBuf, bufferSOL, sizeof(bufferSOL));


		  if (pclose (ptr2) == 0)
		    {
		      /* If the connection hasn't been marked as dead... */
		      if (!conDown)
			cout << "Got reply from " << fbip << endl;
		      /* If it has been marked as dead... */
		      else{
			system(cmd4up.c_str());
			cout << fbip << " replies. Connection back up!" << endl;
		      }
		      fbResp = true;
		      rets = 0;
		    }
		  else
		    {
		      cout << fbip << " is not responding. Connection broken."
			   << endl;
		      fbResp = false;
		    }
		}
	      else
		{
		  cout <<
		    "No fallback ip specified. Assuming connection is unavailable."
		       << endl;
		  fbResp = false;
		}
	      /* fbResp is set to false if the fallback ip fails to reply as well.
	         Write logs and start timer accordingly
	      */

	      if (!fbResp)
		{

		  std::ofstream toLog (logFile.c_str (), std::ios::app);

		  std::ifstream readTot (totFile.c_str (), std::ios::app);
		  while (getline (readTot, totals))
		    {
		      loops++;
		      if (loops == 1)
			total = atoi (totals.c_str ());
		      else if (loops == 2)
			totSec = atoi (totals.c_str ());
		      totals = "";
		    }
		  loops = 0;
		  readTot.close ();
		  if (!conDown)
		    {
		      ofstream toErr(errors.c_str(), ios::app);
		      /* TODO  Maybe a function which parses errBuf to only contain
			 line #2 and 3 from the output here?
		       */
		      toErr << "[IPv4] " <<  asctime(tblock) << errBuf << endl;
		      toErr.close();
		      system(cmd4dn.c_str());
		      timer = time (NULL);
		      tblock = localtime (&timer);
		      starttime = time (NULL);
		      toLog << "IPv4: Connection down: " << asctime (tblock);
		      if (haveHTML)
			{
			  std::ofstream toHTML (htmlFile.c_str (), std::ios::app);
			  toHTML << "IPv4: Connection down: " <<
			    asctime (tblock) << "<br>";
			  toHTML.close ();
			}
		      total += 1;
		      if (remove (statFile.c_str ()) == -1)
			{
			  cout << "Could not remove " << statFile << "..." <<
			    endl;
			  perror ("remove()");
			}
		      std::ofstream toStat (statFile.c_str (), std::ios::app);
		      toStat << "DOWN" << endl <<
			asctime (tblock) << endl << ip
			     << endl << interval << endl <<
			dinterval << endl << logFile
			     << endl << htmlFile << endl << ip6 << endl << logFile6
			     << endl << htmlFile6 << endl;
		      toStat.close ();
		    }
		  toLog.close ();

		  conDown = true;
		}
	    }

	  if (rets == 0)
	    {
	      /* If we're recovering from downtime, possibly beep and 
	         exclaim joy! 
               */
	      if (conDown)
		{
		  if (!noSound && !noDaemon)
		    cout << "\a \a \a" << endl;
		  if (!fbResp){
		    cout << "Got ICMP reply from " << ip <<
		      ".. Connection available!" << endl;
		    system (cmd4up.c_str());
		  }
		  endtime = time (NULL);
		  seconds = endtime - starttime;
		  /* Read the file containing the info on the total downtime and total
		     amounts of lost connection....
		  */
		  std::ifstream readTot (totFile.c_str (), ios::binary);
		  while (getline (readTot, totals))
		    {
		      loops++;
		      if (loops == 1)
			total = atoi (totals.c_str ());
		      else if (loops == 2)
			totSec = atoi (totals.c_str ());
		      totals = "";
		    }
		  loops = 0;
		  readTot.close ();
		  totSec += seconds;
		  /* ... and remove it... */
		  if ((remove (totFile.c_str ()) == -1) && errno != ENOENT)
		    {
		      cout << "Could not overwrite " << totFile << "..." <<
			endl;
		      perror ("remove()");
		    }
		  /* ... just to recreate it with the updated values */
		  std::ofstream toTot (totFile.c_str (), std::ios::app);
		  toTot << total << endl << totSec << endl;
		  toTot.close ();
		  /* Parse the seconds... */
		  if (seconds >= 60)
		    {
		      minutes = seconds / 60;
		      if (minutes >= 60)
			{
			  hours = minutes / 60;
			  minutes %= 60;
			}
		      seconds = seconds % 60;
		    }


		  if (totSec >= 60)
		    {
		      totMin = totSec / 60;
		      if (totMin >= 60)
			{
			  totHour = totMin / 60;
			  totMin %= 60;
			}
		      totSec = totSec % 60;
		    }

		  timer = time (NULL);
		  tblock = localtime (&timer);
		  /* And write it to the logfile */ 
		  std::ofstream toLog (logFile.c_str (), std::ios::app);

		  toLog << "IPv4: connection up: " <<
		    asctime (tblock) << "\tDown for: " <<
		    setw (2) << setfill ('0') << hours <<
		    ":" << setw (2) << setfill ('0') <<
		    minutes << ":" << setw (2) <<
		    setfill ('0') << seconds << endl;
		  toLog << "\t\tIPv4 Total: " << setw (2) <<
		    setfill ('0') << totHour << ":" <<
		    setw (2) << setfill ('0') << totMin <<
		    ":" << setw (2) << setfill ('0') << totSec << endl;
		  totDrop++;
		  toLog << "\tIPv4 connection dropped " << totDrop << " times." << endl;
		  toLog.close ();

		  /* And duplicate the log to the HTML-file*/
		  std::ofstream toHTML (htmlFile.c_str (), std::ios::app);
		  toHTML << "IPv4: connection up: " <<
		    asctime (tblock) <<
		    "\tDown for: " << setw (2) <<
		    setfill ('0') << hours << ":"
			 << setw (2) << setfill ('0')
			 << minutes << ":" << setw (2)
			 << setfill ('0') << seconds << endl;
		  toHTML << "\t\tIPv4 Total: " << setw (2)
			 << setfill ('0') << totHour <<
		    ":" << setw (2) <<
		    setfill ('0') << totMin << ":"
			 << setw (2) << setfill ('0') << totSec << endl;

		  toHTML << "\tIPv4 connection dropped " << totDrop << " times." << endl;
		  toHTML.close ();
		  /* Reset everything */
		  seconds = 0;
		  minutes = 0;
		  hours = 0;
		  totSec = 0;
		  totMin = 0;
		  totHour = 0;
		  if (remove (statFile.c_str ()) == -1)
		    {
		      cout << "Could not overwrite status file.. " << endl;
		      perror ("remove()");
		    }
		  /* Update the stats file */
		  std::ofstream toStat (statFile.c_str (), std::ios::app);
		  toStat << "UP" << endl << asctime (tblock) <<
		    endl << ip << endl << interval << endl
			 << dinterval << endl << logFile <<
		    endl << htmlFile << endl << ip6 << endl << logFile6 <<
		    endl << htmlFile6 << endl;
		  toStat.close ();
		  conDown = false;
		}
	    }
	}

 
#ifdef HAVE_IPV6


      /* This code is just about identical to the one for IPv4 above */
      if (haveV6 && type != 4)
	{


	  cout << flush;
	  strncpy (buf6, "", 1);
	  cout << "Sending ICMP request to " << ip6 << "..." << endl;
	  ptr6 = popen (command6.c_str (), "r");
	  if (ptr6 == NULL)
	    {
	      perror ("popen()");
	      cout << "Cleaning up and aborting..." << endl;
	      openlog ("ICPLD", LOG_PID, LOG_USER);
	      syslog (LOG_ERR,
		      "popen() failed. Could open pipe to shell, reason unknown. Terminating..");
	      closelog ();
	      raise(SIGTERM);
	    }

	  if (feof(ptr6) != 0 || ferror(ptr6) != 0)
	    clearerr(ptr6);
	  /* Empty the buffer */
	  strncpy(bufferSOL6, "", 1);
	  strncpy(errBuf6, "",1);
	  while (fgets(bufferSOL6, sizeof(bufferSOL6), ptr6) != NULL){
	    /* And write the ping output to errBuf so we can put it in
	       the log later on if the connection has been broken */
	    strncat(errBuf6, bufferSOL6, sizeof(bufferSOL6)); 
	  }

	  rets6 = pclose (ptr6);
	  if (rets6 == 32256 || rets == 32512 || rets6 == 127 || rets6 == 126)	/* Gutty guesses. */
	    {
	      cout <<
		"You do not seem to have the permissions to send ICMP requests on this system. Use\n";
	      cout << "a more privilegied user or alter permissions" << endl;
	      openlog ("ICPLD", LOG_PID, LOG_USER);
	      syslog (LOG_ERR,
		      "Could not execute 'ping6'. Place the ping6 binary with proper permissions somewhere in your $PATH");
	      closelog ();
	      raise(SIGTERM);

	    }


	  if (rets6 != 0)		/* ping6 also returns 0 on success */
	    {

	      if (fbip6.size () > 0)
		{
		  cout << ip6 << " isn't responding. Trying fallback ip: " << fbip6 << endl;
		  ptr62 = popen (command62.c_str (), "r");
		  if (ptr62 == NULL)
		    {
		      perror ("popen()");
		      cout << "Cleaning up and aborting..." << endl;
		      openlog ("ICPLD", LOG_PID, LOG_USER);
		      syslog (LOG_ERR, "popen() failed. Could open pipe to shell, reason unknown. Terminating..");
		      closelog ();
		      raise (SIGTERM);
		    }
          while (fgets(bufferSOL6, sizeof(bufferSOL6), ptr62) != NULL)
            strncat(errBuf6, bufferSOL6, sizeof(bufferSOL6));

		  if (pclose (ptr62) == 0) /* if ping6 returns 0 on fbip6 */
		    {
		      if (!conDown6)
			cout << "Got reply from " << fbip6 << endl;
		      else{
			system(cmd6up.c_str());
			cout << fbip6 << " replies. Connection back up!" << endl;
		      }
		      fbResp6 = true;
		      rets6 = 0;
		    }
		  else /* Anything other than 0 is considered failure */
		    {
		      cout << fbip6 << " is not responding. Connection broken." <<
			endl;
		      fbResp6 = false;
		    }
		}
	      else /* No fallback */
		{
		  cout <<
		    "No IPv6 fallback ip specified. Assuming connection is unavailable."
		       << endl;
		  fbResp6 = false;
		}

	      if (!fbResp6)
		{
		  std::ofstream toLog6 (logFile6.c_str (), ios::app);
		  std::ifstream readTot6 (totFile6.c_str (), ios::binary);
		  while (getline (readTot6, totals6))
		    { 
		      loops6++;
		      if (loops6 == 1)
			total6 = atoi (totals6.c_str ());
		      else if (loops6 == 2)
			totSec6 = atoi (totals6.c_str ());
		      totals6 = "";
		    }
		  loops6 = 0;
		  readTot6.close ();
		  if (!conDown6)
		    {

		      std::ofstream toErr6(errors6.c_str(), std::ios::app);
		      /* TODO  Maybe a function which parses errBuf to only contain
			 line #2 and 3 from the output here?
		       */
		      toErr6 << "[IPv6] " << asctime(tblock) << errBuf6 << endl;
		      toErr6.close();
		      system(cmd6dn.c_str());
		      timer6 = time (NULL);
		      tblock6 = localtime (&timer6);
		      starttime6 = time (NULL);
		      toLog6 << "IPv6: Connection down: " << asctime (tblock6);
		      toLog6.close();
		      std::ofstream toHTML (htmlFile6.c_str (), ios::app);
		      toHTML << "IPv6: Connection down: " << asctime (tblock6);
		      toHTML.close ();
		      total6 += 1;
		      if (remove (statFile.c_str ()) == -1)
			{
			  cout << "Could not remove " << statFile << "..." << endl;
			  perror ("remove()");
			}
		      std::ofstream toStat (statFile.c_str (), std::ios::app);
		      toStat << "IPv6: DOWN" << endl <<
			asctime (tblock6) << endl << ip
			     << endl << interval << endl <<
			dinterval << endl << logFile
			     << endl << htmlFile << endl << ip6;
		      toStat.close ();
		    }

		  conDown6 = true;
		}
	    }

	  if (rets6 == 0)
	    {
	      if (conDown6)
		{
		  if (!noSound && !noDaemon)
		    cout << "\a \a \a" << endl;
		  if (!fbResp6){
		    system (cmd6up.c_str());
		    cout << "Got ICMP reply from " << ip6 <<
		      ".. Connection available!" << endl;
		  }
		  endtime6 = time (NULL);
		  seconds6 = endtime6 - starttime6;
		  std::ifstream readTot6 (totFile6.c_str (), ios::app);
		  while (getline (readTot6, totals6))
		    {
		      loops6++;
		      if (loops6 == 1)
			total6 = atoi (totals6.c_str ());
		      else if (loops6 == 2)
			totSec6 = atoi (totals6.c_str ());
		      totals6 = "";
		    }
		  loops6 = 0;
		  readTot6.close ();
		  totSec6 += seconds6;
		  if (remove (totFile6.c_str ()) == -1)
		    {
		      cout << "Could not overwrite " << totFile6 << "..." << endl;
		      perror ("remove()");
		    }
		  std::ofstream toTot6 (totFile6.c_str (), ios::app);
		  toTot6 << total6 << endl << totSec6 << endl;
		  toTot6.close ();
		  if (seconds6 >= 60)
		    {
		      minutes6 = seconds6 / 60;
		      if (minutes6 >= 60)
			{
			  hours6 = minutes6 / 60;
			  minutes6 %= 60;
			}
		      seconds6 = seconds6 % 60;
		    }


		  if (totSec6 >= 60)
		    {
		      totMin6 = totSec6 / 60;
		      if (totMin6 >= 60)
			{
			  totHour6 = totMin6 / 60;
			  totMin6 %= 60;
			}
		      totSec6 = totSec6 % 60;
		    }


		  timer6 = time (NULL);
		  tblock6 = localtime (&timer6);
		  std::ofstream toLog (logFile6.c_str (), std::ios::app);

		  toLog << "IPv6: connection up: " <<
		    asctime (tblock6) << "\tDown for: " <<
		    setw (2) << setfill ('0') << hours6 <<
		    ":" << setw (2) << setfill ('0') <<
		    minutes << ":" << setw (2) <<
		    setfill ('0') << seconds6 << endl;
		  toLog << "\t\tIPv6 Total: " << setw (2) <<
		    setfill ('0') << totHour6 << ":" <<
		    setw (2) << setfill ('0') << totMin6 <<
		    ":" << setw (2) << setfill ('0') << totSec6 << endl;
		  totDrop6++;
		  toLog << "\tIPv6 connection dropped: " << totDrop6 << " times." << endl;
		  toLog.close ();
		    
		  std::ofstream toHTML (htmlFile6.c_str (), ios::app);
		  toHTML << "IPv6: connection up: " << asctime (tblock6) <<
		    "\tDown for: " << setw (2) << setfill ('0') << hours6 <<
		    ":" << setw (2) << setfill ('0') << minutes6 << ":" <<
		    setw (2) << setfill ('0') << seconds6 << endl;
		  toHTML << "\t\tIPv6 Total: " << setw (2) << setfill ('0') <<
		    totHour6 << ":" << setw (2) << setfill ('0') << totMin6 <<
		    ":" << setw (2) << setfill ('0') << totSec6 << endl;
		  toHTML << "\tIPv6 connection dropped: " << totDrop6 << " times." << endl;
		  toHTML.close ();
		   
		  seconds6 = 0;
		  minutes6 = 0;
		  hours6 = 0;
		  totSec6 = 0;
		  totMin6 = 0;
		  totHour6 = 0;
		  if (remove (statFile.c_str ()) == -1)
		    {
		      cout << "Could not overwrite statFile.. " << endl;
		      perror ("remove()");
		    }
		  std::ofstream toStat (statFile.c_str (), std::ios::app);
		  toStat << "UP" << endl << asctime (tblock6) <<
		    endl << ip6 << endl << interval << endl
			 << dinterval << endl << logFile6 << endl << htmlFile6;
		  toStat.close ();
		  conDown6 = false;
		}
	    }
	}
#endif
#ifdef HAVE_IPV6
      if (conDown || conDown6)
	sleep (dinterval);
      else
	sleep (interval);
#endif
#ifndef HAVE_IPV6
      if (conDown)
	sleep (dinterval);
      else
	sleep (interval);
#endif
    }				/*do */
  while (1);
  return EXIT_SUCCESS; /* This is executed extremely often.. */
}
