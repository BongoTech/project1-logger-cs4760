/**
 *  @section INFO
 *
 *  @file loglib.c
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
 *  loglib.c implements the functions defined in log.h. The functions 
 *  should be able to add entries to the log, return the log to the 
 *  calling program, save the log to disk, and clear the log of all messages.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "log.h"

/** Helper function declarations. For details, see function definitions. */
static int invalid_msg(const char, const char*);
static int num_of_log_entries();
static char * msg_to_str(message_t);
void remove_newline(char *);

/**
 *  Define log_t. The log is implemented as a queue data structure 
 *  where each node contains a message and a pointer to the next 
 *  node.
 */
typedef struct list_struct {
  message_t message;          ///< The message to be logged, defined in log.h.
  struct list_struct *next;   ///< The pointer to the next log_t node.
} log_t;

static log_t *headptr = NULL;
static log_t *tailptr = NULL;

static const int MX_MSG_LEN = 200;

extern int errno;

/**
 *  addmsg creates a message_t and appends it to the log.
 *
 *  @param[in] type One of the following: (I/W/E/F).
 *  @param[in] msg String containing the message.
 *  @return int 0 if successful, -1 otherwise.
 */
int addmsg(const char type, const char *msg) {
  
  /** 
   *  Check to ensure that message type is either I, W, E, or F and
   *  that msg is not NULL.
   */
  if (invalid_msg(type, msg)) {
    fprintf(stderr, "Error: Invalid Message.\n");
    return -1;
  }

  /** 
   *  Begin creating the new message. msg length arbitrarily capped at
   *  MX_MSG_LEN characters for security.
   */
  message_t new_msg;
  if ((new_msg.string = (char *)malloc(sizeof(char)*MX_MSG_LEN)) == NULL) {
    fprintf(stderr, "Error: malloc could not allocate memory for string.\n");
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return -1;
  }
  strncpy(new_msg.string, msg, MX_MSG_LEN);
  new_msg.type = type;
  time(&new_msg.time);

  /**
   *  Check to see if the log is empty. If so, add the first message by
   *  setting both pointers to it.
   */
  if (headptr == NULL && tailptr == NULL) {
    if ((headptr = tailptr = (log_t *)malloc(sizeof(log_t))) == NULL) {
      fprintf(stderr, "Error: malloc could not allocate memory for log.\n");
      fprintf(stderr, "Error: %s\n", strerror(errno));
      return -1;
    }
    headptr->message = new_msg;
    headptr->next = NULL;
    return 0;
  }

  /** If this part is executing, the log was unempty.
   *  Create a new node and append it to the end of the log.
   */
  log_t *new_entry;
  if ((new_entry = (log_t *)malloc(sizeof(log_t))) == NULL) {
    fprintf(stderr, "Error: malloc could not allocate memory for log.\n");
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return -1;
  }
  new_entry->message = new_msg;
  new_entry->next = NULL;
  tailptr->next = new_entry;
  tailptr = tailptr->next;

  return 0;
}

/**
 *  clearlog releases all the storage that has been allocated
 *  for the logged messages and empties the list of logged 
 *  messages.
 *  NOTE: The log is not saved first. If clearlog is called
 *  before the log has been saved to disk, the log will
 *  be lost.
 */
