CC := gcc
CFLAGS := -Wall
LDFLAGS := -L/home/vlad/psoProiect/
LDLIBS := -lso_client

all: client

client: client.o libso_client.so
	$(CC) $^ -o $@ -L. -lso_client


libso_client.so: so_client.o
	$(CC) -shared $^ -o $@

so_client.o: so_client.c
	$(CC) -c -fPIC $^ -o $@

.PHONY: clean

clean:
	rm -f *.o client libso_client.so