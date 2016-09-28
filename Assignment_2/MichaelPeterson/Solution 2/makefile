#!/usr/bin/sh

all: assignment2_char.c
	gcc assignment2_char.c -o assignment2_char
	./assignment2_char > assignment2_nochar.c
	gcc assignment2_nochar.c  -o assignment2_nochar
	echo " ------ Producing final source ----------"
	./assignment2_nochar > assignment2.c
	gcc assignment2.c -o assignment2
	echo " ------ Performing diff --------- "
	diff assignment2.c assignment2_nochar.c
