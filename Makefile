CC=gcc # Define the compiler
CFLAGS=-Wall -std=c99 -g -O2 -I./include # Define any compile-time flags
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
TESTDIR=./tests
INCDIR=./include

CBCDIR=./src/lea_cbc
CTRDIR=./src/lea_ctr

# Object files
OBJS=$(OBJDIR)/lea_core.o $(OBJDIR)/lea_utils.o $(OBJDIR)/lea_tests.o \
    $(OBJDIR)/lea_modes.o $(OBJDIR)/lea_mode_tests.o \
    $(OBJDIR)/lea_movs.o $(OBJDIR)/lea_movs.o $(OBJDIR)/lea_movs.o \
	$(OBJDIR)/lea_cbc_kat.o $(OBJDIR)/lea_cbc_mmt.o $(OBJDIR)/lea_cbc_mct.o \
	$(OBJDIR)/lea_ctr_kat.o $(OBJDIR)/lea_ctr_mmt.o $(OBJDIR)/lea_ctr_mct.o \
	$(OBJDIR)/main.o
     

# Define the target executable name
TARGET=$(BINDIR)/a.out
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
$(OBJDIR)/%.o: $(CBCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(CTRDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include automatically generated dependencies
-include $(OBJS:.o=.d)

# Dependency for main.c
# $(OBJDIR)/main.o: main.c $(INCDIR)/lea.h
# 	$(CC) $(CFLAGS) -c main.c -o $@

# Dependencies
# $(OBJDIR)/lea_core.o: $(SRCDIR)/lea_core.c $(INCDIR)/lea.h
# $(OBJDIR)/lea_utils.o: $(SRCDIR)/lea_utils.c $(INCDIR)/lea.h
# $(OBJDIR)/lea_modes.o: $(SRCDIR)/lea_modes.c $(INCDIR)/lea_modes.h
# $(OBJDIR)/lea_movs.o: $(SRCDIR)/lea_movs.c $(INCDIR)/lea_movs.h

# $(OBJDIR)/lea_cbc_kat.o: $(CBCDIR)/lea_cbc_kat.c $(INCDIR)/lea_movs.h
# $(OBJDIR)/lea_cbc_mmt.o: $(CBCDIR)/lea_cbc_mmt.c $(INCDIR)/lea_movs.h
# $(OBJDIR)/lea_cbc_mct.o: $(CBCDIR)/lea_cbc_mct.c $(INCDIR)/lea_movs.h

# $(OBJDIR)/lea_ctr_kat.o: $(CTRDIR)/lea_ctr_kat.c $(INCDIR)/lea_movs.h
# $(OBJDIR)/lea_ctr_mmt.o: $(CTRDIR)/lea_ctr_mmt.c $(INCDIR)/lea_movs.h
# $(OBJDIR)/lea_ctr_mct.o: $(CTRDIR)/lea_ctr_mct.c $(INCDIR)/lea_movs.h

# $(OBJDIR)/lea_tests.o: $(TESTDIR)/lea_tests.c $(INCDIR)/lea.h $(INCDIR)/lea_modes.h
# $(OBJDIR)/lea_mode_tests.o: $(TESTDIR)/lea_mode_tests.c $(INCDIR)/lea_modes.h

# Define the file paths
CBCFILES_TO_DELETE = LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)KAT.rsp \
				  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.rsp \
				  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.req \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.fax \
                  LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.rsp

CTRFILES_TO_DELETE = LEA128\(CTR\)MOVS/LEA128\(CTR\)KAT.req \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)KAT.fax \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)KAT.rsp \
				  LEA128\(CTR\)MOVS/LEA128\(CTR\)MMT.req \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)MMT.fax \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)MMT.rsp \
				  LEA128\(CTR\)MOVS/LEA128\(CTR\)MCT.req \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)MCT.fax \
                  LEA128\(CTR\)MOVS/LEA128\(CTR\)MCT.rsp

# Rule for cleaning up the project
clean:
	rm -rf $(OBJS) $(OBJDIR)/*.d $(TARGET)
	@echo "Removing MOVS files ..."
	rm -rf $(CBCFILES_TO_DELETE)
	rm -rf $(CTRFILES_TO_DELETE)
	@echo "MOVS files removal completed  successfully."
# rm -f lea_test

# Create necessary directories
dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./lea128_ctr_movs)
#	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./lea128_cbc_movs)
