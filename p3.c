#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>



//question 3

//firstly did that with shared memory as suggested in the title then deleted as it was not needed.

void parent_process(int n, int x) {
    char input[1000];
    int arr[1000];

    pid_t pid[n]; //pid array


    //reading the input with fgets and store in array
    int i = 0;
    while (fgets(input, 1000, stdin) != NULL) {
        arr[i++] = atoi(input);
    }


    // i=number of numbers in the array

    int slice = i / n;
    int rem = i % n;
    int init = 0;

    //changing end iteratively to distribute elements among children
    for (int j = 0; j < n; j++) {
        int end = init + slice;

        if (j < rem) {
            end++;
        }

        pid[j] = fork();

        if (pid[j] == 0) {
            //searching for x
            for (int k = init; k < end; k++) { //disjointed
                if (arr[k] == x) {

                    printf("The index of %d is %d", x, k);
                    exit(0); //if found exit 0
                }

            }

            exit(1); //if not found

        }
        init = end;

    }

    pid_t cur_pid;

    int status;
    while ((cur_pid = wait(&status)) > 0) {

        // check if x is found
        if (status == 0) {

            // kill all children and exit with status 0
            for (int i = 0; i < n; i++) {
                kill(pid[i], SIGSEGV);


            }
            exit(0);

        }

    }

}


int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); //n child processes

    int x = atoi(argv[2]); //number to be found

    parent_process(n, x);


}

