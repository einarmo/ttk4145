CC = g++

CFLAGS =
SRC = tcp.cpp Message.cpp Queue.cpp BaseThread.cpp Client.cpp
LDFLAGS = -pthread -std=c++14

TARGET = tcp

all: $(TARGET)

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean
