/* 
   This function writes the headers to
   all the logs used by icpld
*/
#include "cuc.h"
#include <sys/utsname.h>

int wlogs (int type, string logFile, string logFile6, string ip6, bool haveV4, bool haveV6, string htmlFile, string htmlFile6, string ip, string fbip, string fbip6, bool haveHTML, string statFile )


{

  time_t timer;
  struct tm *tblock;
  timer = time (NULL);
  tblock = localtime (&timer);
  struct utsname u;
  uname (&u);
#ifdef HAVE_IPV6
  if (logFile == logFile6 && type == 4){
    std::ofstream toLog (logFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }

  else if (logFile == logFile6 && type == 6){
    std::ofstream toLog (logFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }

  else if (logFile == logFile6 && type == 0){
    std::ofstream toLog (logFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    if (ip6.size() > 0)
      toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }
  if (logFile != logFile6 && ip6.size() > 0){
    std::ofstream toLog (logFile6.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();
  }
  if (logFile != logFile6 && ip.size() > 0){
    std::ofstream toLog (logFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();
  }

#endif

#ifndef HAVE_IPV6
    

  if (ip.size() > 0){
    std::ofstream toLog (logFile.c_str (), std::ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    toLog << "Using IPv4 target(s):  " << ip << " & " << fbip << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close ();
    if (haveHTML){
      std::ofstream toHTML (htmlFile.c_str(), std::ios::app);
      toHTML << endl << "---------------------" << endl;
      toHTML << "ICPLD started at: " << asctime (tblock);
      toHTML << "Running on " << u.nodename << endl;
      toHTML << "Using IPv4 target(s):  " << ip << " & " << fbip << endl;
      toHTML << "---------------------" << endl << endl;
      toHTML.close ();
    }
  }
#endif






#ifdef HAVE_IPV6
  if (htmlFile == htmlFile6 && type == 4){
    std::ofstream toLog (htmlFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }

  else if (htmlFile == htmlFile6 && type == 6){
    std::ofstream toLog (htmlFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }

  else if (htmlFile == htmlFile6 && type == 0){
    std::ofstream toLog (htmlFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    if(ip.size() > 0)
      toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    if (ip6.size() > 0)
      toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();

  }
  if (htmlFile != htmlFile6 && ip6.size() > 0){
    std::ofstream toLog (htmlFile6.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    toLog << "Using IPv6 target(s): " << ip6 << " & " << fbip6 << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();
  }
  if (htmlFile != htmlFile6 && ip.size() > 0){
    std::ofstream toLog (htmlFile.c_str(), ios::app);
    toLog << endl << "---------------------" << endl;
    toLog << "ICPLD started at: " << asctime (tblock);
    toLog << "Running on " << u.nodename << endl;
    toLog << "Using IPv4 target(s): " << ip << " & " << fbip << endl;
    toLog << "---------------------" << endl << endl;
    toLog.close();
  }

#endif


      

  if (remove (statFile.c_str ()) == -1)
    {
      cout << "Could not remove status file" << endl;
      perror("remove()");
    }
  return EXIT_FAILURE;
}
