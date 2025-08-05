#include "minishell.h"

char	**resize_argv(char **old_argv, size_t old_size
		, size_t new_capacity, t_minishell *sh)
{
	char	**new_argv;
	size_t	i;

	new_argv = gc_malloc(&sh->gc, sizeof(char *) * (new_capacity + 1));
	i = 0;
	while (i < old_size)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
	return (new_argv);
}

int	match_pattern(const char *pattern, const char *text)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			while (*text)
			{
				if (match_pattern(pattern, text))
					return (1);
				text++;
			}
			return (0);
		}
		else if (*pattern == *text)
		{
			pattern++;
			text++;
		}
		else
			return (0);
	}
	return (*text == '\0');
}
