#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "process_thread.h"


#define ZIGBEE_PIPETOGW     "/tmp/pipeZBToGW"
#define TCP_BUFFER_SIZE     1024

#define JSON_DATA_1               "{" \
                                  "\"method\": \"HOST_GET_Ret_Eui64\"," \
                                  "\"longAddress\":[" \
                                  "{\"eui64\": \"AAA\", \"type\": 1, \"deviceEndpoint\":1}," \
                                  "{\"eui64\": \"BBB\", \"type\": 2, \"deviceEndpoint\":2}]" \
                                  "}"

/*
    * 功能:  管道的写端
    * 返回值, >=0表示成功, <0表示失败
*/
int fifoWrite()
{
    int nRet     = -1;
    int pipe_fd  = -1;

    pipe_fd = open(ZIGBEE_PIPETOGW, O_WRONLY|O_NONBLOCK);
    if(pipe_fd == -1)
    {
        printf("fifoWrite pipe error\n");
        return -1;
    }

    nRet = write(pipe_fd, JSON_DATA_1, strlen(JSON_DATA_1) + 1);
    if(nRet == -1)
    {
        printf("pipe write error\n");
        close(pipe_fd);
        return -1;
    }

    close(pipe_fd);

    return 0;

}


/*
    * 功能:  进行有名管道的初始化
    * 返回值, >=0表示成功, <0表示失败
*/
int fifoInit()
{
    int nCode = 0;

    if(-1 == access(ZIGBEE_PIPETOGW, F_OK))
    {
        nCode = mkfifo(ZIGBEE_PIPETOGW, 0777);
        if(0 != nCode)
        {
            printf("fifoInit could not creat fifo(%s)!\n", ZIGBEE_PIPETOGW);
            return -1;
        }
    }

    return 0;
}


/*
    * 功能:  管道的读端
    * 返回值, >=0表示成功, <0表示失败
*/
int fifoRead()
{
    int nRet        = -1;
    int pipe_fd   = -1;
    int maxSock;
    fd_set readfd;
    int u32ReadByte;
    char  u8RecvBuf[TCP_BUFFER_SIZE]  = {0};

    printf("fifoRead start\n");

    while(1)
    {
        pipe_fd = open(ZIGBEE_PIPETOGW, O_RDONLY | O_NONBLOCK);
        if(pipe_fd == -1)
        {
            printf("fifoRead open pipe error\n");
            exit(1);
        }

        //超时设置
        struct timeval stTV;

        /*
            * FD_ZERO
            * 将指定的文件描述符集清空，在对文件描述符集合进行设置前，
            * 必须对其进行初始化，如果不清空，由于在系统分配内存空间后，
            * 通常并不作清空处理，所以结果是不可知的。
        */

        while(1)
        {
            //超时,成功每次都需要重新设置
            stTV.tv_sec  = 10;
            stTV.tv_usec = 0;

            //每次循环都要清空集合，否则不能检测描述符变化
            FD_ZERO(&readfd);
            FD_SET(pipe_fd, &readfd);
            maxSock = pipe_fd;

            nRet = select(maxSock+1, &readfd, (fd_set*)NULL, (fd_set*)NULL, NULL);
            if(0 > nRet)
            {
                perror("fifoRead select error!\n");
                break;
            }
            else if(0 == nRet)
            {
                 printf("fifoRead timeout!\n");
                 continue;
            }

            if(FD_ISSET(pipe_fd, &readfd))
            {
                memset(u8RecvBuf, 0, TCP_BUFFER_SIZE);
                u32ReadByte = read(pipe_fd, u8RecvBuf, TCP_BUFFER_SIZE);
                if(u32ReadByte > 0){
                    printf("fifoRead readlength(%d)\n", u32ReadByte);
                    u8RecvBuf[u32ReadByte] = '\0';
                    printf("fifoRead u8RecvBuf(%s)!\n", u8RecvBuf);
                }
                else if(0 == u32ReadByte)
                {
                    //没有数据可读,一般是写端关闭,跳出
                    printf("fifoRead end!\n");
                    break;
                }
                else
                {
                    printf("fifoRead read data error(%d)!\n", errno);
                }
            }
        }

        close(pipe_fd);
    }

    return 0;
}
