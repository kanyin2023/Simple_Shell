#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LEN 1024
#define MAX_ARGS 64

ssize_t my_getline(char **line, size_t *len, FILE *stream) 
{
    return getline(line, len, stream);
}

void handle_exit(void)
{
	exit(0);
}

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
	tokens[i] = NULL;
	return (tokens);
}
void execute_command(char **args)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("Fork error");
		exit(1);
	}
	else if (pid == 0)
	{
		/* Child process*/
		if (strchr(args[0], '/') != NULL)
		{
		/* If the command contains a '/' character, try to execute it as a path.*/
		if (execv(args[0], args) == -1)
		{
			perror("Execution error");
			exit(2);
		}
		}
		else
		{
			/*
			 * Otherwise, search for the command in
			 * directories from the PATH environment variable.
			 */
			char *path = getenv("PATH");
			char *path_copy = strdup(path);
			char *dir = strtok(path_copy, ":");
			
			while (dir != NULL)

			{
				char full_path[MAX_INPUT_LEN];
				snprintf(full_path, MAX_INPUT_LEN, "%s/%s", dir, args[0]);
				
				if (access(full_path, X_OK) == 0)
				{
					execv(full_path, args);
					perror("Execution error");
					exit(2);
				}
				dir = strtok(NULL, ":");
			}
			/* If the command was not found in any directory, report an error.*/
			fprintf(stderr, "Command not found: %s\n", args[0]);
			exit(2);
		}
	}
	else
	{
		/* Parent process*/
		wait(NULL);
	}
}
int main(void)	
{
	char *input = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		char **args;
		/* Display a prompt */
		printf("$ ");
		read = my_getline(&input, &len, stdin);
		if (read == -1)
		{
			printf("\n");
			free(input);
			exit(0);
			/* Remove the newline character from the input */
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
			if (strcmp(args[0], "exit") == 0)
			{
				free(args);
				free(input);
				handle_exit();
			}
		execute_command(args);
			free(args);
		}
		free(input);
	}

		return (0);
}
