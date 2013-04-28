TARGET = client
OBJS = ./obj/client_shmem.o 
SOURCES = client_shmem.c 

CFLAGS = -Wall -O -Wuninitialized -g -lpthread


.SILENT:

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm $(CFLAGS)

$(OBJS) : $(SOURCES) 
	gcc -c $(SOURCES)
	mv *.o ./obj

clean:
	-rm -rf ./obj/*.o
	-rm -rf servidor
