#include "minishell.h"

static void	bubble_sort_env(char **sorted, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_variable(char *env_var)
{
	char	*eq;

	eq = ft_strchr(env_var, '=');
	if (eq)
	{
		*eq = '\0';
		printf("declare -x %s=\"%s\"\n", env_var, eq + 1);
		*eq = '=';
	}
	else
		printf("declare -x %s\n", env_var);
}

void	print_sorted_env_export(char **envp, t_gc *gc)
{
	int		count;
	int		i;
	char	**sorted;

	count = 0;
	while (envp[count])
		count++;
	sorted = gc_malloc(gc, sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		sorted[i] = envp[i];
		i++;
	}
	sorted[count] = NULL;
	bubble_sort_env(sorted, count);
	i = 0;
	while (i < count)
	{
		print_env_variable(sorted[i]);
		i++;
	}
}

static int	check_syntax_errors(t_token *token, t_token *prev
	, int *paren_count, t_minishell *sh)
{
	if ((token->type == T_AND_IF || token->type == T_OR_IF
			|| token->type == T_PIPE)
		&& (!prev || !token->next || prev->type == token->type
			|| token->next->type == token->type))
		return (printf("Syntax error: unexpected token near '%s'\n"
				, token->value), sh->last_exit_status = 2, 0);
	if (token->type == T_PAREN_L)
		(*paren_count)++;
	else if (token->type == T_PAREN_R)
		(*paren_count)--;
	if (*paren_count < 0)
		return (printf("Syntax error: unexpected ')'\n")
			, sh->last_exit_status = 2, 0);
	if ((token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
		&& (!token->next || token->next->type != T_WORD))
		return (printf("Syntax error: redirection without target\n")
			, sh->last_exit_status = 2, 0);
	return (1);
}

int	validate_token_stream(t_token *token, t_minishell *sh)
{
	t_token	*prev;
	int		paren_count;

	prev = NULL;
	paren_count = 0;
	if (!token)
		return (0);
	while (token)
	{
		if (!check_syntax_errors(token, prev, &paren_count, sh))
			return (0);
		prev = token;
		token = token->next;
	}
	if (paren_count != 0)
		return (printf("Syntax error: unmatched parentheses\n")
			, sh->last_exit_status = 2, 0);
	return (1);
}
