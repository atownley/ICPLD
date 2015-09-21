/* Should be self explanatory - simply dumps the error file to
   stdout
*/
#include "cuc.h"

/* mode indicates whether it's the IPv4 or IPv6 log we're dumping */
void dispErrs(std::string errFile, int mode) {
  string buf;
  int cnt = 0;

  if(mode == 4){
    ifstream readErr(errFile.c_str(), ios::in);
    while (getline(readErr, buf)){
      cout << buf << endl;
      cnt++;
    }
  }

  else if(mode == 6){
    ifstream readErr6(errFile.c_str(), ios::in);
    while (getline(readErr6,buf)){
      cout << buf << endl;
      cnt++;	
    }
  }
  if (cnt == 0)
    cout << "No errors to display. Are you sure your connection has been unavailable?" << endl;

}
