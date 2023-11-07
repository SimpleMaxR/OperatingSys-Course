#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int pipefd[2];
    pid_t pid1, pid2;

    char msg1[] = "Child1 is sending a message!";
    char msg2[] = "Child2 is sending a message!";
    char buf[50];

    // 创建管道
    pipe(pipefd);

    pid2 = fork();
    if (pid2 == 0){
        // child2 进程
        close(pipefd[0]); // 关闭读取端
        write(pipefd[1], msg2, sizeof(msg2));
        close(pipefd[1]); // 关闭写入端
        exit(EXIT_SUCCESS);
    }

    // 创建子进程1
    pid1 = fork();
    if (pid1 == 0){
        // child1 进程
        close(pipefd[0]); // 关闭读取端
        write(pipefd[1], msg1, sizeof(msg1));
        close(pipefd[1]); // 关闭写入端
        exit(EXIT_SUCCESS);
    }



    close(pipefd[1]);
    read(pipefd[0], buf, sizeof(buf));
    printf("%s\n", buf);

    read(pipefd[0], buf, sizeof(buf));
    printf("%s\n", buf);

    close(pipefd[0]); // 关闭读取端

    return 0;
}