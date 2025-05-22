/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeudes <aeudes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:57:55 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/22 14:49:32 by aeudes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return ( c == ' ' || (c >= 9 && c <= 13));
}

int	skip_space(char *input)
{
	int	i;

	i = 0;
	while (input[i] && is_space(input[i]))
		i++;
	return(i);
}

t_token	*create_token(t_token **head, char *str, t_type type, t_quote quote)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
		return (NULL);
	new_token->type = type;
	new_token->quote = quote;
	new_token->next = NULL;
	if (!*head)
		*head = new_token;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
	return (new_token);
}

t_type	get_token_type(char *str)
{
	if (ft_strncmp(str, ">>", 2) == 0 && get_quote_type(str) == NONE)
		return (APPEND);
	else if (ft_strncmp(str, "<<", 2) == 0 && get_quote_type(str) == NONE)
		return (HEREDOC);
	else if (ft_strncmp(str, ">", 1) == 0 && get_quote_type(str) == NONE)
		return (TRUNC);
	else if (ft_strncmp(str, "<", 1) == 0 && get_quote_type(str) == NONE)
		return (INPUT);
	else if (ft_strncmp(str, "|", 1) == 0 && get_quote_type(str) == NONE)
		return (PIPE);
	else
		return (CMD);
}

t_quote	get_quote_type(char *str)
{
	int		i;
	t_quote	quote;

	i = 0;
	quote = NONE;
	while (str[i])
	{
		if (str[i] == '\'' && quote == NONE)
			return (SINGLE);
		if (str[i] == '"' && quote == NONE)
			return (DOUBLE);
		i++;
	}
	return (NONE);
}

void	free_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->str);
		free(temp);
	}
}

bool	check_for_expansion(char *str)
{
	int	i;

	i = 0;
	while(str[i] != '\0')
	{
		if (str[i] == '$')
			return (true);
		i++;
	}
	return (false);
}