# Application source
APP_SRC ?= test_tinyp256.c

# Use Emscripten's compiler
CC = emcc

# Flags for WebAssembly compilation
CFLAGS := -Os -std=c99 -Wall -Wextra -D_ISOC99_SOURCE -MMD -I. -s WASM=1 --no-entry -sERROR_ON_UNDEFINED_SYMBOLS=0 -sEXPORTED_FUNCTIONS="['_memory_grow','_read_args','_write_result']" 

SRC = tinyp256.c ecc.c ecc_dsa.c
OBJ = $(SRC:.c=.o)

%.o:%.c
	$(CC) -c $< $(CFLAGS)

all: test_tinyp256

test_tinyp256: $(basename $(APP_SRC)).o $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(basename $(APP_SRC)).o \
		-o test_tinyp256.wasm

.PHONY: clean
clean:
	rm -rf *.o test_tinyp256 *.d *.wasm