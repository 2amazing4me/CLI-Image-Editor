#Copyright 2023 Baldovin Razvan-Mihai-Marian 312CA

CC=gcc
CFLAGS=-Wall -Wextra -g -lm

build: image_editor

image_editor: main.c handler.c commands.c helpers.c
	$(CC) $^ -o $@ $(CFLAGS);

pack:
	zip -FSr 312CA_BaldovinRazvan_Tema3.zip README Makefile *.c *.h

clean:
	rm -f image_editor
