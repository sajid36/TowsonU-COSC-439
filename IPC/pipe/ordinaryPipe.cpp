// parent writes, child reads
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        // child process
        close(fd[1]);                 // close write end in child
        char buf[128];
        ssize_t n = read(fd[0], buf, sizeof(buf) - 1);
        if (n < 0) { perror("read"); exit(1); }
        buf[n] = '\0';
        printf("child received: %s\n", buf);
        printf("%d\n", fd[0]);
        close(fd[0]);
        exit(0);
    } else {
        // parent process
        close(fd[0]);                 // close read end in parent
        const char *msg = "hello from parent";
        if (write(fd[1], msg, strlen(msg)) < 0) { perror("write"); }
        printf("Parent finished writing: %s\n", msg);
        close(fd[1]);                 // send EOF to child
        wait(NULL);                   // reap child
    }

    return 0;
}