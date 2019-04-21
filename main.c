/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: abdullah
 *
 * Created on February 1, 2019, 6:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void add(char* ptr);

/*
 * 
 */
int main(int argc, char** argv) {
    char str[100];
    //input string
    char buf[50];
    //prompt messages/errors

    while (true) {
        strcpy(buf, "Input : ");
        write(STDOUT_FILENO, buf, strlen(buf));
        read(STDIN_FILENO, str, sizeof (str));
        //input string
        char* ptr;
        //points to each word in the string
        ptr = strtok(str, " \n");
        
        if (strcmp(ptr, "add") == 0) {
            add(ptr);
        } else if (strcmp(str, "exit") == 0) {
            exit(0);
        } else {
            //buf = "Error: Invalid command : ";
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
        
        /*if (ptr == NULL) {
            char str[100];
            read(STDIN_FILENO, str, sizeof(str));
        }*/
        
        if (ptr != NULL) {
            float i = atof(ptr);
            //parses from string to int
            res += i;
        }
    }
    char buf[50];
    sprintf(buf, "Result is : %.2f\n", res);
    write(STDOUT_FILENO, buf, strlen(buf));
}