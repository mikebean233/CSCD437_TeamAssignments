CSCD437, Secure Coding, Assignment 3: Buffer Overrun Exploit

The Wise Guys
Team members:

Heath Nickerbocker
Michael Peterson
Matt McGee

The compilers used for this assignment were the Borland compiler and gcc.

The Borland compiler was used in Windows 10, the script for this exploit 
(Hack_Borland.pl) only gets the program to run its bar() function.

The GCC compiler was used in Ubuntu 14.04, the script for this exploit 
(Hack_GNU.pl) places a payload on the stack (machine instructions to spawn a new shell)
then executes the payload.


building:
I have written a make file that will build with the program with either compiler at the users option,
for building with gcc run “make gnu”, for building with Borland run “make borland”.

Notes: 
- During the build for Linux the makefile will execute a command that requires root permission,
it will likely prompt for the root password.
- Building for windows using the makefile will require the use of Borlands make implementation 
so make sure that all of the Borland tools are accessible through the PATH environment variable.
- This exploit relies on a functional version of perl to be installed for both operating systems.





