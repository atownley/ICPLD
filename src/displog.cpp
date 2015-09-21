/*
  This function prints the log to STDOUT
  This is where we go when we catch -log

  Thanks to Pete Wilson for the "get char by char"-code 
  See copyright notice in the file footer for further notes

  OBS! I have heavily modified the implementation of his 
  functions, and all possible bugs is on my responsibility 
  as Pete's code is excellent.

*/
#include "cuc.h"
#include <sys/ioctl.h>

#ifdef SOLARIS
#include <stropts.h>
#endif

#include <iomanip>
#include <termios.h>
#include <csignal>

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

extern "C" {
static struct termios termattr, save_termattr;
static int ttysavefd = -1;

static enum 
{ 
  RESET, RAW, CBREAK 
} ttystate = RESET;

int
set_tty_raw(void) 
{
  int i;

  i = tcgetattr (STDIN_FILENO, &termattr);
  if (i < 0) 
  {
    cout << "\ntcgetattr() returned " << i << " for fildes=" << STDIN_FILENO << endl;
    perror ("");
    return -1;
  }
  save_termattr = termattr;

  termattr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  termattr.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  termattr.c_cflag &= ~(CSIZE | PARENB);
  termattr.c_cflag |= CS8;
  termattr.c_oflag &= ~(OPOST);
   
  termattr.c_cc[VMIN] = 1;  /* or 0 for some Unices?? */
  termattr.c_cc[VTIME] = 0;

  i = tcsetattr (STDIN_FILENO, TCSANOW, &termattr);

  if (i < 0) 
  {
    cout << "\ntcsetattr() returned " << i << " for fildes=" << STDIN_FILENO << endl;
    perror("");
    return -1;
  }
   
  ttystate = RAW;
  ttysavefd = STDIN_FILENO;

  return 0;
}

int
set_tty_cooked() 
{
  int i;
  if (ttystate != CBREAK && ttystate != RAW) 
  {
    return 0;
  }
  i = tcsetattr (STDIN_FILENO, TCSAFLUSH, &save_termattr);
  if (i < 0) 
  {
    return -1;
  }
  ttystate = RESET;
  return 0;
}

unsigned char kb_getc(void) 
{
  unsigned char ch;
  ssize_t size;

  size = read (STDIN_FILENO, &ch, 1);
  if (size == 0)
  {
    return 0;
  }
  else
  {
    return ch;
  }
}

} // extern "C" {
void dispLog (std::string logFile, bool gaveMore)
{
  string tmp, nonSense;
  /* Wheter we should show one line, or one screen
     at a time. Return key outputs one screen ahead, any
     other key outputs the next line
  */
  bool goOne = true;
  int lines = 0, loops = 0;
  /* loops2 is never resetted and is used together with
     totLines to calculate how many percent of the file 
     that's been sent to output 
  */
  double loops2 = 0, totLines = 0;
  unsigned char ch;
  struct winsize terminal;
  ioctl(0, TIOCGWINSZ, &terminal);
  lines = terminal.ws_row;

  if (gaveMore && (lines == 0)){
    cout << "Something went wrong when obtaining your terminal size. Ignoring -m..."<< endl;
    gaveMore = false;

  }
  /* If we're piped to let's say 'more', we don't want this behaviour */
  if (isatty(STDOUT_FILENO) == 0)
    gaveMore=false;
  if (fileCheck (logFile) == "NE" || fileCheck(logFile) == "ENP")
    {
      cout << "There doesn't seem to be a logfile present. Make sure you have\n";
      cout << "the proper permissions, as well as that it's you that's the owner of the\n";
      cout << "ICPLD process, before trying again." << endl;
      exit (EXIT_FAILURE);
    }
  ifstream readLog (logFile.c_str (), ios::in);
  /* Get total amount of lines in the file in order to
     display how many percent of the file that's been
     displayed 
  */
  while (!readLog.eof()){
    getline(readLog, tmp);
    totLines++;
  }
  /* Reset ifstream so we can start outputting the
     log from the beginning of the file 
   */
  readLog.clear();
  readLog.seekg(0);

  while (getline (readLog, tmp)){
    cout << tmp << endl;
    if (gaveMore){
      loops++;
      loops2++;
      if (loops == lines-3){
        cout << setprecision(3) << (double)((loops2/totLines)*100) << "%" << flush; 
	if (set_tty_raw() == -1){ 
	  cout << "Please contact erik@ibiblio.org if you are reading this. I would be\n";
	  cout << "most interested in knowing your setup." << endl;
	  raise(SIGINT);
	}
	  
	ch = kb_getc();
	/* Let's remove the percent count */
	cout << "\r\r\r\r\r       "<< flush;
	cout << "\r\r\r\r\r" << flush;
	/* If space is pressed, go one screen full ahead */
	if (32 == (int)ch)
	  goOne=true;
	else
	  goOne=false;
	if (0x03 == ch)      /* might be control-C */
	  {
	    set_tty_cooked();  /* control-C, restore normal TTY mode */
	    raise(SIGINT);     /* and kill ourselves.. bye cruel system :-( */    
	  }

	set_tty_cooked();
	if (goOne)
	  loops = 0;
        else
	  loops--;
	goOne=true;
      }
    }
  }
  /* Just for cosmetic purposes. 100% won't be displayed if the whole log fits in one screen */
  if (gaveMore && lines < loops2)
    cout << "100%" << endl;
  readLog.close ();
  
}

/* ***************************************************************************
 *
 *          Copyright 1992-2004 by Pete Wilson All Rights Reserved
 *           50 Staples Street : Lowell Massachusetts 01851 : USA
 *        http://www.pwilson.net/   pete@pwilson.net   +1 978-454-4547
 *
 * This item is free software: you can redistribute it and/or modify it as 
 * long as you preserve this copyright notice. Pete Wilson prepared this item 
 * hoping it might be useful, but it has NO WARRANTY WHATEVER, not even any 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *
 *************************************************************************** */


