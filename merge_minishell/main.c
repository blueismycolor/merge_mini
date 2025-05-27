/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egatien <egatien@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:52:50 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/27 13:55:56 by egatien          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

t_token	*get_token(char *str)
{
	t_token	*head; // tete de la liste au'on va renvoyer
	t_token	*node; // t_token temporaire qui sert a initialiser les noeuds de la liste
	char	**cut_str; // tableau de chaine de caracteres qui sert a recuperer les tokens dans str
	int		i; // variable pour defiler dans cut_str

	i = 0;
	head = NULL;
	cut_str = put_token_in_tabstr(str); // on recupere les tokens et on les place dans le tableau
	head = create_token(&head, cut_str[i], get_token_type(cut_str[i]), get_quote_type(cut_str[i])); // on cree le premier noeud de la liste
	head->has_expansion = check_for_expansion(head->str); // on regarde si le token contient une expansion
	node = head->next;
	free(cut_str[i]);
	i++;
	while (cut_str[i]) //boucle qui va creer la liste chainee
	{
		node = create_token(&head, cut_str[i], get_token_type(cut_str[i]), get_quote_type(cut_str[i]));
		node->has_expansion = check_for_expansion(node->str);
		node = node->next;
		free(cut_str[i]);
		i++;
	}
	free(cut_str);
	return (head);
}

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

// int	main(int argc, char **argv)
// {
// 	t_token	*arg_tokens;
// 	t_cmd	*result;
// 	char	*str = "ls 'dfvbdfgb'-al >> outfile";

// 	if (!argc || process_input(str) == -1)
// 		return (0);
// 	if (!argc || !argv)
// 		return (0);
// 	arg_tokens = get_token(str);
// 	result = create_list_tcmd(arg_tokens);

// 	t_cmd	*temp = result;
// 	while (temp)
// 	{
// 		// printf("cmd : %s\n", temp->cmd);
// 		if (temp->has_redir == true)
// 			printf("redirec : %s\n", temp->redir->filename);
// 		int	i = 0;
// 		while (temp->args[i])
// 		{
// 			printf("t_cmd args : %s\n", temp->args[i]);
// 			i++;
// 		}
// 		printf("------\n");
// 		temp = temp->next;
// 	}
// }


// static void	print_tokens(t_cmd *input)
// {
// 	int	i;
// 	while(input)
// 	{
// 		i = 0;
// 		printf("Token: ");
// 		while (input->args[i])
// 		{
// 			printf("\033[32m[\033[0m%s\033[32m]\033[0m", input->args[i]);
// 			i++;
// 		}
// 		// printf(", Type: %d, Quote: %d",input->type, input->quote);
// 		if (input->has_redir == true)
// 			printf(" | name of file : [%s], redirection type : %d", input->redir->filename, input->redir->type);
// 		printf("\n");
// 		input = input->next;
// 	}
// }

static void print_redirections(t_redir *redir)
{
	while (redir)
	{
		printf(" | name of file : [%s], redirection type : %d", redir->filename, redir->type);
		redir = redir->next;
	}
}

static void print_tokens(t_cmd *input)
{
	int i;
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
		input = set_env(input, envp);
		if (input && input[0] != '\0')
		{
			if (process_input(input) == ERROR)
				continue;
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

/*

	CAS PARTICULIER A SURVEILLER :
	- ls > outfile > file -> deux redirections a mettre dans la liste chainee
	- ls > outfile -al -> effectue ls -al > outfile : Pourquoi ? jsp

*/
