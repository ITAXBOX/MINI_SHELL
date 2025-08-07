#include "minishell.h"

char	**resize_if_needed(char **argv, size_t *i,
		size_t *capacity, t_minishell *sh)
{
	if (*i >= *capacity)
	{
		*capacity *= 2;
		argv = resize_argv(argv, *i, *capacity, sh);
	}
	return (argv);
}

char	**add_expanded_args(char **argv, char **expanded,
		t_gather_data *data)
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
