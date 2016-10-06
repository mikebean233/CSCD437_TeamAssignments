Assignment 3: Team - Buffer Overrun exploit

- compile and run StackOverrun.c: StackOverrun.c View in a new window
- note the address of the bar function
- add that address to HackOverrun.pl (remember to put the address in in reverse order): HackOverrun.pl  View in a new window
- note that you may need to pad the initial set of letters in HackOverrun.pl to overflow to the return address location on the stack
- You'll need a Perl interpreter to run the Perl script on the windows side. ActivePerl is a good free one for windows
- You must implement the exploit with TWO different compilers. The in class example used the Borland C++ command line 
  compiler: borland_compiler.zip. View in a new window  The exploit can also be implemented with gcc, but depending on the version and 
  platform on which you run you might need to turn of stack protection with: -fno-stack-protector

Finally, you must load a payload (shell script) and spawn a shell on one of the compilers/environments you select.  Seeking help from 
Google for this task is acceptable!

Provide output captures that show your exploit worked with two different compilers.  Be sure and specify which compilers you used 
and on what OS you ran your code.  Submit a zip file with your results (output captures, associated source code, etc.).  Be sure and 
include team member names as part of your submission.

Below is an excellent link to discussion of how gcc works to protect the stack.  The link also contains excellent discussion of how 
buffer overrun works.

http://www.drdobbs.com/security/anatomy-of-a-stack-smashing-attack-and-h/240001832?pgno=1 (Links to an external site.)

  (Links to an external site.)

Have fun!