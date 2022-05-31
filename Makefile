# Copyright 2022 Dumitru Bianca Stefania & Nioata Alexandra 312CA

CFLAGS=-Wall -Wextra -g -std=c99 -m32

all: build

build:
	gcc $(CFLAGS) main.c tree.c linked_list.c -o sd_fs

clean:
	rm *.o sd_fs

run:
	./sd_fs
