#include "process_thread.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void zoom_wait(){

    pid_t pid, w_pid;
    int status;

    pid = fork();

    if(pid == 0){
        printf("i am child process...my pid is : %d\n", getpid());
        sleep(1);
//        int a = 100/0;
        exit(100);
    }else if(pid > 0){

        w_pid = wait(&status);
        if(w_pid == -1){
            perror("wait error: ");
            exit(1);
        }

        if(WIFEXITED(status)){
            printf("child process exit with code: %d\n", WEXITSTATUS(status));
        }

        if(WIFSIGNALED(status)){
            printf("child process exit with signal: %d\n", WTERMSIG(status));
        }

        while(1){
            printf("i am parent, my pid is: %d\n", getpid());
            sleep(1);
        }

    }else{
        perror("fork error: ");
        exit(1);
    }

    return 0;
}



void zoom_waitpid(){

    pid_t pid, w_pid, q;
    int status, i, n, num;

    n = 4;
    num = 0;

    for(i = 0; i< n; i++){
        pid = fork();
        if(pid == 0){
            break;
        }else if(i == 3){
            q = pid;
        }else if(pid == -1){
            perror("fork error: ");
            exit(1);
        }
    }

    if(i == n){
        sleep(n);
        printf("i am parent process: %d\n", getpid());

        do{
            w_pid = waitpid(-1, &status, WNOHANG);
            if(w_pid > 0){
                num++;
            }
            sleep(1);
            if(WIFEXITED(status)){
                printf("child process exit with code: %d with pid: %d\n", WEXITSTATUS(status), w_pid);
            }

            if(WIFSIGNALED(status)){
                printf("child process exit with signal: %d with pid: %d\n", WTERMSIG(status), w_pid);
            }
        }while(num < n);

    }else{
        sleep(i);
        printf("i am child process, my pid is: %d\n", getpid());
        exit(100);
    }
    return 0;
}



