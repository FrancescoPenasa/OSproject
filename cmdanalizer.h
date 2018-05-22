#ifndef CMDANALIZER_H 
#define CMDANALIZER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define NPIPE 1
#define SPIPE 2
#define READ 0
#define WRITE 1
#define NOT_EXIST_PIPE 0

void execution(char *, int, int);
int analize_cmd (char *, int*); 
void parsecmd(char *, int, int, char *);
void exec_pipe(char *, int, int, int, char);
void pipe_handler(char *, int *);
void cmd_handler(char *);

/* search in the command if there is a single | or a |& */
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

/* take a command, an index i and an index j. Copies all the char between the index in another string. */
void parsecmd(char *argv, int start, int stop, char *res){
  int i;
  for (i = 0; i<stop; i++)
    res[i] = argv[start++];
}

/* create a pipe */ 
void exec_pipe(char *cmd, int first, int limit, int end, char type){
  if (type=='n'){ //a pipe created by a "|"
    int fd [2];
    pipe (fd); /* Create an unamed pi2pe */
    if (fork () != 0) { /* Parent, writer */
      close (fd[READ]); /* Close unused end */
      dup2 (fd[WRITE], 1); /* Duplicate used end to stdout */
      close (fd[WRITE]); /* Close original used end */
      execution(cmd, first, limit); /* Execute this part of the cmd */
      perror ("connect"); /* Should never execute */
    } else { /* Child, reader */
      close (fd[WRITE]); /* Close unused end */
      dup2 (fd[READ], 0); /* Duplicate used end to stdin */
      close (fd[READ]); /* Close original used end */
      execution(cmd, limit+1,end); /* Execute this part of the cmd */
      perror ("connect"); /* Should never execute */
    }
  }
  if (type=='s'){ //a pipe created by a "|&"
    int fd [2];
    pipe (fd); /* Create an unamed pipe */
    if (fork () != 0) { /* Parent, writer */
      close (fd[READ]); /* Close unused end */
      dup2 (fd[WRITE], 1); /* Duplicate used end to stdout */
      dup2 (fd[WRITE], 2); /* Duplicate used end to stderr, this is required by '|&' */
      close (fd[WRITE]); /* Close original used end */
      execution(cmd, first,limit); /* Execute this part of the cmd */
      perror ("connect"); /* Should never execute */
    } else { /* Child, reader */
      close (fd[WRITE]); /* Close unused end */
      dup2 (fd[READ], 0); /* Duplicate used end to stdin */
      close (fd[READ]); /* Close original used end */
      execution(cmd, limit+2,end); /* Execute this part of the cmd */
      perror ("connect"); /* Should never execute */
    }
  }
}

/* if there is a pipe, look what type of pipe it is*/
void pipe_handler(char *cmd, int *res){
  int i;
  for (i=0;i<strlen(cmd);i++){
    if (res[i]==NPIPE){
      exec_pipe(cmd,0,i,strlen(cmd),'n');  //funziona solo con una pipe
      i=strlen(cmd);                       // per uscire dal ciclo, non so usare più di una pipe.
    } 
    if (res[i]==SPIPE){
      exec_pipe(cmd,0,i,strlen(cmd),'s');      
      i=strlen(cmd);
    }
  }
}

/* If there is not a pipe, call execution with the whole cmd*/
void cmd_handler(char *cmd){
  execution(cmd,0,strlen(cmd));
}

#endif