#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>
#include <limits.h>
#include <sys/stat.h>

#define IN_BUFF_LENGTH       50
#define NUMBER_BUFF_LENGTH   12
#define READ_WITHIN_BUFFER   0
#define READ_EXCEEDED_BUFFER 1
#define READ_ZERO_BYTES     -1
#define INPUT_FILE           1
#define OUTPUT_FILE          2


typedef struct {
	char *regex;
	int shouldMatch; // 1: true, 0: false
	char *failDescription;
} regexVerifier;

int isRegexMatch(char *regex, char *input);
int runRegexTestCases(char *regex, char *testCases[]);
void getValidatedString(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[]);
int readInput(char *inputBuffer, int bufferLength);
long long getVerifiedInteger(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[]);
FILE* getValidFile(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[], int fileType);

int main(int argc, char** argv) {
	// First things first, make sure this isn't being ran as root
	if(!getuid()){
		fprintf(stderr, "\nThis program must not be run as the root user!\n");
		exit(1);
	}

#if __GNUC__ < 4
	fprintf(stderr, "\nThis is only designed to be compiled using GCC 4.0 or newer\n");
	exit(1);
#endif
#ifdef _WIN32
	fprintf(stderr, "\nThis program is not designed to run in windows\n");
	exit(1);
#endif
#ifdef _WIN64
	fprintf(stderr, "\nThis program is not designed to run in windows\n");
	exit(1);
#endif

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
	getValidatedString("Enter your last name: " , lastName,  IN_BUFF_LENGTH, nameVerifiers);

#ifdef TEST
	runRegexTestCases(nameRegex, nameTestCases);
#endif

	/**
	 * 2) get 2 32 bit ints from user
	 */
	regexVerifier numberVerifiers[] = {{numberRegex, 1, "you must only enter digits which may be preceded with an \"+\" or \"-\""}, emptyVerifier};
	//getValidatedString("Enter a 32 bit integer: "     , intA, NUMBER_BUFF_LENGTH, numberVerifiers);
	//getValidatedString("Enter another 32 bit integer:", intB, NUMBER_BUFF_LENGTH, numberVerifiers);
	long long integerA = getVerifiedInteger("Enter the first 32 bit integer: " ,intA, NUMBER_BUFF_LENGTH, numberVerifiers);
	long long integerB = getVerifiedInteger("Enter the second 32 bit integer: ",intB, NUMBER_BUFF_LENGTH, numberVerifiers);
	long long addResult  = integerA + integerB;
	long long multResult = integerA * integerB;

#ifdef TEST
	runRegexTestCases(numberRegex, numberTestCases);
#endif

	/**
	 * 3) get input/output filename from user
	 */
	regexVerifier filenameVerifiers[] = {
			{filenameRegex_HasAcceptedExtension, 1, "only files with the extensions .txt and .text are allowed"},
			{filenameRegex_HasRelativePath,      0, "you are not allowed to use relative paths"},
			{filenameRegex_InCurrentDir,         1, "you may only specify files in the current directory"},
			emptyVerifier
	};
	//getValidatedString("Enter an input filename from the current Directory: " , inFilename , IN_BUFF_LENGTH, filenameVerifiers);
	//getValidatedString("Enter an output filename from the current Directory: ", outFilename, IN_BUFF_LENGTH, filenameVerifiers);
	FILE* inputFile  = getValidFile("Enter an input file path from the current directory" , inFilename , IN_BUFF_LENGTH, filenameVerifiers, INPUT_FILE);
	FILE* outputFile = getValidFile("Enter an output file path from the current directory", outFilename, IN_BUFF_LENGTH, filenameVerifiers, OUTPUT_FILE);


#ifdef TEST
	runRegexTestCases(filenameRegex_InCurrentDir,         filenameTestCases);
	runRegexTestCases(filenameRegex_HasAcceptedExtension, filenameTestCases);
	runRegexTestCases(filenameRegex_HasRelativePath,      filenameTestCases);
#endif

	/**
	 * 4) get password from user
	 */



	fclose(inputFile);
	fclose(outputFile);
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
	int reti = regcomp(&regext, regex, REG_EXTENDED | REG_ENHANCED);
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
					printf("\n- %s\n", thisVerifier.failDescription);
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
		if (copyCount < bufferLength - 1)
			inputBuffer[copyCount++] = thisChar;
		else
			exceededBuffer = 1;
	}
	inputBuffer[copyCount] = '\0';
	return (exceededBuffer) ? READ_EXCEEDED_BUFFER : (copyCount == 0) ? READ_ZERO_BYTES : READ_WITHIN_BUFFER;
}

long long getVerifiedInteger(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[]){
	int isValid = 0;
	long long returnValue = 0;
	int problemCount = 0;
	while(!isValid){
		getValidatedString(prompt, inputBuffer, inputBufferSize, verifiers);
		errno = 0;
		returnValue = strtoll(inputBuffer, NULL, 10);
		isValid = 1;
		if(returnValue > INT_MAX){
			printf("\n- The provided value exceeds the maximum value for a 32 bit signed integer");
			isValid = 0;
		}

		if(returnValue < INT_MIN){
			printf("\n- The provided value is less then the minimum value for a 32 but signed integer");
			isValid = 0;
		}

		if(errno){ // This should never happen...
			printf("%s\n", strerror(errno));
			isValid = 0;
		}
	}
 return returnValue;
}

FILE* getValidFile(char *prompt, char *inputBuffer, int inputBufferSize, regexVerifier verifiers[], int fileType){
	assert(fileType == INPUT_FILE || fileType == OUTPUT_FILE);

	FILE* validFile = NULL;

	int isValid = 0;
	while(!isValid){
		isValid = 1;
		getValidatedString(prompt, inputBuffer, inputBufferSize, verifiers);

		errno = 0;
		struct stat thisStat;
		int statReturnValue = lstat(inputBuffer, &thisStat);

		if(fileType == INPUT_FILE){
			// Fail if something went wrong trying to get the input files status (this includes files that don't exist)
			if (statReturnValue) {
				printf("\n- %s\n", strerror(errno));
				isValid = 0;
			}
			else{
				mode_t mode = thisStat.st_mode;
				int ownerId = thisStat.st_uid;

				// Make sure we are dealing with a regular file
				if(!S_ISREG(mode)){
					printf("\n- You must specify a regular file (directories, character/block devices, named pipes, sockets, and symbolic links are not allowed)\n");
					isValid = 0;
				}

				// Make sure the current user is actually the owner of the input file
				if(ownerId != getuid()){
					printf("\n- You must specify a file that you own\n");
					isValid = 0;
				}

				// Make sure the owner has read privileges
				if(!(mode & S_IRUSR)){
					printf("\n- You must specify a file that you have read privileges to\n");
					isValid = 0;
				}
			}
			if(isValid){
				errno = 0;
				if(!(validFile = fopen(inputBuffer, "r"))){
					printf("\n- %s\n", strerror(errno));
					isValid = 0;
				}
			}
		}
		else if(fileType == OUTPUT_FILE){
			if(errno == ENOENT){
				printf("\n- You must specify a file that doesn't exist yet\n");
				isValid = 0;
			}
			else{

			}
		}

		isValid = 0;
	}
	return validFile;
}
