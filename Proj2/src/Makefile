CC=gcc
CFLAGS = -Wall -Wextra
LIBS = -pthread

SERVER_SRC = server_src/server.c server_src/delay.c server_src/lib.c server_src/parsing.c server_src/communication.c
CLIENT_SRC = client_src/client.c client_src/parsing.c client_src/communication.c

SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

SERVER_EXEC = s
CLIENT_EXEC = client_src/c

all: $(SERVER_EXEC) $(CLIENT_EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -DDELAY=100 -o $@ $(SERVER_OBJ) $(LIBS)

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ) $(LIBS)

clean:
	rm -rf $(SERVER_EXEC) $(CLIENT_EXEC) *.o *.d client_src/*.o client_src/*.d server_src/*.o server_src/*.d
