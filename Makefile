CC = gcc
CFLAGS = -Wall -Wextra -O2

# Tüm objeler
OBJS = main.o powers.o timelapse.o

# Derlenecek program
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c powers.h timelapse.h
	$(CC) $(CFLAGS) -c main.c

powers.o: powers.c powers.h
	$(CC) $(CFLAGS) -c powers.c

timelapse.o: timelapse.c timelapse.h
	$(CC) $(CFLAGS) -c timelapse.c

clean:
	rm -f $(OBJS) $(TARGET)