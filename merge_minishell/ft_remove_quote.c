

#include "minishell.h"

static int	pass_quote_to_remove(char *str, int i)
{
	while (str[i] == '\'' || str[i] == '"')
		i++;
	return (i);
}

char	*ft_remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] != '\'' && str[i] != '"')
			count++;
		i++;
	}
	result = malloc(sizeof(char) * (count + 1));
	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		i = pass_quote_to_remove(str, i);
		result[count] = str[i];
		i++;
		count++;
	}
	result[count] = '\0';
	free(str);
	return (result);
}
