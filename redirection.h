#ifndef REDIRECTION_H 
#define REDIRECTION_H

//#include "config.h"
//potrebbe non funzionare.
//TO IMPLEMENT BETTER
int fdout;
int fderr;
void redirect_stdout(char *outfile) {
    fdout = open(outfile, O_WRONLY | O_CREAT);
    dup2 (fdout, 1);
}

void reset_stdout() {
    close(fdout);
}


void redirect_stderr(char *errfile) {
    fderr = open(errfile, O_WRONLY | O_CREAT);
    dup2 (fderr, 2);
}

void reset_stderr() {
    close(fderr);
}


#endif