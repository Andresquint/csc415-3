#include <stdio.h>
#include <stdlib.h> //exit()
#include <time.h> //time()

#define STRLEN 100

int main (){
	//initialize variables
	int referenceString[STRLEN];
	int random, i, numFrames, pageFault, skip, counter, lru;
	int *frame, *counters;

	srand(time(NULL));

	//make reference string randomly from 1-16
	//without same consecutive numbers
	random =  1 + (rand() % 16);
	referenceString[0] = random;
	for(i = 1; i < STRLEN; i++){
		while(referenceString[i-1] == random){
			random =  1 + (rand() % 16);
		}
		referenceString[i] = random;
	}

	//run simulation for 1-16 frames
	for(numFrames = 1; numFrames <= 16; numFrames++){
		//make frame and initialize to 0
		frame = (int*)malloc(sizeof(int) * numFrames);
		for(i = 0; i < numFrames; i++){
			frame[i] = 0;
		}

		//run FIFO
		pageFault = 0;
		for(counter = 0; counter < STRLEN; counter++){
			//check if page number is in frame
			skip = 0;
			for(i = 0; i < numFrames; i++){
				if(referenceString[counter] == frame[i]){
					skip = 1;
				}
			}
			//put page num in frame
			if(!skip){
				for(i = (numFrames - 1); i > 0; i--){
					frame[i] = frame[i - 1];
				}
				frame[0] = referenceString[counter];
				pageFault++;
			}
		}

		printf("Frame[%d]FIFO:%d, ", numFrames, pageFault);

		//run LRU
		//allocate memory for counters
		counters = (int*)malloc(sizeof(int) * numFrames);
		//initialize frames to 0 and counters to -1
		for(i = 0; i < numFrames; i++){
			frame[i] = 0;
			counters[i] = -1;
		}

		//check if existing in frame
		pageFault = 0;
		for(counter = 0; counter < STRLEN; counter++){
			//check if page number is in frame
			//if it is, update counter in counters
			skip = 0;
			for(i = 0; i < numFrames; i++){
				if(referenceString[counter] == frame[i]){
					skip = 1;
					counters[i] = counter;
				}
			}
			//put page num in frame 
			if(!skip){
				//get last recently used frame
				lru = 0;
				for(i = 0; i < numFrames; i++){
					if(counters[lru] > counters[i]){
						lru = i;
					}
				}
				frame[lru] = referenceString[counter];
				counters[lru] = counter;
				pageFault++;
			}
		}
		printf("LRU:%d\n", pageFault);

		free(counters);
		free(frame);
	}

	return 0;
}