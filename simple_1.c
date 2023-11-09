#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LEN 1024
#define MAX_ARGS 64

char **split_line(char *line)
{
	int i = 0;
	char **tokens = malloc(MAX_ARGS * sizeof(char *));
	char *token;
	if (tokens == NULL)
	{
		perror("Memory allocation error");
		exit(1);
	}
	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, " \t\n");
	}
	{
		tokens[i] = NULL;
		return (tokens);
	}
	int main(void)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t read;
	while (1)
	{
		char **args;
		pid_t pid;
		/* Display a prompt */
		read = getline(&input, &len, stdin);
		if (read == -1)
		{
			/*Handle "end of file" (Ctrl+D)*/
			printf("\n");
			free(input);
			exit(0);
		}
		/*Remove the newline character from the input*/
		if (input[read - 1] == '\n')
		{
			input[read - 1] = '\0';
		}
		args = split_line(input);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}
		/* new code to handle built-in exit command */
		if (strcmp(args[0], "exit") == 0)
		{
			free(args);
			free(input);
			exit(0);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Fork error");
			exit(1);
		}
		else if (pid == 0)
	{
			/* Child process*/
			if (execvp(args[0], args) == -1)
			{
				perror("Command not found");
			}
			exit(2);
		}
		else
		{
			/* Parent process*/
			wait(NULL);
			/*Wait for the child to complete*/
		}
		free(args);
	}
	free(input);
	return (0);
}
