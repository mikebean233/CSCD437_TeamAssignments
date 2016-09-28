char source[] = "#include<stdio.h>\n\nint main(){\n\tint i;\n\tprintf(\"char source[] = {\");\n\tfor(i = 0; source[i + 1]; i++)\n\t\tprintf(\"%d,\", source[i]);\n\tprintf(\"%d,0};\\n%s\", source[i], source);\n\treturn 0;\n}\n";
#include<stdio.h>

int main(){
	int i;
	printf("char source[] = {");
	for(i = 0; source[i + 1]; i++)
		printf("%d,", source[i]);
	printf("%d,0};\n%s", source[i], source);
	return 0;
}
