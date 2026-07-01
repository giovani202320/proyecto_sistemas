CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = build/admin_linux
SRCS = main.c src/comandos/comandos.c src/archivos/archivos.c 

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
