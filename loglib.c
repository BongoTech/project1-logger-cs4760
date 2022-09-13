/**
 *  @section INFO
 *
 *  @file loglib.c
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
int invalid_msg(const char, const char*);

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
    printf("%s\n", current_node->message.string); //TODO: Remove line after testing.
    prev_node = current_node;
    current_node = current_node->next;
    free(prev_node->message.string);
    free(prev_node);
  }
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
  return NULL;
}

/**
 *  savelog saves the logged messages to a disk file. The default
 *  file will be messages.log.
 *
 *  @param[in] filename A pointer to the file where the log will be saved.
 *  @return int 0 if successful, -1 otherwise.
 */
int savelog(char *filename) {
  return 0;
}

/**
 *  invalid_msg is a helper function used by addmsg to check to see if
 *  the message type and contents are valid.
 */
int invalid_msg(const char type, const char *msg) {
  
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














