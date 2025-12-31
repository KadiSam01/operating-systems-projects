#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    int x = 100; //the child inherits 100 from the parents
    int rc = fork(); //creates a new processs from the parent process
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        x=2; //2 in the child but 100 stays in the parents and independent
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
	    rc, (int) getpid());
        x=5;
    }
    
}