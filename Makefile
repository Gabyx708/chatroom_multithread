CC = gcc
CFLAGS = -Wall -I src/include/ -fopenmp
SRC = src
LIBS = -pthread
DST_DIR = out

all: create_dir server.out client.out

create_dir:
	mkdir -p $(DST_DIR)

server.out: server.o user_table_functions.o
	$(CC) $(CFLAGS) $(LIBS) -o $(DST_DIR)/server.out server.o user_table_functions.o
client.out: client.o message_functions.o
	$(CC) $(CFLAGS) $(LIBS) -o $(DST_DIR)/client.out client.o
server.o: $(SRC)/server/server.c
	$(CC) $(CFLAGS) -c $(SRC)/server/server.c
client.o: $(SRC)/client/client.c
	$(CC) $(CFLAGS) -c $(SRC)/client/client.c
user_table_functions.o: $(SRC)/server/user_table_functions.c
	$(CC) $(CFLAGS) -c $(SRC)/server/user_table_functions.c
message_functions.o: $(SRC)/client/message_functions.c
	$(CC) $(CFLAGS) -c $(SRC)/client/message_functions.c

clean:
	rm -rf ./$(DST_DIR) && rm  *.out  *.o
