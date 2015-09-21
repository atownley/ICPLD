/* 
   This function is only called if the user
   enters an invalid options, no 'ip' 
   or 'ip6' is set or --help alternatively -h is given.

   Displays icpld's options
*/

#include "cuc.h"
void
help ()
{
#ifdef HAVE_IPV6
	cout << "usage:  icpld [OPTIONS] [-ip <ip>] [-ip6 <ipv6>]" << endl;
#endif
#ifndef HAVE_IPV6
	cout << "usage: icpld [OPTIONS] [-ip <ip>]" << endl;
#endif
	cout << " Options are as follows:" << endl;
	cout << " -h, --help\tWhat you are reading now." << endl;
	cout << " -fbip\t\tFallback ip. ICPLD verifies a lost connection towards this ip" << endl;
#ifdef HAVE_IPV6 
	cout << " -fbip6\t\tSame as -fbip but for the IPv6 checking" << endl;
	cout << " -6\t\tOnly check IPv6 connection (override configuration file)" << endl;
	cout << " -4\t\tOnly check IPv4 connection (override configuration file)" << endl;
#endif
	cout << " -nd\t\tPrevent ICPLD from daemonizing" << endl;
	cout << " -d\t\tForce ICPLD to daemonize" << endl;
	cout << " -detach\tDetaches an interactive (-nd) icpld process" << endl;
	cout << " -s\t\tSilent, produces no output what so ever" << endl;
	cout << " -iface\t\tTells icpld which device or source address to use" << endl;
	cout << " -status\tShows the current state of ICPLD" << endl;
	cout << " -log\t\tDisplays the ICPLD log " << endl;
	cout << " -m\t\tThis option together with -log cancels the 'more' behaviour" << endl;
	cout << " -turn\t\tTurns the log file over. Old one is saved as log.old" << endl;
	cout << " -config\tLocation of config file (default /usr/local/etc/icpld.conf)" << endl;
	cout << " -reset\t\tResets ICPLD (kills a running ICPLD process)" <<	endl;
	cout << " -quit\t\tTerminates a running ICPLD process" << endl;
	cout << " -logfile\tSets to which file ICPLD should log (default ~/.icpld/log)" << endl;
	cout << " -err\t\tDumps the results for the failing pings for diagnostic purposes" << endl;
#ifdef HAVE_IPV6
	cout << " -err6\t\tSame as above but for the IPv6 error log (Same as that\n\t\tof -err by default)" << endl;
#endif
	cout << " -errfile\tSets to which file ICPLD should log the output of failed\n";
        cout << " \t\tpings (default ~/.icpld/errors)" << endl;
#ifdef HAVE_IPV6
	cout << " -errfile6\tSame as above but for the IPv6 connection" << endl;
	cout << " -logfile6\tSpecifies which file ICPLD should log IPv6 connections to" << endl;
	cout << " -htmlfile6\tSame as below, but for the IPv6 log" << endl;
#endif
	cout << " -htmlfile\tDecides which file to duplicate the log in HTML format to" << endl;
	cout << " -pint\t\tSets the ICMP packet sending frequence (default 1 per sec)" << endl;
	cout << " -interval\tInterval for availability checking (Unit: seconds)" << endl;
	cout << " -dinterval\tInterval for availability checking when connection is down" << endl;
	cout << " -nobeep\tNo beep on available connection" << endl;
	cout << " -v, --version\tDisplay version info" << endl;
	cout << endl << " See manpage for further information." << endl;
}
