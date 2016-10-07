gcc: StackOverrun.c
	gcc -m32 StackOverrun.c -o StackOverrun_GNU -execstack -fno-stack-protector
	echo "0" | sudo dd of=/proc/sys/kernel/randomize_va_space

borland: StackOverrun.c
	bcc32  StackOverrun.c
	move StackOverrun.exe StackOverrun_Borland.exe

microsoft: StackOverrun.c
	cl StackOverrun.c /FeStackOverrun_Microsoft.exe

asm: StackOverrun.c
	gcc -S -g StackOverrun.c -o StackOverrun.s -fno-stack-protector
