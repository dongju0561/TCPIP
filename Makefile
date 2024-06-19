# Makefile for building Client and Server

# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Directories
CLIENT1_DIR = Client1
CLIENT2_DIR = Client2
SERVER_DIR = Server

# Targets
all: client1 client2 server

client1:
	@echo "Building client1..."
	$(MAKE) -C $(CLIENT1_DIR)

run-client1:
	@echo "Running client1..."
	@$(CLIENT1_DIR)/client1

run-client1-debug:
	@echo "Running client1 in debug mode..."
	@gdb $(CLIENT1_DIR)/client1 

client2:
	@echo "Building client2..."
	$(MAKE) -C $(CLIENT2_DIR)

server:
	@echo "Building server..."
	$(MAKE) -C $(SERVER_DIR)

run-client2:
	@echo "Running client2..."
	@$(CLIENT2_DIR)/client2

run-client2-debug:
	@echo "Running client2 in debug mode..."
	@gdb $(CLIENT2_DIR)/client2 

run-server:
	@echo "Running server..."
	@$(SERVER_DIR)/server

run-server-debug:
	@echo "Running server in debug mode..."
	@gdb $(SERVER_DIR)/server

clean:
	@echo "Cleaning up..."
	$(MAKE) -C $(CLIENT1_DIR) clean
	$(MAKE) -C $(CLIENT2_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
