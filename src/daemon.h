#include "mytime.h" 
#define START 0
#define END 1
#define MAXPIPE 2
#define STD 0
#define COMPLETE 1
#define MAXFILE 64    //max of char in a file name

time_t start_clock[MAXPIPE];  //start clock
time_t stop_clock[MAXPIPE];   //end clock
pid_t dpid;                   //daemon pid

char *pidlog = "/tmp/daemon_pid.txt";        //where to save daemon pid
char *run2daemon = "/tmp/run2daemon.txt";    //shared file with the main process

int pipecounterstart;
int pipecounterstop;
int id;

char LogFile[MAXFILE];
char FormatType[MAXFILE];

void signal_handler();
void sighandle_int(int);
void readrun2daemon ();
void output_manager();
int parse_format (char *);
void complete_format(FILE *);
void std_format(FILE *);
void init();


/* read what cmd has been used and his return code */
int read_info(char *cmd){
    int res,len;
    char *infolog = "/tmp/inforun.txt";  
    FILE *fd = fopen(infolog, "r");       //open file 
    if (!fd){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
    fscanf(fd, "%d",&len);
    fscanf(fd, "%s",cmd);
    fscanf(fd, "%d",&res);
    fflush(fd);
    fclose_w(fd);                           //close file
    remove_w(infolog);                      //delete file
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
    fprintf(out,"\nProcess returned: %d\n", value);
    time_string = ctime_w(&stop_clock[id++]); 
    fprintf(out,"\nProcess ended   at:%s\n", time_string);
}

/* format complete print function */
void complete_format(FILE *out){
    char *time_string;
    char *cmd_name;
    int value;
    time_string = ctime_w(&start_clock[id]);
    fprintf(out,"\n\n---------------------------------------------------------------------------------------\n"); 
    fprintf(out,"Process started at:%s\n", time_string);
    value = read_info(cmd_name);
    fprintf(out,"\nProcess %s -->     %d\n",cmd_name, value); 
    time_string = ctime_w(&stop_clock[id++]); 
    fprintf(out,"\nProcess ended   at:%s\n", time_string);
    double diff = difftime_w(stop_clock[id-1], start_clock[id-1]);
    fprintf(out,"\nTotal time        :%f\n",diff);
    fprintf(out,"---------------------------------------------------------------------------------------\n\n\n");
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

/* decide quale tipo di formato utilizzare e apre lo stream per il Log file*/
void output_manager(){
    FILE *print = fopen(LogFile, "a+");
    if (!print){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
    
    int format = parse_format(FormatType);
    switch (format){
        case STD : std_format(print) ; break;
        case COMPLETE : complete_format(print) ; break;
        default: fprintf(stderr,"Error! Format type required not supported");  break;
    }
    fclose_w(print);
}

/* retrive Log and Format from a shared file*/ 
void readrun2daemon (){
  FILE *fd = fopen(run2daemon, "r");
  if (!fd){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
  fscanf(fd, "%s",LogFile);
  fscanf(fd, "%s",FormatType);
  fclose_w(fd);
}






//--------------------------SIGNAL
/* prende il segnale di inizio e fine processo per registrare i dati */
void sighandle_int(int sig) {
  
  /*END OF THE EXECUTION*/
  if (sig == SIGUSR2){       
    stop_clock[pipecounterstop++] = time_w();
  }

  /*START OF THE EXECUTION*/
  else if (sig == SIGUSR1){
    start_clock[pipecounterstart++] = time_w();
  }

  /* PRINT SIGNAL */
  else if (sig == SIGINT){   
    readrun2daemon();
    output_manager(); 
  }
  /* INIT SIGNAL */
  else if(sig == SIGCONT)
    init();

  else
    printf("Error: Signal: %d not handled by this function",sig);
  signal_handler();                               
}

/* init signal SIGUSR1 and SIGUSR2 and SIGINT*/
void signal_handler() { 
  signal(SIGUSR1, sighandle_int);
  signal(SIGUSR2, sighandle_int);
  signal(SIGINT ,sighandle_int);
  signal(SIGCONT ,sighandle_int);
}


//CREATE DAEMON
int daemonize(){
  pid_t child;
  //fork, detach from process group leader
  child=fork_w();
  if (child>0) { //parent
    exit(EXIT_SUCCESS);
  }
  setsid_w();

  //catch/ignore signals
  signal(SIGCHLD,SIG_IGN);
  signal(SIGHUP,SIG_IGN);
  signal_handler(); 
  
  //fork second time
  child=fork_w();
  if( child>0 ) { //parent
    dpid=child;
    exit(EXIT_SUCCESS);
  }
  //new file permissions
  umask(0);

  //save daemon pid on a file
  FILE *fp;
  fp = fopen(pidlog, "w");
  if (!fp){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
  fprintf(fp, "%d", getpid());
  fclose (fp);
  while(1) {                  //stay alive
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
  fclose_w(fp);
  signal_handler();
  kill(dpid, SIGCONT);
  return dpid;  
}


