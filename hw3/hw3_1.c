#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

#define BUFFER_SIZE 2048

int main (){
	//loop until user exits
	while(1){
		//prompt user input
		printf("Myshell> ");

		//get input
		char buf[BUFFER_SIZE];
		char bufcopy[BUFFER_SIZE];
		char input[BUFFER_SIZE + 7];
		int size, i;

		fgets(buf, BUFFER_SIZE, stdin);
		strncpy(bufcopy, buf, BUFFER_SIZE);

		//parse and store first token of bufcopy
		char del[2] = " \n";
		char *token;

		token = strtok(bufcopy, del);
		
		if(strcmp(token, "exit") == 0){
			exit(0);
		}
		
		if(token != NULL){
			//fix buf into a input by adding prefix (cmd /c) and suffix (replace \n with \0)
			size = strlen(buf);
			buf[size - 1] = '\0';
			strcpy(input, "cmd /c ");
			strcat(input, buf);
			
			//create new process
			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			// Start the child process. 
			if(!CreateProcess(NULL, input, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
				printf("CreateProcess failed (%d).\n", GetLastError());
				return;
			}

			// Wait until child process exits
			WaitForSingleObject(pi.hProcess, INFINITE);

			// Close process and thread handles
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	return 0;
}