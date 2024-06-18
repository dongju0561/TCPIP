# Makefile for building Client and Server

# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Directories
CLIENT_DIR = Client
SERVER_DIR = Server

# Targets
all: client server

client:
	@echo "Building client..."
	$(MAKE) -C $(CLIENT_DIR)

server:
	@echo "Building server..."
	$(MAKE) -C $(SERVER_DIR)

run-client:
	@echo "Running client..."
	@$(CLIENT_DIR)/client

run-client-debug:
	@echo "Running client in debug mode..."
	@gdb $(CLIENT_DIR)/client 

run-server:
	@echo "Running server..."
	@$(SERVER_DIR)/server

run-server-debug:
	@echo "Running server in debug mode..."
	@gdb $(SERVER_DIR)/server

clean:
	@echo "Cleaning up..."
	$(MAKE) -C $(CLIENT_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
