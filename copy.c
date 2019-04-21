/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: abdullah
 *
 * Created on February 7, 2019, 11:50 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

void checkError(int fd);
void printTime(int t);

/*
 * 
 */
int main(int argc, char** argv) {
    /*open two files. one for reading and one for writing
     checkError() will check if the argument received is -1 then print out an
     error statement along with the error number
     both files given the O_CREAT flag in case the file does not exist
     S_IRWXU mode so that only the owner can read, write and execute*/
    int fd1 = open("src", O_RDONLY | O_CREAT, S_IRWXU);
    checkError(fd1);

    int fd2 = open("dest", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
//    open() function for O_SYNC
//    int fd2 = open("dest", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRWXU);
    checkError(fd2);
    
    //time() returns the seconds elapsed from unix epoch
    int t = time(NULL);
    
    char buff[1];
//    char buff[1000];
    
    //rb is the number of bytes returned from the read call
    int rb = read(fd1, buff, sizeof (buff));
    checkError(rb);
    int fs;
    //if rb is 0 it indicates end of file
    while (rb != 0) {
        int wb = write(fd2, buff, rb);
        checkError(wb);
//        fs = fsync(fd2);
//        checkError(fs);
//        fs = fdatasync(fd2);
//        checkError(fs);
        rb = read(fd1, buff, sizeof (buff));
        checkError(rb);
    }
    
    //calculate time difference and print to console
    t = time(NULL) - t;
    printTime(t);

    if (close(fd1) == -1) {
        perror("Close Error on fd1 ");
    }

    if (close(fd2) == -1) {
        perror("Close Error on fd2 ");
    }

    return (EXIT_SUCCESS);
}

void checkError(int a) {
    if (a == -1) {
        char buf[10];
        sprintf(buf, "Error %d : ", errno);
        write(STDOUT_FILENO, buf, strlen(buf));
        perror("");
    }
}

void printTime(int t) {
    char buf[50];
    sprintf(buf, "Time is : %d seconds\n", t);
    write(STDOUT_FILENO, buf, strlen(buf));
}