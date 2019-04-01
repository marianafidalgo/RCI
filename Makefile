Projeto: iamroot.o root_com_protocol.o udpclient.o tcpclient.o tcpserver.o udpserver.o
		gcc -o iamroot iamroot.o udpclient.o tcpclient.o tcpserver.o udpserver.o root_com_protocol.o -g

iamroot.o: iamroot.c initvar.h
		gcc -c iamroot.c -g

udpclient.o: udpclient.c defs.h
		gcc -c udpclient.c -g

tcpclient.o: tcpclient.c defs.h
		gcc -c tcpclient.c -g

tcpserver.o: tcpserver.c defs.h
		gcc -c tcpserver.c -g

udpserver.o: udpserver.c defs.h
		gcc -c udpserver.c -g

root_com_protocol.o: root_com_protocol.c defs.h
		gcc -c root_com_protocol.c -g

clean:
	rm -f *.o *.~iamroot *.gch
