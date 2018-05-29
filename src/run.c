#include <sys/ipc.h>
#include <sys/stat.h> //wait()
#include <signal.h> //signal()
#include <string.h> //string function
#include <stdlib.h> //exit(), EXIT_SUCCESS
#include <stdio.h> //printf()
#include <unistd.h> //fork() pipe() close()
#include <sys/wait.h> //wait()
#include <fcntl.h> //dup2()
#include "daemon.h" //TODO format function  TODO output function
#include "cmdanalizer.h"
#include "redirection.h" //redirect_stdout(char *), redirect_stderr(char*) 
#include "config.h"

#define FORMAT 1  //argv[FORMAT]
#define LOG 2     //argv[LOG]
#define CMD 3     //argv[CMD]
#define ERRFILE 4 //argv[ERRFILE]
#define OUTFILE 5 //argv[OUTFILE]

#define MAXCMD 120

pid_t dpid;

int outredirect = 0; //works as a boolean
char *outfile;       //per redirector.h
int errredirect = 0; //works as a boole an
char *errfile;       //per redirector.h


void write_info(char *cmd, int res);
/*
void reset_daemon(){
    char *filename = "daemon_pid.txt";
    open();
    read();
    char *cmd = strcat("kill ", dpid);
    system(cmd);
    close();
    remove(filename);
}
*/


/* manda segnale di inizio al daemon, esegue il comando, manda un segnale di fine, e fa stampare */ 
void execution(char *argv, int istart, int istop){     
  char cmd[MAXCMD];                         //variabile in cui fare il parsing del comando
  int i = 0;
  while (i<MAXCMD)
    cmd[i++]=' ';
  int result;                               //result del comando
  parsecmd(argv, istart, istop, cmd);       //divide il cmd con istart e istop, utile per un eventuale pipe.
  
  //redireziono se necessario stdout e stderr TO IMPLEMENT
  if (outredirect!=0 && istop==strlen(argv))                       
    redirect_stdout(outfile);
  if (errredirect!=0 && istop==strlen(argv))
    redirect_stderr(errfile);
  //TIL HERE

  kill(dpid, SIGUSR1);                    //start clock
  result = system_w(cmd);                   //exec cmd
  kill(dpid, SIGUSR2);                    //stop clock
  
  write_info(cmd, result);                //scrive su un file il comando usato e il risultato
  kill(dpid, SIGINT);                     //fa stampare al daemon i risultati su file


  //redireziono se necessario stdout e stderr TO IMPLEMENT
  if (outredirect!=0 && istop==strlen(argv))                     
    reset_stdout();
  if (errredirect!=0 && istop==strlen(argv))
    reset_stderr();
    //TIL HERE
}

/* scrive in inforun le informazioni riguardanti il processo eseguito */
void write_info(char *cmd, int res){
  char *infolog = "/tmp/inforun.txt";              //nome del file
  FILE *fd = fopen(infolog, "w");                  //apre il file in scrittura, se non esiste lo crea
  if (!fd){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
  int len = strlen(cmd);                           //len è un refuso della precedente implementazione con write() e read()
  fprintf(fd, "%d %s\n",len,cmd);                  //salva il comando usato su un file   
  fprintf(fd, "%d",res);                           //salva il risultato del comando usato su un file   
  fclose_w(fd);                                      //chiude il file
}


/* scrive in /tmp/run2daemon.txt come si chiama il file di LOG e il format type 
 * salva il nome del file dove stampare l'output e il file dove stampare gli errori se è stato attivato il flag
*/
void write_run2daemon(char *argv[]){
  char *run2daemon = "/tmp/run2daemon.txt";       //save the logfile path and the format type in a file.
  remove (run2daemon);                             //se esiste già il file lo cancella
  FILE *fd = fopen(run2daemon, "w");              //apre il file in scrittura
  if (!fd){
      fprintf(stderr,"error: failed fopen\n");
      exit(EXIT_FAILURE);
    }
  fprintf(fd, "%s\n",argv[LOG]);                  //stampa il nome del file di LOG che si vuole utilizzare
  fprintf(fd, "%s\n",argv[FORMAT]);               //stampa il tipo di formato che si vuole utilizzare per questa esecuzione
  fclose_w(fd);                                     //chiude il file


  //TO IMPLEMENT
  if ((atoi(argv[OUTFILE]))!=0){                  //if the outfile is specified 
    outredirect++;              
    outfile=argv[OUTFILE];                       
  }
  if ((atoi(argv[ERRFILE]))!=0){                  //if the errfile is specified
    errredirect++;
    errfile=argv[ERRFILE];  
  } 
}


 /*./runexe $format $log $cmd */
int main(int argc, char *argv[]){
  write_run2daemon(argv);                        //salvo gli argomenti
  
  dpid = create_daemon();                        //creo il daemon se non esiste, se esiste recupero il suo pid;
  
  int res [strlen(argv[CMD])];
  if (analize_cmd(argv[CMD], res) != 0)          //controllo se il comando comprende dei pipe ("|" or "|&")
    pipe_handler(argv[CMD],res);                 //se ci sono pipe li eseguo in parallelo
  else
    cmd_handler(argv[CMD]);                      //se non ci sono pipe esguo solo il comando
  
  wait(NULL);
  exit(EXIT_SUCCESS);
}


//TODO RESET DAEMON!!!