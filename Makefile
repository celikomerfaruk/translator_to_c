CC=gcc
CFLAGS=-I.

matlang2c: main.o check.o expr_parser.o evaluate_postfix.o
	$(CC) -o matlang2c main.o check.o expr_parser.o evaluate_postfix.o