OBJS:=$(wildcard *.c)
server:$(OBJS)
	gcc $^ -g -o $@ -I../include -pthread -lcrypt `mysql_config --cflags --libs`
clean:
	rm server

