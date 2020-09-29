#
# Homework 3 Makefile
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
	$(BIN)/odc_seq_gen    \
	$(BIN)/viterbi        \
	$(BIN)/viterbi_alg_ex

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/odc_seq_gen: $(OBJ)/odc_seq_gen.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/viterbi: $(OBJ)/viterbi.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(BIN)/viterbi_alg_ex: $(OBJ)/viterbi_alg_ex.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

# -----------------------------------------------------------------------------
# Static Libraries                                                         {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cnds.a: $(OBJ)/cn_string.o
	ar cr $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/odc_seq_gen.o: $(SRC)/odc_seq_gen.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/viterbi.o: $(SRC)/viterbi.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/viterbi_alg_ex.o: $(SRC)/viterbi_alg_ex.c
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Object Files (Libraries)                                                 {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cn_string.o: $(CNDS)/cn_string.c $(CNDS)/cn_string.h
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) obj/* submit.tar.xz

package: submit.tar.xz

submit.tar.xz: $(BIN) data $(LIB) $(OBJ) $(SRC) distro.pdf makefile report.txt
	tar -cJvf $@ $^
