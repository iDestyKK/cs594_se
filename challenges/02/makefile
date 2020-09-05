#
# Challenge 2 Makefile
#

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
	$(BIN)/needleman-wunsch       \
	$(BIN)/end-gap-free-alignment \
	$(BIN)/local-alignment

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/needleman-wunsch: $(OBJ)/needleman-wunsch.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/end-gap-free-alignment: $(OBJ)/end-gap-free-alignment.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/local-alignment: $(OBJ)/local-alignment.o
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Object Files                                                             {{{1
# -----------------------------------------------------------------------------

$(OBJ)/needleman-wunsch.o: $(SRC)/needleman-wunsch.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/end-gap-free-alignment.o: $(SRC)/end-gap-free-alignment.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/local-alignment.o: $(SRC)/local-alignment.c
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) obj/*
