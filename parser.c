#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void handle_token(CList tokens, Pipeline *pipeline, int *command_index, char *errmsg, size_t errmsg_sz);

Pipeline *parse_tokens(CList tokens, char *errmsg, size_t errmsg_sz) {
    
    Pipeline *pipeline = Pipeline_new();
    int command_index = 0; 

    if (tokens == NULL) return NULL;
    
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

static void handle_token(CList tokens, Pipeline *pipeline, int *command_index, char *errmsg __attribute__((unused)), size_t errmsg_sz __attribute__((unused))) {
    Token token = CL_nth(tokens, 0);

    if (CL_length(tokens) == 0) {
        _puts("Empty token list");
        return;
    }

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
            
            *command_index = 0;
            break;

        case TOK_LESSTHAN:
            if (CL_length(tokens) > 1) {
                Token next_token = CL_nth(tokens, 1);
                if (next_token.type == TOK_WORD || next_token.type == TOK_QUOTED_WORD) {
                    Pipeline_set_input_file(pipeline, next_token.value);
                    TOK_consume(tokens); 
                } else {
                    _puts( "Expected filename after '<'");
                }
            } else {
                _puts("Expected filename after '<'");
            }
            break;

        case TOK_GREATERTHAN:
            if (CL_length(tokens) > 1) {
                Token next_token = CL_nth(tokens, 1);
                if (next_token.type == TOK_WORD || next_token.type == TOK_QUOTED_WORD) {
                    Pipeline_set_output_file(pipeline, next_token.value);
                    TOK_consume(tokens); 
                } else {
                    _puts("Expected filename after '>'");
                }
            } else {
                _puts("Expected filename after '>'");
            }
            break;

        default:
            _puts("Unexpected token");
            break;
    }

    TOK_consume(tokens); 
}