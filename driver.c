/**
 *  @section INFO
 *
 *  @file driver.c
 *  @author Cory Mckiel
 *  @version 1.0
 *  @date September 12, 2022 Created
 *  @date September 13, 2022 Last modified
 *
 *  @section LICENSE 
 *
 *  This program currently has no licensing. Do with it what
 *  you will.
 *
 *  @section DESCRIPTION
 *  
 *  driver.c is a program meant to prompt the user for log messages
 *  and save them using the loglib library.
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "log.h"

const int MX_FILENAME_LEN = 50;
const int MX_WAITTIME_DIGITS = 20;

/** Helper functions. See definitions for more details. */
void help_msg(char *);
int valid_number(char[MX_WAITTIME_DIGITS]);

int main(int argc, char *argv[]) {

  /** Begin extracting arguments from argv. */
  int seconds = 0;
  int time_option = 0;
  char filename[MX_FILENAME_LEN];
  char tempstr[MX_WAITTIME_DIGITS];   ///< Temporarily holds time string for error checking.

  int option;
  while ((option = getopt(argc, argv, "ht:")) != -1) {
    switch (option) {
      case 'h':
        help_msg(argv[0]);
        return 0;
      case 't':
        strncpy(tempstr, optarg, MX_WAITTIME_DIGITS-1);
        time_option = 1;
        break;
      case '?':
        help_msg(argv[0]);
        return -1;
    }
  }

  /** If true, then the filename was specified. */
  if (optind < argc) { 
    strncpy(filename, argv[optind], MX_FILENAME_LEN-1);
  } 
  else {
    /** Use the default filename */
    strcpy(filename, "messages.log");
  }

  /** Assign number of seconds. */
  if (time_option) {
    if (valid_number(tempstr)) {
      seconds = atoi(tempstr);
    }
    else {
      fprintf(stderr, "Error: in main(): -t must be a positive integer.\n");
      return -1;
    }
  }
 
  /** Print all options for testing */
  printf("Seconds: %d\n", seconds);
  printf("Filename: %s\n", filename);
 
  return 0;
}

/**
 *  help_msg prints usage info to the console. Takes
 *  in the name of the executable which was invoked to 
 *  run this program.
 */
void help_msg(char *exname) {

  printf("Help: %s prompts users for messages and saves them to a log.\n",
    exname);
  printf("The log may then be saved to a file name.\n\n");
  printf("Usage: %s [-h] [-t sec] [logfile]\n", exname);
  printf("All parameters are optional.\n");
  printf("-h displays this help message.\n");
  printf("-t sec adds a random time to sleep between prompts.\n");
  printf("\tThe average sleep time is sec where sec is an integer.\n");
  printf("logfile is the filename you will save the log to.\n");
  printf("\tThe default filename is messages.log.\n");
  printf("\tNOTE: Providing an existing filename will erase the file.\n");
}

/**
 *  valid_number takes in a string and returns 1 if it is comprised
 *  of valid digits 0-9 and 0 otherwise.
 */
int valid_number(char numstr[MX_WAITTIME_DIGITS]) {
  
  int i = 0;
  char digit = numstr[i];

  while (i < MX_WAITTIME_DIGITS && digit != '\0') {
    if ( (int)digit > 57 || (int)digit < 48 ) {
      return 0;
    }
    i += 1;
    digit = numstr[i];
  }

  /** 
   *  If the function makes it to this point all characters in
   *  the string were numbers.
   */
  return 1;
}


