/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egatien <egatien@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:52:50 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/28 17:47:30 by egatien          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void	print_tab(char	**tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("tab :%s\n", tab[i]);
		i++;
	}
}

static void	print_redirections(t_redir *redir)
{
	while (redir)
	{
		printf(" | name of file : [%s], redirection type : %d", redir->filename, redir->type);
		redir = redir->next;
	}
}

static void	print_tokens(t_cmd *input)
{
	int	i;

	while (input)
	{
		i = 0;
		printf("Token: ");
		while (input->args && input->args[i])
		{
			printf("\033[32m[\033[0m%s\033[32m]\033[0m", input->args[i]);
			i++;
		}
		if (input->has_redir)
			print_redirections(input->redir);
		printf("\n");
		input = input->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*arg_tokens;
	t_cmd		*result;

	if (!argc || !argv)
		return (1);
	while (1)
	{
		
		input = NULL;
		input = readline("\033[1;92m╰─➤ \033[0m");
		if (input && input[0] != '\0')
		{
			if (process_input(input) == ERROR)
				continue;
			input = set_env(input, envp);
		}
		else
		{
			free(input);
			continue;
		}
		arg_tokens = get_token(input);
		result = create_list_tcmd(arg_tokens);
		free_list(arg_tokens);
		print_tokens(result);
		free_tcmd(result);
		free(input);
	}
	return (0);
}
