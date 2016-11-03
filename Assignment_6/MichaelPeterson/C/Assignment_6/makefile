all: md5.o assignment6.c
	gcc assignment6.c md5.o -o assignment6 -pedantic -Wall -Wextra -Werror

md5.o: md5.c
	gcc md5.c -c -o md5.o

test: md5.o assignment6.c
	gcc assignment6.c md5.o -o assignment6_test -D __GNU__=4 -pedantic -Wall -Wextra -Werror -D TEST

run: assignment6
	./assignment6

run_test: assignment6_test
	./assignment6_test
