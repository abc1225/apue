#include "process_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void dup2_fun(){


    int fd = open("out.txt",O_RDWR | O_CREAT | O_APPEND);
    dup2(fd, 1);

    execlp("ls", "ls", "-l", "-a", NULL);

    return;
}
