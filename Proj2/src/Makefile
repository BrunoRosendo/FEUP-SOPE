CC=gcc
CFLAGS = -Wall -Wextra
LIBS = -pthread

SERVER_SRC = server_src/server.c server_src/delay.c server_src/lib.c
CLIENT_SRC = client_src/client.c client_src/parsing.c client_src/communication.c

SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

SERVER_EXEC = s
CLIENT_EXEC = c

all: $(SERVER_EXEC) $(CLIENT_EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJ) $(LIBS)

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ) $(LIBS)

clean:
	rm -rf $(SERVER_EXEC) $(CLIENT_EXEC) *.o *.d client_src/*.o client_src/*.d server_src/delay.o


# MAKEFILE FORNECIDO
#all: s c

#s: server.o lib.o delay.c delay.h
#	gcc -Wall -DDELAY=0 -o s delay.c lib.o server.o -pthread

#c: client.c common.h
#	gcc -Wall -o c client.c -pthread

# server.o: server.c common.h
# 	gcc -Wall -c -o server.o server.c

# lib.o: lib.c lib.h
# 	gcc -Wall -c -o lib.o lib.c

#clean:
#	rm -f s c
