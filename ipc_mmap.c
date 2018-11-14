#include "process_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


void ipc_mmap()
{
    char *p = NULL;
    int fd = open("mytest.txt", O_CREAT | O_RDWR, 0644);
    if(fd < 0){
        perror("open file error: ");
        exit(1);
    }
    int len = ftruncate(fd, 4);
    if(len < 0){
        perror("ftruncate error: ");
        exit(1);
    }
    char *str = "hello, world!";
    //
    p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap error: ");
        exit(1);
    }

    close(fd);
    strcpy(p, "abc");
    int ret = munmap(p, 4);
    if(ret == -1){
        perror("munmap error: ");
        exit(1);
    }

    return;
}



void ipc_mmap_parent_child()
{
    int *p;
    int var = 100;
    pid_t pid;
    int fd = open("temp", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(fd < 0){
        perror("open file error: ");
        exit(1);
    }
    unlink("temp"); // del temp denty, it will be released after run over;
    int len = ftruncate(fd, 4);
    if(len < 0){
        perror("ftruncate error: ");
        exit(1);
    }

    p = (int *)mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//    p = (int *)mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if(p == MAP_FAILED){
        perror("mmap error: ");
        exit(1);
    }

    close(fd);

    *p = 888;
    pid = fork();
    if(pid == 0){
        var = 2000;
        *p = 999;
        printf("child p val: %d, var: %d\n", *p, var);

    }else if(pid > 0){
        sleep(1);
        printf("parent p val: %d, var: %d\n", *p, var);
        wait(NULL);

        int ret = munmap(p, 4);
        if(ret == -1){
            perror("munmap error: ");
            exit(1);
        }
    }else{
        perror("fork error: ");
        exit(1);
    }
    return;
}


void ipc_mmap_parent_child_anon()
{
    int *p;
    int var = 100;
    pid_t pid;

    p = (int *)mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//    p = (int *)mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if(p == MAP_FAILED){
        perror("mmap error: ");
        exit(1);
    }

    *p = 888;
    pid = fork();
    if(pid == 0){
        var = 2000;
        *p = 999;
        printf("child p val: %d, var: %d\n", *p, var);

    }else if(pid > 0){
        sleep(1);
        printf("parent p val: %d, var: %d\n", *p, var);
        wait(NULL);

        int ret = munmap(p, 4);
        if(ret == -1){
            perror("munmap error: ");
            exit(1);
        }
    }else{
        perror("fork error: ");
        exit(1);
    }
    return;
}


