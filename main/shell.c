#include <shell.h>
extern int pipe_count, indx[MAX_PIPES], bg_flag;
pid_t pid;
int ch_flag;
char buff[MAX_BUFF_SIZE], string[100];
jobs *head = NULL;

/* main function */
int main()
{

	char *prompt = "msh", **argv, shell[PATH_MAX];
	int wstatus, idx, jdx, proc;
	struct sigaction int_act, stop_act, int_oldact, stop_oldact, child_act, child_oldact;
	memset(&int_act, 0, sizeof(int_act));
	memset(&stop_act, 0, sizeof(int_act));
	memset(&int_oldact, 0, sizeof(int_act));
	memset(&stop_oldact, 0, sizeof(int_act));
	memset(&child_oldact, 0, sizeof(int_act));
	memset(&child_oldact, 0, sizeof(int_act));


	int_act.sa_handler = int_handler;
	sigaction(SIGINT, &int_act, &int_oldact);

	child_act.sa_sigaction = child_handler;
	child_act.sa_flags = SA_SIGINFO|SA_RESTART;

	sigaction(SIGCHLD, &child_act, &child_oldact);
	getcwd(shell, PATH_MAX);
	
	while (1)
	{	
		if (!pid)
		{
			memset(&stop_act, 0, sizeof(int_act));
			stop_act.sa_handler = SIG_IGN;
			sigaction(SIGTSTP, &stop_act, &stop_oldact);
		}

		printf("%s : ", prompt);
		memset(&buff, '\0', sizeof (buff));
		scanf("%[^\n]", buff);
		__fpurge(stdin);
		if (!strlen(buff))
		{
			continue;
		}

		memset(&stop_act, 0, sizeof(int_act));
		stop_act.sa_handler = stop_handler;
		sigaction(SIGTSTP, &stop_act, &stop_oldact);

		if (!strncmp(buff, "exit", 4))
		{
			_exit(0);
		}
		else if (!strncmp(buff, "PS1=", 4))
		{
			prompt = new_prompt(buff + 4);
			continue;
		}
		else if (!strncmp(buff, "echo $?", 7))
		{
			if (WIFEXITED(wstatus))
			{
				printf("%d\n", WEXITSTATUS(wstatus));
			}
			continue;
		}
		else if (!strncmp(buff, "echo $$", 7))
		{
			printf("%d\n", getpid());
			continue;
		}
		else if (!strncmp(buff, "echo $SHELL", 11))
		{
			printf("%s\n", shell);
			continue;
		}
		else if (!strncmp(buff, "pwd", 3))
		{
			printf("%s\n", getcwd(NULL, PATH_MAX));
			continue;
		}
		else if (!strncmp(buff, "cd", 2))
		{
			if (buff[3] == '\0')
				strcpy (&buff[3], "/home/lebowski/");

			chdir (&buff[3]);
			continue;
		}
		else if (!strncmp(buff, "fg", 2))
		{
			fg_bg(1);
			continue;
		}
		else if (!strncmp(buff, "bg", 2))
		{
			fg_bg(2);
			continue;
		}
		else if (!strncmp(buff, "jobs", 4))
		{
			fg_bg(3);
			continue;
		}

		/*else
			{
			printf("%s: Command not found\n", buff);
			continue;
			}*/

		get_command(buff, &argv);

		/*printf("Pipe - %d\n", pipe_count);
			for (kdx = 0; argv[kdx] != NULL; kdx++)
			{
			printf("%p\t%s\n", argv[kdx], argv[kdx]);
			}*/

		proc = pipe_count + 1;
		int pd[pipe_count][2];

		for (idx = 0; idx < pipe_count; idx++)
		{
			// creating pipes
			if (pipe(pd[idx]) == -1)
			{
				perror("pipe");
				return 1;
			}
		}

		/*for (kdx = 0; kdx < proc; kdx++)
			printf("idx - %d\n", indx[kdx]);*/

		for (jdx = 0; jdx < proc; jdx++)
		{
			// creating child
			switch (pid = fork())
			{
				//error handling
				case -1:
					perror("fork");
					return 2;

					//child process
				case 0:
					if (pipe_count)
					{
						for (idx = 0; idx < proc - 1; idx++)
						{	
							// closing unwanted write descriptors
							if (idx == jdx)
								dup2(pd[idx][1], 1);
							else	
								close(pd[idx][1]);
						}

						if (jdx)
							dup2(pd[jdx - 1][0], 0);
					}

					//printf("%s\n", argv[indx[jdx]]);
					// performing exec
					if (execvp(argv[indx[jdx]], argv + indx[jdx]) == -1)
					{
						perror("exec failure");
						_exit(4);
					}

					break;

					// parent process 
				default:
					if (pipe_count)
					{
						for (idx = 0; idx < proc; idx++)
							close(pd[idx][1]);
					}

					if (!bg_flag) // && ch_flag)
					{
//						while ((waitpid(pid, &wstatus, 0) != -1) && proc--);
						while (!ch_flag);
						ch_flag = 0;
					}
					else
					{
						insert_job(buff, "Running");
						pid = 0;
					}
			
					continue;
			}
		}
	}
}
