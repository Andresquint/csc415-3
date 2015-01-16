#include <stdio.h>
#include <stdlib.h> //exit()
#include <windows.h> //win32 thread
#include <string.h>

#define BUFLEN 80000
#define NUM_THREAD 8

//Global variables
char buffer[BUFLEN + 1];
int words[NUM_THREAD];

struct param
{
	int index;
	int start;
	int end;	
};

//thread
void wcount(struct param *arg);


int main (int argc, char *argv[]){
	//open and read file in buffer
	FILE *fp = fopen(argv[1], "r");
	if (fp != NULL) {
		size_t newLen = fread(buffer, sizeof(char), BUFLEN, fp);
		if (newLen == 0) {//file without input will produce an error
			fputs("Error reading file\n", stderr);
			exit(0);
		} else {
			buffer[++newLen] = '\0';
		}

		//spawn threads
		DWORD tid[NUM_THREAD];
		HANDLE thandle[NUM_THREAD];
		struct param p[NUM_THREAD];
		int part = BUFLEN/NUM_THREAD;
		int i, cut;

		//initialize tid and param
		for(i = 0; i < NUM_THREAD; i++){
			p[i].index = i;
			//partition buffer
			if(i == 0){
				p[i].start = 0;
			}else{
				p[i].start = (p[i - 1].end) + 1;
			}
			cut = (i + 1) * part;
			while(buffer[cut] != ' ' && buffer[cut] != '\n' && cut != (i + 2) * part){
				cut++;
			}
			p[i].end = cut;

		}

		//create threads
		for(i = 0; i < NUM_THREAD; i++){
			thandle[i] = CreateThread(NULL, 0, wcount, &p[i], 0, &tid[i]);
		}
		
		WaitForMultipleObjects(NUM_THREAD,thandle,TRUE,INFINITE);

		//bring everything together
		for(i = 0; i < NUM_THREAD; i++){
			CloseHandle(thandle[i]);
		}

		//print total
		int total = 0;
		for(i = 0; i < NUM_THREAD; i++){
			total += words[i];
		}
		printf("%d word(s)\n", total);

		fclose(fp);
	}
	
	return 0;
}

//count words in buffer
void wcount(struct param *arg){
	int count = 0;
	int i;

	//count for the first word
	if(((buffer[arg->start]) >='a' && (buffer[arg->start]) <= 'z') || 
		((buffer[arg->start]) >='A' && (buffer[arg->start]) <= 'Z') ||
		((buffer[arg->start]) >='0' && (buffer[arg->start]) <= '9')){
		count++;
	}

	//count up when you see a character after space or newline
	for(i = arg->start; i < (arg->end); i++){
		if(buffer[i] == ' ' || buffer[i] == '\n'){
			if(((buffer[i + 1]) >='a' && (buffer[i + 1]) <= 'z') || 
				((buffer[i + 1]) >='A' && (buffer[i + 1]) <= 'Z') ||
				((buffer[i + 1]) >='0' && (buffer[i + 1]) <= '9')){
				count++;
			}
		}
	}

	words[arg->index] = count;
	return;
}
