/**
 * CSCD 437
 * Fall 2016
 * Assignment: Defend Your Code
 *
 * Date: November 4, 2016
 *
 * Team : The Wise Guys
 *    Matt Mcgee
 *    Heath Knickerbocker
 *    Michael Peterson
 *
 */
#include "assignment6.h"

#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "md5.h"

int main() {
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
	fprintf(stderr, "\nThis program is only designed to run in Linux\n");
	exit(1);
#endif
#ifdef _WIN64
	fprintf(stderr, "\nThis program is only designed to run in Linux\n");
	exit(1);
#endif

	// input buffers
	char firstName[IN_BUFF_LENGTH], lastName[IN_BUFF_LENGTH];
	char inFilename[IN_BUFF_LENGTH], outFilename[IN_BUFF_LENGTH];
	char intA[NUMBER_BUFF_LENGTH], intB[NUMBER_BUFF_LENGTH];

	// file pointers
	FILE *inputFile = NULL, *outputFile = NULL, *logFile = NULL;

	// regexes
	char *nameRegex                          = "^[a-zA-Z]{1,}$";
	char *numberRegex                        = "^(()|\\+|\\-)[0-9]{1,10}$";
	char *filenameRegex_InCurrentDir         = "^(\\.\\/|[^\\/])[a-zA-Z0-9\\s]+(\\.[a-zA-Z]{1,4})$";
	char *filenameRegex_HasAcceptedExtension = "\\.(text|txt)$"; // White list of valid extensions, we need more ideas ...
	char *filenameRegex_HasRelativePath      = "\\.\\.";
	char *passwordRegex_hasLowerCase         = "[a-z]+";
	char *passwordRegex_hasUpperCase         = "[A-Z]+";
	char *passwordRegex_hasDigit             = "[0-9]+";
	char *passwordRegex_hasPunct             = "[`~!@#$%^&*()_+=;\\.]+";


#ifdef TEST
	// Regex test cases
	char * nameTestCases[]     = {"", "\0", "\n", "Jhon", " Jhon", "1ll124lk1 1 4l ", "bob", "a", "13", "bob ", "0", "..", "Jo hn", 0};
	char * numberTestCases[]   = {"", "\0", "\n", "0", " 0", "0 ", " 0 ", "-0", "+0", "123", "-123", "+123", "- 123", "123-", "+0123546789", "+01235467891", " ", "apple", "abc", "-12345678901", "--12345678", "-+34567890", "- 34567890", "123456789-","123456789 +", "123456789++", "-123456789-", "(123*456)", "0000000000", "-0000000000", "*1234567890", "1-23456789", "--", "--0", "0000000000000001", 0};
	char * filenameTestCases[] = {"", "\0", "\n", "file.dog", "file.Txt", "file.txt", "./file/../.txt", "/file.txt", "./file.txt", "../file.txt", "/../file.txt", "./ /", "\\/bin\\/file.txt", "*.txt", "file.*",  0};
	char * passwordTestCases[] = {"", "\0", "\n", "abcde", "Hello there.8", "Slg3k4k23j4Dkj4k23j4kn234jh", ";2l23k23l SFLk#lk429' saf\nsf3 ", "abcDEF123@#$abc", "!@12asAS90123456fifty-one-charachers789012345678901","abCD$%789012345fifty-characters2345678901234567890", 0};

	runRegexTestCases(nameRegex                         , nameTestCases);
	runRegexTestCases(numberRegex                       , numberTestCases);
	runRegexTestCases(filenameRegex_InCurrentDir        , filenameTestCases);
	runRegexTestCases(filenameRegex_HasAcceptedExtension, filenameTestCases);
	runRegexTestCases(filenameRegex_HasRelativePath     , filenameTestCases);
	runRegexTestCases(passwordRegex_hasLowerCase        , passwordTestCases);
	runRegexTestCases(passwordRegex_hasUpperCase        , passwordTestCases);
	runRegexTestCases(passwordRegex_hasDigit            , passwordTestCases);
	runRegexTestCases(passwordRegex_hasPunct            , passwordTestCases);

#endif

	regexValidator emptyValidator = {NULL, 0, NULL};

	// Try to open a log file for output
	errno = 0;
	if(!(logFile = fopen("error.log", "w"))){
		fprintf(stderr, "\n%s\n", strerror(errno));
		exit(1);
	}

	/**
	 * 1) get first and last name from user
	 */
	regexValidator nameValidators[] = {{nameRegex, 1, "You must enter only letters"}, emptyValidator};
	getValidatedString("Enter your first name: ", firstName, IN_BUFF_LENGTH, nameValidators);
	getValidatedString("Enter your last name: " , lastName,  IN_BUFF_LENGTH, nameValidators);

	/**
	 * 2) get 2 32 bit ints from user
	 */
	regexValidator numberValidators[] = {{numberRegex, 1, "you may enter up to ten digits which may optionally be precedded by \"+\" or \"-\""}, emptyValidator};
	long long integerA = getVerifiedInteger("Enter the first 32 bit integer: " ,intA, NUMBER_BUFF_LENGTH, numberValidators);
	long long integerB = getVerifiedInteger("Enter the second 32 bit integer: ",intB, NUMBER_BUFF_LENGTH, numberValidators);
	long long addResult  = integerA + integerB;
	long long multResult = integerA * integerB;

	/**
	 * 3) get input/output filename from user
	 */
	regexValidator filenameValidators[] = {
			{filenameRegex_HasAcceptedExtension, 1, "only files with the extensions .txt and .text are allowed"},
			{filenameRegex_HasRelativePath,      0, "you are not allowed to use relative paths"},
			{filenameRegex_InCurrentDir,         1, "you may only specify files in the current directory"},
			emptyValidator
	};
	inputFile  = getValidFile("Enter an input file path from the current directory" , inFilename , IN_BUFF_LENGTH, filenameValidators, INPUT_FILE);
	outputFile = getValidFile("Enter an output file path from the current directory", outFilename, IN_BUFF_LENGTH, filenameValidators, OUTPUT_FILE);
	writeOutputFile(firstName, lastName, addResult, multResult, inputFile, outputFile, logFile);

	/**
	 * 4) get password from user
	 */
	regexValidator passwordValidators[] = {
			{passwordRegex_hasLowerCase, 1, "You must enter at least one lower case letter."},
			{passwordRegex_hasUpperCase, 1, "You must enter at least one upper case letter"},
			{passwordRegex_hasDigit,     1, "You must enter a digit"},
			{passwordRegex_hasPunct,     1, "You must enter at least on punctuation character"},
			emptyValidator
	};

	doPasswordThing("Enter a password of 12-56 characters, with at least one capital, one lower case, one digit, and one special ( `~!@#$%^&*()_+= ) symbol: ", passwordValidators);

	fclose(inputFile);
	fclose(outputFile);
	fclose(logFile);

	return 0;
}// End Main


