#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/**Usage: 
 *time_w() to get current time. 
 *ctime_w(time_t &time) to get a readable date
 *difftime_w (time_t end,time_t beginning) to get the difference between two times.
*/

//wrapper of time(NULL)
time_t time_w(){    
    time_t current_time;    
    current_time = time(NULL);                 // current time
    if (current_time == ((time_t)-1)) {			
		fprintf(stderr, "Failure to obtain the current time.\n");
		exit(EXIT_FAILURE);						
	};
    return current_time; 
}

//verifica se "double difftime(end,begginning)" ha funzionato
double difftime_w (time_t end, time_t beginning){
  double difftime = time_w(NULL);   
  if (difftime == (-1)) {			
    fprintf(stderr, "Failure to obtain the difference time.\n");
    exit(EXIT_FAILURE);						
  };
  return difftime; 
}

//verifica se la conversione di "char *ctime(time_t time)" ha funzionato.
char *ctime_w(time_t *time){
    char *time_s;
    time_s = ctime(time);
    if (time_s == NULL) {		
		fprintf(stderr, "Failure to convert the time.\n");
		exit(EXIT_FAILURE);
	};
    return time_s;
}


#if DEBUG
//test case
int main (){ 
  time_t current_time, start, end;
  char *c_time_string;
  start = time_w();  
  printf("Start \t--> done\n");
  current_time = time_w();            // current time	
  c_time_string = ctime_w(&current_time); // current time in readable format
  printf("Current time is %s\n", c_time_string); // newline already set
  end = time_w();
  printf("End \t--> done\n");
  printf("Difftime between start and end is: %f\n", difftime_w(end,start));
  exit(EXIT_SUCCESS);
}
#endif

