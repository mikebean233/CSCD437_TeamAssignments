#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <assert.h>

#define IN_BUFF_LENGTH 50
#define NUMBER_BUFF_LENGTH 12
#define READ_WITHIN_BUFFER 0
#define READ_EXCEEDED_BUFFER 1
#define READ_ZERO_BYTES -1


typedef struct {
	char *regex;
	int shouldMatch; // 1: true, 0: false
	char *failDescription;
} regexVerifier;

int isRegexMatch(char *regex, char *input);

int runRegexTestCases(char *regex, char *testCases[]);

void getValidatedString(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[]);

int readInput(char *inputBuffer, int bufferLength);

int main() {
	// input buffers
	char firstName[IN_BUFF_LENGTH], lastName[IN_BUFF_LENGTH];
	char inFilename[IN_BUFF_LENGTH], outFilename[IN_BUFF_LENGTH];
	char intA[NUMBER_BUFF_LENGTH], intB[NUMBER_BUFF_LENGTH];

	// regexes
	char *nameRegex = "^[a-zA-Z]{1,}$";
	char *numberRegex = "^(()|\\+|\\-)[0-9]{1,10}$";
	char *filenameRegex_InCurrentDir = "^(\\.\\/|[^\\/])[a-zA-Z0-9\\s]+(\\.[a-zA-Z]{1,4})$";
	char *filenameRegex_HasAcceptedExtension = "\\.(text|txt)$"; // White list of valid extensions, we need more ideas ...
	char *filenameRegex_HasRelativePath = "\\.\\.";

#ifdef TEST
	// Regex test cases
	char * nameTestCases[]     = {"", "\0", "\n", "", "Jhon", " Jhon", "1ll124lk1 1 4l ", "bob", "a", "13", "bob ", 0};
	char * numberTestCases[]   = {"", "\0", "\n", "0", " 0", "0 ", " 0 ", "-0", "+0", "123", "-123", "+123", "- 123", "123-", "+0123546789", "+01235467891", " ", "apple", 0};
	char * filenameTestCases[] = {"", "\0", "\n", "", "file.dog", "file.Txt", "file.txt", "./file/../.txt", "/file.txt", "./file.txt", "../file.txt", "/../file.txt", "./ /", 0};
#endif


	regexVerifier emptyVerifier = {NULL, 0, NULL};

	/**
	 * 1) get first and last name from user
	 */
	regexVerifier nameVerifiers[] = {{nameRegex, 1, "You must enter only letters"}, emptyVerifier};
	getValidatedString("Enter your first name: ", firstName, IN_BUFF_LENGTH, nameVerifiers);
	getValidatedString("Enter your last name: " , lastName, IN_BUFF_LENGTH, nameVerifiers);

#ifdef TEST
	runRegexTestCases(nameRegex, nameTestCases);
#endif

	/**
	 * 2) get 2 32 bit ints from user
	 */
	regexVerifier numberVerifiers[] = {{numberRegex, 1, "you must only enter digits that may start with a \"+\" or \"-\""}, emptyVerifier};
	getValidatedString("Enter a 32 bit integer: "     , intA, NUMBER_BUFF_LENGTH, numberVerifiers);
	getValidatedString("Enter another 32 bit integer:", intB, NUMBER_BUFF_LENGTH, numberVerifiers);

#ifdef TEST
	runRegexTestCases(numberRegex, numberTestCases);
#endif

	/**
	 * 3) get input/output filename form user
	 */
	regexVerifier filenameVerifiers[] = {
			{filenameRegex_HasAcceptedExtension, 1, "only files with the extensions .txt and .text are allowed"},
			{filenameRegex_HasRelativePath, 0, "you are not allowed to use relative paths"},
			{filenameRegex_InCurrentDir, 1, "you may only specify files in the current directory"},
			emptyVerifier
	};
	getValidatedString("Enter an input filename from the current Directory: " , inFilename , IN_BUFF_LENGTH, filenameVerifiers);
	getValidatedString("Enter an output filename from the current Directory: ", outFilename, IN_BUFF_LENGTH, filenameVerifiers);


#ifdef TEST
	runRegexTestCases(filenameRegex_InCurrentDir,         filenameTestCases);
	runRegexTestCases(filenameRegex_HasAcceptedExtension, filenameTestCases);
	runRegexTestCases(filenameRegex_HasRelativePath,      filenameTestCases);
#endif

	/**
	 * 4) get password from user
	 */





	return 0;
}


int runRegexTestCases(char *regex, char *testCases[]) {
#ifdef TEST
	printf("\n\n---- Test Cases for Regex: \"%s\" --------\n", regex);
	printf("  Is Match  |  Input  \n");
	printf("----------------------\n");
#endif
	char *thisTestCase;
	int i = 0, passCount = 0, havePass = 0;
	while ((thisTestCase = testCases[i++]) != 0) {
		passCount += (havePass = isRegexMatch(regex, thisTestCase)) ? 1 : 0;
#ifdef TEST
		printf("%s|\"%s\"\n", (havePass) ? "     Yes    " : "     No     ", thisTestCase);
#endif
	}
	return passCount;
}

int isRegexMatch(char *regex, char *input) {
	if (regex == NULL || input == NULL)
		return 0;
	char errorMessage[100];

	regex_t regext;
	int reti = regcomp(&regext, regex, REG_EXTENDED);
	if (reti) {
		regerror(reti, &regext, errorMessage, sizeof(errorMessage));
		fprintf(stderr, "%s\n", errorMessage);
		exit(1);
	}

	reti = regexec(&regext, input, 0, NULL, 0);
	regfree(&regext);

	if (!reti) {
		return 1;
	} else if (reti == REG_NOMATCH) {
		return 0;
	} else {
		regerror(reti, &regext, errorMessage, sizeof(errorMessage));
		fprintf(stderr, "%s\n", errorMessage);
		return 0;
	}
}

void getValidatedString(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[]) {
	assert (prompt != NULL && inputBuffer != NULL && inputBufferSize != 0 && verifiers != NULL);

	int isValid = 0;

	while (!isValid) {
		printf("\n%s\n>", prompt);
		int readResult = readInput(inputBuffer, inputBufferSize);
		if (readResult == READ_EXCEEDED_BUFFER) {
			printf("\nYou must enter fewer then %d characters, try again:\n", inputBufferSize);
			isValid = 0;
		} else if (readResult == READ_ZERO_BYTES) {
			printf("\nYou must enter something...\n");
			isValid = 0;
		} else {
			int verifierIndex = 0;
			int failCount = 0;
			regexVerifier thisVerifier;
			while ((thisVerifier = verifiers[verifierIndex++]).regex != NULL) {
				if (thisVerifier.shouldMatch != isRegexMatch(thisVerifier.regex, inputBuffer)) {
					failCount++;
					isValid = 0;
					printf("%s\n", thisVerifier.failDescription);
				}
			}
			if (failCount == 0)
				isValid = 1;
		}
	}
}

int readInput(char *inputBuffer, int bufferLength) {
	unsigned char thisChar;
	int exceededBuffer = 0;
	int copyCount = 0;
	while ((thisChar = getchar()) != '\n') {
		if (copyCount < bufferLength - 1) {
			inputBuffer[copyCount] = thisChar;
			copyCount++;
		} else
			exceededBuffer = 1;
	}
	inputBuffer[copyCount] = '\0';
	return (exceededBuffer) ? READ_EXCEEDED_BUFFER : (copyCount == 0) ? READ_ZERO_BYTES : READ_WITHIN_BUFFER;
}

