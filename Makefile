# CCiler and Linking Variables
CC = gcc
CFLAGS = -Wall -fPIC
LFLAGS = -pthread -lm
MM_LIB = libmemory_manager.so
MM_L = lmemory_manager

# Source and Object Files
MM_C = memory_manager.c
MM_O = $(MM_C:.c=.o)

LL_C = linked_list.c
LL_O = $(LL_C:.c=.o)

TEST_MM = test_memory_manager
TEST_LL = test_linked_list

# Default target
all: mmanager list test_mmanager test_list
	@echo "Make all built"

# Rule to create libmemory_manager.so
$(MM_LIB): $(MM_O)
	@$(CC) -shared -o $@ $(MM_O) $(LFLAGS)

# Generic rule to CCile source files into object files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Build the memory manager
mmanager: libmemory_manager.so

# Build the linked list
list: $(LL_O)

# Test target to run the memory manager test program
test_mmanager: $(MM_LIB)
	@$(CC) -o $(TEST_MM) $(TEST_MM).c -lm -L. -$(MM_L) $(LFLAGS) -Wl,-rpath=.

# Test target to run the linked list test program
test_list: $(MM_LIB) linked_list.o
	@$(CC) -o $(TEST_LL) $(LL_C) $(TEST_LL).c -lm -L. -$(MM_L) $(LFLAGS) -Wl,-rpath=.

# Clean target to clean up build files
clean:
	@rm -f $(MM_O) $(MM_LIB) $(LL_O) $(TEST_MM) $(TEST_LL)
	@echo "Files removed"