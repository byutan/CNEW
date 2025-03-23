CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = cnew
SRC = main.c
PWD = $(shell pwd)

all: $(TARGET)
$(TARGET): $(SRC)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "cnew installed successfully."

uninstall:
	@sudo rm -f /usr/local/bin/cnew
	@echo "cnew uninstalled successfully."

clean:
	@rm -f $(TARGET)
