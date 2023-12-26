#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(){
    char *input;

    while(1){

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
    }
    return 0;
}