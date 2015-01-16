#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main ( int argc, char *argv[] )
{
	if(argc != 3)//argc should be 3 for correct execution
	{
		printf("Error: must have 2 file names\n");
	}
	else
	{
		//Get input and output file name
		char *inFileName = argv[1];
		char *outFileName = argv[2];
		HANDLE inFile;
		HANDLE outFile;

		//Open file if file exits
   		inFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL); 
   		if(inFile == INVALID_HANDLE_VALUE)
   		{
   			printf("Error in opening file\n");
   			exit(0);
   		}

		//Create output file in file doesn't exist
   		outFile = CreateFile(argv[2], GENERIC_WRITE, 0, NULL, CREATE_NEW,
   			FILE_ATTRIBUTE_NORMAL, NULL);
   		if(outFile == INVALID_HANDLE_VALUE)
   		{
   			printf("Error in creating output file\n");
   			exit(0);
   		}
		

		DWORD bytesRead = 256;
		DWORD bytesWrittten = 0;
		int check = 1;
		int num = 0;
		int bytes = 0;
		char str[256];

		
		//loop until end of file
		while(bytesRead == 256)
		{
			//Read from input file
			check = ReadFile(inFile, str, 256, &bytesRead, NULL);
			if(check == 0)
			{
				printf("Error during reading a file\n");
				exit(0);
			}

			//Write to output file
			num = WriteFile(outFile, str, bytesRead, &bytesWrittten, NULL);
			if(num == 0)
			{
				printf("Error during writing a file\n");
				exit(0);
			}
			bytes += bytesWrittten;

		}
	
		//Close files
		CloseHandle(inFile);
		CloseHandle(outFile);
		//Print completion message to screen
		printf("Copied %d bytes\n", bytes);

	}
	
	return 0;
}