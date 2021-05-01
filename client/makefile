

TARGET= ipc.out 
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= server.o userStruct.o userMng.o protocol.o serverAppMain.o lib_struct.a

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)	

server.o: server.c server.h list.h list_itr.h lib_struct.a
	$(CC) $(CFLAGS) -c server.c	

userStruct.o: userStruct.c userStruct.h list.h list_itr.h lib_struct.a
	$(CC) $(CFLAGS) -c userStruct.c	lib_struct.a
	
userMng.o: userMng.c userMng.h userStruct.c	userStruct.h HashMap.h lib_struct.a
	$(CC) $(CFLAGS) -c userMng.c	lib_struct.a
	
protocol.o: protocol.c protocol.h lib_struct.a
	$(CC) $(CFLAGS) -c protocol.c   lib_struct.a

serverApp.o: serverApp.c serverApp.h userMng.o server.o lib_struct.a
	$(CC) $(CFLAGS) -c serverApp.c   lib_struct.a
	
serverAppMain.o: serverAppMain.c serverApp.c serverApp.h lib_struct.a
	$(CC) $(CFLAGS) -c serverAppMain.c   lib_struct.a


clean:
	rm -f server.o userStruct.o userMng.o protocol.o serverApp.o serverAppMain.o
	
	# gcc -ansi -pedantic -g chatOpener.c clientApp.c clientMainUI.c clientNet.c clientUI.c protocol.c lib_struct.a 
