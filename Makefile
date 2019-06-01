Projeto: iamroot.o udpclient.o tcpclient.o tcpserver.o udpserver.o
		gcc -o iamroot iamroot.o udpclient.o tcpclient.o tcpserver.o udpserver.o

iamroot.o: iamroot.c initvar.h
		gcc -c iamroot.c

udpclient.o: udpclient.c defs.h
		gcc -c udpclient.c

tcpclient.o: tcpclient.c defs.h
		gcc -c tcpclient.c

tcpserver.o: tcpserver.c defs.h
		gcc -c tcpserver.c

udpserver.o: udpserver.c defs.h
		gcc -c udpserver.c

clean:
	rm -f *.o *.~iamroot *.gch
