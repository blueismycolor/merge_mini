/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeudes <aeudes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:53:05 by aeudes            #+#    #+#             */
/*   Updated: 2025/05/22 17:54:51 by aeudes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SIZE_MAX 20
# define SPACE ' '
# include <stdbool.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../libft-complete/libft/libft.h"
# include "../libft-complete/ft_printf/ft_printf.h"


# define ERR_PIPE				"Error: failed to create pipe.\n"
# define ERR_REDIR				"Error: invalid redirection.\n"
# define ERR_MALLOC				"Error: memory allocation failed,\n"
# define ERR_QUOTE				"Error: unmatched or invalid quote.\n"
# define OPEN_SNG_QUOTE			"Error: unclosed single quote.\n"
# define OPEN_DBL_QUOTE			"Error: unclosed double quote.\n"
# define ERR_CMD				"Error: command execution failed.\n"
# define ERR_EXECVE				"Error: execve system call failed.\n"
# define ERR_FORK				"Error: fork failed.\n"
# define ERR_DUP				"Error: dup failed.\n"
# define NO_PATH				"Error: no such file or directory.\n"
# define ERR_EOF				"Error: unexpected EOF.\n"
# define TOO_LONG				"Error: argument list too long.\n"
# define ERR_VA_ENV				"Error: unboud variable.\n"			// Erreur avec variable non initialisee (set -u)
# define ERR_SYN				"Error: syntax error in expression.\n"	// Manque un i++ ou qqch dans le genre
# define ERR_FD					"Error: bad file descriptor.\n"		// Fermeture accidentelle dun fd
# define SUCCESS				0
# define ERROR					-1
# define FAIL					1



typedef enum s_type
{
	INPUT = 1,	// "<"  : redirection de l'entrée
	HEREDOC,	// "<<" : redirection here-doc
	TRUNC,		// ">"  : redirection de la sortie (écrasement)
	APPEND,		// ">>" : redirection de la sortie (ajout à la fin)
	PIPE,		// "|"  : pipe
	CMD,		// commande principale et argument(ex: ls -al, cat)
}	t_type;

typedef enum s_quote
{
	NONE,
	SINGLE,
	DOUBLE
}	t_quote;

typedef struct s_token
{
	char			*str;
	t_type			type;
	t_quote			quote;
	bool			has_expansion;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*filename;
	t_type			type;
	struct s_redir	*next; 
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	bool			is_builtin;
	char			**args;
	int				nb_params;
	bool			has_redir;
	t_redir			*redir;
	t_quote			quote;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	t_type			type;
}	t_cmd;

typedef struct s_data
{
	t_cmd			*cmd;
	char			**env;
	char			*pwd;
	char			*old_pwd;
	int				return_value;
	char			**historic;
}	t_data;


//parsing0_0.c
t_cmd	*ft_parse(char *str);
bool	is_in_quotes(char *input, int i);
bool	is_in_double_quotes(char *input, int i);

//parsing0_1.c
bool	check_pipe_syntax(char *input, int i);
int		check_quote_syntax(char *input, int i);
bool	check_redirection_syntax(char *input, int i, int j);
bool	validate_input(char *input);
int		process_input(char *input);

//tokens_in_tabstr_part

//get_nbr_token.c
int		check_for_quotes(char *out, int i);
int		get_nbr_tokens(char *out);

//get_token_in_tabstr.c
int		check_redirections(char *str, int end);
char	**setup_variables(bool *command, int *j, int *i, char *str);
int		get_second_tokens(char *str, char **result, int *j, int end);
char	**put_token_in_tabstr(char *str);

//get_token_utils.c
char	*get_str_token(char *str, int i, int end);
char	*put_second_tokens(char *str, int end);
int		get_end_of_token(int i, char *str, bool *command);
int		pass_quotes(int i, char *str);


//t_token_utils.c
t_token		*create_token(t_token **head, char *str, t_type type, t_quote quote);
t_type		get_token_type(char *str);
t_quote		get_quote_type(char *str);
void		free_list(t_token *head);

//get_args.c
int		count_args(char *str);
char	*get_word(int i, int end, char *str);
char	**ft_getargs(char *str, char **result);
char	**set_args(char *str);
char	*ft_remove_quotes(char *str);

//expansion
char	*ft_tabstrnstr(const char *str, const char *to_find);
int		last_exit_status(int exit_status);
char	*search_env(char *env_name, char **envp);
char	*get_env_value(char *str, int index, char **envp);
int		ft_isspecial(char c);
int		parsing_env_var(char *str, char *result, int tmp);
char	*get_special_env_value(char *str, int index, char **envp);
char	*get_env_name(char *str, int index, char **envp);

//create_list_tcmd.c
t_cmd	*tcmd_init(char *input);
t_cmd	*init_cmd_node(t_cmd **cmd_list, t_cmd **current);
char 	**realloc_args(char **args, int size);
void 	add_arg(t_cmd *cmd, char *str);
void 	add_redir(t_cmd *cmd, char *filename, t_type type);
void 	fill_cmd_from_tokens(t_cmd *cmd, t_token **token);
t_cmd 	*create_list_tcmd(t_token *tokens);


//main.c
bool	check_for_expansion(char *str);
t_token	*get_token(char *str);
void	print_tab(char	**tab);



#endif