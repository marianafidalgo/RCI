Projeto: iamroot.o root_com_protocol.o udpclient.o
		gcc  -o iamroot iamroot.o udpclient.o root_com_protocol.o

iamroot.o: iamroot.c funcs.h 
		gcc  -c iamroot.c

udpclient.o: udpclient.c funcs.h
		gcc  -c udpclient.c

root_com_protocol.o: root_com_protocol.c funcs.h 
		gcc  -c root_com_protocol.c

clean:
	rm -f *.o *.~iamroot *.gch
