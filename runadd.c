/* 
 * File:   main.c
 * Author: abdullah
 *
 * Created on February 22, 2019, 7:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>

/*
 * 
 */

void add(char* ptr);
void run(char* ptr);

int main(int argc, char** argv) {
    char str[100];
    //input string
    char buf[50];
    //prompt messages

    while (true) {
        char * ptr;
        strcpy(buf, "Input : ");
        int wr = write(STDOUT_FILENO, buf, strlen(buf));

        if (wr == -1) {
            perror("Write ");
            exit(0);
        }

        int rd = read(STDIN_FILENO, str, 100);

        if (rd == -1) {
            perror("Read ");
            //            printf("%d", errno);
            exit(0);
        }

        ptr = strtok(str, " \n");

        if (strcmp(ptr, "add") == 0) {
            add(ptr);
        } else if (strcmp(ptr, "run") == 0) {
            run(ptr);
        } else if (strcmp(ptr, "exit") == 0) {
            exit(0);
        } else {
            strcpy(buf, "Error: Invalid command : ");
            strcat(buf, ptr);
            strcat(buf, "\n");
            write(STDOUT_FILENO, buf, strlen(buf));
        }
    }

    return (EXIT_SUCCESS);
}

void add(char* ptr) {
    float res = 0;
    //result for the calculations

    while (ptr != NULL) {
        //if ptr is null it means the entire string has been tokenized
        ptr = strtok(NULL, " ");
        //null passed so the string continues to tokenize
        //delimiter is " "

        if (ptr != NULL) {
            float i = atof(ptr);
            //parses from string to int
            if (i == 0) {
                break;
            }
            res += i;
        }
    }
    char buf[50];
    sprintf(buf, "Result is : %.2f\n", res);
    write(STDOUT_FILENO, buf, strlen(buf));
}

void run(char* ptr) {
    int pid = fork();
    //fork a new process
    
    char buf[50];
    //buffer to build the path string for execl

    if (pid == -1) {
        perror("Fork ");
    } else if (pid == 0) {
        ptr = strtok(NULL, " \n");
        //tokenize the string further
        
        sprintf(buf, "/usr/bin/%s", ptr);
        //build the string
        int ex = execl(buf, ptr, NULL);
        //execute the process
        
        if (ex == -1) {
            perror("Exec");
        }
    }
}