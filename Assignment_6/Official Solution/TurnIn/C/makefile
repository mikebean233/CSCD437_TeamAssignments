
assignment6: md5.o assignment6.c
	gcc assignment6.c md5.o -o assignment6 -pedantic -Wall -Wextra -Werror -std=c99

md5.o: md5.c
	gcc md5.c -c -o md5.o

test: md5.o assignment6.c
	gcc assignment6.c md5.o -o assignment6_test -pedantic -Wall -Wextra -Werror -D TEST -std=c99

run: assignment6
	./assignment6

run_test: assignment6_test
	./assignment6_test

clean:
	rm -f assignment6 assignment6_test md5.o