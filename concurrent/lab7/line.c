#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "err.h"

#define NR_PROC 5

void createChildProcess(int leftToCreate, pid_t parentPid) {
    pid_t pid;
    leftToCreate--;
    switch (pid = fork()) {
        case -1:
            syserr("Error in fork\n");

        case 0:
            printf("I am a child and my pid is %d and my parent's pid is %d\n", getpid(), parentPid);
            if(leftToCreate > 0) {
                createChildProcess(leftToCreate, getpid());
            }
            else {
                printf("%d ending\n", getpid());
                //exit(0);
            }
            break;

        default:
            printf("I am a parent and my pid is %d and my child's pid is %d\n", getpid(), pid);
            if (wait(0) == -1)
                syserr("Error in wait\n");
            printf("%d ending\n", getpid());
            exit(0);
    }
}

int main ()
{
    createChildProcess(NR_PROC, getpid());
    printf("I didn't end: %d", getpid());

}
