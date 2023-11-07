#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <fcntl.h>

void p_daughter(){
	while(1){
		int fd = open("file.txt", O_RDWR);
		lockf(fd, F_LOCK, 0);
		printf("daughter with ID %d \n", getpid());
		lockf(fd, F_ULOCK, 0);
		close(fd);
	}
}

void p_son(){
	while(1){
		int fd = open("file.txt", O_RDWR);
		lockf(fd, F_LOCK, 0);
		printf("son with ID %d \n", getpid());
		lockf(fd, F_ULOCK, 0);
		close(fd);
	}
}

void p_parent(){
	while(1){
		int fd = open("file.txt", O_RDWR);
		lockf(fd, F_LOCK, 0);
		printf("parent with ID %d \n", getpid());
		lockf(fd, F_ULOCK, 0);
		close(fd);
	}
}

int main()
{
	pid_t child_pid1 = fork(); // make p1

	if (child_pid1 < 0) { // if p1 create fail
		exit(1);
	}
	else if (child_pid1 == 0) { // if is p1
		p_daughter();
	}
	else { // if is parent process
		pid_t child_pid2 = fork();
		if ( child_pid2 == 0) {
			p_son();
		}
		if ( child_pid2 > 0) {
			p_parent();
		}
	}
	return 0;
}





	    
