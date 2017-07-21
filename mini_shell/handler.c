#include <shell.h>
extern pid_t pid;
extern int ch_flag;
extern jobs *head;
extern char buff[MAX_BUFF_SIZE];

void int_handler(int signal)
{
	if (!pid)
	{
		printf("k\n");
		return;
	}
	else
	{
		printf("H\n");
		del_job(pid);
		kill(pid, SIGKILL);
		return;
	}
}

void stop_handler(int signal)
{
	printf("pid - %d\n", pid);
	kill(pid, SIGSTOP);
	return;
}

void child_handler(int signal, siginfo_t *siginfo, void *dum)
{
	char exit[10];
	jobs *jptr = head;

	if (siginfo->si_code == CLD_EXITED)
	{
		sprintf(exit, "%d", siginfo->si_status);

		while (jptr != NULL && jptr->pid != siginfo->si_pid)
			jptr = jptr->next;

		if (jptr != NULL)
		{
			strcpy(jptr->status, "Exit");
			strcat(jptr->status, exit);
		//	check_priority();
		}
		ch_flag = 1;
	}

	else if (siginfo->si_code == CLD_KILLED)
	{
		ch_flag = 1;
	}

	else if (siginfo->si_code == CLD_STOPPED)
	{

		while (jptr != NULL && jptr->pid != siginfo->si_pid)
			jptr = jptr->next;

		if (jptr != NULL)
		{			
			strcpy(jptr->status, "Stopped");
			strcpy(jptr->proc_name, buff);
			//check_priority();
		}

		else
			insert_job(buff, "Stopped");

		ch_flag = 1;
	}

	else if (siginfo->si_code == CLD_CONTINUED)
	{
		ch_flag = 0;

//		while (jptr != NULL && jptr->pid != pid)
	//		jptr = jptr->next;

	}
}
