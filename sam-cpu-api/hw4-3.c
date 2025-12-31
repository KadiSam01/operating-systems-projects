#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { //child 1
         int rc2 = fork(); //child 2 is created from child 1
        if (rc2 < 0) {
        // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        }else if (rc2 == 0) {
            // child (new process
            printf("hello, I am child (pid:%d)\n", (int) getpid());
        sleep(1);
        exit(0);
        } else {
            int wc2 = waitpid(rc2, NULL, 0); //parents waits for child2 to run
            // parent goes down this path (original process)
            //int wc2 = wait(NULL);
            printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
            rc2, wc2, (int) getpid());
        }
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
	sleep(1);
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
	       rc, wc, (int) getpid());
    }
    //second child
    return 0;
}
