#include "mytime.h" //time_w(); ctime_w(time_t &time); difftime_w(time_t end, time_t beginning);
//#include "config.h"
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
#define MAXPIPE 2
#define STD 0
#define COMPLETE 1

time_t start_clock[MAXPIPE];
time_t stop_clock[MAXPIPE];
pid_t dpid;


char *pidlog = "/tmp/daemon_pid.txt";  
char *run2daemon = "/tmp/run2daemon.txt";
int pipecounterstart;
int pipecounterstop;
int id;
long position = 0;


char LogFile[20];
char FormatType[20];
char *output_file = "Log.txt";
char *output_format = "STD";

void signal_handler();
void sighandle_int(int);
void readrun2daemon ();
void output_manager();
int parse_format (char *);
void complete_format(FILE *);
void std_format(FILE *);
void init();



int read_info(char *cmd){
    int res,len;
    char *infolog = "/tmp/inforun.txt";
    int fd = open(infolog, O_RDONLY);
    //lseek(fd, position, SEEK_SET);
    read(fd, &len, sizeof(int));
    read(fd, cmd,  ((strlen(cmd))*(sizeof(char))));
    read(fd, &res, sizeof(int));
    //fflush(fd);
    //position = lseek(fd, position, SEEK_CUR);
    close(fd);
    remove (infolog);
    return res;
}


/* format std print function */
void std_format(FILE *out){
    char *time_string;
    char cmd_name[120];
    int value;
    time_string = ctime_w(&start_clock[id]); 
    fprintf(out,"\n\nProcess started at:%s", time_string);
    value = read_info(cmd_name);
    fprintf(out,"\nProcess name: %s\n", cmd_name);
    fprintf(out,"\nProcess returned: %d\n", value);
    time_string = ctime_w(&stop_clock[id++]); 
    fprintf(out,"\nProcess ended   at:%s\n", time_string);
    printf("stampato!\n");
}

/* format complete print function */
void complete_format(FILE *out){
    char *time_string;
    char *cmd_name;
    int value;
    time_string = ctime_w(&start_clock[id]); 
    fprintf(out,"\n\nProcess started at:%s\n", time_string);
    fprintf(out,"\nProcess started at:%s\n", time_string);
    value = read_info(cmd_name);
    fprintf(out,"\nProcess returned: %d\n", value);
    time_string = ctime_w(&stop_clock[id++]); 
    fprintf(out,"Process ended   at:%s\n", time_string);
}

/* change from string format to a int format*/ 
int parse_format (char *format){
    int output_type = -1;
    if ((strcmp(format, "STD")) == 0)
        output_type = STD;
    if ((strcmp(format, "COMPLETE")) == 0)
        output_type = COMPLETE;
    /* to extend the implementation use this... 
    if ((strcmp(format, "todoformattype")) == 0)
        output_type = todoformattype;
    */
    return output_type;
}

/* decide quale tipo di formato utilizzare e da dove cominciare a leggere il file INFO */
void output_manager(){
    FILE *print = fopen(LogFile, "a+");
    
    int format = parse_format(FormatType);
    switch (format){
        case STD : std_format(print) ; break;
        case COMPLETE : complete_format(print) ; break;
        default: fprintf(stderr,"Error! Format type required not supported");  break;
    }
    fclose(print);
}

void readrun2daemon (){
  //retrive Log and Format
  int fd = open(run2daemon, O_RDONLY);
  read(fd, LogFile, sizeof(LogFile));
  read(fd, FormatType, sizeof(FormatType));
  strcpy(LogFile,"Log.txt");
  strcpy(FormatType,"STD");
  close(fd);
  printf("finito readrun2daemon\n Logfile:%s\n Formattype:%s\n",LogFile,FormatType);
  //retrive cmd and result
}






//--------------------------_SIGNAL
/* prende il segnale di inizio e fine processo per registrare i dati */
void sighandle_int(int sig) {
  
  /*END OF THE EXECUTION*/
  if (sig == SIGUSR2){   
    ///////////////7
    printf("hey man, here is: %d\n",getpid());                           
    printf("\n\n ---- STOP  ---- \n\n");
    fflush(stdout);
    ///////////////7
    
    stop_clock[pipecounterstop++] = time_w();
  }

  /*START OF THE EXECUTION*/
  else if (sig == SIGUSR1){
    ///////////////7
    printf("\n\n ---- START  ---- \n\n");
    fflush(stdout);
    ///////////////7

    start_clock[pipecounterstart++] = time_w();
  }

  else if (sig == SIGINT){
    printf("STAMPO,,, : pipecounterstart:%d\n",pipecounterstart);                   //>>>>>>>>>>>>>>>cominciano qui i problemi
    if (pipecounterstart == 1)
      readrun2daemon();
    output_manager(); 
  }
  else if(sig == SIGCONT)
    init();

  else
    printf("Error: Signal: %d not handled by this function",sig);
  signal_handler();
  //sleep(1);                                
}

/* init signal SIGUSR1 and SIGUSR2 and SIGINT*/
void signal_handler() { 
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
  signal(SIGINT ,sighandle_int);
  signal(SIGCONT ,sighandle_int);
}








//-------------------------------------------------CREATE DAEMON
int daemonize(){
  //printf("%s %s %s %s\n",name,path,outfile,infile);
  pid_t child;
  //fork, detach from process group leader
  child=fork();
  if (child>0) { //parent
    exit(EXIT_SUCCESS);
  }
  setsid();

  //catch/ignore signals
  signal(SIGCHLD,SIG_IGN);
  signal(SIGHUP,SIG_IGN);
  signal_handler(); 
  
  //fork second time
  child=fork();
  if( child>0 ) { //parent
    dpid=child;
    fprintf(stdout,"dpid= %d",dpid);
    exit(EXIT_SUCCESS);
  }
  //new file permissions
  umask(0);

  chdir("/home/chrx/OS/env/");

  //Close all open file descriptors
  /*int fd;
  for( fd=sysconf(_SC_OPEN_MAX); fd>0; --fd )
      close(fd);
*/

///////////////////7
  fprintf(stdout, "name: mydaemo\nLOG_PID: %d\nLOG_DAEMON: %d",LOG_PID,LOG_DAEMON);
  printf("process_id of CREATEDdaemon process %d \n", getpid());
  fflush(stdout);
///////////////////7

  FILE *fp;
  fp = fopen(pidlog, "w");
  fprintf(fp, "%d", getpid());
  fclose (fp);
  while(1) {                  //rimane attivo
    sleep(1);
  }
  return(0);
}

void init(){
  pipecounterstart = 0;
  pipecounterstop = 0;
  id = 0;
}

/* if daemon already exist return his pid else create a daemon and return his pid*/
/* pid is saved in a .txt file */
pid_t create_daemon(){
  FILE *fp;
  fp = fopen (pidlog,"r");

  if (fp==NULL)    //if daemon don't exist
    if (fork()==0) // child create a daemon
      daemonize();
  //parent    
  while(fp==NULL)       //wait for the file to be written
    fp=fopen(pidlog,"r");
  fscanf(fp,"%d",&dpid);
  fclose(fp);
  ///////////////7//
  printf("process_id of EXISTdaemon process %d \n", dpid);
  ///////////////7
  signal_handler();
  kill(dpid, SIGCONT);
  return dpid;  
}