void clearlog() {
  
  log_t *current_node, *prev_node;  ///< two nodes for traversing the queue.
  current_node = headptr;

  /** traverse the list freeing all the message strings and log nodes. */
  while (current_node != NULL) {
    prev_node = current_node;
    current_node = current_node->next;
    free(prev_node->message.string);
    free(prev_node);
  }
  
  headptr = tailptr = NULL;
}

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
char * getlog() {

  int num_of_messages = num_of_log_entries();
  char *log_string = NULL;
 
  /** If true, then the log was empty. Therefore, return NULL. */
  if (num_of_messages == 0) {
    return NULL;
  }

  /** 
   *  Formula for size of string containing the entire log in bytes.
   *
   *  Each log message has a string of length MX_MSG_LEN, so we need
   *  at least [ sizeof(char)*MX_MSG_LEN*num_of_messages ] bytes.
   *  But each message has a one character type, which in string form
   *  will be represented as "I: ". Therefore, we also need 
   *  [ 3*sizeof(char)*num_of_messages ] more bytes to represent each
   *  type. Finally, time in the format " HH:MM:SS\n" needs
   *  [ 10*sizeof(char)*num_of_messages ] bytes. Add 1 sizeof(char) for
   *  a null terminator. Factor out num_of_messages and sizeof(char)
   *  from each term and simplify.
   */
  int log_size = sizeof(char) * ( num_of_messages * (MX_MSG_LEN + 13) + 1);

  /** Allocate space for the entire log string and check any errors. */ 
  if ((log_string = (char *)malloc(log_size)) == NULL) {
    fprintf(stderr, "Error: getlog(): malloc failed: %s\n", strerror(errno));
    return NULL;
  }

  log_t *current_node = headptr;

  /** Flag to track when we are copying the first message. */
  int first_entry = 1; 

  /** Traverse the list and store all message strings into log_string */
  while (current_node != NULL) {
    char *msg_str = msg_to_str(current_node->message);

    if (first_entry) {
      /** Use strncpy instead of strncat for the first string. */
      strncpy(log_string, msg_str, MX_MSG_LEN+13);
      free(msg_str);
      first_entry = 0;
      current_node = current_node->next;
      continue;
    }

    strncat(log_string, msg_str, MX_MSG_LEN+13);
    free(msg_str);
    current_node = current_node->next;
  }
  
  return log_string;

}

/**
 *  savelog saves the logged messages to a disk file. The default
 *  file will be messages.log.
 *
 *  @param[in] filename A pointer to the file where the log will be saved.
 *  @return int 0 if successful, -1 otherwise.
 */
int savelog(char *filename) {

  if (headptr == NULL) {
    fprintf(stderr, "Error: in savelog(): Cannot save empty log.\n");
    return -1;
  }

  FILE *file;
  if ((file = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "Error: in savelog(): fopen(): %s\n", strerror(errno));
    return -1;
  }
  
  char *log = getlog();
  fprintf(file, "%s", log);
  free(log);
  fclose(file);

  return 0;
}

/**
 *  invalid_msg is a helper function used by addmsg to check to see if
 *  the message type and contents are valid.
 */
static int invalid_msg(const char type, const char *msg) {
  
  if (msg == NULL) {
    fprintf(stderr, "Error: Message content cannot be empty.\n");
    return 1;
  }

  switch (type) {
    case 'I':
      break;
    case 'W':
      break;
    case 'E':
      break;
    case 'F':
      break;
    default:
      fprintf(stderr, "Error: Invalid message type.\n");
      fprintf(stderr, "Must be a single capital character from I, W, E, F.\n");
      return 1;
  }

  return 0;
}

/**
 *  num_of_log_entries is a helper function that returns the number of
 *  messages currently in the log.
 */
static int num_of_log_entries() {
  
  if (headptr == NULL) {
    return 0;
  }
  
  log_t *current_node = headptr;
  int num_of_messages = 0;

  while (current_node != NULL) {
    num_of_messages += 1;
    current_node = current_node->next;
  }

  return num_of_messages;
}

/**
 *  msg_to_str is a helper function that allocates memory and 
 *  returns a pointer to a string containing the entire contents
 *  of a message_t in string format.
 *
 *  The format is "[type]: [message string] HH:MM:SS\n".
 *
 *  IMPORTANT: The calling program is responsible for freeing the
 *  memory this rountine creates.
 */
static char * msg_to_str(message_t msg) {
  
  /** Create space for string and check for errors */
  char *msg_str;
  if ((msg_str = (char *)malloc(sizeof(char)*(MX_MSG_LEN+13))) == NULL) {
    fprintf(stderr, "Error: in msg_to_str(): malloc(): %s\n", strerror(errno));
    return NULL;
  }
  
  /** Convert time_t into a string with format HH:MM:SS */
  struct tm *time_info;
  char time_str[9];
  time_info = localtime(&msg.time);
  strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);

  /** Build the string to return. */
  remove_newline(msg.string); ///< For formatting.
  sprintf(msg_str, "%c: %s %s\n", msg.type, msg.string, time_str);

  return msg_str;
}

/**
 *  remove_newline is a helper function that removes the newline
 *  charater at the end of a message string.
 */
void remove_newline(char *str) {

  int i = 0;
  while (str[i] != '\0') {
    if (str[i] == '\n' ) {
      str[i] = '\0';
      break;
    }
    i += 1;
    if (i >= MX_MSG_LEN) {
      break;
    }
  }
}
