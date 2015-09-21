/*
   This function reads ~/.icpld/stat
   and displays the current status of 
   icpld and the connection(s)
*/
#include "cuc.h"
void
status (std::string pidFile, std::string statFile, int type)
{
  std::string buf, pidNum;
  int loops = 0;
  if (fileCheck (pidFile) == "EX" && fileCheck (statFile) == "EX")
    {
      ifstream readPid (pidFile.c_str (), ios::binary);
      ifstream readStat (statFile.c_str (), ios::binary);
      while (getline (readPid, buf))
	  pidNum = buf;
 
      cout << "ICPLD is running with PID: " << pidNum << endl;
      buf = "";
      while (getline (readStat, buf))
	{
	  loops++;
	  if (loops == 1)
	    cout << "Connection is currently: " << buf <<
	      endl;
	  else if (loops == 2)
	    cout << "Since: " << buf << endl;
	  else if (loops == 4)
	    cout << "Using ip: " << buf << endl;
	  else if (loops == 5)
	    cout << "Using interval: " << buf << endl;
	  else if (loops == 6)
	    cout << "Using dinterval: " << buf << endl;
	  else if (loops == 7) 
	    cout << "Logging to: " << buf << endl;
	  else if (loops == 8)
	    cout << "HTML duplicating to: " << buf << endl;
#ifdef HAVE_IPV6

	  else  if (loops == 9)
	    cout << "Using IPv6 ip: " << buf << endl;
	  else if (loops == 10)
	    cout << "Logging IPv6 connection to: " << buf << endl;
	  else if (loops == 11)
	    cout << "IPv6 log duplicated to: " << buf << endl;

#endif
	}

    }
  else if (fileCheck (pidFile) == "NE" || fileCheck (pidFile) == "ENP")
    cout << "User '" << getenv("LOGNAME") <<"' does not own a running ICPLD process."  << endl;
}
