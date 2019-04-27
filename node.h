#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
    int pid, ppid, starttime, endtime, timeelapsed;
    struct Node *next;
} node;
void addNode(node *n);
void printNodes();
node *head;


void addNode(node *n)
{

    if ((void*)head == NULL)
    {
        head = n;
    }
    else
    {
        while (head->next != NULL)
        {
            head = head->next;
        }
        head->next = n;
    }
}

node * findNode(int pid){
    while (head->pid != pid) {
        head = head->next;
    }
    return head;
}

void printNodes()
{
    printf("pid\tppid\tstarttime\tendtime\ttimeelapsed\n");
    while (head != NULL)
    {
        printf("%d\t%d\t%d\t%d\t%d\n", head->pid, head->ppid, head->starttime, head->endtime, head->timeelapsed);
        head = head->next;
    }
}
