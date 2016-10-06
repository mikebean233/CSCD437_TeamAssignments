#include<unistd.h>
//#include<conio.h>

void main(){

  char* args[] = {"C:\\Windows\\System32\\cmd.exe",  NULL};
  execv("C:\Windows\\System32\\cmd.exe", args);
}
