#include <stdio.h>
#include <stdint.h>

#include <pthread.h>

void* runner(void* arg)
{
    printf("thread called!\n");

    return NULL;
}

int32_t main(int32_t argc, char* argv[])
{
    printf("Hello world\n");

    pthread_t thread;
    pthread_create(&thread, NULL, runner, NULL);
    pthread_join(thread, NULL);
    return 0;
}