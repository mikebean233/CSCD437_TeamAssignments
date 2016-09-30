#! /usr/bin/bash
function verboseExecute {
	command=$1
	echo $command
	eval "$command"
}

echo; echo "--------- Compiling source ----------"
verboseExecute "gcc assignment2.c -o assignment2"

echo; echo "--------- capturing output of program into assignment2_out.c ----------"
verboseExecute "./assignment2 > assignment2_out.c"

echo; echo "--------- printing output of program -----------"
verboseExecute "./assignment2"

echo; echo "--------- printing source of program ----------"
verboseExecute "cat ./assignment2.c"

echo; echo "--------- Performing diff on both source files ---------"
verboseExecute "diff assignment2.c assignment2_out.c"
