/**
 *  @section INFO
 *
 *  @file log.h
 *  @author Cory Mckiel
 *  @version 1.0
 *  @date September 7, 2022 Created
 *  @date September 8, 2022 Last modified
 *
 *  @section LICENSE 
 *
 *  This program currently has no licensing. Do with it what
 *  you will.
 *
 *  @section DESCRIPTION
 *
 *  log.h is a header file for a logging library. The purpose
 *  of this library is to provide logging functionality to 
 *  future projects in the course CS4760 - Intro to Operating 
 *  Systems. This file will include function definitions for
 *  managing the log. The functions should be able to add entries
 *  to the log, return the log to the calling program, save the log 
 *  to disk, and clear the log of all messages.
 */

#ifndef LOG_H
#define LOG_H

#include <time.h>

/**
 *  The log will be composed of individual messages. Define a 
 *  data type message_t to represent each message.
 *
 *  Each message has a type. 
 *  Message type can take on one of four values:
 *
 *  INFO  (I): An informational message.
 *  WARN  (W): A warning message.
 *  ERROR (E): An error message.
 *  FATAL (F): A message to note a fatal error occured.
 *             This library will then save the message to
 *             the log, save the log to a file 'savelog' and
 *             the calling program will be terminated.
 */
typedef struct message_struct {
  time_t  time;     ///<  Time stamp of message creation.
  char    type;     ///<  Message type (I/W/E/F).
  char    *string;  ///<  Message content.
} message_t;


/**
 *  addmsg creates a message_t and appends it to the log.
 *
 *  @param[in] type One of the following: (I/W/E/F).
 *  @param[in] msg String containing the message.
 *  @return int 0 if successful, -1 otherwise.
 */
int addmsg(const char type, const char *msg);

/**
 *  clearlog releases all the storage that has been allocated
 *  for the logged messages and empties the list of logged 
 *  messages.
 *  NOTE: The log is not saved first. If clearlog is called
 *  before the log has been saved to disk, the log will
 *  be lost.
 */
void clearlog();

/**
 *  getlog allocates enough space for a string containing 
 *  the entire log, copies the log into this string, and
 *  returns a pointer to this string.
 *
 *  IMPORTANT: The calling program is responsible for
 *  freeing the memory that getlog allocates.
 *
 *  @return char* Ptr to log string if successful, NULL otherwise.
 */
char * getlog();

/**
 *  savelog saves the logged messages to a disk file. The default
 *  file will be messages.log.
 *
 *  @param[in] filename A pointer to the file where the log will be saved.
 *  @return int 0 if successful, -1 otherwise.
 */
int savelog(char *filename);

#endif
