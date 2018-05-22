#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "daemon.h" //TODO format function  TODO output function
#include "cmdanalizer.h"
#include "redirection.h" //redirect_stdout(char *), redirect_stderr(char*) 

#define FORMAT 1
#define LOG 2
#define CMD 3
#define ERRFILE 4
#define OUTFILE 5

#define MAXWRITE 100
#define MAXCMD 120

pid_t dpid;
int outredirect = 0; //works as a boolean
char *outfile;
int errredirect = 0; //works as a boolean
char *errfile;

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
void execution(char *argv, int istart, int istop){     
  char cmd[MAXCMD];
  int result;
  parsecmd(argv, istart, istop, cmd);       //divide il cmd con istart e istop, utile per un eventuale pipe.
  
  //redireziono se necessario stdout e stderr
  if (outredirect!=0 && istop==strlen(argv))                       
    redirect_stdout(outfile);
  if (errredirect!=0 && istop==strlen(argv))
    redirect_stderr(errfile);
  
  
  int i=kill(dpid, SIGUSR1); //start clock
  result = system(cmd);         //exec cmd
  int j =kill(dpid, SIGUSR2); //stop clock
  
  write_info(cmd, result);
  int k =0;
  k= kill(dpid, SIGINT);  //stampa risultati su file
  printf("i=%d;j=%d;k=%d\n ",i,j,k);

  //redireziono se necessario stdout e stderr
  if (outredirect!=0 && istop==strlen(argv))                     
    reset_stdout();
  if (errredirect!=0 && istop==strlen(argv))
    reset_stderr();

  printf("\nexecuted: %s  dpid = %d\n",cmd,dpid);  //scrivo il cmd usato e il valore restituito
}

/* scrive in inforun le informazioni riguardanti il processo eseguito */
void write_info(char *cmd, int res){
  char *infolog = "/tmp/inforun.txt";
  int fd = open(infolog, O_WRONLY | O_CREAT);
  printf("QUELLO CHE MI INTERESSAAAAAAAAAAAAAAAA%d",res);
  int len = strlen(cmd);
  write(fd, &len, sizeof(int));
  write(fd, cmd, (((strlen(cmd))+1)*(sizeof(char))));                       //size?????
  write(fd, &res, sizeof(int));
  close(fd);
}

/* scrive in /tmp/run2daemon.txt come si chiama il file di LOG e il format type 
 * salva il nome del file dove stampare l'output e il file dove stampare gli errori se è stato attivato il flag
*/
void write_run2daemon(char *argv[]){
  char *run2daemon = "/tmp/run2daemon.txt";       //save the logfile path and the format type in a file.
  remove (run2daemon);
  int fd = open(run2daemon, O_WRONLY | O_CREAT);
  write(fd, argv[LOG], MAXWRITE);
  write(fd, argv[FORMAT], MAXWRITE);
  close(fd);

  if ((atoi(argv[OUTFILE]))!=0){                  //if the outfile is specified 
    outredirect++;              
    outfile=argv[OUTFILE];                       
  }
  if ((atoi(argv[ERRFILE]))!=0){                  //if the errfile is specified
    errredirect++;
    errfile=argv[ERRFILE];  
  }
}

 /*./runexe $format $log $cmd $errfile $outfile */
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