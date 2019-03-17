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
int tcpc(char *name, char *ip, char *port);
int tcps(char *PORT);
int udps(char *PORT);
int user_interface(char *out, char *command);
int udpc(char *out, char *command, int interface, int hex);
int root_communication_protocol(char* out, char *input, int interface, int hex);

#endif