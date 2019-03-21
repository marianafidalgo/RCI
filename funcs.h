#ifndef _STRUCT_H_
#define _STRUCT_H_

int check_arg(int argc, char **argv);
int tcpc_init(char *ip, char *port);
int tcps_init(char *command, char *PORT);
int udps_init(char *ipaddr, char *uport);
int udps_SA (char *streamID, char *ipaddr, char *tport, int fd);
int user_interface(char *out, char *command);
int udpc_RS(char *out, char *command, /*int interface, /*int hex,*/ char *rsaddr);
int udpc_POP (char *ipSA, char *portSA, char *out);
int root_communication_protocol(char* out, char *input, /*int interface, /*int hex,*/ char* rsaddr);

#endif