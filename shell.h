#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void print_error_string(char *str);
int print_error_character(char c);
int print_character_to_fd(char c, int fd);
int print_string_to_fd(char *str, int fd);

#endif /* SHELL_H */
