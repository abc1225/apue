#include "process_thread.h"
#include<stdio.h>
#include<stdlib.h>


/**
 * how to use env variables? 3 methods
 * getenv()
 * setenv()
 * unsetenv()
 *
 * @brief env_params
 */
void env_params(){
    // extern params
    extern char ** environ;

    int i = 0;
    for(i = 0; environ[i]; i++){
        printf("%s\n", environ[i]);
    }

    char* path;
    path = getenv("PATH");
    printf("\n\npath is : %s\n", path);


    const char* name = "ABC";
    const char* val = "i am a env variable!";

    int ret = setenv(name, val, 0);

    printf("setenv result is: %d\n", ret);
    printf("getenv %s it's value is : %s\n", name, getenv(name));

    ret = unsetenv(name);

    printf("unsetenv result is: %d\n", ret);
    printf("After unsetenv,  getenv %s it's value is : %s", name, getenv(name));

}
