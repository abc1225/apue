#include "process_thread.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

/**
 * for one process
 * @brief process_fork
 */
void process_fork()
{

    pid_t pid;

    printf("begin fork a child process!\n");

    pid = fork();

    if(pid == -1){
        perror("fork error: ");
        exit(1);
    }else if(pid == 0){
        // child process
        sleep(3);
        printf("child process: this is child, and mypid is: %d\n", getpid());
        exit(1);
    }

    sleep(5);
    printf("parent process: this is parent, and mypid is: %d, my child pid is: %d\n", getpid(), pid);

    return ;

}


/**
 * fork many child process
 * @brief process_fork_n
 */
void process_fork_n(){

    int n = 5;
    int i;

    pid_t pid;

    for(i = 0; i < 5; i++){

        pid = fork();
        if(pid == -1){
            perror("fork error: ");
        }else if(pid == 0){
            break;
        }else{
            printf("parent creat a child process: %d\n", pid);
        }
    }

    if( pid == 0){
        sleep(i+1);
        printf("this is child: %d, my pid is: %d\n", i+1, getpid());
    }else{
        sleep(i+2);
        printf("parent process is game over!");
    }

    return;
}
