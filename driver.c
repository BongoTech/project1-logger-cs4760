/**
 *  @section INFO
 *
 *  @file driver.c
 *  @author Cory Mckiel
 *  @version 1.0
 *  @date September 12, 2022 Created
 *  @date September 14, 2022 Last modified
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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "log.h"

const int MX_FILENAME_LEN = 50;
const int MX_WAITTIME_DIGITS = 20;
const int MX_MSG_LEN = 200;

extern int errno;

/** Helper functions. See definitions for more details. */
int new_message(char *);
void display_main_menu();
void help_msg(char *);
int valid_number(char[MX_WAITTIME_DIGITS]);


int main(int argc, char *argv[]) {

  /** Begin extracting arguments from argv. */
  int seconds = 0;
  int time_option = 0;                ///< flag: 1 if -t is used, 0 otherwise.
  char filename[MX_FILENAME_LEN];
  char tempstr[MX_WAITTIME_DIGITS];   ///< Store time until check for errors.

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
 
  /** Begin prompting user for messages and logging them. */
  printf("Welcome to this logging program.\n");

  char menu_choice;
  char buff[3];

  while (1) {
    display_main_menu();
    
    if (fgets(buff, sizeof(buff), stdin) == NULL) {
      fprintf(stderr, "%s: Error: invalid input: %s\n", 
        argv[0], strerror(errno));
      return -1;
    }

    if (buff[0] == 'q') {
      break;
    }

    if (buff[0] == 'a' || buff[0] == 's' || buff[0] == 'p' || buff[0] == 'c') {
      menu_choice = buff[0];
    }
    else {
      fprintf(stderr, "Error: invalid menu choice.\n");
      continue;
    }

    char *log;
    char type;  ///< Used to check for fatal messages.

    switch (menu_choice) {
      case 'a':
        if (new_message(&type) == -1) {
          fprintf(stderr, "%s: Error: Could not add new message.\n", argv[0]);
          return -1;
        }
        printf("Message saved!\n");
        break;
      case 's':
        if (savelog(filename) == -1) {
          fprintf(stderr, "%s: Error: Could not save log.\n", argv[0]);
          return -1;
        }
        printf("Log saved!\n");
        break;
      case 'p':
        if ((log = getlog()) == NULL) {
          fprintf(stderr, "%s: Error: Could not print log.\n", argv[0]);
          return -1;
        }
        printf("%s", log);
        free(log);
        break;
      case 'c':
        clearlog();
        printf("Log cleared!\n");
        break;
    }

    /** Check for a fatal message. */
    if (type == 'F') {
      fprintf(stderr, "Fatal message! Saving log and exiting program...\n");
      if (savelog(filename) == -1) {
        fprintf(stderr, "%s: Error: Could not save log.\n", argv[0]);
      }
      break;
    }
  }

  clearlog(); 
  printf("Goodbye!\n");   
    
  return 0;
}

/**
 *  new_message prompts user for message and adds the message to log.
 *  returns -1 on failure, 0 on success.
 */
int new_message(char *type) {

    char message[MX_MSG_LEN];
    
    printf("Enter your message type (I/W/E/F):\n");
    
    if (fgets(message, sizeof(message), stdin) == NULL) {
      fprintf(stderr, "Error: invalid input: %s\n", strerror(errno));
      return -1;
    }

    if (message[0] == 'I' || message[0] == 'W' || message[0] == 'E' ||
      message[0] == 'F') {
      *type = message[0];
    }
    else {
      fprintf(stderr, "Error: invalid type: %s\n", strerror(errno));
      return -1;
    }
    
    printf("Enter your message:\n");
   
    if (fgets(message, sizeof(message), stdin) == NULL) {
      fprintf(stderr, "Error: invalid input: %s\n", strerror(errno));
      return -1;
    }

    message[MX_MSG_LEN-1] = '\0'; ///< Make sure the msg is null terminated.

    if (addmsg(*type, message) == -1) {
      fprintf(stderr, "Error: in new_message(): Unable to add message.\n");
      return -1;
    }

    return 0;
}

/**
 *  display_main_menu does exactly what you would expect.
 */
void display_main_menu() {
  
  printf("\nMain Menu:\n");
  printf("Press 'a' to add message.\n");
  printf("Press 's' to save the log.\n");
  printf("Press 'p' to print the log to console.\n");
  printf("Press 'c' to clear the log.\n");
  printf("Press 'q' to quit.\n");
  printf("--> ");
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


