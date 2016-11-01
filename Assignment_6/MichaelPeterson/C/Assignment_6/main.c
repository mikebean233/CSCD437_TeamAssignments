#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>

#define IN_BUFF_LENGTH 51

int isRegexMatch(char* regex, char* input);
int runRegexTestCases(char* regex, char* testCases[]);

int main() {
    printf(__FILE__);
    // input buffers
    char *firstName[IN_BUFF_LENGTH], lastName[IN_BUFF_LENGTH];

    // regexes
    char* nameRegex     = "^[a-zA-Z]{1,}$";
    char* numberRegex   = "^(()|\\+|\\-)[0-9]{1,10}$";
    char* filenameRegex_InCurrentDir = "^(\\.\\/|[^\\/])[a-zA-Z0-9\\s]+(\\.[a-zA-Z]{1,4})$";
    char* filenameRegex_HasAcceptedExtension = "\\.(text|txt)$"; // White list of valid extensions, we need more ideas ...

    #ifdef TEST
    // Regex test cases
    char * nameTestCases[]     = {"", "\0", "\n", "", "Jhon", " Jhon", "1ll124lk1 1 4l ", "bob", "a", "13", "bob ", 0};
    char * numberTestCases[]   = {"", "\0", "\n", "0", " 0", "0 ", " 0 ", "-0", "+0", "123", "-123", "+123", "- 123", "123-", "+0123546789", "+01235467891", " ", "apple", 0};
    char * filenameTestCases[] = {"", "\0", "\n", "", "file.dog", "file.Txt", "file.txt", "./file/../.txt", "/file.txt", "./file.txt", "../file.txt", "/../file.txt", "./ /", 0};
    #endif

    /**
     * 1) get first and last name from user
     */
    #ifdef TEST
    runRegexTestCases(nameRegex, nameTestCases);
    #endif

    /**
     * 2) get 2 32 bit ints from user
     */
    #ifdef TEST
    runRegexTestCases(numberRegex, numberTestCases);
    #endif

    /**
     * 3) get input file name form user
     */
     #ifdef TEST
     runRegexTestCases(filenameRegex_InCurrentDir, filenameTestCases);
     runRegexTestCases(filenameRegex_HasAcceptedExtension, filenameTestCases);
     #endif

    return 0;
}



int runRegexTestCases(char* regex, char* testCases[]){
    #ifdef TEST
    printf("\n\n---- Test Cases for Regex: \"%s\" --------\n", regex);
    printf("  Is Match  |  Input  \n");
    printf("----------------------\n");
    #endif
    char* thisTestCase;
    int i = 0, passCount = 0, havePass = 0;
    while((thisTestCase = testCases[i++]) != 0){
        passCount += (havePass = isRegexMatch(regex, thisTestCase)) ? 1 : 0;
        #ifdef TEST
        printf("%s|\"%s\"\n", (havePass) ? "     Yes    " : "     No     ", thisTestCase);
        #endif
    }
    return passCount;
}

int isRegexMatch(char* regex, char* input){
    if(regex == NULL || input == NULL)
        return 0;
    char errorMessage[100];

    regex_t regext;
    int reti = regcomp(&regext, regex, REG_EXTENDED);
    if(reti){
        regerror(reti, &regext, errorMessage, sizeof(errorMessage));
        fprintf(stderr, "%s\n", errorMessage);
        exit(1);
    }

    reti = regexec(&regext, input, 0, NULL, 0);
    regfree(&regext);

    if(!reti){
        return 1;
    }
    else if(reti == REG_NOMATCH){
        return 0;
    }
    else{
        regerror(reti, &regext, errorMessage, sizeof(errorMessage));
        fprintf(stderr, "%s\n", errorMessage);
        return 0;
    }
}