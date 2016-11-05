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
} regexVerifier;

#endif
