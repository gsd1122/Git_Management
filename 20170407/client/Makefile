OBJS:=$(wildcard *.c)
client:$(OBJS)
	gcc $^ -g -o $@ -I../include -pthread -lcrypt
clean:
	rm client

