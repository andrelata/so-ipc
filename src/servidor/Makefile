TARGET = server
#Compilar servidor fifo
#OBJS = ./obj/servidor.o ./obj/servidor_fifo.o 
#SOURCES = servidor_fifo.c servidor.c

#Compilar servidor socket
#OBJS = ./obj/server_Socket.o ./obj/servidor.o 
#SOURCES = server_Socket.c servidor.c

#Compilar servidor socket
OBJS = ./obj/servidor_Socket.o ./obj/servidor.o 
SOURCES = servidor_Socket.c servidor.c

#Compilar servidor queue
#OBJS = ./obj/servidor_queue.o ./obj/servidorC.o 
#SOURCES = servidor_queue.c servidorC.c

#Compilar servidor shmem
#OBJS = ./obj/servidor_shmem.o ./obj/servidorC.o 
#SOURCES = servidor_shmem.c servidorC.c

CFLAGS = -Wall -O -Wuninitialized -g -lpthread
#CFLAGS = -Wall -O -Wuninitialized -g -lrt -lpthread

.SILENT:

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm $(CFLAGS)

$(OBJS) : $(SOURCES) 
	gcc -c $(SOURCES)
	mv *.o ./obj

clean:
	-rm -rf ./obj/*.o
	-rm -rf server
