#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	#define NAME "Nao"

	char str[30];

	sprintf(str, "Hello %s, welcome to 415!\n", NAME);

	write(1, str, 30);

	return 0;
}