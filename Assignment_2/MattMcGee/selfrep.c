#include<stdio.h>

char c[300] = "int main()\n{\n  FILE *file;\n\n  file = fopen( \"repeater.c\", \"w\" );\n\n  fprintf( file, \"#include<stdio.h>\" );\n\n  fprintf( file,  \"char c[300] =\" );\n\n  int i;\n\n  for( i = 0; i < 300; i++ )\n      {\n	fprintf( file, \"%d\", c[i] );\n      }\n\n  fprintf( file, \";\" );\n  fprintf( file, \"%s\", c );\n\n  return 0;\n}";

int main()
{
  FILE *file;

  file = fopen( "repeater.c", "w" );
  
  fprintf( file, "#include<stdio.h>\n\n" );
  fprintf( file,  "char c[300] =" );

  int i;

  fprintf(file,  "\"%s\"", c );                                             //Prints program with actual quotation marks and newlines in place - Doesn't repeat correctly as initialization of char string.
  /* for( i = 0; i < 300; i++ ) */                                      //Prints ASCII values for each element in char string in repeated code.
  /*     { */
  /* 	fprintf( file, "%d", c[i] ); */
  /*     } */
  
  fprintf( file, ";" );
  fprintf( file, "%s", c );

  return 0;
}
