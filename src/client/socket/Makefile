TARGET = cliente
OBJS = ./obj/client.o ./obj/client_fifo.o 
SOURCES = client_fifo.c client.c

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
