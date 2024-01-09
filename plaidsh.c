#include "shell.h"

void execute_pipeline(Pipeline *pipeline);
void execute_command(char *cmd, char **args);
int is_builtin_command(char *cmd);
void handle_builtin_command(char *cmd, char **args);
extern char **environ;

/**
 * main - check the code
 *
 * Return: Always 0.
 */
int main()
{
    char *input;
    CList tokens = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    char errmsg[128];

    _puts("Welcome to Plaid Shell!\n");

    while (nread != EOF)
    {
        _puts("\n#? ");
        fflush(stdout);  

        nread = getline(&input, &len, stdin); 
        if(input[nread-1] == '\n') {
            input[nread-1] = '\0';
        }

        if (input[0] == '\0') { 
            continue;
        }
        
        tokens = TOK_tokenize_input(input);

        if (tokens == NULL)
        {
            _puts(errmsg);
        }
        else
        {
            Pipeline *pipeline = parse_tokens(tokens, errmsg);
            if (pipeline == NULL)
            {
                _puts(errmsg);
            }
            else
            {
                execute_pipeline(pipeline);
                Pipeline_free(pipeline);
            }

            TOK_free_tokens(tokens);
        }

       
    }
    return 0;
}

char* find_command_in_path(char* cmd) {
    char *path = getenv("PATH");
    char *path_copy = _strdup(path);
    char *dir = strtok(path_copy, ":");

    if (!path) return NULL; 

    while (dir != NULL) {
        size_t full_path_len = _strlen(dir) + _strlen(cmd) + 2; 
        char *full_path = malloc(full_path_len); 

        if (!full_path) {
            free(path_copy);  
            return NULL;  
        }
        
        _strcpy(full_path, dir);
        _strcat(full_path, "/");
        _strcat(full_path, cmd);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path; 
        }

        free(full_path); 
        dir = strtok(NULL, ":"); 
    }

    free(path_copy); 
    return NULL; 
}

void execute_pipeline(Pipeline *pipeline)
{
    int num_pipes = pipeline->command_count - 1;
    int *pipe_fds;
    int i, j;

    pipe_fds = (int *)malloc(2 * num_pipes * sizeof(int));
    if (pipe_fds == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
   
    for (i = 0; i < num_pipes; i++)
    {
        if (pipe(pipe_fds + i * 2) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < pipeline->command_count; i++)
    {
        Command *cmd = &pipeline->commands[i];
        pid_t pid = fork();
        
        int num_args = CL_length(cmd->args);
        char **args = malloc((num_args + 2) * sizeof(char *)); 
        args[0] = find_command_in_path(cmd->name);
        if (!args[0]) {
            args[0] = cmd->name; 
        }
        for (j = 0; j < num_args; j++)
        {
            Token arg = CL_nth(cmd->args, j); 
            args[j + 1] = arg.value;
        }
        args[num_args + 1] = NULL;
        
        if (is_builtin_command(cmd->name))
        {
            handle_builtin_command(cmd->name, args);
            if (args[0] != cmd->name) free(args[0]);
            free(args);
            continue;
        }

        
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        { 
            
            for (j = 0; j < 2 * num_pipes; j++)
            {
                close(pipe_fds[j]);
            }

            if (execve(args[0], args, environ) == -1)
            {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        if (args[0] != cmd->name) free(args[0]); 
        free(args);
    }

    
    for (i = 0; i < 2 * num_pipes; i++)
    {
        close(pipe_fds[i]);
    }
    
    for (i = 0; i < pipeline->command_count; i++)
    {
        wait(NULL);
    }
}

void execute_command(char *cmd, char **args)
{
    int i;
    char *full_path = find_command_in_path(cmd);
    pid_t pid = fork();
    if (!full_path) {
        full_path = cmd; 
    }

    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        _puts(cmd);
        for (i = 0; args[i] != NULL; i++)
        {
            _puts(args[i]);
        }

        if (execve(full_path, args, environ) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
    if (full_path != cmd) {
        free(full_path);
    }
}

int is_builtin_command(char *cmd)
{
    int i;
    char *builtins[] = {"exit", "cd", "quit", "author", "pwd", NULL};

    for (i = 0; builtins[i] != NULL; i++)
    {
        if (_strcmp(cmd, builtins[i]) == 0)
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
        _puts(author);
    }
    else if (strcmp(cmd, "cd") == 0)
    {

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
            _puts(cwd);
        }
        else
        {
            perror("getcwd");
        }
    }
}
