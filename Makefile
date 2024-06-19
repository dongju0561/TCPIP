# Makefile for building Client and Server

# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Directories
CLIENT1_DIR = Client1
CLIENT2_DIR = Client2
CLIENT3_DIR = Client3
SERVER_DIR = Server

# All targets
all: client1 client2 client3 server

# Client1 targets
client1:
	@echo "Building client1..."
	$(MAKE) -C $(CLIENT1_DIR)

run-client1:
	@echo "Running client1..."
	@sudo $(CLIENT1_DIR)/client1

run-client1-debug:
	@echo "Running client1 in debug mode..."
	@sudo gdb $(CLIENT1_DIR)/client1 

# Client2 targets
client2:
	@echo "Building client2..."
	$(MAKE) -C $(CLIENT2_DIR)

run-client2:
	@echo "Running client2..."
	@sudo $(CLIENT2_DIR)/client2

run-client2-debug:
	@echo "Running client2 in debug mode..."
	@sudo gdb $(CLIENT2_DIR)/client2 

# Client3 targets
client3:
	@echo "Building client3..."
	$(MAKE) -C $(CLIENT3_DIR)

run-client3:
	@echo "Running client3..."
	@sudo $(CLIENT3_DIR)/client3

run-client3-debug:
	@echo "Running client3 in debug mode..."
	@sudo gdb $(CLIENT3_DIR)/client3

# Server targets
server:
	@echo "Building server..."
	$(MAKE) -C $(SERVER_DIR)

run-server:
	@echo "Running server..."
	@$(SERVER_DIR)/server

run-server-debug:
	@echo "Running server in debug mode..."
	@gdb $(SERVER_DIR)/server

# Clean
clean:
	@echo "Cleaning up..."
	$(MAKE) -C $(CLIENT1_DIR) clean
	$(MAKE) -C $(CLIENT2_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
