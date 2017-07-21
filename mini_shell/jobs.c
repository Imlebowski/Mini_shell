#include <shell.h>

extern jobs *head;
extern pid_t pid;
extern char buff[MAX_BUFF_SIZE];

void insert_job(char *name, char *str)
{ 
	if (head == NULL)
	{ 
		head = malloc(sizeof (jobs));
		head->pid =  pid;
		strcpy(head->status, str);
		strcpy(head->proc_name, name);
		head->proc_no = 1;
		head->prior = '+';
		head->next = NULL;
		head->prev = NULL;
	}

	else
	{ 
		jobs *ptr;

		ptr = head;


		while (ptr->next != NULL)
			ptr = ptr->next;

		ptr->next = malloc(sizeof (jobs));
		ptr->next->prev = ptr;
		ptr->next->next = NULL;

		ptr->next->proc_no = ptr->proc_no + 1;
		strcpy(ptr->next->status, str);
		strcpy(ptr->next->proc_name, name);
		ptr->next->prior = ' ';
		ptr->next->pid = pid;


		check_priority();
	}
}


void del_job(pid_t pid)
{
	jobs *ptr;

	ptr = head;

	while (ptr != NULL && ptr->pid != pid)
		ptr = ptr->next;

	if (ptr == NULL)
		printf("Process not present\n");

	else
	{
		if (ptr != head)
		{
			ptr->prev->next = ptr->next;

			if (ptr->next != NULL)
				ptr->next->prev = ptr->prev;
		}

		else
			head = head->next;

		free(ptr);
	}

	if (head != NULL)
		check_priority();
}


void print_jobs(void)
{
	if (head == NULL)
		return;

	else
	{
		jobs *ptr;

		ptr = head;

		while (ptr != NULL)
		{
			printf("[%d]%c  %s %s\n", ptr->proc_no, ptr->prior, ptr->status, ptr->proc_name);
			ptr = ptr->next;
		}
	
		ptr = head;
		while (ptr != NULL)
		{
			if (strncmp(ptr->status, "Exit", 4) == 0)
				del_job(ptr->pid);

			ptr = ptr->next;
		}
	}

}



void check_priority(void)
{

	int flag1 = 0;
	jobs *ptr = head;

	if (ptr->next == NULL)
	{
		ptr->prior = '+';
		return;
	}

	while (ptr->next !=  NULL)
	{
		ptr->prior = ' ';
		ptr = ptr->next;
	}
	ptr->prior = ' ';

	jobs *last = ptr;

	while (ptr->prev != NULL && strcmp(ptr->status, "Stopped") != 0)
	{
		ptr = ptr->prev;
	}

	if (ptr != NULL)
	{
		ptr->prior = '+';

		ptr = ptr->prev;
		while (ptr->prev != NULL && strcmp(ptr->status, "Stopped") != 0)
		{
			ptr = ptr->prev;
		}

		if (ptr != NULL)
			ptr->prior = '-';

		else
		{
			while (strcmp(last->status, "Running") != 0)
			{
				last = last->prev;
			}

			if (last != NULL)
				last->prior = '-';

		}
	}

	else
	{ 
		while (last->prev != NULL && strcmp(last->status, "Running") != 0)
		{
			last = last->prev;
		}

		if (last != NULL)
		{
			last->prior = '+';

			last = last->prev;

			while (last->prev != NULL && strcmp(last->status, "Running") != 0)
			{
				last = last->prev;
			}

			if (last != NULL)
				last->prior = '-';
		}
	}
}












