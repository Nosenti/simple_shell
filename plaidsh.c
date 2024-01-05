#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>

// Declarations
void execute_pipeline(Pipeline *pipeline);
void execute_command(char *cmd, char **args);
int is_builtin_command(char *cmd);
void handle_builtin_command(char *cmd, char **args);

// Main
int main(int argc, char *argv[])
{
    char *input;
    CList tokens = NULL;
    char errmsg[128];

    printf("Welcome to Plaid Shell!\n");

    while (1)
    {
        errmsg[0] = '\0';

        input = readline("\n#? ");
        if (input == NULL || strcasecmp(input, "exit") == 0 || strcasecmp(input, "quit") == 0)
        {
            free(input);
            break;
        }

        if (*input == '\0')
        {
            free(input);
            continue;
        }

        add_history(input);
        tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

        if (tokens == NULL)
        {
            fprintf(stderr, "%s\n", errmsg);
        }
        else
        {
            Pipeline *pipeline = parse_tokens(tokens, errmsg, sizeof(errmsg));
            if (pipeline == NULL)
            {
                fprintf(stderr, "Parser error: %s\n", errmsg);
            }
            else
            {
                execute_pipeline(pipeline);
                Pipeline_free(pipeline);
            }

            TOK_free_tokens(tokens);
        }

        free(input);
    }
    return 0;
}

// Implementations of helper functions

void execute_pipeline(Pipeline *pipeline)
{
    int num_pipes = pipeline->command_count - 1;
    int pipe_fds[2 * num_pipes];

    // creating pipes
    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipe_fds + i * 2) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < pipeline->command_count; i++)
    {
        Command *cmd = &pipeline->commands[i];

        // Convert arguments list to array format
        int num_args = CL_length(cmd->args);
        char **args = malloc((num_args + 2) * sizeof(char *)); 
        args[0] = cmd->name;
        for (int j = 0; j < num_args; j++)
        {
            Token arg = CL_nth(cmd->args, j); 
            args[j + 1] = arg.value;
        }
        args[num_args + 1] = NULL; // NULL-terminate the arguments array

        // For built-in command, handle it directly without forking
        if (is_builtin_command(cmd->name))
        {
            handle_builtin_command(cmd->name, args);
            free(args);
            continue;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        { // Child process
            // If not the first command, then this process should get input from the previous pipe
            if (i > 0)
            {
                dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
            }

            // If not the last command, then this process should output to the next pipe
            if (i < num_pipes)
            {
                dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close fds in the child
            for (int j = 0; j < 2 * num_pipes; j++)
            {
                close(pipe_fds[j]);
            }

            if (execvp(cmd->name, args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }

        free(args);
    }

    // Close fds in the parent
    for (int i = 0; i < 2 * num_pipes; i++)
    {
        close(pipe_fds[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < pipeline->command_count; i++)
    {
        wait(NULL);
    }
}

void execute_command(char *cmd, char **args)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        printf("Executing command: %s\n", cmd);
        for (int i = 0; args[i] != NULL; i++)
        {
            printf("arg[%d]: %s\n", i, args[i]);
        }

        if (execvp(cmd, args) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

int is_builtin_command(char *cmd)
{
    // List of built-in commands
    char *builtins[] = {"exit", "cd", "quit", "author", "pwd", NULL};

    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(cmd, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void handle_builtin_command(char *cmd, char **args)
{
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
    {
        exit(0);
    }
    else if (strcmp(cmd, "author") == 0)
    {
        char *author = "Innocent Ingabire";
        printf("%s\n", author);
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        // const char *dir = args[1] ? args[1] : getenv("HOME");

        const char *dir = getenv("HOME");

        if(args[1] && strcmp(args[1], "~") != 0){
            dir = args[1];
        }

        if (chdir(dir) != 0)
        {
            perror("chdir");
        }
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("getcwd");
        }
    }
}
