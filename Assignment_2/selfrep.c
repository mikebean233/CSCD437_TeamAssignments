#include<stdio.h>

char c[] = "\n\nint main()\n{\n  FILE *file;\n\n  file = fopen( \"repeated.c\", \"w\" );\n\n  fprintf( file, \"#include<stdio.h>\" );\n\n  fprintf( file,  \"char c[] =\" );\n\n  int i;\n\n  for( i = 0; c[ i + 1 ]; i++ )\n      {\n	fprintf( file, \"%d, \", c[i] );\n      }\n\n  fprintf( file, \";\" );\n  fprintf( file, \"%s\", c );\n\n  return 0;\n}";

int main() // The problem with this repeating program is in the string above, in the first line, toward the right side, 
{                  // where the string reprints the line fprintf( file, "#include<stdio.h>\n\n" ).  If the newline characters are inserted in
  FILE *file;   // the string, the newlines (actual - not the symbol) appear in the code below, making the code uncompilable in the
  int i;          // second iteration. If the newliine characters are left out, there is no "return" (newline)
                   // after the #include<stdio.h> line at the top of the second iteration, making it uncompilable.
                   // I realize that the easy fix for this is to put the "#include" and the "char[c]" lines in the character array,
                   // but I was trying to remedy it in its current form, because the example code that we were given did it this way.
                   // Nothing to spend a lot of time on, but if you have any thoughts, I'd be interested.
                  // Note: for the second iteration, change the filename in the " file = fopen( "repeated.c", "w" );" to "repeated2.c", or
                  // something.
  file = fopen( "repeated.c", "w" );
  
  fprintf( file, "#include<stdio.h>\n\n" );
  fprintf( file,  "char c[] = {" );

  for( i = 0; c[ i + 1 ]; i++ )
      {
  	fprintf( file, "%d, ", c[i] ); // Saves individual chars to c array
      }
  
  fprintf( file, "};" );
  fprintf( file, "%s", c ); // Prints actual compilable code

  return 0;
}


