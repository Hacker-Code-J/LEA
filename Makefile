CC=gcc # Define the compiler
CFLAGS=-Wall -std=c99 -g -O2 -I./include # Define any compile-time flags
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
TESTDIR=./tests
INCDIR=./include

# Object files
OBJS=$(OBJDIR)/lea_core.o $(OBJDIR)/lea_utils.o $(OBJDIR)/lea_tests.o \
      $(OBJDIR)/lea_modes.o $(OBJDIR)/lea_mode_tests.o \
      $(OBJDIR)/lea_cbc_movs.o $(OBJDIR)/lea_cbc_movs.o $(OBJDIR)/lea_cbc_movs.o \
	  $(OBJDIR)/lea_cbc_kat.o $(OBJDIR)/lea_cbc_mmt.o $(OBJDIR)/lea_cbc_mct.o \
	  $(OBJDIR)/main.o
#      $(OBJDIR)/lea_cbc_kat.o $(OBJDIR)/lea_cbc_mmt.o $(OBJDIR)/lea_cbc_mct.o\
	  $(OBJDIR)/main.o
     

# Define the target executable name
TARGET=$(BINDIR)/lea128_cbc_movs 
# TARGET=lea128_test

# Phony targets
.PHONY: all clean dir rebuild

# Default target
all: dir $(TARGET)

# Rule for building the final target
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule for compiling source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include automatically generated dependencies
-include $(OBJS:.o=.d)

# Dependency for main.c
$(OBJDIR)/main.o: main.c $(INCDIR)/lea.h
	$(CC) $(CFLAGS) -c main.c -o $@

# Dependencies
$(OBJDIR)/lea_core.o: $(SRCDIR)/lea_core.c $(INCDIR)/lea.h
$(OBJDIR)/lea_utils.o: $(SRCDIR)/lea_utils.c $(INCDIR)/lea.h
$(OBJDIR)/lea_modes.o: $(SRCDIR)/lea_modes.c $(INCDIR)/lea_modes.h

$(OBJDIR)/lea_cbc_kat.o: $(SRCDIR)/lea_cbc_kat.c $(INCDIR)/lea_cbc_movs.h
$(OBJDIR)/lea_cbc_mmt.o: $(SRCDIR)/lea_cbc_mmt.c $(INCDIR)/lea_cbc_movs.h
$(OBJDIR)/lea_cbc_mct.o: $(SRCDIR)/lea_cbc_mct.c $(INCDIR)/lea_cbc_movs.h
$(OBJDIR)/lea_cbc_movs.o: $(SRCDIR)/lea_cbc_movs.c $(INCDIR)/lea_cbc_movs.h

$(OBJDIR)/lea_tests.o: $(TESTDIR)/lea_tests.c $(INCDIR)/lea.h $(INCDIR)/lea_modes.h
$(OBJDIR)/lea_mode_tests.o: $(TESTDIR)/lea_mode_tests.c $(INCDIR)/lea.h $(INCDIR)/lea_modes.h

# Define the file paths
FILES_TO_DELETE = LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.rsp \
				  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.rsp \
				  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.rsp

# Rule for cleaning up the project
clean:
	rm -f $(OBJS) $(OBJDIR)/*.d $(TARGET)
	@echo "Removing MOVS files ..."
	rm -f $(FILES_TO_DELETE)
	@echo "MOVS files removal completed successfully."
# rm -f lea_test

# Create necessary directories
dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./lea128_cbc_movs)
