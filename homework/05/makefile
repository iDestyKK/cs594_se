#
# Homework 5 Makefile
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
	$(BIN)/globalign      \
	$(BIN)/fasta_split    \
	$(BIN)/naive_hamming

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/globalign: $(OBJ)/globalign.o $(OBJ)/helper.o $(OBJ)/arg_parse.o \
                  $(OBJ)/dp.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/fasta_split: $(OBJ)/fasta_split.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(BIN)/naive_hamming: $(OBJ)/naive_hamming.o $(OBJ)/helper.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Static Libraries                                                         {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cnds.a: $(OBJ)/cn_string.o $(OBJ)/cn_vec.o
	ar cr $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/globalign.o: $(SRC)/globalign.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/fasta_split.o: $(SRC)/fasta_split.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/naive_hamming.o: $(SRC)/naive_hamming.c
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

$(OBJ)/cn_vec.o: $(CNDS)/cn_vec.c $(CNDS)/cn_vec.h
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) obj/* submit.tar.xz

package: submit.tar.xz

submit.tar.xz: $(BIN) data $(LIB) $(OBJ) $(SRC) distro.pdf makefile report.txt
	tar -cJvf $@ $^
