#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFF_SIZE 64
#define MAX_PIPES 6

typedef struct Data
{
	pid_t pid;
	short proc_no;
	char status[8];
	char prior;
	char proc_name[10];
	struct Data *next, *prev;
}jobs;

void get_command(char *buff, char ***argv);
char *new_prompt(char *prompt);
char *get_shell(void);
void int_handler(int signal);
void stop_handler(int signal);
void child_handler(int signal,  siginfo_t *ptr, void *ptr_1);
void check_priority(void);
void print_jobs(void);
void del_job(pid_t pid);
void insert_job(char *name, char *str);
int fg_bg(int op);

#endif
