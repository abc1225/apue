#include "process_thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>



unsigned long get_file_size(const char *path) {
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;
}



/**
 * copy a file from input to output with mutil process
 * @brief mutil_process_copy
 * @param input_file
 * @param output_file
 * @param process_num
 */
void mutil_process_copy(const char *input_file, const char *output_file, int child_process_num){

    char *p_in;
    char *p_out;

    int i, num, status;
    pid_t pid, w_pid;

    int BUFF_SIZE = 8192 * 10;

    // 1. process num
    int process_num = (child_process_num < 1)?1:child_process_num;

    // 2. get file size
    unsigned long file_size = get_file_size(input_file);
    if(file_size <= 0){
        perror("file is empty!");
        exit(1);
    }

    // 3. open input and output file
    int fd_input = open(input_file, O_RDWR);
    if(fd_input < 0){
        perror("open input_file error: ");
        exit(1);
    }
    int fd_output = open(output_file, O_CREAT | O_RDWR | O_TRUNC, 0664);
    if(fd_output < 0){
        perror("open input_file error: ");
        exit(1);
    }

    // 4. truncate output file with input file size
    int len = ftruncate(fd_output, file_size);
    if(len < 0){
        perror("ftruncate output file error: ");
        exit(1);
    }

    // 5. create mmap for input and output file
    p_in = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_input, 0);
    if(p_in == MAP_FAILED){
        perror("mmap error: ");
        exit(1);
    }

    p_out = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_output, 0);
    if(p_out == MAP_FAILED){
        perror("mmap error: ");
        exit(1);
    }

    // 6. caculate every child process
    int last_process_size = file_size % process_num;
    int average_size = file_size / process_num;

    // 7. create child process
    for(i = 0; i < process_num; i++){

        pid = fork();
        if(pid == -1){
            perror("fork error: ");
        }else if(pid == 0){
            break;
        }else{
            printf("parent creat a child process: %d\n", pid);
        }


    }

    if(i == process_num){

        printf("i am parent process: %d, i'm waiting child process copy file! \n", getpid());

        num = 0;
        do{
            w_pid = waitpid(-1, &status, WNOHANG);
            if(w_pid > 0){
                num++;
                if(WIFEXITED(status)){
                    printf("child process exit with code: %d with pid: %d\n", WEXITSTATUS(status), w_pid);
                }

                if(WIFSIGNALED(status)){
                    printf("child process exit with signal: %d with pid: %d\n", WTERMSIG(status), w_pid);
                }
            }
            sleep(1);
        }while(num < process_num);

        printf("i am parent process, child process copy file end!\n");

        // 8. release mmap
        int ret = munmap(p_in, file_size);
        if(ret == -1){
            perror("munmap error: ");
            exit(1);
        }

        ret = munmap(p_out, file_size);
        if(ret == -1){
            perror("munmap error: ");
            exit(1);
        }

    }else{
//        sleep(i);
        // child process do copy
        printf("i am child process, my pid is: %d, i'm going to copy file\n", getpid());

        int start_file_size, end_file_size;
        start_file_size = i * average_size;
        end_file_size = (i+1) * average_size - 1;
        // the last process
        if(i == process_num - 1){
            end_file_size += last_process_size;
        }

        int total_copy_size = end_file_size - start_file_size + 1;
        int least_size = total_copy_size;
        do{
            int copy_size = BUFF_SIZE > least_size ? least_size : BUFF_SIZE;
            memcpy(p_out + start_file_size + total_copy_size - least_size, p_in  + start_file_size + total_copy_size - least_size, copy_size);
            least_size -= copy_size;

            printf("i am child process, my pid is: %d, i'm going to copy total file size: %d, least file size: %d \n",
                   getpid(), total_copy_size, least_size);

        }while(least_size > 0);
        exit(0);
    }
    return ;
}
