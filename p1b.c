#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//question 1-b




void zombie_process() {
    //forking one child process
    pid_t pid = fork();

    if (pid == 0) { //for child process
        
        wait(NULL);
        exit(0);

    }
    sleep(8); //for program to be remain at least 5 seconds (found the problem, last minute change)
    kill(pid, SIGSEGV);

    printf("Parent: Child process changed to Zombie process");

}

int main(int argc, char *argv[]) {

    zombie_process();

}


