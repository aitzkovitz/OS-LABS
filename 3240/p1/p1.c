// Aaron Itzkovitz
// Dr. Butler
// CSCI 3240
// 2/8/18
// The goal of this project is to replicate the basic functionality of the XXD utility.
#include <fcntl.h> 
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[] ){

	unsigned char holder[16];				// buffer to hold current line
	int i;
	int fileLocation = 0;				// starting at 0
	int fd = open(argv[1],O_RDONLY,"r");
	int curLine = read(fd,holder,16); 		// read 16 bytes at a time
					
	while ( curLine != 0){				// EOF returns 0
		printf("%07x: ",fileLocation );		// print location with padding
		for(i = 0; i < curLine; i++ ){		// for each char on that line
			printf("%02x", holder[i]);
			if (i % 2 != 0){
				printf(" ");
			}
		}		

		while ( i < 16 ){
			printf("  ");
			if (i % 2 != 0){
				printf(" ");
			}
			i++;	
		}
		printf(" ");
		for (i = 0; i < curLine; i++){
			if (isprint(holder[i])){
				printf("%c",holder[i]);
			} else {
				printf(".");
			}

		}	
		printf("\n");
		curLine = read(fd,holder,16);		
		fileLocation += 16;			// after each iteration, increment file location	
	}
	return 0;
}
