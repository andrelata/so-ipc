TARGET = servidor
OBJS = ./obj/servidor.o ./obj/servidor_fifo.o 
SOURCES = servidor_fifo.c servidor.c

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