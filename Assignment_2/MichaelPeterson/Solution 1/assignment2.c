#include <stdio.h>
#include <string.h>

int main(int argc, const char * argv[]) {
	char* source = SOURCE;
	int len = strlen(source), i;
	for(i = 0; i < len; ++i) {
		if(source[i] == 'n' && source[i + 1] == '_') {
			printf(NEWLINE);
			++i;
		}
		else if(source[i] == 'q' && source[i + 1] == '_'){
			printf(QUOTE);
			++i;
		}
		else {
			printf("%c", source[i]);
		}
	}

    return 0;
}
