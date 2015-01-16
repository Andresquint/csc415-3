#include <stdio.h>
#include <stdlib.h> //exit()
#include <pthread.h> //posix thread
#include <string.h>
#include <time.h> //clock()

#define BUFLEN 80000
#define NUM_THREAD 8

//Global variables
char buffer[BUFLEN + 1];
int wordcount;
pthread_mutex_t lock;

struct param
{
	int index;
	int start;
	int end;	
};

//thread
void wcount(struct param *arg);
//time
double sec(void);

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
		pthread_t tid[NUM_THREAD];
		pthread_attr_t attr;
		struct param p[NUM_THREAD];
		int part = BUFLEN/NUM_THREAD;
		int i, cut;

		pthread_attr_init(&attr);//get default attributes

		//initialize param
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

		//inititalize mutex
		if (pthread_mutex_init(&lock, NULL) != 0){
			printf("mutex init failed\n");
			return 1;
		}

		//measure time
		double t;
		t = sec();

		//create threads
		for(i = 0; i < NUM_THREAD; i++){
			pthread_create(&tid[i], &attr, (void*)wcount, &p[i]);
		}

		//bring everything together
		for(i = 0; i < NUM_THREAD; i++){
			pthread_join(tid[i], NULL);
		}

		t = sec() - t;

		//print wordcount
		printf("%d word(s): Runtime %f\n", wordcount, t);

		pthread_mutex_destroy(&lock);
		fclose(fp);
	}
	
	return 0;
}

double sec(void){
	return (double)clock()/CLOCKS_PER_SEC;
}

//count words in buffer
void wcount(struct param *arg){
	pthread_mutex_lock(&lock);
	int i;

	//count for the first word
	if(((buffer[arg->start]) >='a' && (buffer[arg->start]) <= 'z') || 
		((buffer[arg->start]) >='A' && (buffer[arg->start]) <= 'Z') ||
		((buffer[arg->start]) >='0' && (buffer[arg->start]) <= '9')){
		wordcount++;
	}

	//count up when you see a character after space or newline
	for(i = arg->start; i < (arg->end); i++){
		if(buffer[i] == ' ' || buffer[i] == '\n'){
			if(((buffer[i + 1]) >='a' && (buffer[i + 1]) <= 'z') || 
				((buffer[i + 1]) >='A' && (buffer[i + 1]) <= 'Z') ||
				((buffer[i + 1]) >='0' && (buffer[i + 1]) <= '9')){
				wordcount++;
			}
		}
	}

	pthread_mutex_unlock(&lock);
	
	pthread_exit(0);
}
