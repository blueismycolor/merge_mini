/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egatien <egatien@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:56:31 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/28 17:40:31 by egatien          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input(char *input)
{
	if (!validate_input(input))
		return (ERROR);
	return (SUCCESS);
}

bool	validate_input(char *input)
{
	if (!check_pipe_syntax(input, 0))
		return (false);
	if (check_quote_syntax(input, 0) == ERROR)
		return (false);
	if (!check_redirection_syntax(input, 0, 0))
		return (false);
	return (true);
}

bool	check_pipe_syntax(char *input, int i)
{
	int		j;
	char	quote;

	quote = 0;
	i = skip_space(input, 0);
	if (input[i] == '|')
		return (ft_putstr_fd(ERR_SYN, STDERR_FILENO), false);
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && quote == 0)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		if (quote == 0 && input[i] == '|')
		{
			j = i + 1;
			j = skip_space(input, j);
			while (input[j] == '|' || input[j] == '\0')
				return (ft_putstr_fd(ERR_SYN, STDERR_FILENO), false);
		}
		i++;
	}
	return (true);
}

int	check_quote_syntax(char *input, int i)
{
	while (input[i] && input[i] != '|' && input[i] != '>' && input[i] != '<')
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			if (!input[i])
				return (ft_putstr_fd(OPEN_SNG_QUOTE, STDERR_FILENO), ERROR);
		}
		else if (input[i] == '\"')
		{
			i++;
			while (input[i] && input[i] != '\"')
				i++;
			if (!input[i])
				return (ft_putstr_fd(OPEN_DBL_QUOTE, STDERR_FILENO), ERROR);
		}
		i++;
	}
	return (i);
}

bool	check_redirection_syntax(char *input, int i, int j)
{
	while (input[i])
	{
		if ((input[i] == '<' || input[i] == '>')
			&& !is_in_quotes(input, i) && !is_in_double_quotes(input, i))
		{
			if ((input[i] == '<' && input[i + 1] == '>')
				|| (input[i] == '>' && input[i + 1] == '<')
				|| (input[i] == '<' && input[i + 1] == '<'
					&& (input[i + 2] == '<' || input[i + 2] == '>'))
				|| (input[i] == '>' && input[i + 1] == '>'
					&& (input[i + 2] == '<' || input[i + 2] == '>')))
				return (ft_putstr_fd(ERR_SYN, STDERR_FILENO), false);
			if (input[i + 1] == '<' || input[i + 1] == '>')
				i++;
			j = i + 1;
			while (input[j] && input[j] == ' ')
				j++;
			if (!input[j] || input[j] == '<'
				|| input[j] == '>' || input[j] == '|')
				return (ft_putstr_fd(ERR_SYN, STDERR_FILENO), false);
		}
		i++;
	}
	return (true);
}
