/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing0_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeudes <aeudes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:43:27 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/28 16:04:43 by aeudes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		is_in_quotes(char *input, int i)
{
	int index;
	int	quote_count;
	
	index = 0;
	quote_count = 0;
	while (index < i)
	{
		if (input[index] == '\'')
			quote_count++;
		index++;
	}
	if (quote_count % 2 != 0)
		return (true);
	return (false);
}

bool		is_in_double_quotes(char *input, int i)
{
	int	index;
	int	quote_count;

	index = 0;
	quote_count = 0;
	while (index < i)
	{
		if (input[index] == '"')
			quote_count++;
		index++;
	}
	if (quote_count % 2 != 0)
		return (true);
	return (false);
}

int	is_space(char c)
{
	return ( c == ' ' || (c >= 9 && c <= 13));
}

int	skip_space(char *input, int i)
{
	
	while (input[i] && is_space(input[i]))
		i++;
	return(i);
}