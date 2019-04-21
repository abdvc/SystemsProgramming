/* 
 * File:   main.c
 * Author: abdullah
 *
 * Created on February 26, 2019, 9:11 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <errno.h>
#include <error.h>

/*
 * 
 */

void add(char * ptr);
void sum(char str[]);

int main(int argc, char** argv) {
    char str[100];
    //input string
    char buf[100];
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
            exit(0);
        }

        ptr = strtok(str, " \n");

        if (strcmp(ptr, "add") == 0) {
            add(ptr);
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

void add(char * ptr) {
    char buf[100];
    //input buffer
    char str[500];
    //main string which will be a concat of all the input strings
    float res = 0;
    //result of the addition
    ptr = strtok(NULL, "\n");
    //tokenise remainder of the string and copy it into str
    strcpy(str, ptr);

    while (str[strlen(str) - 1] != ';') {
        int rd = read(STDIN_FILENO, buf, 100);
        
        if (rd == -1) {
            perror("Read ");
            exit(0);
        }
        
        strcat(str, " ");
        strcat(str, buf);
        //concat input buffer to string
        str[strlen(str) - 1] = '\0';
        //last character of string will be \n because of enter key, 
        //this will change it to null char
        memset(buf, '\0', strlen(buf));
        //clear input buffer
    }

    char * ptr2 = strtok(str, ";");
    //tokenise each list
    char * ptr3;
    //pointer to tokenise each number in the list
    char buff[50];
    //output message buffer
    char strings[100][100];
    //array of strings to store individual lists
    int i = 0;

    while (ptr2 != NULL) {
        strcpy(strings[i], ptr2);
        //populate array of strings with lists

        ptr2 = strtok(NULL, ";");
        i++;
    }

    for (int j = 0; j < i; j++) {
        ptr3 = strtok(strings[j], " ");
        while (ptr3 != NULL) {
            float a = atof(ptr3);
            res += a;
            ptr3 = strtok(NULL, " ");
        }
        sprintf(buff, "Result : %.2f\n", res);
        int wr = write(STDOUT_FILENO, buff, strlen(buff));
        
        if (wr == -1) {
            perror("Write ");
            exit(0);
        }
        
        res = 0;
    }
}