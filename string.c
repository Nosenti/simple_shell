#include "shell.h"

int _putchar(char c)
{
	return (write(1, &c, 1));
}
/**
 * _puts - prints a string
 * @str: string
 * Return: void
 */

void _puts(char *str)
{
	while (*str != '\0')
	{
		_putchar(*str);
		str++;
	}
	/*_putchar('\n');*/
}

/**
 * _strlen - returns the length of a string
 * @s: string
 * Return: integer
 */

int _strlen(char *s)
{
	int len;

	len = 0;
	while (*s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

/**
 * _strcpy - copy string pointed by src
 * @dest: destination
 * @src: source
 * Return: pointer to dest
 */

char *_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcmp - compare two strings
 * @s1: string one
 * @s2: string two
 * Return: integer
 */

int _strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0')
		{
			return (0);
		}
		i++;
	}

	return (s1[i] - s2[i]);
}

/**
 * _strcat - concatenates two strings
 * @dest: destination string
 * @src: source string
 * Return: character
 */

char *_strcat(char *dest, char *src)
{
	int i, j;

	for (i = 0; dest[i] != '\0'; i++)
	{
	}
	for (j = 0; src[j] != '\0'; j++)
	{
		dest[i + j] = src[j];
	}
	dest[i + j] = '\0';
	return (dest);
}

/**
 * _memset - fills memory are with constant byte
 * @s: pointer to the area filled
 * @b: constant byte
 * @n: first n memory spaces
 * Return: pointer to s
 */

char *_memset(char *s, char b, unsigned int n)
{
	int i;

	i = 0;
	for (; n > 0; i++)
	{
		s[i] = b;
		n--;
	}
	return (s);
}

/**
 * _strdup - copy of the string
 * @str: string paramter
 * Return: pointer or NULL
 */

char *_strdup(char *str)
{
	char *cpy;
	int len, i;

	if (str == NULL)
	{
		return (NULL);
	}

	len = _strlen(str) + 1;
	cpy = (char *) malloc(sizeof(char) * len);
	if (cpy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (str[i] != '\0')
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);

}

int _isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}
