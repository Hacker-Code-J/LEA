CC=gcc
CFLAGS=-Wall -O3 -I./include
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
INCDIR=./include

# Object files
OBJS=$(OBJDIR)/lea_core.o $(OBJDIR)/lea_utils.o $(OBJDIR)/main.o

# Executable
TARGET=$(BINDIR)/lea_alg

# Default target
all: dir $(TARGET)

# Target for LEA algorithm
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependency for main.c
$(OBJDIR)/main.o: main.c $(INCDIR)/lea.h
	$(CC) $(CFLAGS) -c main.c -o $@

# Dependencies
$(OBJDIR)/lea_core.o: $(SRCDIR)/lea_core.c $(INCDIR)/lea.h
$(OBJDIR)/lea_utils.o: $(SRCDIR)/lea_utils.c $(INCDIR)/lea.h

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Create necessary directories
dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

# Phony targets
.PHONY: all clean directories