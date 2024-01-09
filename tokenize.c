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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



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


CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz) {
    CList tokens = CL_new();

    const char *curr = input;
    while (*curr != '\0') {
       
        if (_isspace((unsigned char)*curr)) {
            curr++;
            continue;
        }

        Token token;
        char buffer[1024]; 
        int buffer_index = 0;
        _memset(buffer, 0, sizeof(buffer));

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
            
            bool in_quotes = false;
            if (*curr == '\"') {
                in_quotes = true;
                curr++; 
            }

            while (*curr != '\0' && (in_quotes || (!_isspace((unsigned char)*curr) && *curr != '<' && *curr != '>' && *curr != '|'))) {
                if (in_quotes && *curr == '\"') {
                    curr++; 
                    break;
                }

                if (*curr == '\\' && *(curr + 1) != '\0') {
                    curr++; 
                }

                buffer[buffer_index++] = *curr;
                curr++;
            }

            token.type = in_quotes ? TOK_QUOTED_WORD : TOK_WORD;
        }
        token.value = strdup(buffer);
        CL_append(tokens, token);
    }

    return tokens;
}

TokenType TOK_next_type(CList tokens)
{

  Token token = CL_nth(tokens, 0);
  if (token.type == TOK_END && token.value == 0)
  {
    return TOK_END;
  }
  return token.type;
}

Token TOK_next(CList tokens)
{

  return CL_nth(tokens, 0);
}

void TOK_consume(CList tokens)
{

  CL_pop(tokens);
}

void print_token(int pos, Token token, void *cb_data)
{

  printf("%s %s;", TT_to_str(token.type), token.value);
}

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

    CL_free(tokens); 
}


