#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){
	FILE *fp;
	int size;
	char *output = argv[1];
	char *stringa = "daaadddadadaaaaaaaaassss11a";
	fp = fopen(argv[1], "w+");		
	fprintf(fp,"\n\n\t");       //format type			
	fprintf(fp,"%s", stringa);			
	fclose(fp);					//close stream
	return(0);
}
