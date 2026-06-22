# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = build/admin_linux

# Regla principal
all: $(TARGET)

# Como compilar el ejecutable
$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

# Limpiar los archivos generados
clean:
	rm -f $(TARGET)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)
