#ifndef PROCESS_THREAD_H
#define PROCESS_THREAD_H

// envrion params test
void env_params();

// how to use fork
void process_fork();
void process_fork_n();

// exec functions
void exec_execlp();

void dup2_fun();

void zoom_wait();
void zoom_waitpid();

// IPC pipe
void ipc_pipe();

// IPC fifo
int fifoWrite();

int fifoInit();
int fifoRead();

// IPC mmap
void ipc_mmap();
void ipc_mmap_parent_child();
void ipc_mmap_parent_child_anon();

// mutil process copy
void mutil_process_copy(const char *input_file, const char *output_file, int child_process_num);

#endif // PROCESS_THREAD_H
