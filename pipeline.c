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


void Pipeline_set_input_file(Pipeline *pipeline, const char *filename) {
    if (pipeline->input_file != NULL) free(pipeline->input_file);
    pipeline->input_file = strdup(filename);
}

void Pipeline_set_output_file(Pipeline *pipeline, const char *filename) {
    if (pipeline->output_file != NULL) free(pipeline->output_file);
    pipeline->output_file = strdup(filename);
}

void Pipeline_add_command(Pipeline *pipeline, const char *command_name) {
    pipeline->commands = realloc(pipeline->commands, sizeof(Command) * (pipeline->command_count + 1));
    pipeline->commands[pipeline->command_count].name = strdup(command_name);
    pipeline->commands[pipeline->command_count].args = CL_new();
    pipeline->command_count++;
}

void Pipeline_add_argument(Pipeline *pipeline, const char *argument) {
    
    Token argToken;
    Command *last_command = &pipeline->commands[pipeline->command_count - 1];
    
    if (pipeline == NULL || pipeline->command_count == 0 || argument == NULL) return;

    
    argToken.type = TOK_WORD;
    argToken.value = strdup(argument);

    CL_append(last_command->args, *(CListElementType *)&argToken);
}


