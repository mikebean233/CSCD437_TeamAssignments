gnu: StackOverrun.c
	gcc -m32 StackOverrun.c -o StackOverrun_GNU -zexecstack -fno-stack-protector -g
	echo "0" | sudo dd of=/proc/sys/kernel/randomize_va_space

borland: StackOverrun.c
	bcc32  StackOverrun.c
	move StackOverrun.exe StackOverrun_Borland.exe

microsoft: StackOverrun.c
	cl StackOverrun.c /FeStackOverrun_Microsoft.exe

asm: StackOverrun.c
	gcc -S -m32 StackOverrun.c -o StackOverrun_GNU -zexecstack -fno-stack-protector -g
