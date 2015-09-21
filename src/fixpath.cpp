/*
  This function extracts ~ to $HOME
  and 'filename' to '/full/path/filename'
  
  Needed since daemonize() currently calls chdir()
*/
#include "cuc.h"

/* apa = homedir, fileName = filename that we want to fix, savepwd=current working dir */
string fixPath (std::string fileName, std::string apa, std::string savepwd)
{
  string tempFile ="";


  /* Don't do anything, or we'll end up with stuff like
     htmlFile6 being "     /     " 
  */
  if(fileName.size() == 0)
    return fileName;

  /* No / . or  ~ given, let's assume it's not a path and that 
     the user wishes to use the current directory. */
  if(fileName[0] !=  '/' && fileName[0] != '.' && fileName[0] != '~')
    fileName = savepwd + "/" + fileName;
      

  /* Expand ./ to CWD */

  if (fileName[0] == '.' && fileName[1] == '/'){
    for (unsigned int i = 2; i < fileName.size(); i++)
      tempFile += fileName[i];
    fileName = savepwd + "/" + tempFile;
     
  }

  /* Expand ~ to $HOME */
  else if (fileName[0] == '~'){
    for (unsigned int j = 1; j < fileName.size (); j++)
      tempFile += fileName[j];
    fileName = apa + tempFile;
  }
  else 
    return fileName;
  return fileName;
}
