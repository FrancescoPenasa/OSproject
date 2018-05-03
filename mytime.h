#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/*Usage: time_w() to get current time or ctime_w(&time) to get a readable date*/

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

//verifica se la conversione 1ha funzionato.
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
int testcase (){
    time_t current_time;
	char *c_time_string;
	current_time = time_w();            // current time	
	c_time_string = ctime_w(&current_time); // current time in readable format
	
    printf("Current time is %s", c_time_string); // newline already set
	exit(EXIT_SUCCESS);
}
#endif