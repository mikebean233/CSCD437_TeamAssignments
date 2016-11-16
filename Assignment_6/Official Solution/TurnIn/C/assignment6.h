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
#ifndef _ASSIGNMENT6_H
#define _ASSIGNMENT6_H

#define _GNU_SOURCE

#include <stdio.h>

#define IN_BUFF_LENGTH       51
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
} regexValidator;

int       isRegexMatch(      char *regex,       char *input);
int       runRegexTestCases( char *regex,       char *testCases[]);
void      getValidatedString(char *prompt,      char *inputBuffer,           int inputBufferSize, regexValidator validators[]);
int       readInput(         char *inputBuffer, int bufferLength);
long long getVerifiedInteger(char *prompt,      char *inputBuffer,           int inputBufferSize, regexValidator validators[]);
FILE*     getValidFile(      char *prompt,      char *inputBuffer,           int inputBufferSize, regexValidator validators[], int fileType);
void      writeOutputFile(   char* firstName,   char* lastName,              long long addResult, long long multResult,        FILE* inputFile, FILE* outputFile, FILE* logFile);
void      doPasswordThing(   char* prompt,      regexValidator validators[]);

#endif
