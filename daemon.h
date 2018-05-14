#include "mytime.h" //time_w(); ctime_w(time_t &time); difftime_w(time_t end, time_t beginning);
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h> //read write
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>  

#define START 0
#define END 1
#define MAXPID 17
extern int errno;

time_t start_clock, stop_clock;
char *output_format = "0";
char *output_file = "Log.txt";


void signal_handler();
void output_manager(){
  FILE *fp;
  fp = fopen("MYFILE.txt", "a");
  fprintf(fp, "%s\n ", "Hello World, Where there is will, there is a way.");
  fclose(fp) ;
}

/* prende il segnale di inizio e fine processo per registrare i dati */
void sighandle_int(int sig) {
  
  /*END OF THE EXECUTION*/
  if (sig == SIGUSR2){                              
    stop_clock = time_w(); //stop counting  
    
    time_t current_time;
    char *c_time_string;
    current_time = time_w(NULL);              // start time	
    c_time_string = ctime_w(&current_time);   // start time in readable format

    printf("\n\n ---- STOP  ---- \n\n");
    fflush(stdout);
    //        printf("Error: wrong signal code\n");
  }

  /*START OF THE EXECUTION*/
  else if (sig == SIGUSR1){
    time_t current_time;
    char *c_time_string;
    current_time = time_w(NULL);              // start time	
    c_time_string = ctime_w(&current_time);   // start time in readable format

    //output_manager();
    printf("\n\n ---- START  ---- \n\n");
    fflush(stdout);

    start_clock = time_w(); //start counting   ///potrebbe andare più giù
  }
  else
    printf("Error: Signal: %d not handled by this function",sig);
  signal_handler();
  exit(EXIT_SUCCESS);

}

/* init signal SIGUSR1 and SIGUSR2 */
void signal_handler() { 
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
}

/* if daemon already exist return his pid else create a daemon and return his pid*/
/* pid is saved in a .txt file */
/* d_pid = create_daemon(int format, char * output)*/
pid_t create_daemon(char *format, char *output){
  FILE *fp, *file;
  char *pidlog = "daemon_pid.txt";
  fp = fopen (pidlog,"r");
  int i;
  pid_t process_id = 0; 
  pid_t sid = 0;
  output_format = format; 
  output_file = output;
  signal_handler();

  //https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
  if (!fp){ //must create the process
    process_id = fork();
    if (process_id > 0)
      exit(EXIT_SUCCESS);      
    sid = setsid();  //set new session; setsid_w();
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    process_id = fork();   //fork_w neeeeed
    if (process_id > 0){ // PARENT PROCESS. Need to kill it.
        printf("process_id of CREATEDdaemon process %d \n", process_id);
        file = fopen(pidlog, "w");
        fprintf(file, "%d", process_id);
        fclose (file);
	return process_id;
      }
    umask(0);

    //chdir("/"); // Change the current working directory to root. 
    
    close(STDIN_FILENO); // Close stdin. stdout and stderr
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    //while (1) //mantain the daemon alive
      //sleep(1);
    
  }
  else  {//return the pid of the existing daemon
    fscanf(fp,"%d",&process_id);
    fclose(fp);
    printf("process_id of EXISTdaemon process %d \n", process_id);
    signal_handler();
    return process_id;
  }

  
  printf("Daemon terminated");
  remove(pidlog);
  
  return 0;
}



