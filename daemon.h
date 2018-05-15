#include "mytime.h" //time_w(); ctime_w(time_t &time); difftime_w(time_t end, time_t beginning);
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <fcntl.h> //read write
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>  

#define START 0
#define END 1
#define MAXPID 17

time_t start_clock, stop_clock;
char *output_format = "0";
char *output_file = "Log.txt";
pid_t dpid;
FILE *fp;
char *pidlog = "daemon_pid.txt";  

void signal_handler();

/* prende il segnale di inizio e fine processo per registrare i dati */
void sighandle_int(int sig) {
  
  /*END OF THE EXECUTION*/
  if (sig == SIGUSR2){    
    printf("hey man, here is: %d\n",getpid());                           
    printf("\n\n ---- STOP  ---- \n\n");
    fflush(stdout);
  }


  /*START OF THE EXECUTION*/
  else if (sig == SIGUSR1){
    printf("\n\n ---- START  ---- \n\n");
    fflush(stdout);
  }

  else
    printf("Error: Signal: %d not handled by this function",sig);
  signal_handler();
  sleep(1);
  //exit(EXIT_SUCCESS);
}

/* init signal SIGUSR1 and SIGUSR2 */
void signal_handler() { 
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
}

int daemonize(char* name, char* path, char* outfile, char* errfile, char* infile ){
  if(!path) { path="/home/chrx/OS/env"; }
  if(!name) { name="medaemon"; }
  if(!infile) { infile="/dev/null"; }
  if(!outfile) { outfile="/dev/null"; }
  if(!errfile) { errfile="/dev/null"; }
  //printf("%s %s %s %s\n",name,path,outfile,infile);
  pid_t child;
  //fork, detach from process group leader
  if( (child=fork())<0 ) { //failed fork
    fprintf(stderr,"error: failed fork\n");
    exit(EXIT_FAILURE);
  }
  if (child>0) { //parent
    exit(EXIT_SUCCESS);
  }
  if( setsid()<0 ) { //failed to become session leader
    fprintf(stderr,"error: failed setsid\n");
    exit(EXIT_FAILURE);
  }
  //catch/ignore signals
  signal(SIGCHLD,SIG_IGN);
  signal(SIGHUP,SIG_IGN);
  signal_handler(); 
  
  //fork second time
  if ( (child=fork())<0) { //failed fork
    fprintf(stderr,"error: failed fork\n");
    exit(EXIT_FAILURE);
  }
  if( child>0 ) { //parent
    dpid=child;
    fprintf(stdout,"dpid= %d",dpid);
    exit(EXIT_SUCCESS);
  }
  //new file permissions
  umask(0);
  //change to path directory
  //chdir(path);

  //Close all open file descriptors
  int fd;
  /*for( fd=sysconf(_SC_OPEN_MAX); fd>0; --fd )
    {
      close(fd);
    }
*/
  //reopen stdin, stdout, stderr
  //stdin=fopen(infile,"r");   //fd=0
  //stdout=fopen(outfile,"w+");  //fd=1
  //stderr=fopen(errfile,"w+");  //fd=2
  
  //open syslog
  openlog(name,LOG_PID,LOG_DAEMON);
  fprintf(stdout, "name: %s\nLOG_PID: %d\nLOG_DAEMON: %d",name,LOG_PID,LOG_DAEMON);

  //scrivo il pid
  printf("process_id of CREATEDdaemon process %d \n", getpid());
  fflush(stdout);
  fp = fopen(pidlog, "w");
  fprintf(fp, "%d", getpid());
  fclose (fp);
  while(1) {
    sleep(1);
  }
  return(0);
}

/* if daemon already exist return his pid else create a daemon and return his pid*/
/* pid is saved in a .txt file */
//./a.out pippo logsasd.txt "ls -l"
/* d_pid = create_daemon(int format, char * output)*/
pid_t create_daemon(char *format, char *output){
  fp = fopen (pidlog,"r");
  output_format = format; 
  output_file = output;

  if (fp==NULL){ //if daemon don't exist
    if (fork()==0){
      daemonize("mydaemon",NULL,NULL,NULL,NULL);
      return dpid;
    }
  }
  while(fp==NULL)
    fp=fopen(pidlog,"r");
  fscanf(fp,"%d",&dpid);
  fclose(fp);
  printf("process_id of EXISTdaemon process %d \n", dpid);
  signal_handler();
  return dpid;
  
  
}


