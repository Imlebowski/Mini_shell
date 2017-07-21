#include <shell.h>
extern pid_t pid;
extern int ch_flag;
extern jobs *head;

int fg_bg(int op)
{
	jobs *ptr = head;

	if (op == 1)
	{
		while (ptr != NULL && ptr->prior != '+')
			ptr = ptr->next;

		printf("prior - %p\n", ptr);

		if (ptr != NULL)
		{ pid = ptr->pid;
			strcpy(ptr->status, "Running");
//			check_priority();
			printf("proc - %s\n",ptr->proc_name);
			kill(ptr->pid, SIGCONT);
			while (!ch_flag);
			ch_flag = 0;
		}
		else
		{
			printf("NO process\n");
		}

	}

	else if (op == 2)
	{
		while (ptr != NULL && ptr->prior != '+')
			ptr = ptr->next;

		if (ptr != NULL && !strcmp(ptr->status, "Stopped"))
		{
			strcpy(ptr->status, "Running");
			//check_priority();
			kill(ptr->pid, SIGCONT);
		}
		else
		{
			printf("NO process\n");
		}

		printf("i m out\n");
	}

	else if (op == 3)
	{
		print_jobs();
	}

	else
		return 0;

	return 1;

}
