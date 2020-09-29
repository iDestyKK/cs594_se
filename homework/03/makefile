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
	$(BIN)/odc_seq_gen

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/odc_seq_gen: $(OBJ)/odc_seq_gen.o
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/odc_seq_gen.o: $(SRC)/odc_seq_gen.c
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) obj/* submit.tar.xz

package: submit.tar.xz

submit.tar.xz: $(BIN) data $(LIB) $(OBJ) $(SRC) distro.pdf makefile report.txt
	tar -cJvf $@ $^
