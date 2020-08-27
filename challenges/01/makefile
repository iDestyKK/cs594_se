CC = gcc
CFLAGS = --std=gnu89 -g
EXEC = dna_gen reverse complement

all: $(EXEC)

dna_gen: dna_gen.c
	$(CC) $(CFLAGS) -o $@ $^

reverse: reverse.c
	$(CC) $(CFLAGS) -o $@ $^

complement: complement.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(EXEC)
