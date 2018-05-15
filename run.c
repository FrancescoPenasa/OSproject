#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "daemon.h" //TODO format function  TODO output function

#define FORMAT 1
#define LOG 2
#define CMD 3
#define NPIPE 1
#define SPIPE 2
#define READ 0
#define WRITE 1
#define NOT_EXIST_PIPE 0

pid_t dpid;

int analize_cmd (char *cmd, int *res){
  int i;
  int counter = 0;
  for (i=0; i<strlen(cmd); i++) //init
    res[i] = 0;  
  for (i=0; i<strlen(cmd); i++){
    if (cmd[i] == '|'){    //se trova un simbolo '|'
      if (cmd[i+1] == '|')  //se rappresenta un OR
	res[++i] = 0;      //non fare niente
      else{                //ALTRIMENTI se rappresenta un PIPE or un "|&"
	if (cmd[i+1] == '&'){
	  res[i]=2;
	  counter++;
	}
	if (cmd[i+1] != '&'){
	  res[i]=1;
	  counter++;
	}
      }
    }
  }
  return counter;
}

/* DA SISTEMARE TUTTOTS*/
void execution(char *argv, int istart, int istop){     //neeed to divide the cmd
  int j;
  j =kill(dpid, SIGUSR1);
  printf("arrivato fin qui");
  fflush(stdout);
  system(argv);
  //wait(NULL);
  int i = kill(dpid, SIGUSR2);
  printf("kill = %d%d",j,i); 
  printf("dpid = %d",dpid);
}

void exec_pipe(char *cmd, int first, int limit, int end, char type){
  if (type=='n'){
    int fd [2];
    pipe (fd); /* Create an unamed pipe */
    if (fork () != 0) { /* Parent, writer */
      close (fd[READ]); /* Close unused end */
      dup2 (fd[WRITE], 1); /* Duplicate used end to stdout */
      close (fd[WRITE]); /* Close original used end */
      execution(cmd, first, limit);
      //execvp (cmd[0], cmd, NULL); /* Execute writer program */       ////7to manage all the execlp
      //perror ("connect"); /* Should never execute */
    } else { /* Child, reader */
      close (fd[WRITE]); /* Close unused end */
      dup2 (fd[READ], 0); /* Duplicate used end to stdin */
      close (fd[READ]); /* Close original used end */
      execution(cmd, limit+1,end);
      //execvp (argv[2], argv[2], NULL); /* Execute reader program */
      //perror ("connect"); /* Should never execute */
    }
  }
  if (type=='s'){
    int fd [2];
    pipe (fd); /* Create an unamed pipe */
    if (fork () != 0) { /* Parent, writer */
      close (fd[READ]); /* Close unused end */
      dup2 (fd[WRITE], 1); /* Duplicate used end to stdout */
      dup2 (fd[WRITE], 2); /* Duplicate used end to stderr, this is required by '|&' */
      close (fd[WRITE]); /* Close original used end */
      execution(cmd, first,limit);
      //execvp (argv[1], argv[1], NULL); /* Execute writer program */
      //perror ("connect"); /* Should never execute */
    } else { /* Child, reader */
      close (fd[WRITE]); /* Close unused end */
      dup2 (fd[READ], 0); /* Duplicate used end to stdin */
      close (fd[READ]); /* Close original used end */
      execution(cmd, limit+2,end);
      //execvp (argv[2], argv[2], NULL); /* Execute reader program */
      //perror ("connect"); /* Should never execute */
    }
  }
}


void pipe_handler(char *cmd, int *res){
  int i;
  for (i=0;i<strlen(cmd);i++){
    if (res[i]==NPIPE){//todopipe
      exec_pipe(cmd,0,i,strlen(cmd),'n');  //funziona solo con una pipe
      i=strlen(cmd);    //<<-- per uscire dal ciclo, non so usare più di una pipe
    } 
    if (res[i]==SPIPE){//todospipe
      exec_pipe(cmd,0,i,strlen(cmd),'s');      
      i=strlen(cmd);
    }
  }
}

/******** DA FARE */
void cmd_handler(char *cmd){
  execution(cmd,0,strlen(cmd));
}

 /*./runexe $format $log $cmd*/
int main(int argc, char *argv[]){
  //input manager(argv);
  pid_t daemon_pid = create_daemon(argv[FORMAT], argv[LOG]);
  dpid = daemon_pid;
  printf("dai dai");
  fflush(stdout);
  int res [strlen(argv[CMD])];
  if (analize_cmd(argv[CMD], res) != 0)
    pipe_handler(argv[CMD],res);
  else
    cmd_handler(argv[CMD]);
  wait(NULL);
  exit(EXIT_SUCCESS);
}






//NICE SHIT TO DO
/*  
void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
*/
