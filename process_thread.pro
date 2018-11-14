TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    env_params.c \
    process_fork.c \
    exec_fun.c \
    dup2_fun.c \
    zoom_wailt.c \
    ipc.c \
    fifo_rw.c \
    ipc_mmap.c \
    mutil_process_copy.c

HEADERS += \
    process_thread.h
