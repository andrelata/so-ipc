TARGET = client

#Compilar fifo
#OBJS = ./obj/client.o ./obj/client_fifo.o 
#SOURCES = client_fifo.c client.c

#Compilar socket
#OBJS = ./obj/client.o  ./obj/client_socket.o 
#SOURCES = client_socket.c client.c

#Compilar queue
OBJS = ./obj/client.o  ./obj/client_queue.o 
SOURCES = client_queue.c client.c

#Compilar shmem
#OBJS = ./obj/client.o  ./obj/client_shmem.o 
#SOURCES = client_shmem.c client.c

CFLAGS = -Wall -O -Wuninitialized -g -lrt -lpthread 


.SILENT:

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm $(CFLAGS)

$(OBJS) : $(SOURCES) 
	gcc -c $(SOURCES)
	mv *.o ./obj

clean:
	-rm -rf ./obj/*.o
	-rm -rf client
