TARGET = client

#Compilar fifo
OBJS = ./obj/cliente.o ./obj/client_fifo.o 
SOURCES = client_fifo.c cliente.c

#Compilar socket
#OBJS = ./obj/cliente.o  ./obj/cliente_socket.o 
#SOURCES = cliente_socket.c cliente.c

#Compilar socket
#OBJS = ./obj/cliente2.o  ./obj/client_socket.o 
#SOURCES = client_socket.c cliente2.c

#Compilar queue
OBJS = ./obj/cliente.o  ./obj/client_queue.o 
SOURCES = client_queue.c cliente.c

#Compilar shmem
#OBJS = ./obj/cliente.o  ./obj/client_shmem.o 
#SOURCES = client_shmem.c cliente.c

#CFLAGS = -Wall -O -Wuninitialized -g -lpthread 
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
