
TARGET = Fileinfo

SRC = main.c

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
