#include <shell.h>

int pipe_count, indx[MAX_PIPES], bg_flag;

void get_command(char *buff, char ***argv)
{
	pipe_count = 0;
	bg_flag = 0;
	int idx = 1, jdx = 0;
	char *ch = buff;
	*argv = malloc(sizeof (char *) * idx++);
	*argv[idx - 2] = buff;
	indx[jdx++] = idx - 2;
	
	while (*ch != '\0')
	{
		if (*ch == ' ')
		{
			*ch = '\0';
			*argv = realloc(*argv, sizeof (char *) * idx++);
			if (*(ch + 1) == '|')
			{
				pipe_count++;
				(*argv)[idx - 2] = NULL;
				indx[jdx++] = idx - 1;
			}
			else if (*(ch + 1) == '&')
				bg_flag = 1;
			else
				(*argv)[idx - 2] = ch + 1;
		}
		ch++;
	}

	*argv = realloc(*argv, sizeof (char *) * idx++);
	(*argv)[idx - 2] = NULL;
	indx[jdx++] = idx - 2;

	return;
}
