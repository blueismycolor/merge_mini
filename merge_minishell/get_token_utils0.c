/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_utils0_0.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeudes <aeudes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:24:59 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/28 16:25:02 by aeudes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pass_quotes(int i, char *str)
{
	if (str[i] == '"')
	{
		// printf("character avant le quotes : %c\n", str[i - 1]);
		i++;
		if (str[i] == '\0')
			return (i);
		while (str[i] != '"')
			i++;
		return (i);
	}
	if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'')
			i++;
		return (i);
	}
	return (i);
}

int	get_end_of_token(int i, char *str, bool *command)
{
	bool	check;

	check = false;
	while (str[i] != '\0' && str[i] != '|'
		&& str[i] != '>' && str[i] != '<')
	{
		i = pass_quotes(i, str);
		if (str[i] != ' ')
		{
			check = true;
			*command = true;
		}
		i++;
	}
	if (check == false)
		*command = false;
	return (i);
}

char	*get_str_token(char *str, int i, int end)
{
	char	*result;
	int		index;

	index = 0;
	result = malloc(sizeof(char) * (end - i + 1));
	end--;
	while (str[end] == ' ')
		end--;
	while (i <= end)
	{
		result[index] = str[i];
		index++;
		i++;
	}
	result[index] = '\0';
	return (result);
}

char	*put_second_tokens(char *str, int end)
{
	char	*result;

	if (str[end] == '|')
	{
		result = malloc(sizeof(char) * 2);
		result[0] = str[end];
		result[1] = '\0';
		return (result);
	}
	if ((str[end] == '>' && str[end + 1] == '>')
		|| (str[end] == '<' && str[end + 1] == '<'))
	{
		result = malloc(sizeof(char) * 3);
		ft_strlcpy(result, &str[end], 3);
		return (result);
	}
	result = malloc(sizeof(char) * 3);
	ft_strlcpy(result, &str[end], 2);
	return (result);
}
