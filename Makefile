CC = gcc
CFLAGS = -Wall -Wextra
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OUT_DIR = out
SERVER_OUT_DIR = $(OUT_DIR)/
CLIENT_OUT_DIR = $(OUT_DIR)/

SERVER_SRC = $(SRC_DIR)/server/server.c $(SRC_DIR)/server/user_table_functions.c
CLIENT_SRC = $(SRC_DIR)/client/client.c
SERVER_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SERVER_SRC))
CLIENT_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(CLIENT_SRC))

all: server client

server: $(SERVER_OBJ)
	@mkdir -p $(SERVER_OUT_DIR)
	$(CC) $(CFLAGS) -o $(SERVER_OUT_DIR)/server $(SERVER_OBJ)

client: $(CLIENT_OBJ)
	@mkdir -p $(CLIENT_OUT_DIR)
	$(CC) $(CFLAGS) -o $(CLIENT_OUT_DIR)/client $(CLIENT_OBJ)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OUT_DIR)
