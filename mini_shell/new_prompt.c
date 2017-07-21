#include <shell.h>

char *new_prompt(char *prompt)
{
	char *new = malloc(sizeof (char) * (strlen(prompt) + 1));
	strcpy(new, prompt);
	return new;
}