/**************************************************************
*                                                            *
*                      HELPER FUNCTIONS                      *
*                                                            *
*      |        |        |        |        |        |        *
*      v        v        v        v        v        v        *
**************************************************************/


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
		printf("%8s    |    \"%s\"\n", (havePass) ? "YES" : "No", thisTestCase);
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

void getValidatedString(char *prompt, char *inputBuffer, int inputBufferSize, regexValidator validators[]) {
	assert (prompt != NULL && inputBuffer != NULL && inputBufferSize != 0 && validators != NULL);

	int isValid = 0;

	while (!isValid) {
		printf("\n%s\n>", prompt);
		int readResult = readInput(inputBuffer, inputBufferSize);
		if (readResult == READ_EXCEEDED_BUFFER) {
			printf("\nYou must enter %d or fewer characters. Try again:\n", inputBufferSize - 1 );
			isValid = 0;
		} else if (readResult == READ_ZERO_BYTES) {
			printf("\nYou must enter something...\n");
			isValid = 0;
		} else {
			int validatorIndex = 0;
			int failCount = 0;
			regexValidator thisValidator;
			while ((thisValidator = validators[validatorIndex++]).regex != NULL) {
				if (thisValidator.shouldMatch != isRegexMatch(thisValidator.regex, inputBuffer)) {
					failCount++;
					isValid = 0;
					printf("\n- %s\n", thisValidator.failDescription);
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

long long getVerifiedInteger(char *prompt, char *inputBuffer, int inputBufferSize, regexValidator validators[]){
	int isValid = 0;
	long long returnValue = 0;
	while(!isValid){
		getValidatedString(prompt, inputBuffer, inputBufferSize, validators);
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

FILE* getValidFile(char *prompt, char *inputBuffer, int inputBufferSize, regexValidator validators[], int fileType){
	assert(fileType == INPUT_FILE || fileType == OUTPUT_FILE);

	FILE* validFile = NULL;

	int isValid = 0;
	while(!isValid){
		getValidatedString(prompt, inputBuffer, inputBufferSize, validators);

		errno = 0;
		struct stat thisStat;
		int statReturnValue = lstat(inputBuffer, &thisStat);

		isValid = 1;
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
				if(ownerId != (int)getuid()){
					printf("\n- You must specify a file that you own\n");
					isValid = 0;
				}

				// Make sure the owner has read privileges
				if(!(mode & S_IRUSR)){
					printf("\n- You must specify a file that you have read privileges to\n");
					isValid = 0;
				}

				errno = 0;
				// Try opening the file, complain if something goes wrong
				if(isValid && !(validFile = fopen(inputBuffer, "r"))){
					printf("\n- %s\n", strerror(errno));
					isValid = 0;
				}
			}// End else
		}// End if(fileType == INPUT_FILE)
		else if(fileType == OUTPUT_FILE){
			if(errno != ENOENT){
				printf("\n- You must specify a file that doesn't exist yet\n");
				isValid = 0;
			}
			else{

				errno = 0;
				// Try opening a new file for output, if something goes wrong complain
				if(!(validFile = fopen(inputBuffer, "w"))){
					printf("\n- %s\n", strerror(errno));
					isValid = 0;
				}
			}// End else
		}// End if(fileType == OUTPUT_FILE)
	}// End while(!isValid)

	return validFile;
}

void writeOutputFile(char* firstName, char* lastName, long long addResult, long long multResult, FILE* inputFile, FILE* outputFile, FILE* logFile){
	char multResultString[21];
	char addResultString[12];
	char outputFileHeader[IN_BUFF_LENGTH*2 + sizeof(multResultString) + sizeof(addResultString) + 4];

	snprintf(multResultString, sizeof(multResultString), "%lld", multResult);
	snprintf(addResultString , sizeof(addResultString) , "%lld", addResult);
	snprintf(outputFileHeader, sizeof(outputFileHeader), "%s\n%s\n%s\n%s\n", firstName, lastName, addResultString, multResultString);

	char thisChar;
	int headerIndex = 0;
	errno = 0;
	while((thisChar = outputFileHeader[headerIndex++]) != '\0') {
		putc(thisChar, outputFile);
		if (errno) {
			fprintf(stderr, "\n%s\n", strerror(errno));
			fclose(inputFile);
			fclose(outputFile);
			exit(1);
		}
	}

	while(!feof(inputFile)){
		errno = 0;
		int thisChar = fgetc(inputFile);
		if(feof(inputFile))
			break;
		if(errno){
			char* errorOutput = strerror(errno);
			printf("\n%s\n",errorOutput);
			fprintf(logFile, "%s\n",errorOutput);
			fclose(outputFile);
			fclose(inputFile);
			exit(1);
		}
		else{
			errno = 0;
			putc(thisChar, outputFile);
			if(errno){
				char* errorOutput = strerror(errno);
				printf("\n%s\n",errorOutput);
				fprintf(logFile, "%s\n",errorOutput);
				fclose(inputFile);
				fclose(outputFile);
				exit(1);
			}
		}
	}

}

void doPasswordThing(char* prompt, regexValidator validators[]){
	char password[64];
	char passwordGuess[sizeof(password)];
	unsigned int originalHash[4];
	unsigned int guessHash[4];
	char salt[8];

	// generate a random salt
	srand(clock());
	int i;
	for(i = 0; i < (int)sizeof(salt); ++i)
		salt[i] = (char)((rand() % (UCHAR_MAX - 1)) +1);

	// get the password from the user
	getValidatedString(prompt,password, sizeof(password) - 8, validators);
	int passwordLength = strlen((char*)password);

	// Append the salt to the end of the string and fill the rest with '\0's
	int saltIndex = 0;
	for(i = passwordLength; i < passwordLength + (int)sizeof(salt); ++i)
		password[i] = salt[saltIndex++];
	for(; i < (int)sizeof(password); ++i)
		password[i] = '\0';

	// Generate the md5 hash
	md5_vfy((unsigned char*)password, sizeof(password), originalHash, originalHash+1, originalHash+2, originalHash+3);

	// Empty the original password
	for(i = 0; i < (int)sizeof(password); ++i)
		password[i] = '\0';

	int foundPassword = 0;
	while(!foundPassword){
		foundPassword = 1;

		// Get the password guess from the user
		getValidatedString("Enter your password guess", passwordGuess, sizeof(passwordGuess), validators);
		int passwordGuestLength = strlen((char*)passwordGuess);

		// Append the salt to the end of the string and fill the rest with '\0's
		saltIndex = 0;
		for(i = passwordLength; i < passwordGuestLength + (int)sizeof(salt); ++i)
			passwordGuess[i] = salt[saltIndex++];
		for(; i < (int)sizeof(password); ++i)
			passwordGuess[i] = '\0';

		// Hash the password
		md5_vfy((unsigned char*)passwordGuess, sizeof(password), guessHash, guessHash+1, guessHash+2, guessHash+3);

		for(i = 0; i < 4; ++i)
			if(originalHash[i] != guessHash[i])
				foundPassword = 0;

		if(foundPassword){
			printf("\ncongratulations, you found the password!!\n\n");
			break;
		}
		else
			printf("\n- That was the wrong password\n");
	}

}
