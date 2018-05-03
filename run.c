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
/*#define MULT1(X,Y) X*Y			//definisco una macro.*/

/* tmp solution to pipe manager*/
int pipe_manager(char *cmd, int size){
    int i;
    int counter=1;
    for(i=0; i<size;i++)
        if (cmd[i]== '|' && cmd[i+1]!='|')  //sono presenti pipe?
            counter++; i++;                 //i++ per evitare il caso ||
    return counter;
}
//pid_t pid
void padre(char *argv[], int pid){
    kill(pid, SIGUSR1);
    system(argv[CMD]);
    //execvp(argv[CMD], argv);        //execvp pipe func()
    kill(pid, SIGUSR2);
}

void execution(char *argv[], int pipe){
    if (pipe != 1){/*todo tmp solution to pipemanager*/
        printf ("Not ready yet.");
        exit(EXIT_SUCCESS);
    }                 
    figlio();
    int pid = fork();       //if not exist daemon    
    if(pid==0)
        sleep(2);
    else
        padre(argv, pid);
}
 /*./runexe $format $log $cmd*/
int main(int argc, char *argv[]){
    //input manager if i cant do the script
    //int pipe = pipe_manager(argv[CMD],strlen(argv[LOG]));    // TODO redirect function test2.c if needed
    execution(argv,1);
    //communicate(argv[LOG]);
    // TODO chiama il logger, evoca il logger 
    exit(EXIT_SUCCESS);
}

/*
//called by the daemon  /output_writer(argv[LOG],strlen(argv[LOG]));
void output_writer(char *output, int size){
	FILE *fp;  				    //open stream
	fp = fopen(output,O_WRONLY);		//stream bind to file, and access mode (read,write)
	fprintf(fp,"\n\n\t");       //format type			
    fprintf(fp,"%s", stringa);			
	fclose(fp);					//close stream
	return(0);
}
*/
//called after the exec
//void logger(){
    //se non esiste
    //pid = fork()
    //if (pid !=0) return
    //else create logger
//}
