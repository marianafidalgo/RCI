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

char streamID[64];
char streamNAME[64];
char streamADDR[15];
char streamPORT[10];

char ipaddr[15];
char rsaddr[15];
char rsport[15];
unsigned int tcpsessions;
unsigned int bestpops;
unsigned int tsecs;
char tport[15];
char uport[15];

int fdUP ;
int fdDOWN;
int fdSA ;

int display;
int debug ;
int hex ;
int interface;

#endif