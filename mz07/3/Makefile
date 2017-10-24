.PHONY: all clean

CC=gcc
CFLAGS=-c -Wall -Wextra -std=gnu11 -O2
LFLAGS=
SRC=main.c rand.c
OBJ=main.o rand.o
TARGET=3

all: $(TARGET)

%.o: %.c
	gcc $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	gcc $(LFLAGS) $^ -o $@

rand.c: rand.h

clean:
	rm -rf $(OBJ) $(TARGET)
