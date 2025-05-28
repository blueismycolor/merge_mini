

#include "minishell.h"

/*
	EXEMPLE :

	COMMAND : cat input.txt | grep hello | wc -l
	TOKENS : [cat] [input.txt] [|] [grep] [hello] [|] [wc] [-l]

	FIRST T_CMD 
		args : ["cat", "input.txt' sdfvsdfv'"]
		redir : none
	SECOND TCMD
		args : ["grep", "hello"]
		redir : none
	THIRD TCMD 
		args : ["wc", "-l"]
		redir : none

*/
t_cmd	*tcmd_init(char *input)
{
	t_token *arg_tokens;
	t_cmd *result;

	if (!input || process_input(input) == -1)
		return (NULL);
	arg_tokens = get_token(input);
	if (!arg_tokens)
		return (NULL);
	// quote_removal(arg_tokens); TODO
	// expansion(arg_tokens); TODO
	result = create_list_tcmd(arg_tokens);
	return (result);
}

t_cmd	*init_cmd_node(t_cmd **cmd_list, t_cmd **current)
{
	t_cmd *new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->is_builtin = false;
	new->args = NULL;
	new->nb_params = 0;
	new->has_redir = false;
	new->redir = NULL;
	new->quote = NONE;
	new->next = NULL;
	new->prev = NULL;
	if (!*cmd_list)
		*cmd_list = new;
	else
	{
		(*current)->next = new;
		new->prev = *current;
	}
	*current = new;
	return (new);
}

char **realloc_args(char **args, int size)
{
	char **new_args;
	int i;

	i = 0;
	new_args = malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (NULL);
	while (i < size)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[size] = NULL;
	free(args);
	return (new_args);
}

void add_arg(t_cmd *cmd, char *str)
{
	cmd->args = set_args(str);
	if (!cmd->args)
		return;
	if (!cmd->cmd)
		cmd->cmd = cmd->args[0];
}

void add_redir(t_cmd *cmd, char *filename, t_type type)
{
	t_redir *new;
	t_redir *tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return;
	new->filename = ft_strdup(filename);
	new->type = type;
	new->next = NULL;
	if (!cmd->redir)
		cmd->redir = new;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	cmd->has_redir = true;
}

void fill_cmd_from_tokens(t_cmd *cmd, t_token **token)
{
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type == CMD)
			add_arg(cmd, (*token)->str);
		else if ((*token)->type == INPUT || (*token)->type == HEREDOC || (*token)->type == TRUNC
				|| (*token)->type == APPEND)
		{
			if ((*token)->next)
			{
				add_redir(cmd, (*token)->next->str, (*token)->type);
				*token = (*token)->next;
			}
		}
		*token = (*token)->next;
	}
}

char	**remove_filename(char **args)
{
	char	**tab;
	int		i;
	int		index;

	i = 0;
	index = 1;
	while (args[i])
		i++;
	tab = malloc(sizeof(char *) * i);
	i = 0;
	while (args[index])
	{
		tab[i] = ft_strdup(args[index]);
		i++;
		index++;
	}
	free_tab(args);
	tab[i] = NULL;
	return (tab);
}

t_cmd	*fill_special_cmd(t_token **tokens, t_cmd **head, t_cmd **last, t_cmd **new_cmd)
{
	char	**args;

	args = set_args((*tokens)->next->str);
	(*new_cmd) = init_cmd_node(head, last);
	if (!(*new_cmd))
		return (NULL);
	add_redir((*new_cmd), args[0], (*tokens)->type);
	*tokens = (*tokens)->next->next;
	(*new_cmd)->args = remove_filename(args);
	while ((*tokens) && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == INPUT || (*tokens)->type == HEREDOC 
			|| (*tokens)->type == TRUNC || (*tokens)->type == APPEND)
		{
			if ((*tokens)->next)
			{
				add_redir((*new_cmd), (*tokens)->next->str, (*tokens)->type);
				*tokens = (*tokens)->next;
			}
		}
		*tokens = (*tokens)->next;
	}
	if (*tokens)
		*tokens = (*tokens)->next;
	return ((*new_cmd));
}

t_cmd *create_list_tcmd(t_token *tokens)
{
	t_cmd *head;
	t_cmd *last;
	t_cmd *new_cmd;

	head = NULL;
	last = NULL;
	new_cmd = NULL;
	if (tokens->type == INPUT || tokens->type == HEREDOC || tokens->type == TRUNC 
		|| tokens->type == APPEND)
		{
			fill_special_cmd(&tokens, &head, &last, &new_cmd);
			new_cmd = new_cmd->next;
		}
	while (tokens)
	{
		new_cmd = init_cmd_node(&head, &last);
		if (!new_cmd)
			return (NULL);
		fill_cmd_from_tokens(new_cmd, &tokens);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
/*

	checker le type du premier element de t_token :
	- si c'est une redirection = 
		-> on fait un set_arg du deuxieme element de la liste t_token
		-> set_arg[0] sera le nom du fichier (donc t_cmd.redir.filename) 
			-> donc les autres elements apres set_arg[0] sont mis dans t_cmd.args
			! si il y'a d'autres redirections on les met a la suite dans t_redir

*/