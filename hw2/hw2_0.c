#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main ( int argc, char *argv[] ){
	if(argc != 3)//argc should be 3 for correct execution
	{
		printf("Error: must have 2 file names\n");
	}
	else
	{
		//Get input and output file name
		char *inFileName = argv[1];
		char *outFileName = argv[2];
		errno = 0;

		//check for input file existence
		int check;
		check = access (inFileName, F_OK); 
		int inFile;
		if ( check == 0 )
		{//open file
   			inFile = open(inFileName, O_RDONLY, NULL);
   			if(inFile == -1)
   			{
   				printf("Error in opening file: %s\n", strerror(errno));
   				exit(0);
   			}
		}
		else
		{//if file doesn't exist, abort
   			printf("Error: %s doesn't exist\n",inFileName);
   			exit(0);
		}


		//check for output file existence
		check = access(outFileName, F_OK);
		int outFile;
		if (check == 0)
		{//if file exists, abort
			printf("Error: File %s already exists\n", outFileName);
			exit(0);
		}
		else
		{//Create output file
   			outFile = open(outFileName, O_CREAT|O_WRONLY|O_APPEND, S_IRWXU);
   			if(outFile == -1)
   			{
   				printf("Error in creating output file: %s\n", strerror(errno));
   				exit(0);
   			}
		}
		
		int bytes = 0;
		check = 1;
		char str[256];
		//loop until check is at 0(end of file)
		while(check > 0)
		{
			//Read from input file
			check = read(inFile, str, 256);
			if(check == -1)
			{
				printf("Error during reading a file\n");
				exit(0);
			}
			else if(check == 0)
			{
				break;
			}

			//Write to output file
			int num = 0;
			num = write(outFile, str, check);
			if(num == -1)
			{
				printf("Error during writing a file: %s\n", strerror(errno));
				exit(0);
			}
			bytes += num;

		}
		//Close files
		close(inFile);
		close(outFile);
		//Print completion message to screen
		printf("Copied %d bytes\n", bytes);

	}
	
	return 0;
}