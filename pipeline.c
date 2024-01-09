/*
 * pipeline.c
 *
 * Implementation of the pipeline data structure.
 *
 * Author: Innocent Ingabire
 */
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



Pipeline *Pipeline_new() {
    Pipeline *pipeline = malloc(sizeof(Pipeline));
    pipeline->commands = NULL;
    pipeline->command_count = 0;
    pipeline->input_file = NULL;
    pipeline->output_file = NULL;
    return pipeline;
}

void Pipeline_free(Pipeline *pipeline) {
    int i;
    
    if (pipeline == NULL) return;

    for (i = 0; i < pipeline->command_count; ++i) {
        Command *command = &pipeline->commands[i];
        
        while (CL_length(command->args) > 0) {
            Token arg = CL_pop(command->args);
            if (arg.value != NULL) {
                free(arg.value);
                arg.value = NULL; 
            }
        }
        CL_free(command->args); 
        if (command->name != NULL) {
            free(command->name); 
            command->name = NULL; 
        }
    }

    if (pipeline->commands != NULL) {
        free(pipeline->commands);
        pipeline->commands = NULL;
    }

    if (pipeline->input_file != NULL) {
        free(pipeline->input_file);
        pipeline->input_file = NULL;
    }

    if (pipeline->output_file != NULL) {
        free(pipeline->output_file);
        pipeline->output_file = NULL; 
    }

    free(pipeline);
}


void Pipeline_set_input_file(Pipeline *pipeline, char *filename) {
    if (pipeline->input_file != NULL) free(pipeline->input_file);
    pipeline->input_file = _strdup(filename);
}

void Pipeline_set_output_file(Pipeline *pipeline, char *filename) {
    if (pipeline->output_file != NULL) free(pipeline->output_file);
    pipeline->output_file = _strdup(filename);
}

void Pipeline_add_command(Pipeline *pipeline, char *command_name) {
    pipeline->commands = realloc(pipeline->commands, sizeof(Command) * (pipeline->command_count + 1));
    pipeline->commands[pipeline->command_count].name = _strdup(command_name);
    pipeline->commands[pipeline->command_count].args = CL_new();
    pipeline->command_count++;
}

void Pipeline_add_argument(Pipeline *pipeline, char *argument) {
    Command *last_command;
    Token argToken;

    if (pipeline == NULL || pipeline->command_count == 0 || argument == NULL) return;
    last_command = &pipeline->commands[pipeline->command_count - 1];
    argToken.type = TOK_WORD;
    argToken.value = _strdup(argument);

    CL_append(last_command->args, *(CListElementType *)&argToken);
}



void Pipeline_print(const Pipeline *pipeline) {
    int i;
    int j;
    if (pipeline == NULL) return;

    for (i = 0; i < pipeline->command_count; ++i) {
        Command *cmd = &pipeline->commands[i];
        _puts(cmd->name);

        for (j = 0; j < CL_length(cmd->args); ++j) {
            Token arg = CL_nth(cmd->args, j);
            _puts(arg.value);
        }

        _puts("\n");
    }
    
    if (pipeline->input_file) {
        _puts(pipeline->input_file);
        _puts("\n");
    }
    if (pipeline->output_file) {
        _puts(pipeline->output_file);
         _puts("\n");
    }
}

