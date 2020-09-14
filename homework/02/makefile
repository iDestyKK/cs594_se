#
# Homework 2 Makefile
#

# -----------------------------------------------------------------------------
# Configuration                                                            {{{1
# -----------------------------------------------------------------------------

# Compiler Configuration
CC = gcc
CFLAGS = --std=gnu89 -g

# Directories
SRC  = src
OBJ  = obj
LIB  = lib
BIN  = bin
CNDS = $(LIB)/cnds

# Executables
EXEC = \
	$(BIN)/globalign              \
	$(BIN)/localign               \
	$(BIN)/end-gap-free-alignment \
	$(BIN)/local-alignment

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/globalign: $(OBJ)/globalign.o $(OBJ)/helper.o $(OBJ)/arg_parse.o \
                  $(OBJ)/dp.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/localign: $(OBJ)/localign.o $(OBJ)/helper.o $(OBJ)/arg_parse.o \
                 $(OBJ)/dp.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/end-gap-free-alignment: $(OBJ)/end-gap-free-alignment.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/local-alignment: $(OBJ)/local-alignment.o
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Static Libraries                                                         {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cnds.a: $(OBJ)/cn_string.o $(OBJ)/cn_cmp.o $(OBJ)/cn_map.o
	ar cr $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/globalign.o: $(SRC)/globalign.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/localign.o: $(SRC)/localign.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/end-gap-free-alignment.o: $(SRC)/end-gap-free-alignment.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/local-alignment.o: $(SRC)/local-alignment.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/dp.o: $(SRC)/dp.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/helper.o: $(SRC)/helper.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/arg_parse.o: $(SRC)/arg_parse.c
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Object Files (Libraries)                                                 {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cn_string.o: $(CNDS)/cn_string.c $(CNDS)/cn_string.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/cn_cmp.o: $(CNDS)/cn_cmp.c $(CNDS)/cn_cmp.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/cn_map.o: $(CNDS)/cn_map.c $(CNDS)/cn_map.h
	$(CC) $(CFLAGS) -o $@ -c $<


# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) obj/*
