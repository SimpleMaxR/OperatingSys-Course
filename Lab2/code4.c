#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pipefd[2];
    pid_t pid1, pid2;
    char buf1[50], buf2[50];

    if (pipe(pipefd) == -1){
        perror("pipe");
    }

    pid1 = fork();
    if (pid1 == 0){
        // child1 进程
        close(pipefd[0]);
        write(pipefd[1], "Child1 is sending a message!", 30);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
        // CHild 2 进程
        close(pipefd[0]);
        write(pipefd[1], "Child2 is sending a message!", 30);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
        } else {
            // parent 进程
            close(pipefd[1]);
            wait(NULL);
            read(pipefd[0], buf1, 30);
            printf("%s\n", buf1);
            wait(NULL);
            read(pipefd[0], buf2, 30);
            printf("%s\n", buf2);
            close(pipefd[0]);
            exit(EXIT_SUCCESS);
        }
    }
}