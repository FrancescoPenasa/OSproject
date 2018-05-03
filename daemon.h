#include "mytime.h"

#include <sys/ipc.h>
#include<signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define START 0
#define END 1

void format(){

}
/*
void output_writer(char *output, int type){
    if (type==START){
	FILE *fp;  				    //open stream
	fp = fopen(output,"a");		//stream bind to file, and access mode (read,write)
	fprintf(fp,"\n\n\t");       //format type			
    fprintf(fp,"%s", stringa);			
	fclose(fp);					//close stream
	return(0);
    }

}
*/


void sighandle_int1(int sig) {
    time_t current_time;
    char *c_time_string;
    printf("sig: %d\n",sig);
        current_time = time_w(NULL);              // start time	
        c_time_string = ctime_w(&current_time);   // start time in readable format
        printf("Signa11l... ");            /*output func*/
         printf("Current time is %s\n   ", c_time_string);
        fflush(stdout);
    signal(SIGUSR1, sighandle_int1);
}

void sighandle_int2(int sig) {
    time_t current_time;
    char *c_time_string;
    printf("sig: %d\n",sig);

    current_time = time_w(NULL);              // start time	
	c_time_string = ctime_w(&current_time);   // start time in readable format
    printf("Signal.2.. ");            /*output func*/
     printf("Current time is %s\n", c_time_string);
    fflush(stdout);
//        printf("Error: wrong signal code\n");
    signal(SIGUSR2, sighandle_int2);
}

void figlio() { /*associo il signal*/
    signal(SIGUSR1, sighandle_int1);
    signal(SIGUSR2, sighandle_int2);
};