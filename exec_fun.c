#include "process_thread.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

void exec_execlp(){

    pid_t pid;
//    extern char **environ;

    printf("ENV PATH is: %s\n", getenv("PATH"));

    pid = fork();
    if(pid == -1){
        perror("fork error: ");
        exit;
    }else if(pid == 0){
        sleep(1);
        printf("this is child process, my pid is: %d\n", getpid());
        // execlp("ls", "ls", "-l", "-a", NULL);
        execl("/bin/ls", "ls", "-l", "-a", NULL);
        perror("exec error: ");
        exit(1);
    }else{
        sleep(3);
        printf("this is parent process, and my pid is: %d, my child's pid is: %d\n", getpid(), pid);
    }

    return;
}
