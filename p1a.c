#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//question 1 a

void create_processes(int level, int n) {
    //creating n child processes
    for (int i=0; i<n; i++) {

        int pid = fork();
         if (pid == 0) {
            // child process
            printf("Process ID: %d, Parent ID: %d, Level: %d \n ", getpid(), getppid(), level++);

        } else {
            // parent process waits for the child processes
            wait(NULL);
        }
    }
    exit(0);
}


int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    int level=0;

    printf("Main Process ID: %d, level: %d\n", getpid(), level);
    create_processes(1, n);


}

