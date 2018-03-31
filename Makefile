all: test.c cJSON.c
	gcc -W -g -o test.out test.c cJSON.c -lpthread -lm