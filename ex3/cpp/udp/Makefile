CC = g++

CFLAGS =
SRC = udp.cpp Message.cpp Queue.cpp BaseThread.cpp Client.cpp
LDFLAGS = -pthread -std=c++14

TARGET = udp

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
