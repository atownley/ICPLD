/*
   This function is called after everything else
   and merely displays what values has been given
   and will be used.
*/

#include "cuc.h"
void info(int type, string ip, string fbip, string ip6, string fbip6, string pint, double interval, double dinterval, bool noDaemon, bool noSound, string logFile, string logFile6, string htmlFile, string htmlFile6)
{


  cout << "Using values:" << endl << endl;
  if (type != 6)
    {
      cout << "ip=" << ip << endl;
      cout << "fbip=" << fbip << endl;
    }
  else
    cout << "ip=" << endl << "fbip=" << endl;
#ifdef HAVE_IPV6
  cout << "ip6=" << ip6 << endl;
  cout << "fbip6=" << fbip6 << endl;
#endif
  cout << "pint=" << pint << endl;
  cout << "interval=" << interval << endl;
  cout << "dinterval=" << dinterval << endl;
  if (noDaemon)
    cout << "daemonize=false" << endl;
  else
    cout << "daemonize=true" << endl;
  if (noSound)
    cout << "nobeep=true" << endl;
  else
    cout << "nobeep=false" << endl;
  cout << "logFile=" << logFile << endl;
  cout << "htmlFile=" << htmlFile << endl;
#ifdef HAVE_IPV6
  if (type != 4)
    {
      cout << "logFile6=" << logFile6 << endl;
      cout << "htmlFile6=" << htmlFile6 << endl;
    }
#endif
}
