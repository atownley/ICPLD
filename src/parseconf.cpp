/*
   This function parses a configure file
   for values, and sets variables accordingly
 */

#include "cuc.h"
#include <sys/utsname.h>

void parseconf(std::string &logFile6, std::string &htmlFile6, std::string &fbip6, std::string &ip6, std::string &dir, std::string apa,std::string &logFile, std::string &totFile,  std::string &htmlFile, std::string confFile, std::string &ip, std::string &fbip, std::string &pint,  int &dinterval, int &interval, bool &noDaemon, bool &noSound, std::string &cmd4up, std::string &cmd4dn, std::string &cmd6dn,std::string cmd6up, std::string &errors, std::string &errors6, std::string &iface )
{  
		struct utsname u;
		uname (&u);
		string confVar, tempInterval, tempdInterval, tempV6;
		int confLine = 0;
		ifstream readConf (confFile.c_str (), ios::binary);
		while (getline (readConf, confVar))
		{
			confLine++;

			if (strncmp (confVar.c_str (), "ip=", strlen ("ip="))   == 0)
			  {
			  for (unsigned int i = 3; i < confVar.size ();  i++)
					ip += confVar[i]; 
			}
			else if (strncmp (confVar.c_str(), "iface=", strlen ("iface=")) == 0)
			  {
			    for (unsigned int i = strlen("iface="); i < confVar.size(); i++)
			      iface +=confVar[i];
			  }
			
			else if (strncmp (confVar.c_str(), "ip6=", strlen ("ip6=")) == 0)
			  {
			    for (unsigned int i = strlen("ip6="); i < confVar.size(); i++)
			      ip6 += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "fbip6=", strlen ("fbip6=")) == 0)
			  {
			    for (unsigned int i = strlen("fbip6="); i < confVar.size(); i++)
			      fbip6 += confVar[i];
			  }
			    
       			else if (strncmp  (confVar.c_str (), "fbip=", strlen ("fbip=")) == 0)
			  {
				for (unsigned int i = strlen ("fbip="); i < confVar.size (); i++)
					fbip += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "cmd4dn=", strlen("cmd4dn")) == 0)
			  {
			    for (unsigned int i = strlen("cmd4dn="); i < confVar.size(); i++)
			      cmd4dn += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "cmd4up=", strlen("cmd4up")) == 0)
			  {
			    for (unsigned int i = strlen("cmd4up="); i < confVar.size(); i++)
			      cmd4up += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "cmd6dn=", strlen("cmd6dn")) == 0)
			  {
			    for (unsigned int i = strlen("cmd6dn="); i < confVar.size(); i++)
			      cmd6dn += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "cmd6up=", strlen("cmd6up")) == 0)
			  {
			    for (unsigned int i = strlen("cmd6up="); i < confVar.size(); i++)
			      cmd6up += confVar[i];
			  }

			else if (strncmp (confVar.c_str(), "errfile6=", strlen("errfile6")) == 0)
			  {
			    errors6="";
			    for (unsigned int i = strlen("errfile6="); i < confVar.size(); i++)
			      errors6 += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "errfile=", strlen("errfile")) == 0)
			  {
			    errors = "";
			    for (unsigned int i = strlen("errfile="); i < confVar.size(); i++)
			      errors += confVar[i];
			  }
			else if (strncmp (confVar.c_str (), "pint=",  strlen ("pint=")) == 0)
			{
					pint = "";
				for (unsigned int i = strlen ("pint=");    i < confVar.size (); i++) {
					if(( isalpha(confVar[i]) != 0) && confVar[i]  != '.' ){
						cout << "The value of pint must not contain anything but decimal numbers\n";
						cout << "Please edit " << confFile << " at line " << confLine << "." << endl;
						cout << "Aborting..." << endl;
						exit( EXIT_FAILURE);
					}			
					pint += confVar[i];			
				}
				  
			}


			else if (strncmp  (confVar.c_str (), "dinterval=",  strlen("dinterval=")) == 0)
			{
				for (unsigned int i = 10; i < confVar.size ();   i++)
					tempdInterval += confVar[i];
				for (unsigned int j = 0;   j < tempdInterval.size (); j++)
				{
					if (isalpha (tempdInterval[j]) != 0)
					{
						cout << "The value of -dinterval must be of type integer" << endl;
						cout << "Edit " << confFile << " at line " << confLine  << "." << endl;
						exit (EXIT_FAILURE);
					}
				}
				dinterval = atoi (tempdInterval.c_str ());
			}

			else if (strncmp	 (confVar.c_str (), "interval=",  strlen ("interval=")) == 0)
			{
				for (unsigned int i = 9; i < confVar.size ();     i++)
					tempInterval += confVar[i];
				for (unsigned int ch = 0;   ch < tempInterval.size (); ch++)
				{
					if (isalpha (tempInterval[ch]) != 0)
					{
						cout << "The value of 'interval' must be of type integer" << endl;
						cout <<  "Edit " << confFile << " at line " << confLine << "." <<  endl;
						exit (EXIT_FAILURE);
					}
				}
				interval = atoi (tempInterval.c_str ());
			}


			else if (strncmp	 (confVar.c_str (), "daemonize=",  strlen ("daemonize=")) == 0)
			{
				if (strstr (confVar.c_str (), "false") != 0)
					noDaemon = true;
				else if (strstr (confVar.c_str (), "true") != 0)
					noDaemon = false;
				else
				{
					cout << "Config misspelled at line "		<< confLine << endl;
					cout << "daemonize must be either true or false" << endl;
					exit (EXIT_FAILURE);
				}
			}

			else if (strncmp	 (confVar.c_str (), "nobeep=",  strlen ("nobeep=")) == 0)
			{
				if (strstr (confVar.c_str (), "false") != 0)
					noSound = false;
				else if (strstr (confVar.c_str (), "true") != 0)
					noSound = true;
				else
				{
					cout << "Config misspelled at line "	<< confLine << endl;
					cout << "nobeep must be either true or false" << endl;
					exit (EXIT_FAILURE);
				}
			}
			else if (strncmp	 (confVar.c_str (), "htmlFile=",  strlen ("htmlFile=")) == 0)
			{
				htmlFile = "";
				for (unsigned int i = 9; i < confVar.size ();    i++)
					htmlFile += confVar[i];
			}
			else if (strncmp	 (confVar.c_str (), "logFile=",	  strlen ("logFile=")) == 0)
			{
				logFile = "";
				for (unsigned int i = 8; i < confVar.size ();     i++)
					logFile += confVar[i];
			}

			else if (strncmp (confVar.c_str(), "htmlFile6=", strlen ("htmlFile6="))==0)
			  {
			    htmlFile6="";
			    for (unsigned int i = strlen("htmlFile6="); i < confVar.size(); i++)
			      htmlFile6 += confVar[i];
			  }
			else if (strncmp (confVar.c_str(), "logFile6=", strlen("logFile6"))==0)
			  {
			    logFile6="";
			    for (unsigned int i = strlen("logFile6="); i < confVar.size(); i++)
			      logFile6 += confVar[i];
			  }
			/* Lines starting with # or spaces are to be considered as comments */
			/* Empty lines are of course also accepted */
			else if (confVar[0] != '#' && confVar[0] != ' '	 && confVar.size () != 0)
			{
			  cout << "Unknown argument: " << confVar << endl << " in config file at line " << confLine << endl;
				exit (EXIT_FAILURE);
			}

		}	

	      
}
