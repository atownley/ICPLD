/* Main header file, all function definitions etc */
#ifndef _CUC_H_
#define _CUC_H_
#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif
/* This will ease my work when dealing
   with eventual bug reports */
#ifdef HAVE_IPV6
#undef PACKAGE
#define PACKAGE PACKAGE_NAME "-" PACKAGE_VERSION "+ipv6"
#endif

#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdio>
#include <csignal>
#if defined(NETBSD) || defined(OPENBSD)
#include <sys/param.h> /* MAXPATHLEN */
#endif
#ifndef SIGUSR1
#define SIGUSR1 16 /* Mainly for Solaris */
#endif


#ifdef SOLARIS
#include <ctype.h>
#endif

/* Using string function values without a declaration fails on 
   systems where the width of a pointer differs from the width 
   of int, and perhaps in other cases. (From GNU Coding Standards)*/
#if HAVE_STRING_H == 1 
#include <cstring>
#elif HAVE_STRINGS_H == 1
#include <strings.h>
#endif


using namespace std;


void signal_handler (int sig);
void help ();
string fileCheck (std::string filename);
bool killProc (std::string fileName, int silent, int signr, std::string statFile);
void firstRun (std::string dir, std::string logFile, std::string totFile,
	       std::string statFile, std::string htmlFile, std::string htmlFile6, std::string totFile6);
void status (std::string pidFile, std::string statFile, int type);
void dispLog (std::string logFile, bool gaveMore);
void logTurn (std::string logFile);
int daemonize(int nochdir, int noclose, int silent);
void closeallfd(int fd);
int detach();
bool verip(std::string ip, int af);
void dispErrs (std::string errFile, int mode);
void reset(string apa);

void parseconf(std::string &logFile6, std::string &htmlFile6, std::string &fbip6, std::string &ip6, std::string &dir, std::string apa, std::string &logFile, std::string &totFile, std::string &htmlFile, std::string confFile, std::string &ip, std::string &fbip, std::string &pint, int &dinterval, int &interval, bool &noDaemon, bool &noSound, std::string &cmd4up, std::string &cmd4dn, std::string &cmd6dn,std::string cmd6up, std::string &errors, std::string &errors6, string &iface );
string fixPath(std::string fileName, std::string apa, std::string savepwd );

int parseArgs(int argc, char * const argv[], std::string &logFile6, std::string &htmlFile6, std::string &fbip6, std::string &ip6, std::string &dir, std::string apa, std::string &logFile, std::string &totFile, std::string &htmlFile, std::string confFile, std::string &ip, std::string &fbip, std::string &pint, int &dinterval, int &interval, bool &noDaemon, bool &noSound, int &silent, double &checkPint, bool &gaveQuit, std::string &pidFile, std::string &statFile, bool &gaveTurn, int &type, std::string savepwd, bool &gaveLog, std::string totFile6, std::string &cmd4up, std::string &cmd4dn, std::string &cmd6dn,std::string &cmd6up, std::string &errors, std::string &errors6, string &iface );

void wrongArg(std::string arg);

void checkInp(std::string logFile6, std::string htmlFile6, std::string htmlFile, std::string logFile, std::string ip6, std::string ip, bool &haveV6, std::string fbip, std::string fbip6, double &checkPint, bool &haveHTML, std::string pint, int interval, int dinterval, bool &haveV4, int &type, std::string errors, std::string errors6);

int wlogs (int type, string logFile, string logFile6, string ip6, bool haveV4, bool haveV6, string htmlFile, string htmlFile6, string ip, string fbip, string fbip6, bool haveHTML, string statFile );

void info(int type, string ip, string fbip, string ip6, string fbip6, string pint, double interval, double dinterval, bool noDaemon, bool noSound, string logFile, string logFile6, string htmlFile, string htmlFile6);

std::string getCwdir();

#endif

