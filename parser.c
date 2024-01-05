/*
 * parser.c
 *
 * Code that implements a parser of tokens and build the pipeline structure
 *
 * Author: Innocent Ingabire <iingabir@andrew.cmu.edu>
 */

#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void handle_token(CList tokens, Pipeline *pipeline, int *command_index, char *errmsg, size_t errmsg_sz);

// Main function to parse tokens into a pipeline
Pipeline *parse_tokens(CList tokens, char *errmsg, size_t errmsg_sz) {
    if (tokens == NULL) return NULL;

    Pipeline *pipeline = Pipeline_new();
    int command_index = 0; 
    while (CL_length(tokens) > 0) {
        handle_token(tokens, pipeline, &command_index, errmsg, errmsg_sz);
        if (*errmsg != '\0') { 
            Pipeline_free(pipeline);
            CL_free(tokens); 
            return NULL;
        }
    }
    return pipeline;
}


// Helper function to process each token and update the pipeline
static void handle_token(CList tokens, Pipeline *pipeline, int *command_index, char *errmsg, size_t errmsg_sz) {

    if (CL_length(tokens) == 0) {
        snprintf(errmsg, errmsg_sz, "Empty token list");
        return;
    }

    Token token = CL_nth(tokens, 0);

    // static int command_index = 0; 

    switch (token.type) {
        case TOK_WORD:
        case TOK_QUOTED_WORD:
            if (*command_index == 0 || CL_length(pipeline->commands[*command_index - 1].args) > 0) {
                Pipeline_add_command(pipeline, token.value);
                (*command_index)++;
            } else {
                Pipeline_add_argument(pipeline, token.value);
            }
            break;

        case TOK_PIPE:
            // Reset command index to indicate a new command is expected after a pipe
            *command_index = 0;
            break;

        case TOK_LESSTHAN:
            // Next token should be the filename for input redirection
            if (CL_length(tokens) > 1) {
                Token next_token = CL_nth(tokens, 1);
                if (next_token.type == TOK_WORD || next_token.type == TOK_QUOTED_WORD) {
                    Pipeline_set_input_file(pipeline, next_token.value);
                    TOK_consume(tokens); 
                } else {
                    snprintf(errmsg, errmsg_sz, "Expected filename after '<'");
                }
            } else {
                snprintf(errmsg, errmsg_sz, "Expected filename after '<'");
            }
            break;

        case TOK_GREATERTHAN:
            // Next token should be the filename for output redirection
            if (CL_length(tokens) > 1) {
                Token next_token = CL_nth(tokens, 1);
                if (next_token.type == TOK_WORD || next_token.type == TOK_QUOTED_WORD) {
                    Pipeline_set_output_file(pipeline, next_token.value);
                    TOK_consume(tokens); 
                } else {
                    snprintf(errmsg, errmsg_sz, "Expected filename after '>'");
                }
            } else {
                snprintf(errmsg, errmsg_sz, "Expected filename after '>'");
            }
            break;

        default:
            snprintf(errmsg, errmsg_sz, "Unexpected token: %s", token.value);
            break;
    }

    TOK_consume(tokens); 
}
