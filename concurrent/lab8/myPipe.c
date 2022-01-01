#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "err.h"

int main(int argc, char *argv[])
{
    int in[2];
    int out[2];
    char msg[PIPE_BUF];
    size_t len;

    for (int i = 1; i < argc; i++) {
        if (pipe(in) == -1) syserr("Error in pipe in\n");
        if (pipe(out) == -1) syserr("Error in pipe out\n");
        switch (fork()) {
            case -1:
                syserr("Error in fork\n");

            case 0:
                if (close (out[0]) == -1) syserr("Error in child, close (out [0])\n");
                if (close (in[1]) == -1) syserr("Error in child, close (in [1])\n");

                if (close (1) == -1)            syserr("Error in child, close (1)\n");
                if (dup2 (out[1], 1) != 1)    syserr("Error in child, dup (out [1])\n");
                if (close (out[1]) == -1) syserr("Error in child, close (out [1])\n");

                if (close (0) == -1)            syserr("Error in child, close (0)\n");
                if (dup2 (in[0], 0) != 0)    syserr("Error in child, dup (in [0])\n");
                if (close (in[0]) == -1) syserr("Error in child, close (in [0])\n");

                execlp(argv[i], argv[i], NULL);

            default:
                if (close (out[1]) == -1) syserr("Error in close (out [1])\n");
                if (close (in[0]) == -1) syserr("Error in close (in [0])\n");
                if (i > 1)  write(in[1], msg, len);
                if (close (in[1]) == -1) syserr("Error in close (in [1])\n");
                if (wait(0) == -1) syserr("Error in wait\n");
                len = read(out[0], msg, sizeof(msg));
                if (i == argc - 1) {
                    for (size_t j = len; j < PIPE_BUF; ++j) {
                        msg[j] = '\0';
                    }
                    printf("%s", msg);
                }
                msg[len] = '\0';
                if (close (out[0]) == -1) syserr("Error in close (out [0])\n");
        }
    }
}
