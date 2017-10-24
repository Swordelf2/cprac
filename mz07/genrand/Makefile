.PHONY: all clean

CC=gcc
CFLAGS=-c -Wall -Wextra -std=gnu11 -O2
LFLAGS=
SRC=main.c rand.c
OBJ=main.o rand.o
TARGET=genrand

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@

rand.c: rand.h

clean:
	rm -rf $(OBJ) $(TARGET)
