/* 
  This function is only executed if ~/.icpld isn't found
  upon which it creates all necessary files.
*/

#include "cuc.h"

#include <sys/stat.h>
#include <sys/utsname.h>
void
firstRun (std::string dir, std::string logFile, std::string totFile,
	  std::string statFile, std::string htmlFile, std::string htmlFile6, std::string totFile6)
{
  struct utsname u;
  uname(&u); 

#ifdef HAVE_IPV6

  if (chdir (dir.c_str()) != -1){ // if ~/.icpld exist, check for the file 
                                  // tot6 file and create it if it's not found
    ifstream tot6 (totFile6.c_str(), ios::in);
    if (!tot6){
    tot6.close();
      ofstream toTot6 (totFile6.c_str(), ios::out);
      if (!toTot6){
	cout << "Could not open " << totFile6 << " for writing: " << strerror(errno) << endl;
	exit(EXIT_FAILURE);
      }
      toTot6 << '0' << endl <<  '0' << endl;
      toTot6.close();
    }
    else
      tot6.close();
  }
#endif


  /* Dir should in this case be ~/.icpld */
  if (chdir (dir.c_str ()) == -1)
    {
      mkdir (dir.c_str (), 0700);
      std::ofstream createLog (logFile.c_str (), std::ios::app);
      if(!createLog){
	cout << "Could not open " << logFile << " for writing. Aborting.." << endl;
	exit(EXIT_FAILURE);
      }
      createLog << endl;
      createLog.close ();

      std::ofstream createTot (totFile.c_str (), std::ios::app);
      if (!createTot){
	cout << "Could not open " << totFile << " for writing. Aborting.." << endl;
	exit(EXIT_FAILURE);
      }
      createTot << '0' << endl << '0' << endl;
      createTot.close ();

      std::ofstream createStat (statFile.c_str (), std::ios::app);
      if (!createStat){
	cout << "Could not open " << statFile << " for writing. Aborting.." << endl;
	exit(EXIT_FAILURE);
      }
      createStat << "UP" << endl << "JUSTSTARTED" << endl;
      createStat.close ();
		
      if(htmlFile.size() != 0){
	std::ofstream createHTML (htmlFile.c_str(), std::ios::app);
	if (!createHTML){
	  cout << "Could not open " << htmlFile << " for writing. Aborting.." << endl;
	  exit(EXIT_FAILURE);
	}
	createHTML << "<title>Connection performance for " << u.nodename << "</title>" << endl;
	createHTML << "<html>" << endl << "<body>" << endl << "<pre>" << endl;
	createHTML << "Log generated by <a href=\"http://icpld.northernmost.org\">" << PACKAGE << "</a>" << endl;
	createHTML.close();
      }
#ifdef HAVE_IPV6
      if(htmlFile6.size() != 0){
	std::ofstream createHTML6 (htmlFile.c_str(), std::ios::app);
	if (!createHTML6){
	  cout << "Could not open " << htmlFile6 << " for writing. Aborting.." << endl;
	  exit(EXIT_FAILURE);
	}
	createHTML6 << "<title>Connection performance for " << u.nodename << "</title>" << endl;
	createHTML6 << "<html>" << endl << "<body>" << endl << "<pre>" << endl;
	createHTML6 << "Log generated by <a href=\"http://icpld.northernmost.org\">" << PACKAGE << "</a>" << endl;
	createHTML6.close();

      }
      std::ofstream createTot6 (totFile6.c_str(), ios::trunc);
      if (!createTot6){
	cout << "Could not open " << totFile6 << " for writing: " << strerror(errno) << endl;
	exit(EXIT_FAILURE);
      }
      createTot6 << '0' << endl << '0' << endl;
   
      
#endif
    }
}
