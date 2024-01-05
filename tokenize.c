/*
 * tokenize.c
 *
 * Functions to tokenize and manipulate lists of tokens
 *
 * Author: Innocent Ingabire <iingabir@andrew.cmu.edu>
 */

#include "shell.h"
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// Documented in .h file
const char *TT_to_str(TokenType tt)
{
  switch (tt)
  {
  case TOK_WORD:
    return "WORD";
  case TOK_QUOTED_WORD:
    return "QUOTED_WORD";
  case TOK_LESSTHAN:
    return "EQUAL";
  case TOK_GREATERTHAN:
    return "GREATERTHAN";
  case TOK_PIPE:
    return "PIPE";
  case TOK_END:
    return "(end)";
  }

  __builtin_unreachable();
}

// Documented in .h file
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz) {
    CList tokens = CL_new();

    const char *curr = input;
    while (*curr != '\0') {
        // Skip any whitespace
        if (isspace((unsigned char)*curr)) {
            curr++;
            continue;
        }

        Token token;
        char buffer[1024]; // Temporary buffer to store the token's value
        int buffer_index = 0;
        memset(buffer, 0, sizeof(buffer));

        if (*curr == '<') {
            token.type = TOK_LESSTHAN;
            curr++;
        } else if (*curr == '>') {
            token.type = TOK_GREATERTHAN;
            curr++;
        } else if (*curr == '|') {
            token.type = TOK_PIPE;
            curr++;
        } else {
            // Handle word or quoted word
            bool in_quotes = false;
            if (*curr == '\"') {
                in_quotes = true;
                curr++; // Skip the opening quote
            }

            while (*curr != '\0' && (in_quotes || (!isspace((unsigned char)*curr) && *curr != '<' && *curr != '>' && *curr != '|'))) {
                if (in_quotes && *curr == '\"') {
                    curr++; // Skip the closing quote
                    break;
                }

                if (*curr == '\\' && *(curr + 1) != '\0') {
                    // Handle escape character
                    curr++; 
                }

                buffer[buffer_index++] = *curr;
                curr++;
            }

            token.type = in_quotes ? TOK_QUOTED_WORD : TOK_WORD;
        }

        // Store the token's value
        token.value = strdup(buffer);
        CL_append(tokens, token);
    }

    return tokens;
}


// Documented in .h file
TokenType TOK_next_type(CList tokens)
{

  Token token = CL_nth(tokens, 0);
  if (token.type == TOK_END && token.value == 0)
  {
    return TOK_END;
  }
  return token.type;
}

// Documented in .h file
Token TOK_next(CList tokens)
{

  return CL_nth(tokens, 0);
}

// Documented in .h file
void TOK_consume(CList tokens)
{

  CL_pop(tokens);
}

void print_token(int pos, Token token, void *cb_data)
{

  printf("%s %s;", TT_to_str(token.type), token.value);
}

// Documented in .h file
void TOK_print(CList tokens)
{

  CL_foreach(tokens, (CL_foreach_callback)print_token, NULL);
}

void TOK_free_tokens(CList tokens) {
    if (tokens == NULL) {
        return;
    }

    while (CL_length(tokens) > 0) {
        Token token = CL_pop(tokens);
        if (token.value != NULL) {
            free(token.value);
            token.value = NULL;
        }
    }

    CL_free(tokens); // Free the list itself
}


