#include <stdio.h>
#include <stdlib.h> //exit()
#include <pthread.h> //posix thread
#include <string.h>
#include <time.h> //clock()
#include <dispatch/dispatch.h> //semaphores for osx
//#include <sys/types.h> //sem_t
//#include <semaphore.h> //sem_wait(), sem_post()

#define NUM_THREAD 8

//Global variables
int * buffer;
int bufindex;
int slots;
int itemsPerProducer;
int itemsConsumed;
pthread_mutex_t lock;
dispatch_semaphore_t full;
dispatch_semaphore_t empty;
//sem_t full;
//sem_t empty;

//thread
void producer(int * id);
void consumer();
//time
double sec(void);

int main (int argc, char *argv[]){
	//parse comand-line parameters
	if (argc == 5) {
		int numProducer, numConsumer;

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
		full = dispatch_semaphore_create(0);
		empty = dispatch_semaphore_create(slots);
		//sem_init(&full, 0, 0);
		//sem_init(&empty, 0, slots);

		if (pthread_mutex_init(&lock, NULL) != 0){
			printf("mutex init failed\n");
			return 1;
		}

		//spawn threads
		pthread_t * tid;
		tid = (pthread_t*)malloc(sizeof(pthread_t) * (numProducer + numConsumer));
		pthread_attr_t attr;

		pthread_attr_init(&attr);//get default attributes

		//measure time
		double t;
		t = sec();

		//create threads
		int id[numProducer];
		int i, j;
		for(i = 0; i < numProducer; i++){
			id[i] = i;
			pthread_create(&tid[i], &attr, (void*)producer, &id[i]);
		}
		for(j = 0; j < numConsumer; j++){
			pthread_create(&tid[i++], &attr, (void*)consumer, NULL);
		}

		//bring everything together
		for(i = 0; i < (numProducer + numConsumer); i++){
			pthread_join(tid[i], NULL);
		}

		t = sec() - t;

		//print final message
		printf("End of program: Runtime %f\n", t);

		//destroy semaphores and mutex and free space
		dispatch_release(full);
		dispatch_release(empty);
		//sem_destroy(&full);
		//sem_destroy(&empty);
		pthread_mutex_destroy(&lock);
		free(buffer);
		free(tid);

	}else{
		printf("Please input 4 arguments.\n");
	}
	
	return 0;
}

double sec(void){
	return (double)clock()/CLOCKS_PER_SEC;
}

//producer produces products
void producer(int * id){
	int counter = 0, product, i;
	//make items
	while(counter < itemsPerProducer){
		dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
		//sem_wait(&empty);
		pthread_mutex_lock(&lock);

		//produce item
		product = (counter * NUM_THREAD) + *id;
		//print item produced
		printf("Produced:%d\n", product);

		//store in buffer
		buffer[bufindex++] = product;
		counter++;

		pthread_mutex_unlock(&lock);
		dispatch_semaphore_signal(full);
		//sem_post(&full);
	}

	pthread_exit(0);
}

//consumer consumes products
void consumer(){
	int i = 0;
	while(i < itemsConsumed){
		dispatch_semaphore_wait(full, DISPATCH_TIME_FOREVER);
		//sem_wait(&full);
		pthread_mutex_lock(&lock);

		//consume things in slot
		//print item consumed
		bufindex--;
		printf("Consumed:%d\n", buffer[bufindex]);
		i++;
		
		pthread_mutex_unlock(&lock);
		dispatch_semaphore_signal(empty);
		//sem_post(&empty);
	}
	
	pthread_exit(0);
}
