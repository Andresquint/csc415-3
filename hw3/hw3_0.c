#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main (){
	//loop until user exits
	while(1){
		//prompt user input
		printf("Myshell> ");

		//get input
		int buf_size = 2048;
		char buf[buf_size];
		char bufcopy[buf_size];

		fgets(buf, buf_size, stdin);
		strncpy(bufcopy, buf, buf_size);

		//parse and store strings and number of strings
		int myargc = 0;
		char *token;
		char del[2] = " \n";

		//get number of strings
		token = strtok(buf, del);
		while(token != NULL){
			myargc++;
			token = strtok(NULL, del);
		}

		if(myargc != 0){
			//store strings
			char *myargv[myargc + 1];
			int i = 0;
			myargv[i] = strtok(bufcopy, del);
			for(i = 1; i < myargc + 1; i++){
				myargv[i] = strtok(NULL, del);
			}
			//create new process
			pid_t child_pid;
			int status;

			child_pid = fork();
			if(child_pid == 0) {
				//child proccess
				if(execvp(myargv[0], myargv) < 0){
					if(strcmp(myargv[0], "exit") != 0){
						printf("Unknown command\n");
					}
					exit(0);
				}
			}else{
				//parent process wait for child process
				wait(&status);
			}


			if(strcmp(myargv[0], "exit") == 0){
				exit(0);
			}

		}
	}
	return 0;
}