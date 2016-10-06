gcc: StackOverrun.c
	gcc StackOverrun.c -o StackOverrun -fno-stack-protector

borland: StackOverrun.c
	bcc32  StackOverrun.c
	move StackOverrun.exe StackOverrun_Borland.exe

microsoft: StackOverrun.c
	cl StackOverrun.c /FeStackOverrun_Microsoft.exe

asm: StackOverrun.c
	gcc -S -g StackOverrun.c -o StackOverrun.s -fno-stack-protector
