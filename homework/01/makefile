#
# Homework 1 Makefile
#

# -----------------------------------------------------------------------------
# Configuration                                                            {{{1
# -----------------------------------------------------------------------------

# Compiler Configuration
CC = gcc
CFLAGS = --std=gnu89 -g
CMFLAGS = -lm

# Directories
SRC  = src
OBJ  = obj
LIB  = lib
BIN  = bin
CNDS = $(LIB)/cnds

# Executables
EXEC = \
	$(BIN)/reverse_complement      \
	$(BIN)/frequency_analysis      \
	$(BIN)/multinomial_probability \
	$(BIN)/markov_probability      \
	$(BIN)/sequence_gen

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/reverse_complement: $(OBJ)/reverse_complement.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/frequency_analysis: $(OBJ)/frequency_analysis.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/multinomial_probability: $(OBJ)/multinomial_probability.o $(OBJ)/helper.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^ $(CMFLAGS)

$(BIN)/markov_probability: $(OBJ)/markov_probability.o $(OBJ)/helper.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^ $(CMFLAGS)

$(BIN)/sequence_gen: $(OBJ)/sequence_gen.o $(OBJ)/helper.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Static Libraries                                                         {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cnds.a: $(OBJ)/cn_string.o $(OBJ)/cn_cmp.o $(OBJ)/cn_map.o
	ar cr $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/reverse_complement.o: $(SRC)/reverse_complement.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/frequency_analysis.o: $(SRC)/frequency_analysis.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/multinomial_probability.o: $(SRC)/multinomial_probability.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/markov_probability.o: $(SRC)/markov_probability.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/sequence_gen.o: $(SRC)/sequence_gen.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)/helper.o: $(SRC)/helper.c
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
	$(RM) $(EXEC) obj/* submit.tar.xz

package: submit.tar.xz

submit.tar.xz: $(BIN) data $(LIB) $(OBJ) $(SRC) distro.pdf makefile report.txt
	tar -cJvf $@ $^
