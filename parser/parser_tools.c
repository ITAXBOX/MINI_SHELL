#include "minishell.h"

static char	**resize_if_needed(char **argv, size_t *i
		, size_t *capacity, t_minishell *sh)
{
	if (*i >= *capacity)
	{
		*capacity *= 2;
		argv = resize_argv(argv, *i, *capacity, sh);
	}
	return (argv);
}

static char	**add_expanded_args(char **argv, char **expanded
		, t_gather_data *data)
{
	size_t	k;

	k = 0;
	while (expanded[k])
	{
		argv = resize_if_needed(argv, &data->i, &data->capacity, data->sh);
		argv[data->i++] = expanded[k++];
	}
	return (argv);
}

static char	**add_single_arg(char **argv, char *value, t_gather_data *data)
{
	argv = resize_if_needed(argv, &data->i, &data->capacity, data->sh);
	argv[data->i++] = value;
	return (argv);
}

char	**gather_args(t_token **token_ptr, size_t argc, t_minishell *sh)
{
	char			**argv;
	char			**expanded;
	t_gather_data	data;

	data.capacity = argc * 16;
	data.i = 0;
	data.sh = sh;
	argv = gc_malloc(&sh->gc, sizeof(char *) * (data.capacity + 1));
	while (*token_ptr && (*token_ptr)->type == T_WORD)
	{
		expanded = NULL;
		if (ft_strchr((*token_ptr)->value, '*'))
			expanded = expand_wildcard((*token_ptr)->value, &sh->gc);
		if (expanded)
			argv = add_expanded_args(argv, expanded, &data);
		else
			argv = add_single_arg(argv, (*token_ptr)->value, &data);
		*token_ptr = (*token_ptr)->next;
	}
	argv[data.i] = NULL;
	return (argv);
}
