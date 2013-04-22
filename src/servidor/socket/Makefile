TARGET = server
OBJS = ./obj/server_Socket.o 
SOURCES = server_Socket.c

CFLAGS = -Wall -O -Wuninitialized -g


.SILENT:

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm

$(OBJS) : $(SOURCES) 
	gcc -c $(SOURCES)
	mv *.o ./obj

clean:
	-rm -rf ./obj/*.o
	-rm -rf servidor
