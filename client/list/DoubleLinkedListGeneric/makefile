# makefile sample

TARGET= dllist.out
CC= gcc
CFLAGS=  -g -ansi -pedantic -Wall
OBJS=  DoubleLL_main.o DoubleLL_tests.o DoubleLL.o DoubleLLItr.o DoubleLLItr2.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
	
DoubleLL.o: DoubleLL/DoubleLL.c DoubleLL/DoubleLL.h DoubleLL/ADTErr.h DoubleLLService.h
	$(CC) $(CFLAGS) -c DoubleLL/DoubleLL.c

DoubleLL_tests.o: DoubleTests/DoubleLL_tests.c DoubleTests/DoubleLL_tests.h DoubleLL/DoubleLL.h DoubleLL/ADTErr.h DoubleLLService.h
	$(CC) $(CFLAGS) -c DoubleTests/DoubleLL_tests.c

DoubleLL_main.o: DoubleTests/DoubleLL_main.c DoubleTests/DoubleLL_tests.h DoubleLLService.h  
	$(CC) $(CFLAGS) -c DoubleTests/DoubleLL_main.c

DoubleLLItr.o: DoubleLLItr/DoubleLLItr.c DoubleLLItr/DoubleLLItr.h DoubleLLService.h  
	$(CC) $(CFLAGS) -c DoubleLLItr/DoubleLLItr.c

DoubleLLItr2.o: DoubleLLItr/DoubleLLItr2.c DoubleLLItr/DoubleLLItr2.h DoubleLLService.h  
	$(CC) $(CFLAGS) -c DoubleLLItr/DoubleLLItr2.c

clean:
	rm -f $(TARGET) $(OBJS)
