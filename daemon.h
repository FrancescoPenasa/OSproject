#include "mytime.h" //time_w(); ctime_w(time_t &time); difftime_w(time_t end, time_t beginning);
#include <sys/ipc.h>
#include <signal.h>
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

time_t start_clock, stop_clock;

void sighandle_int(int sig) {
  /*END OF THE EXECUTION*/
  if (sig == SIGUSR2){                              
    stop_clock = time_w(); //stop counting
    
    time_t current_time;
    char *c_time_string;
    current_time = time_w(NULL);              // start time	
    c_time_string = ctime_w(&current_time);   // start time in readable format

    printf("\n");
    printf("---\n");   
    printf("sig: %d\n",sig);
    printf("Signal.2.. ");            /*output func*/
    printf("Current time is %s\n", c_time_string);
    printf("START time is %li\n", start_clock);
    printf("STOP time is %li\n", stop_clock);
    printf("Diff time is %f\n", difftime_w(stop_clock, start_clock));    
    printf("---\n");
    printf("\n");
    fflush(stdout);
    //        printf("Error: wrong signal code\n");
  }

  /*START OF THE EXECUTION*/
  else if (sig == SIGUSR1){
    time_t current_time;
    char *c_time_string;
    current_time = time_w(NULL);              // start time	
    c_time_string = ctime_w(&current_time);   // start time in readable format

    
    printf("\n");
    printf("---\n");
    printf("sig: %d\n",sig);
    printf("Signa11l... ");            /*output func*/
    printf("Current time is %s\n", c_time_string);
    printf("---\n");
    printf("\n");
    fflush(stdout);

    start_clock = time_w(); //start counting   ///potrebbe andare più giù
  }
  else
    printf("Error: Signal: %d not handled by this function",sig);
  
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
}


void figlio() { /*associo il signal*/
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
};
