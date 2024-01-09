#include "shell.h"




/**
 * main - check the code
 *
 * Return: Always 0.
 */

int main(void)
{
    char *input;
    CList tokens = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    char errmsg[128];

    while (1)
    {
        _puts("#cisfun$ ");
        fflush(stdout);

        nread = getline(&input, &len, stdin);
        if (nread == -1)
        {
            if (errno == EINTR) 
            {
                continue;
            }
            else
            {
                break; 
            }
        }

        if(input[nread-1] == '\n')
        {
            input[nread-1] = '\0';
        }

        if (input[0] == '\0') 
        { 
            continue;
        }
        
        tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

        if (tokens == NULL)
        {
            _puts( errmsg);
        }
        else
        {
            Pipeline *pipeline = parse_tokens(tokens, errmsg, sizeof(errmsg));
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

        free(input);
        input = NULL;
        len = 0;
    }
    return 0;
} 



