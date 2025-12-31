#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    char buff[1024];
    char *args[64];
    char *token;

    while(1){
        printf("shell: ");
        if(fgets(buff,sizeof(buff),stdin) == NULL){
            break;
        }

        int i =0;
        char *token = strtok(buff, " ");
        while(token != NULL){
            args[i] = token;
            args[i+1] = NULL;
            i++;
            token = strtok(NULL, " ");
        }
        if(args[0] == NULL){
            continue;
        }
        int pid;
        pid = fork();
        if(pid == 0){
            if(execvp(args[0], args)== -1){
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }else if(pid > 0){
            wait(NULL);
        }else{
            perror("fork failed");
        }
    }
    return 0;
}