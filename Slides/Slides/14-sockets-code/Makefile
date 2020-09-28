CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = simple_server simple_client pause_server \
	   unix_client unix_server_single unix_server_looped

all : $(PROGRAMS)

simple_server : simple_server.c
	$(CC) -o $@ $^

simple_client : simple_client.c
	$(CC) -o $@ $^

pause_server : pause_server.c
	$(CC) -o $@ $^

unix_client : unix_client.c
	$(CC) -o $@ $^

unix_server_single : unix_server_single.c
	$(CC) -o $@ $^

unix_server_looped : unix_server_looped.c
	$(CC) -o $@ $^

clean :
	rm -f the.socket *.o $(PROGRAMS)
