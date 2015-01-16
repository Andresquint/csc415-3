#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(){
	#define NAME "Nao"

	HANDLE hfile = GetStdHandle(STD_OUTPUT_HANDLE);

	char str[30];

	sprintf(str, "Hello %s, welcome to 415!\n", NAME);

	LPDWORD numWritten = 0;

	WriteFile(hfile,str,26,numWritten,NULL);

	return 0;
}