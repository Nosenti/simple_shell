#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>

#define SYMBOL_MAX_SIZE 31

typedef enum {
  TOK_WORD,
  TOK_QUOTED_WORD,
  TOK_LESSTHAN,
  TOK_GREATERTHAN,
  TOK_PIPE,
  TOK_END
} TokenType;

typedef struct {
  TokenType type;
  char *value;
} Token;

typedef struct _clist *CList;

typedef Token CListElementType;

typedef struct _command {
    char *name;   
    CList args;  
} Command;

typedef struct _pipeline {
    Command *commands;    
    int command_count;     
    char *input_file;     
    char *output_file;     
} Pipeline;

char *_strcpy(char *dest, char *src);
int _strlen(char *s);
void _puts(char *str);
int _putchar(char c);
int _strcmp(char *s1, char *s2);
char *_strcat(char *dest, char *src);
char *_memset(char *s, char b, unsigned int n);
char *_strdup(char *str);
int _isspace(int c);

const char *TT_to_str(TokenType tt);
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz);
TokenType TOK_next_type(CList tokens);
Token TOK_next(CList tokens);
void TOK_consume(CList tokens);
void TOK_print(CList tokens);
void TOK_free_tokens(CList tokens);

#define INVALID_RETURN ((CListElementType) {TOK_END})

CList CL_new();
void CL_free(CList list);
int CL_length(CList list);
void CL_push(CList list, CListElementType element);
CListElementType CL_pop(CList list);
void CL_append(CList list, CListElementType element);
CListElementType CL_nth(CList list, int pos);
bool CL_insert(CList list, CListElementType element, int pos);
CListElementType CL_remove(CList list, int pos);
void CL_join(CList list1, CList list2);
void CL_reverse(CList list);
typedef void (*CL_foreach_callback)(int pos, CListElementType element, void *cb_data);
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data);

Pipeline *Pipeline_new();
void Pipeline_free(Pipeline *pipeline);
void Pipeline_set_input_file(Pipeline *pipeline, const char *filename);
void Pipeline_set_output_file(Pipeline *pipeline, const char *filename);
void Pipeline_add_command(Pipeline *pipeline, const char *command_name);
void Pipeline_add_argument(Pipeline *pipeline, const char *argument);
void Pipeline_print(const Pipeline *pipeline);
Pipeline *parse_tokens(CList tokens, char *errmsg, size_t errmsg_sz);

#endif