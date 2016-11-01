#define IN_LEN 51 // # define for name input
#define BUFF_SIZE 12
// All protection can be OS specific
// Check for OS? Windows / Linux
/*
#ifdef OS_WINDOWS
else define for Linux --------> error if anything else?
#endif
*/
// Control the name of the executable.

//1. User's first and last name
//JAVA:
// boolean canRead(filename) // True if application can write to file
// boolean canWrite(filename) // True if application can write to file
// public boolean exists() // Tests whether the file or directory denoted by this abstract pathname exists.
// Not sure this will be useful, but it's pretty cool:

/*
String[] split(String regex)
String[] split(String regex, int limit)
Searches for a match as specified by the string argument (which contains a regular expression) and splits this string into an array of strings accordingly. The optional integer argument specifies the maximum size of the returned array. Regular expressions are covered in the lesson titled "Regular Expressions."
 */

//limit size of array
// Regex to limit input to only chars and only 50

//2.
// input digits
// long long in C?
// same as long in java
// int64_t is an int type with 64 bits. 2's complement for negative values. No padding bits
// atoi() to convert to int.
//int64_t is a 64-bit signed int. Multiply int values together by storing 32-bit ints as int_64_t and storing in a int_64_t
//Store user's int in an array of 12: 1 for the sign, 10 for the digits, 1 for the terminating null character.
// Regex to scrub input: ^(|\-)\d{1,10}$
/*
char inBuff[BUFF_SIZE]
int i = 0;
/for(; i < BUFF_SIZE]; i++);
inBuff[i] = "\0"
  fgets( inBuff; BUFF_SIZE; stdin );
long long int val = strtoll(inBuff);
*/
// use errno in strtoll(): errno = 0; before function call. if( errno != 0 ) after call to strtoll() to see if an error has occurred.
// Complaing intelligently using error returned by errno

/*EINVAL (not in C99) The given base contains an unsupported value.

       ERANGE The resulting value was out of range.

       The  implementation  may also set errno to EINVAL in case no conversion
       was performed (no digits seen, and 0 returned).
*/
// 
//3.
// input file
// Confirm that what you're looking at is a file: fopen()
// Limit length of filename
// How to prevent bad filename? No writing to or from bin or root ----> don't allow "/" or ".." or "/./." or ":" in filename
// lstat() doesn't allow symbolic links
// Only files that current user with current priveledge can read from
// Limit the size of the input file.

//4.
// output file
// No writing to / or bin, etc. (see #3):
//fopen to identify as file -
//limit length of output?
//Scrub input file content, or escape it?
//lstat()
// Limit length of filename
//
// New:


//5.
//Password
// Salt and hash password
// Maximum length of 128 characters is common
// Escape characters in password?

//6a.
// Open output file: fopen()
// Only files with current user priveledge
//// lstat() doesn't allow symbolic links
// Limit size of output file
// How to prevent bad filename? No writing to or from bin or root ----> don't allow "/" or ".." or "/./." or ":" in filename

//6b.
// snprintf() prints to a format string, checking length. Be sure to use format string specifiers.

//6c.
// Should be fine, as long as integer input is handled correctly

//6d.
//Contents of the input file:
// Limit length
// Escape everything to render it non-executable?
// Validate input to avoid format string attack - https://www.owasp.org/index.php/Format_string_attack
