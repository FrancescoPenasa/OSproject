#ifndef CONFIG_H 
#define CONFIG_H


/* verifica se "int close(int fd)" ha funzionato */
int close_w (int fd){
    int res = close(fd);
    if (res == -1){
        fprintf(stderr,"error: failed close\n");
        exit(EXIT_FAILURE);
    }
    return res;                     //should be 0
}

/* verifica se "int dup2(int ofd, int nfd)" ha funzionato */
int dup2_w(int oldfd, int newfd){  //Duplicate used end to stdout
    int res = dup2 (oldfd, newfd);
    if (res==-1){
        fprintf(stderr,"error: failed dup2\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/* verifica se "pid_t fork()" ha funzionato */
pid_t fork_w(){
    pid_t pid;
    if( (pid=fork())<0 ) { //failed fork
        fprintf(stderr,"error: failed fork\n");
        exit(EXIT_FAILURE);
    }
    return pid;
}

/* verifica se "int kill(int pid, int sig)" ha funzionato */
int kill_w(pid_t pid, int sig){
    int res = kill(pid,sig);
    if (res == -1){
        fprintf(stderr,"error: failed kill\n");
        exit(EXIT_FAILURE);
    }
    return res;                     //should be 0
}

/* verifica se "int pipe(int fd[])" ha funzionato */
int pipe_w (int filedes [2]){
    int res = pipe(filedes);
    if (res == -1){
        fprintf(stderr,"error: failed pipe\n");
        exit(EXIT_FAILURE);
    }
    return res;                     //should be 0
}

/* verifica se "int system(char *cmd)" ha funzionato */
int system_w(const char *command){
    int res = system(command);
    if (res == -1){
        fprintf(stderr,"error: failed system\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

pid_t setsid_w(){
    pid_t res = setsid();
    if (res == -1){
        fprintf(stderr,"error: failed setsid\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

int fclose_w(FILE *stream){
    int res = fclose(stream);
    if (res == EOF){
        fprintf(stderr,"error: failed fclose\n");
        exit(EXIT_FAILURE);
    }
    return res;
}


int atoi_w(const char *str){
    int res = atoi(str);
    if (res == 0){
        fprintf(stderr,"error: failed atoi\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

int fseek_w(FILE *stream, long int offset, int whence){
    int res = fseek(stream,offset,whence);
    if (res != 0){
        fprintf(stderr,"error: failed fseek\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

int remove_w(const char *filename){
    int res = remove(filename);
    if (res == -1){
        fprintf(stderr,"error: failed fseek\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

pid_t wait_w(int *stat_loc){
    pid_t res = wait(stat_loc);
    if (res == -1){
        fprintf(stderr,"error: failed wait\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t write_w(int fd, const void *buf, size_t count){
    ssize_t res = write(fd,buf,count);
    if (res == -1){
        fprintf(stderr,"error: failed write\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t read_w(int fd, void *buf, size_t count){
    ssize_t res = read(fd,buf,count);
    if (res == -1){
        fprintf(stderr,"error: failed read\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

#if DEBUG
int main (){

    /* fork_w() test case */
    pid_t pid = fork_w();
    if (pid == 0){
        printf("fork() working for the son, my pid is:%d\n",getpid());
        exit(EXIT_SUCCESS);
    }
    else
        printf("fork() working for the father, my pid is:%d\n",getpid());
    wait(NULL);

    /* dup2_w() test case */
    int fw=open("TMPduptest.txt", O_APPEND|O_WRONLY);
    dup2_w(fw,1);
    printf("dup2 it's working \n");

    /* pipe_w() test case */
    //int mypipe[2];
    //pipe_w(mypipe);
    //TL to implement

    /* close_w() */
    //TL to implement

    /* kill_w() test case */
    //TL to implement

    /* system_w() test case */
    //TL to implement

    //TODO test
}
#endif
#endif