CC = cc
CFLAGS = -Wall -I./src
SRC = main.c search.c utils.c globals.c
OBJS = main.o search.o utils.o globals.o

all: $(OBJS)
	$(CC) $(OBJS) -o vdrop
	rm -rf $(OBJS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o vdrop
