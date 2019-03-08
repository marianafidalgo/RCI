#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int check_arg(int argc, char **argv);
int user_interface(char *command);
int udpc(char *command, int interface, int hex);
int root_communication_protocol(char *input, int interface, int hex);

#endif