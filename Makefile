CC=gcc
CFLAGS=-Wall -g -O3 -I./include
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
TESTDIR=./tests
INCDIR=./include

# Object files
OBJS=$(OBJDIR)/lea_core.o $(OBJDIR)/lea_utils.o $(OBJDIR)/lea_tests.o \
      $(OBJDIR)/lea_modes.o $(OBJDIR)/lea_mode_tests.o \
      $(OBJDIR)/lea_cbc_kat.o \
      $(OBJDIR)/main.o


# Executable
TARGET=$(BINDIR)/lea_alg
# TARGET=lea_test

# Default target
all: dir $(TARGET)

# Target for LEA algorithm
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependency for main.c
$(OBJDIR)/main.o: main.c $(INCDIR)/lea.h
	$(CC) $(CFLAGS) -c main.c -o $@

# Dependencies
$(OBJDIR)/lea_core.o: $(SRCDIR)/lea_core.c $(INCDIR)/lea.h
$(OBJDIR)/lea_utils.o: $(SRCDIR)/lea_utils.c $(INCDIR)/lea.h
$(OBJDIR)/lea_modes.o: $(SRCDIR)/lea_modes.c $(INCDIR)/lea_modes.h

$(OBJDIR)/lea_cbc_kat.o: $(SRCDIR)/lea_cbc_kat.c $(INCDIR)/lea_cbc_movs.h

$(OBJDIR)/lea_tests.o: $(TESTDIR)/lea_tests.c $(INCDIR)/lea.h $(INCDIR)/lea_modes.h
$(OBJDIR)/lea_mode_tests.o: $(TESTDIR)/lea_mode_tests.c $(INCDIR)/lea.h $(INCDIR)/lea_modes.h

# Define the file paths
FILES_TO_DELETE = LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.rsp

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Removing MOVS files"
	rm -f $(FILES_TO_DELETE)
	@echo "Removed MOVS files"
# rm -f lea_test

# Create necessary directories
dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

leak: 
	valgrind --leak-check=full --show-leak-kinds=all $(TARGET)

# Phony targets
.PHONY: all clean directories