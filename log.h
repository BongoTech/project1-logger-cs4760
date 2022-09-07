/**
 *  @file log.h
 *  @author Cory Mckiel
 *  @version 1.0
 *  @date September 7, 2022 Created
 *  @date September 7, 2022 Last modified
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

#include <time.h>

/**
 *  The log will be composed of individual messages. Define a 
 *  data type message_t to represent each message.
 */
typedef struct message_struct {
  time_t  time;     ///<  Time stamp of message creation.
  char    type;     ///<  Message type (I/W/E/F) explained below.
  char    *string   ///<  Message content.
} message_t;

/**
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







