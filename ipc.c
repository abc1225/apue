#include "process_thread.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void ipc_pipe(){

    int fd[2];
    int ret = pipe(fd);
    if(ret == -1){
        perror("pipe error: ");
        exit(1);
    }

    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("fork error: ");
        exit(1);
    }else if(pid == 0){
        close(fd[1]);
//        sleep(2);
        char buf[1024];
        printf("child process begin read buf, pid is: %d\n", getpid());
        ret = read(fd[0], buf, sizeof(buf));
        printf("child process read buf over, ret is: %d,  pid is: %d\n", ret, getpid());

        if(ret == 0){
            printf("-----------\n");
        }

        write(STDOUT_FILENO, buf, ret);
        printf("\n\n");
    }else{
        sleep(2);
        close(fd[0]);
        char *str = "hello pipe";
        printf("parent process begin write pipe, pid is: %d\n", getpid());

        write(fd[1], str, strlen(str));

    }
    return;
}
