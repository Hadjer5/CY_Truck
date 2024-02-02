CC= gcc -Wall
SRC= $(wildcard *.c)
HEAD= $(wildcard *.h)
OBJ= $(SRC:.c=.o)
PROG= main

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(HEAD)
	$(CC) -o $@ -c $<
	
clean:
	$(RM) $(OBJ) $(PROG)
	
