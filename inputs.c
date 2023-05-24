#include "shell.h"

#define BUFFER_SIZE 1024
/**
 * inputs - function to print an input.
 * @str: parameter
 * Return: an input string.
 */
void print_error_string(char *str)
{
	int i;
	i = 0;
	
	if (!str)
		return;
	while (str[i] != '\0')
	{
		print_error_character(str[i]);
		i++;
	}
}
int print_error_character(char c)
{
	static int i;
	static char buf[BUFFER_SIZE];
	if (c == '\0' || i >= BUFFER_SIZE)
	{
		write(STDERR_FILENO, buf, i);
		i = 0;
	}
	if (c != '\0')
		buf[i++] = c;
	return (1);
}
int print_character_to_fd(char c, int fd)
{
	static int i;
	static char buf[BUFFER_SIZE];
	if (c == '\0' || i >= BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != '\0')
		buf[i++] = c;
	return (1);
}
int print_string_to_fd(char *str, int fd)
{
	int i;
	i = 0;
	if (!str)
		return (0);
	while (*str)
	{
		i += print_character_to_fd(*str++, fd);
	}
	return (i);
}
