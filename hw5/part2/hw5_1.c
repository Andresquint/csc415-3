#include <stdio.h>
#include <stdlib.h> //exit()
#include <windows.h> //win32 thread
#include <string.h>

#define NUM_THREAD 8

//Global variables
int * buffer;
int bufindex;
int slots;
int itemsPerProducer;
int itemsConsumed;
HANDLE mutex;
HANDLE full;
HANDLE empty;

//thread
void producer(int * id);
void consumer();

int main (int argc, char *argv[]){
	//declare variables
	int numProducer, numConsumer;
	HANDLE * phandle;
	HANDLE * chandle;
	int * id;
	int i;
	
	//parse comand-line parameters
	if (argc == 5) {
		//print message
		printf("Number of slots: %s\n", argv[1]);
		printf("Number of producers: %s\n", argv[2]);
		printf("Number of consumers: %s\n", argv[3]);
		printf("Number of items: %s\n", argv[4]);

		//inititalize buffer and other values
		bufindex = 0;
		slots = atoi(argv[1]);
		numProducer = atoi(argv[2]);
		numConsumer = atoi(argv[3]);
		itemsPerProducer = atoi(argv[4]);
		itemsConsumed = (numProducer * itemsPerProducer / numConsumer);
		buffer = (int*)malloc(sizeof(int) * slots);

		//initialize semaphores and mutex
		full = CreateSemaphore(NULL, 0, slots, NULL);
		if(full == NULL){
			printf("CreateSemaphore error: %d\n", GetLastError());
			return 1;
		}
		empty = CreateSemaphore(NULL, slots, slots, NULL);
		if(empty == NULL){
			printf("CreateSemaphore error: %d\n", GetLastError());
			return 1;
		}

		mutex = CreateMutex(NULL, FALSE, NULL);
		if(mutex == NULL){
			printf("CreateMutex error: %d\n", GetLastError());
			return 1;
		}

		//spawn threads
		phandle = (HANDLE*)malloc(sizeof(HANDLE) * numProducer);
		chandle = (HANDLE*)malloc(sizeof(HANDLE) * numConsumer);
		id = (int*)malloc(sizeof(int) * numProducer);

		//create threads
		for(i = 0; i < numProducer; i++){
			id[i] = i;
			phandle[i] = CreateThread(NULL, 0, (void*)producer, &id[i], 0, NULL);
		}
		for(i = 0; i < numConsumer; i++){
			chandle[i] = CreateThread(NULL, 0, (void*)consumer, NULL, 0, NULL);
		}

		for (i = 0; i < numProducer; i++){
			WaitForSingleObject(phandle[i], INFINITE);
		}
		
		for (i = 0; i < numConsumer; i++){
			WaitForSingleObject(chandle[i], INFINITE);
		}

		//bring everything together
		for(i = 0; i < numProducer; i++){
			CloseHandle(phandle[i]);
		}
		
		for(i = 0; i < numConsumer; i++){
			CloseHandle(chandle[i]);
		}

		//print final message
		printf("End of program\n");

		//destroy semaphores and mutex and free space
		CloseHandle(full);
		CloseHandle(empty);
		CloseHandle(mutex);
		free(buffer);
		free(phandle);
		free(chandle);
		free(id);

	}else{
		printf("Please input 4 arguments.\n");
	}
	
	return 0;
}

//producer produces products
void producer(int * id){
	int counter = 0, product, i;
	//make items
	while(counter < itemsPerProducer){
		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		
		//produce item
		product = (counter * NUM_THREAD) + *id;
		//print item produced
		printf("Produced:%d\n", product);

		//store in buffer
		buffer[bufindex++] = product;
		counter++;
		
		ReleaseMutex(mutex);
		ReleaseSemaphore(full, 1, NULL);
	}

	return;
}

//consumer consumes products
void consumer(){
	int i = 0;
	while(i < itemsConsumed){
		WaitForSingleObject(full, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		
		//consume things in slot
		//print item consumed
		bufindex--;
		printf("Consumed:%d\n", buffer[bufindex]);
		i++;
		
		ReleaseMutex(mutex);
		ReleaseSemaphore(empty, 1, NULL);
	}

	return;
}