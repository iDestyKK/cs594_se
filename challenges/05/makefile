#
# Challenge 5 Makefile
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
	$(BIN)/gen_suffix_tree

all: $(EXEC)

# -----------------------------------------------------------------------------
# Executables                                                              {{{1
# -----------------------------------------------------------------------------

$(BIN)/gen_suffix_tree: $(OBJ)/gen_suffix_tree.o $(OBJ)/cnds.a
	$(CC) $(CFLAGS) -o $@ $^

# -----------------------------------------------------------------------------
# Static Libraries                                                         {{{1
# -----------------------------------------------------------------------------

$(OBJ)/cnds.a: $(OBJ)/cn_string.o $(OBJ)/cn_cmp.o $(OBJ)/cn_map.o \
               $(OBJ)/cn_vec.o
	ar cr $@ $^

# -----------------------------------------------------------------------------
# Object Files (Program Source)                                            {{{1
# -----------------------------------------------------------------------------

$(OBJ)/gen_suffix_tree.o: $(SRC)/gen_suffix_tree.c
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

$(OBJ)/cn_vec.o: $(CNDS)/cn_vec.c $(CNDS)/cn_vec.h
	$(CC) $(CFLAGS) -o $@ -c $<

# -----------------------------------------------------------------------------
# Extras                                                                   {{{1
# -----------------------------------------------------------------------------

clean:
	$(RM) $(EXEC) $(TEST_EXEC) obj/* submit.tar.xz

# Tar package functionality
package: submit.tar.xz

submit.tar.xz: $(BIN) data/*.txt data/*.predict $(LIB) $(OBJ) $(SRC) \
               distro.pdf makefile report.txt
	tar -cJvf $@ $^
