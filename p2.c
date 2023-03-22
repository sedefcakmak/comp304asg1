#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
//defining macros for read and write end and buffer size
#define READ_END 0
#define WRITE_END 1
#define BUF_SIZE 30

//question 2
void print_maxminavg(int n, double *arr) {
    double max = arr[0];
    double min = arr[0];
    double sum = 0.0;
    //finding max min and average of the time array
    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
        if (arr[i] < min) {
            min = arr[i];
        }
        sum += arr[i];
    }

    double avg = sum / n;

    printf("Max: %.2f millis\n", max);
    printf("Min: %.2f millis\n", min);
    printf("Average: %.2f millis\n", avg);
}

//benchmarking
void benchmark(int n, char *cmd) {

    int pipes[n][2]; //pipe
    double exec_times[n]; //to hold execution times of all


    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork(); //forking n child

        //child processes
        if (pid == 0) {


            close(pipes[i][READ_END]); //if child process read-end of pipe is closed

            //initializing time
            struct timeval start, end;
            gettimeofday(&start, NULL);

            //forking children of children
            pid_t pid2 = fork();


            //child of child
            if (pid2 == 0) {
                //Grandchildren executes commands and writes to /dev/null
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);

                execlp(cmd, cmd, NULL);

                perror(cmd);
                exit(EXIT_FAILURE);

            }
                // Parent of grandchild which is child process
            else {
                //endtime is taken and elapsed time is calculated
                double elapsed;
                wait(NULL);
                gettimeofday(&end, NULL);
                elapsed = (((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_usec - start.tv_usec) / 1000.0));



                // Elapsed time is sent to parent process
                close(pipes[i][READ_END]);  //closing the read end
                write(pipes[i][WRITE_END], &elapsed, sizeof(double));
                close(pipes[i][WRITE_END]);//closing the write end of the pipe
                exit(EXIT_SUCCESS);
            }
        }
            // Parent process
        else {
            close(pipes[i][WRITE_END]); //parent process only closes the write end of the pipe
        }
    }

    // Parent process receives execution times from children
    for (int i = 0; i < n; i++) {
        double exec_time;
        read(pipes[i][READ_END], &exec_time, sizeof(double)); //reading exec times from the pipe
        exec_times[i]=exec_time;
        printf("Child %d Executed in %.2f millis\n", i + 1, exec_time);
        close(pipes[i][READ_END]);
    }
    //at the end calculating max min avg times and printing
    print_maxminavg(n, exec_times);
}

int main(int argc, char *argv[]) {


    int n = atoi(argv[1]); //n child processes
    char *cmd = argv[2]; //command name

    benchmark(n, cmd);

    return EXIT_SUCCESS;
}
