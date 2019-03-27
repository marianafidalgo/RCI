#ifndef _STRUCT_H_
#define _STRUCT_H_

int check_arg(int argc, char **argv);
int tcpc_init(char *ip, char *port);
int tcpc_new(char *ip, char *port);
int tcps_init(char *PORT);
int tcps_WE (int fdDOWN, char *fdDOWNsessions);
int udps_init(char *ipaddr, char *uport);
int udps_SA (char *streamID, char *ipaddr, char *tport, int fd);
int user_interface(char *out, char *command);
int udpc_RS(char *out, char *command, char *rsaddr);
int udpc_POP (char *ipADDR, char *uPORT, char *tPORT);
int udpc_PA (char *ipADDR, char *uPORT, char *tPORT);
int root_communication_protocol(char* out, char *input, char* rsaddr);

#endif