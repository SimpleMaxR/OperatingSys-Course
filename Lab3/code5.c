#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 进程控制块（PCB）结构体
struct PCB {
    char process_name[20];
    struct PCB* link;
    int arrival_time;
    int estimated_runtime;
    char status;
};

typedef struct PCB PCB;

// 进程调度器结构体
struct ProcessScheduler {
    PCB* last_process;
    PCB* ready_queue;
    PCB* current_process;
    int num_processes;
};

typedef struct ProcessScheduler ProcessScheduler;


// 初始化进程调度器
void initialize_scheduler(ProcessScheduler* scheduler) {
    scheduler->ready_queue = NULL;
    scheduler->current_process = NULL;
}

// 添加进程到就绪队列
void add_to_ready_queue(ProcessScheduler* scheduler, PCB* process) {
    if (scheduler->ready_queue == NULL) {
        scheduler->ready_queue = process; // 表头指向第一个进来的 process
        process->link = process; // link 指向下一个要运行的 process
        scheduler->last_process = process;
    } else {
        // process->link = scheduler->ready_queue->link;
        // scheduler->ready_queue->link = process; // 当前添加的 process
        // scheduler->ready_queue = process;
        scheduler->last_process->link = process; // 上一个添加的 process 指向最新添加的 process
        scheduler->last_process = process;
        process->link = scheduler->ready_queue;
    }
}

// 运行进程调度
void run_scheduler(ProcessScheduler* scheduler) {
    scheduler->current_process = scheduler->ready_queue;
    while (scheduler->current_process != NULL) {
        printf("Running process: %s\n", scheduler->current_process->process_name);
        scheduler->current_process->estimated_runtime--;

        if (scheduler->current_process->estimated_runtime == 0) {
            scheduler->current_process->status = 'C';
        }

        if (scheduler->current_process->link == scheduler->current_process) {
            // 只有一个进程在队列中
            break;
        }

        if (scheduler->current_process->link->status == 'C') {
            // 移除已完成的进程
            PCB* temp = scheduler->current_process->link;
            scheduler->current_process->link = temp->link;
            free(temp);
        }

        scheduler->current_process = scheduler->current_process->link;
    }

    printf("All processes have finished execution.\n");
}

int main() {
    // 创建进程
    PCB* p1 = (PCB*)malloc(sizeof(PCB));
    strcpy(p1->process_name, "Process 1");
    p1->arrival_time = 0;
    p1->estimated_runtime = 4;
    p1->status = 'R';

    PCB* p2 = (PCB*)malloc(sizeof(PCB));
    strcpy(p2->process_name, "Process 2");
    p2->arrival_time = 1;
    p2->estimated_runtime = 3;
    p2->status = 'R';

    PCB* p3 = (PCB*)malloc(sizeof(PCB));
    strcpy(p3->process_name, "Process 3");
    p3->arrival_time = 2;
    p3->estimated_runtime = 5;
    p3->status = 'R';

    // 创建进程调度器
    ProcessScheduler scheduler;
    initialize_scheduler(&scheduler);

    // 将进程添加到就绪队列
    add_to_ready_queue(&scheduler, p1);
    add_to_ready_queue(&scheduler, p2);
    add_to_ready_queue(&scheduler, p3);

    // 运行进程调度器
    run_scheduler(&scheduler);

    // 释放内存
    free(p1);
    free(p2);
    free(p3);

    return 0;
}
