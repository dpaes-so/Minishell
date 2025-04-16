/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:52:31 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/10 19:19:28 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	skip_wspaces(char **input)
{
	if (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
	{
		while (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
			(*input)++;
		return (true);
	}
	return (false);
}

/// @brief Checks next token to see if its a pipe or a redirection
/// @param input input from readline
/// @return true if there is no pipe or redirection
bool	check_next(char *input)
{
	if (*input && (*input == ' ' || (*input >= 9 && *input <= 13)))
	{
		while (*input && (*input == ' ' || (*input >= 9 && *input <= 13)))
		{
			input++;
		}
		if (ft_strchr("|<>", *input) == NULL)
			return (true);
	}
	return (false);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

void	*ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		*p = 0;
		p++;
		n--;
	}
	return (s);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	freetrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*buffer;
	size_t	i;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = (size_t)ft_strlen(s);
	i = 0;
	if (start + len > str_len)
		len = str_len - start;
	if (start >= str_len)
		len = 0;
	buffer = malloc(sizeof(char) * (len + 1));
	if (buffer == NULL)
		return (NULL);
	while (i < len && s[start + i] != '\0')
	{
		buffer[i] = s[start + i];
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}

void	*ft_calloc(size_t num, size_t size)
{
	void	*buffer;

	if (num && size > (size_t)(-1) / num)
		return (NULL);
	buffer = malloc(num * size);
	if (buffer == NULL)
		return (NULL);
	if (num == 0 || size == 0)
		return (buffer);
	ft_bzero(buffer, num * size);
	return (buffer);
}

/*int main()
{
	char *arr;
	size_t i = 0;
	arr = ft_calloc(5,1);
	while (i < 5) {
		printf("->%c\n", arr[i]);
		i++;
	}
	printf("\n");
	return (0);
}*/