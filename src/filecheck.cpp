/*  
    This code determines the permissions of files
    and is called from various places in the program.
    It only checks for reading permissions
*/

#include "cuc.h"
#include <sys/stat.h>

string
fileCheck (std::string fileName)
{
	
  struct stat s;
  if(stat(fileName.c_str(), &s) == -1)
    return "NE";
			
	
  if (s.st_uid != getuid() && fileName != "/usr/local/etc/icpld.conf"){ 
    cout << "WARNING: You are not the owner of " << fileName << endl;
    cout << "Attempting to proceed..." << endl;
  }
	
		
  ifstream check (fileName.c_str (), ios::binary);
	
  /* 
     ENP = Exists No Permissions
     NE = Non Existant
     EX = Existant
  */
	
  if(!check) {	
    cout << "Could not open " << fileName << " for reading.." << strerror(errno) << endl;
    check.close();
    return "ENP";
  }
	
  else
    {
      check.close ();
      return "EX";
    }
}
