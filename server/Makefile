CC :=gcc
CFLAGS := -Wall
LDFLAGS := -L/home/simi/psoProiect
LDLIBS := -lso_server

all: server

server: server.o libso_server.so
	$(CC) $^ -o $@ -L. -lso_server

libso_server.so: so_server.o
	$(CC) -shared $^ -o $@

so_server.o: so_server.c
	$(CC) -c -fPIC $^ -o $@

.PHONY: clean all

clean:
	rm *.o server libso_server.so