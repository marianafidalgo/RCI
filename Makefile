Projeto: iamroot.o root_com_protocol.o udpclient.o tcpclient.o tcpserver.o udpserver.o
		gcc  -o iamroot iamroot.o udpclient.o tcpclient.o tcpserver.o udpserver.o root_com_protocol.o

iamroot.o: iamroot.c funcs.h 
		gcc  -c iamroot.c

udpclient.o: udpclient.c funcs.h
		gcc  -c udpclient.c

tcpclient.o: tcpclient.c funcs.h
		gcc  -c tcpclient.c

tcpserver.o: tcpserver.c funcs.h
		gcc  -c tcpserver.c

udpserver.o: udpserver.c funcs.h
		gcc  -c udpserver.c

root_com_protocol.o: root_com_protocol.c funcs.h 
		gcc  -c root_com_protocol.c

clean:
	rm -f *.o *.~iamroot *.gch
