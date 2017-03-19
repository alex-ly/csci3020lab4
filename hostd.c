/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
#define MEMORY 1024
#define SHELL "/bin/sh"

// Put global environment variables here

int memory_array[MEMORY];

typedef struct {
	//char name[256];
	int arrival, priority, runtime, memory, printers, scanners, modems, cds;

}proc;

typedef struct {
	int printers, scanners, modems, cds;
}resources;

struct queue {
	proc process;
	struct queue *next;
};

void push(struct queue *head, proc process) {
	struct queue* current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = malloc(sizeof(struct queue));
	current->next->process = process;
	current->next->next = NULL;
}

proc pop(struct queue **head) {
	proc process;
	struct queue *temp = (*head);

	if ((*head) == NULL)return process;
	process = (*head)->process;
	(*head) = temp->next;

	return process;
}

proc remove_any_node(struct queue *node, struct queue *head) {
	struct queue *current = head;
	proc process;
	struct queue *temp;
	if (node == head)return pop(&head);
	while (current->next != NULL) {
		if (current->next == node) {
			temp = current->next;
			process = temp->process;
			current->next = temp->next;
			free(temp);
			return process;
		}
		current = current->next;
	}
	return process;
}

proc my_system(const char *command, struct queue *node)
{
	//int status;
	pid_t pid;

	pid = fork();
	if (pid == 0) {
		/* This is the child process.  Execute the shell command. */
		int wait_time = time(0) + node->process.runtime;

		while (time(0)<wait_time) {
			execl(SHELL, SHELL, "-c", command, NULL);
			//kill(pid,SIGINT);

		}

		//execl (SHELL, SHELL, "-c", command, NULL);
		//node->process.pid = pid;

		//_exit (EXIT_FAILURE);
	}
	else if (pid < 0) {
		/* The fork failed.  Report failure.  */
		//status = -1;
		exit(-1);
	}
	else {
		/* This is the parent process.  Wait for the child to complete.  */
		sleep(5);

		kill(pid, SIGTSTP);
		sleep(10);

		kill(pid, SIGCONT);
		waitpid(pid, 0, 0);
	}

	return node->process;
}


// Define functions declared in hostd.h here

int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //
    
    // Load the dispatchlist
	FILE *file = fopen("dispatchlist.txt", "r");

	char line[256];
	proc process;
	struct queue *head = NULL;
	struct queue *head1 = NULL;
	struct queue *head2 = NULL;
	struct queue *head3 = NULL;

	head = malloc(sizeof(struct queue));
	struct queue *current = head;
	struct queue *current1 = head1;
	struct queue *current2 = head2;
	struct queue *current3 = head3;

	int time = 0;
    // Add each process structure instance to the job dispatch list queue
	while (fscanf(file, "%s", line) != EOF) {
		process.arrival = atoi(line);
		fscanf(file, "%s", line);
		process.priority = atoi(line);
		fscanf(file, "%s", line);
		process.runtime = atoi(line);
		fscanf(file, "%s", line);
		process.memory = atoi(line);
		fscanf(file, "%s", line);
		process.printers = atoi(line);
		fscanf(file, "%s", line);
		process.scanners = atoi(line);
		fscanf(file, "%s", line);
		process.modems = atoi(line);
		fscanf(file, "%s", line);
		process.cds = atoi(line);

		if (process.priority == 0) {
			push(current, process);
			current = current->next;
		}
		else if (process.priority == 1) {
			push(current1, process);
			current1 = current1->next;
		}
		else if (process.priority == 2) {
			push(current2, process);
			current2 = current2->next;
		}
		else {
			push(current3, process);
			current3 = current3->next;
		}

		//time++;
	}

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues

    // Allocate the resources for each process before it's executed
	resources res_avail;
	res_avail.printers = 2;
	res_avail.scanners = 1;
	res_avail.modems = 1;
	res_avail.cds = 2;

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty
     
    return EXIT_SUCCESS;
}
