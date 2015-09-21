/*
  This function parses the arguments given
  from command line. It reads the input, and 
  sets the values accordingly
*/


#include "cuc.h"

int parseArgs(int argc, char * const argv[], std::string &logFile6, std::string &htmlFile6, std::string &fbip6, std::string &ip6, std::string &dir, std::string apa, std::string &logFile, std::string &totFile, std::string &htmlFile, std::string confFile, std::string &ip, std::string &fbip, std::string &pint, int &dinterval, int &interval, bool &noDaemon, bool &noSound, int &silent, double &checkPint, bool &gaveQuit, std::string &pidFile, std::string &statFile, bool &gaveTurn, int &type, std::string savepwd, bool &gaveLog, std::string totFile6, std::string &cmd4up, std::string &cmd4dn, std::string &cmd6dn,std::string &cmd6up, std::string &errors, std::string &errors6, string &iface  )
{

  int i = 1;
  bool gaveMore = true, gaveStatus = false;
  bool gaveIp=false, gaveFbip=false, gaveIp6=false, gaveFbip6=false;
  while (i < argc)
    {
      if (strcmp (argv[i], "-ip") == 0)
	{
	  if (argv[i + 1])
	    ip = argv[i + 1];
	  else
	    wrongArg(argv[i]);
	  gaveIp=true;
	  i++;
	}
#ifdef HAVE_IPV6
      else if (strcmp(argv[i], "-ip6")==0)
	{
	  if (argv[i+1])
	    ip6 = argv[i+1];
	  else
	    wrongArg(argv[i]);
	  i++;
	  gaveIp6=true;
	}
      
      else if (strcmp(argv[i], "-fbip6") == 0)
	{
	  if (argv[i+1])
	    fbip6 = argv[i+1];
	  else
	    wrongArg(argv[i]);
	  i++;
	  gaveFbip6=true;
	}
#endif
      else if (strcmp(argv[i], "-s")==0)
	silent = 0;
        
      else if (strcmp(argv[i], "-detach")==0){
	if (!killProc(pidFile, silent, SIGUSR1, statFile))
	  exit(EXIT_FAILURE);
	else
	  exit(EXIT_SUCCESS);
      }
      else if (strcmp(argv[i], "-iface") ==0){
	if (argv[i+1])
	  iface = argv[i+1];
	else
	  wrongArg(argv[i]);
	i++;
      }
      else if (strcmp (argv[i], "-pint") == 0)
	{
	  if (argv[i + 1])
	    pint = argv[i + 1];
	  else
	    wrongArg(argv[i]);
	  for (unsigned int l = 0; l < strlen (argv[i + 1]); l++)
	    {
	      if ((isalpha (argv[i + 1][l]) != 0)
		  && argv[i + 1][l] != '.')
		{
		  cout << "The value of pint must not contain anything but decimal numbers";
		  cout << "Aborting..." << endl;
		  exit(EXIT_FAILURE);
		}

	    }
	  checkPint = strtod (pint.c_str (), (char **) NULL);
	  i++;
	}
      else if (strcmp (argv[i], "-d") == 0)
	noDaemon = false;
      else if (strcmp (argv[i], "--help") == 0 || strcmp (argv[i], "-h") == 0){
	help ();
	exit(EXIT_SUCCESS);
      }
      else if (strcmp (argv[i], "-nd") == 0)
	noDaemon = true;
      else if (strcmp (argv[i], "-quit") == 0)
	gaveQuit = true;
      else if (strcmp (argv[i], "-status") == 0
	       || strcmp (argv[i], "-stat") == 0)
	{
	  gaveStatus=true;

	}
      else if (strcmp (argv[i], "-fbip") == 0)
	{
	  if (argv[i + 1])
	    fbip = argv[i + 1];
	  else
	    wrongArg(argv[i]);
	       
	  i++;
	  gaveFbip=true;
	}
      else if (strcmp (argv[i], "-nobeep") == 0)
	noSound = true;

      else if (strcmp (argv[i], "-turn") == 0 || strcmp(argv[i], "-rotate") == 0)
	{
	  gaveTurn = true;
	}
      else if (strcmp (argv[i], "-reset") == 0)
	{
	  reset(apa);
	  exit(EXIT_SUCCESS);
	}
#ifdef HAVE_IPV6
      else if (strcmp (argv[i], "-6")==0){
	type = 6;
      }
      else if (strcmp (argv[i], "-4")==0){
	type = 4;
      }
      else if (strcmp (argv[i], "-errfile6")==0){
	if (argv[i+1])
	  errors6 = argv[i+1];
	else
	  wrongArg(argv[i]);
	i++;
      }
#endif
      else if (strcmp (argv[i], "-errfile")==0){
	if (argv[i+1])
	  errors = argv[i+1];
	else
	  wrongArg(argv[i]);
	i++;
      }
      else if (strcmp (argv[i], "-logfile") == 0)
	{
	  logFile = "";
	  if (argv[i + 1]){
	    logFile = argv[i+1]; 
	    logFile = fixPath(logFile, apa, savepwd);
	  }
			
	  else
	    wrongArg(argv[i]);
	  i++;

	}

#ifdef HAVE_IPV6
      else if (strcmp (argv[i], "-logfile6") == 0)
	{
	  logFile6="";
	  if (argv[i+1])
	    {
	      logFile6 = argv[i+1];
	      logFile6 = fixPath(logFile6, apa, savepwd);
	    }
	  else
	     wrongArg(argv[i]);	
	  i++;

	}
      else if (strcmp (argv[i], "-htmlfile6") == 0 && strcmp (argv[i], "-htmlfile") != 0)
	{
	  if (argv[i + 1]){
	    htmlFile6 = "";
	    htmlFile6 = argv[i + 1];
	    htmlFile6 = fixPath (htmlFile6, apa, savepwd);
	  }
		
	  else
	    wrongArg(argv[i]);	
			
		
	  i++;
	}
#endif

      else if (strcmp (argv[i], "-htmlfile") == 0 && strcmp(argv[i], "-htmlfile6") != 0)
	{
	  if (argv[i + 1]){
	    htmlFile = "";
	    htmlFile += argv[i + 1];
	    htmlFile = fixPath (htmlFile, apa, savepwd);
	  }
		
	  else
	    wrongArg(argv[i]);	
			
		
	  i++;
	}

      else if (strcmp (argv[i], "-log") == 0)
	{
	  gaveLog = true;	/* Needs to be done last, if -logfile is specified. */
	}

      else if (strcmp (argv[i], "-nosound") == 0)
	noSound = true;
      else if (strcmp (argv[i], "-interval") == 0)
	{
	  if (!argv[i + 1])
	    wrongArg(argv[i]);
	  for (unsigned int ch = 0; ch < strlen (argv[i + 1]);
	       ch++)
	    {
	      if (isalpha (argv[i + 1][ch]) != 0)
		{
		  cout << "The value of -interval must be of type integer" << endl;
		  exit(EXIT_FAILURE);
		}
	    }
	  interval = atoi (argv[i + 1]);
	  if (interval <= 3)
	    {
	      cout << "-interval should not be less than or equal to 3 seconds, as the timeout for two ICMP requests\n";
	      cout << "is roughly 3 seconds. Setting dinterval to this will result in more than one request at once." << endl;
	      exit(EXIT_FAILURE);
	    }
	  i++;
	}

      else if (strcmp (argv[i], "-err") == 0)
	{

	  dispErrs(errors, 4);

	  exit(EXIT_SUCCESS);
	}

      else if (strcmp (argv[i], "-err6") == 0)
	{

	  dispErrs(errors, 6);

	  exit(EXIT_SUCCESS);
	}


      else if (strcmp (argv[i], "-v") == 0
	       || strcmp (argv[i], "--version") == 0)
	{
	  cout << PACKAGE << endl;
	  exit(EXIT_SUCCESS);
	}
      else if (strcmp (argv[i], "-m") == 0)
	{
	  gaveMore = false;
	}

      else if (strcmp (argv[i], "-dinterval") == 0)
	{
	  if (!argv[i + 1])
	    wrongArg(argv[i]);
	  for (unsigned int ch2 = 0; ch2 < strlen (argv[i + 1]); ch2++)
	    {
	      if (isalpha (argv[i + 1][ch2]) != 0)
		{
		  cout << "The value of -dinterval must be of type integer" << endl;
		  exit(EXIT_FAILURE);
		}
	    }
	  dinterval = atoi (argv[i + 1]);
	  if (dinterval <= 3)
	    {
	      cout << "-dinterval should not be less than or equal to 3 seconds, as the timeout for two ICMP requests\n";
	      cout << "is roughly 3 seconds. Setting dinterval to this will result in more than one request at once." << endl;
	      cout << "Exiting..." << endl;
	      exit(EXIT_FAILURE);
	    }


	  i++;
	}

      else if (strcmp (argv[i], "-config") == 0)
	{
			
	  if (argv[i + 1]){
	    confFile = "";
	    confFile += argv[i + 1];
	    confFile = fixPath(confFile, apa, savepwd);
	  }
	  else
	    wrongArg(argv[i]);
	  if (fileCheck (confFile) == "EX")
	    {
	      /* We need to reset these values, since they may have been specified earlier when
		 the default configuration file is processed

		 A scenario would be if a config file resides in /usr/local/etc and is parsed when the 
		 program starts, but the user specifies -config another.conf. IF another.conf doesn't 
		 hold a ip6 value, the one from the original config file will be used

	      */
	      ip6=""; fbip6=""; ip=""; fbip="";
	      logFile = dir + "/log";
	      logFile6 = dir + "/log";
	      htmlFile = "";
	      htmlFile6 = "";
	      errors = "";
	      errors6 = "";
	      cmd4up=""; cmd4dn=""; cmd6up=""; cmd6dn="";
	      parseconf (logFile6, htmlFile6, fbip6, ip6, dir, apa, logFile,  totFile,   htmlFile, confFile, ip, fbip, pint,  dinterval, interval,  noDaemon, noSound, cmd4up, cmd4dn, cmd6dn, cmd6up, errors, errors6, iface);
	      if (htmlFile.size() > 0)
		htmlFile = fixPath (htmlFile, apa, savepwd);
	      if (htmlFile6.size() > 0)
		htmlFile6 = fixPath (htmlFile6, apa, savepwd);
	      if (logFile.size() > 0)
		logFile = fixPath(logFile, apa, savepwd);
	      if (logFile6.size() > 0)
		logFile6 = fixPath(logFile6, apa, savepwd);
	    }
	  else
	    {
	      cout << "Could not read " << confFile	<< "\nAborting..." << endl;
	      exit(EXIT_FAILURE);
	    }
	  i++;
	}		/* -config */

      else
	{
	  cout << "Unknown command line argument: " << argv[i] << endl << endl;
	  help ();
	  exit(EXIT_FAILURE);
	}
      i++;
    }			/* Command line argument parsing */



  if (gaveLog)
    {
      if (type == 6)
	dispLog (logFile6, gaveMore);
      else
	dispLog (logFile, gaveMore);
      exit(EXIT_SUCCESS);
    }
  if (gaveQuit)
    {
      if (!killProc (pidFile, silent, 15, statFile) && (silent == 1))
	exit(EXIT_FAILURE);
      else
	exit (EXIT_SUCCESS);
    }
  if (gaveTurn)
    {
      logTurn (logFile);
      exit (EXIT_SUCCESS);
    }
  if (gaveStatus)
    {
      status (pidFile, statFile, type);
      exit(EXIT_SUCCESS);
    }


  /* Ugly stuff here, look away! */
  /* This is to cancel the config file values if command line options were given */
  if(gaveIp && !gaveFbip)
    fbip="";
  if (gaveIp6 && !gaveFbip6)
    fbip6="";
  if (gaveIp && !gaveIp6){
    ip6="";
    fbip6="";
  }
  if (gaveIp6 && !gaveIp){
    ip="";
    fbip="";
  }

  return EXIT_SUCCESS;

}

/*
  This function is called whenever a user has given a switch
  which requires an argument, but leaves out the argument

*/
  
void wrongArg (std::string arg)
{
  cout << "Option " << arg << " requires an argument\n" << endl;
  help();
  exit(EXIT_FAILURE);
}
