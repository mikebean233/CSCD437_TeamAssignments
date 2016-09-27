#/usr/bin/sh
outFile="$1"
inFile="$2"
onlyPreFlag=""


if [ "$3" = "true" ]
	then
		onlyPreFlag="-E"
fi

oldIFS="$IFS"
IFS=""
source="\"$(cat -v assignment2.c | tr '\n' '-' | sed -e 's/-/\n_/g' -e 's/"/\q_/g' )\""
newline="\"\\n\""
quote="\"\\\"\""
gcc -D"SOURCE=$source" -D"NEWLINE=$newline" -D"QUOTE=$quote" "$onlyPreFlag" "$inFile" -o "$outFile"
IFS=$oldIFS