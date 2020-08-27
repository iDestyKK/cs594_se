# -----------------------------------------------------------------------------
# Configuration                                                            {{{1
# -----------------------------------------------------------------------------

# Compiler Configuration
CC = gcc
CFLAGS = --std=gnu89 -g

# Directories
SRC = src
OBJ = obj
BIN = bin

# Executables
EXEC = \
	$(BIN)/dna_gen                       \
	$(BIN)/reverse                       \
	$(BIN)/complement                    \
	$(BIN)/multinomial_model_probability

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/dna_gen: $(OBJ)/dna_gen.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/reverse: $(OBJ)/reverse.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/complement: $(OBJ)/complement.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/multinomial_model_probability: $(OBJ)/multinomial_model_probability.o
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Object Files                                                             {{{1
# -----------------------------------------------------------------------------

$(OBJ)/dna_gen.o: $(SRC)/dna_gen.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/reverse.o: $(SRC)/reverse.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/complement.o: $(SRC)/complement.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/multinomial_model_probability.o: $(SRC)/multinomial_model_probability.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(EXEC) obj/*
