CFLAGS=-g -O2 -Wall

CC = mpicc

PROGRAM_NAME= filtro
OBJS = filtro.o readWriteImage.o Filter.o 

$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $? 

clean:
	rm  *.o $(PROGRAM_NAME) *~
