#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child_pid1, child_pid2;

void child_handler(int signum) {
    if (signum == SIGINT) {
        printf("Child process1 is killed by parent!\n");
        exit(0);
    }
}

void child_handler2(int signum) {
    if (signum == SIGINT) {
        printf("Child process2 is killed by parent!\n");
        exit(0);
    }
}

void parent_handler(int signum) {
    if (signum == SIGINT) {
        kill(child_pid1, SIGINT);
        kill(child_pid2, SIGINT);
    } else if (signum == SIGCHLD) {
        // 子进程终止时的处理
        wait(NULL);
        wait(NULL);
        printf("Parent process is killed!\n");
        exit(0);
    }
}

int main() {
    // 创建子进程1
    child_pid1 = fork();

    if (child_pid1 < 0) {
        // 创建子进程1失败
        perror("fork");
        exit(1);
    } else if (child_pid1 == 0) {
        // 子进程1
        signal(SIGINT, child_handler);  // 设置子进程1的信号处理函数
        printf("Child process1 is running...\n");
        while (1) {
            // 子进程1执行的任务
            sleep(1);
        }
    } else {
        // 创建子进程2
        child_pid2 = fork();

        if (child_pid2 < 0) {
            // 创建子进程2失败
            perror("fork");
            exit(1);
        } else if (child_pid2 == 0) {
            // 子进程2
            signal(SIGINT, child_handler2);  // 设置子进程2的信号处理函数
            printf("Child process2 is running...\n");
            while (1) {
                // 子进程2执行的任务
                sleep(1);
            }
        } else {
            // 父进程
            printf("Parent process is running...\n");
            signal(SIGINT, parent_handler);  // 设置父进程的信号处理函数
            signal(SIGCHLD, parent_handler); // 设置父进程捕捉子进程终止的信号
            printf("Press Ctrl+C to terminate...\n");
            while (1) {
                // 父进程执行的任务
                sleep(1);
            }
        }
    }

    return 0;
}
