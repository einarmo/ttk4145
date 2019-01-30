CC = g++

CFLAGS = 
SRC = elevator.cpp Message.cpp Queue.cpp BaseThread.cpp TestThread.cpp Scheduler.cpp
LDFLAGS = -pthread -fsanitize=thread -g

TARGET = elevator

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
