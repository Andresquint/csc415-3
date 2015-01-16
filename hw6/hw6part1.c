#include <stdio.h>
#include <stdlib.h> //exit()
#include <string.h> //strrev()
#include <math.h> //pow()

#define BUFSIZE 10

long hexToDeci(char *);
void deciToHex(long, char *);
void strrev(char *);

int main (int argc, char *argv[]){
	char hexBuf[BUFSIZE];
	long address, pageNum, offset;

	if(argc == 2){
		//get input address in hex and translate to decimal
		address = hexToDeci(argv[1]);
		//calculate page number
		pageNum = address / 8192;
		//translate to hex and print
		deciToHex(pageNum, hexBuf);
		printf("Page number: %s\n", hexBuf);

		//calculate offset
		if(pageNum != 0){
			offset = address % pageNum;
		}else{
			offset = address;
		}
		//translate to hex and print
		deciToHex(offset, hexBuf);
		printf("Offset: %s\n", hexBuf);
	}else{
		printf("Incorrect input.\n");
	}
	
	return 0;
}

long hexToDeci(char * hex){
	int i, length;
	long deci = 0;
	//determine length
	for(length = 0; hex[length] != '\0'; length++);
	//get rid of 0x in front and start from the 3rd char
	length = length - 2;
	for(i = 2; hex[i] != '\0'; i++, length--){
		if(hex[i] >= '0' && hex[i] <= '9'){
			deci += (hex[i] - 48) * pow(16, length - 1);
		}else if(hex[i] >= 'A' && hex[i] <= 'F'){
			deci += (hex[i] - 55) * pow(16, length - 1);
		}else if(hex[i] >= 'a' && hex[i] <= 'f'){
			deci += (hex[i] - 87) * pow(16, length - 1);
		}else{
			printf("Invalid digit in hex string: %c\n", hex[i]);
		}
	}

	return deci;
}

void deciToHex(long deci, char * hex){
	int i = 0, rem;
	if(deci == 0){
		hex[i++] = '0';
	}
	while (deci != 0){
		rem = deci % 16;
		switch(rem){
			case 10:
				hex[i]='A';
				break;
			case 11:
				hex[i]='B';
				break;
			case 12:
				hex[i]='C';
				break;
			case 13:
				hex[i]='D';
				break;
			case 14:
				hex[i]='E';
				break;
			case 15:
				hex[i]='F';
				break;
			default:
				hex[i] = rem + '0';
				break;
		}
		i++;
		deci /= 16;
	}
	hex[i++] = 'x';
	hex[i++] = '0'; 
	hex[i] = '\0';
	strrev(hex);   /* Reverse string */
	return;
}

void strrev(char * str){
	int length, i;
	char * begin, * end, temp;

	for(length = 0; str[length] != '\0'; length++);

	begin = str;
	end = str;

	for(i = 0; i < (length - 1); i++){
		end++;
	}

	for(i = 0; i < length/2; i++){
		temp = * end;
		* end = * begin;
		* begin = temp;

		begin++;
		end--;
	}
}