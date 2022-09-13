/**
 *  @section INFO
 *
 *  @file driver.c
 *  @author Cory Mckiel
 *  @version 1.0
 *  @date September 12, 2022 Created
 *  @date September 12, 2022 Last modified
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

#include <stdio.h>
#include "log.h"

int main(int argc, char *argv[]) {
  printf("(testing) Return value of savelog: %d\n", savelog(NULL));

  addmsg('I', "Hello msg 1.");
  addmsg('W', "Boom");
  addmsg('I', "Ywag");

  clearlog();

  return 0;
}

