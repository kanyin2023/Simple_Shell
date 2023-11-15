#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

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
extern char **environ;

char *input = NULL;
size_t len = 0;

char **split_line(void)
{
	int i = 0;
	char **tokens;
	char *token;

	tokens = malloc(sizeof(char *) * (i + 1));

	if (tokens == NULL)
	{
		perror("Memory allocation error");
		exit(1);
	}
	while ((token = strtok(NULL, " \t\n")) != NULL)
	{
		tokens[i] = token;
		i++;
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

void print_enviroment (void)
{
	char ** env = environ;
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

int main(void)	
{
	char *input = NULL;
	size_t len = 0;

 
	while (1)
	{
		char **args;
		/* Display a prompt */
		printf("$ ");
		my_getline(&input, &len, stdin);
		if (getline(&input, &len, stdin ) == -1)
		{
			printf("\n");
			handle_exit();
		}
			args = split_line(input);
			if (args == NULL)
			{
				if (strcmp(args[0], "exit") == 0)
			{
				free(args);
				free(input);
          				handle_exit();
			}
				else if (strcmp(args[0], "env") == 0){
					print_enviroment ();
					free(args);
					continue;
				}
				else {
					execute_command(args);
				}
			}
			free(args);
		}

		free(input);
		return (0);
}
