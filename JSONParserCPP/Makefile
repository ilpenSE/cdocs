CC = g++
CFLAGS = -Wall -Wextra -O2 -g

CPPS = main.cpp jsonreader.cpp lexer.cpp parser.cpp
TARGET = program.exe

# Varsayılan hedef
all: $(TARGET)

# Programı derle
$(TARGET): $(CPPS)
	$(CC) $(CFLAGS) $(CPPS) -o $(TARGET)

# Çalıştır
run: $(TARGET)
	./$(TARGET)

# GDB ile debug
debug: $(TARGET)
	gdb ./$(TARGET)

# Temizleme
clean:
	rm -f $(TARGET)
