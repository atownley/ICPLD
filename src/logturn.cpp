/* 
   This function is called when -turn is given.
   It copies ~/.icpld/log to ~/.icpld/log.old.n, and 
   creates a new log.

   This code has been C:ified to comply with the poor 
   bastards still running gcc version < 3 (no stringstreams)

*/

#include "cuc.h"
#include <sys/stat.h>

void
logTurn (std::string logFile)
{
  string newLog;
  char tempLog[FILENAME_MAX], conv[17];
  struct stat s;
  for (int i = 0; i < 1001 ; i++){ 
  strncat (tempLog, logFile.c_str(), strlen(logFile.c_str()));
    sprintf(conv, ".%d", i);
    strncat(tempLog, conv, strlen(conv));
    if (stat(tempLog, &s) == -1){
      newLog = tempLog;
      break;
    }
    strcpy(tempLog, "");
    /* Windows-style warning */
    if (i == 1000){
      cout << "You have reached the maximum amount of logs possible. I'm glad you are not my sys-admin." << endl;
      cout << "Investigate your ~/.icpld directory" << endl;
      exit(EXIT_FAILURE);
    }
  }

  if (rename (logFile.c_str(), newLog.c_str()) == -1){
    cout << "Could not rename " << logFile << " to " << newLog << endl;
    perror("rename()");
    exit(EXIT_FAILURE);
  }
  time_t timer;
  struct tm *tblock;
  timer = time (NULL);
  tblock = localtime (&timer);
  remove (logFile.c_str ());
  ofstream createLog (logFile.c_str (), ios::app);
  createLog << endl << "---------------------" << endl;
  createLog << "ICPLD logfile turned at: " << asctime (tblock);
  createLog << "---------------------" << endl << endl;
  createLog.close ();

}
