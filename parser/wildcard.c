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

char	**copy_match_list(char **old, int old_size, int new_cap, t_gc *gc)
{
	char	**new_list;
	int		i;

	new_list = gc_malloc(gc, sizeof(char *) * (new_cap + 1));
	i = 0;
	while (i < old_size)
	{
		new_list[i] = old[i];
		i++;
	}
	new_list[i] = NULL;
	return (new_list);
}

static char	**collect_matches(const char *pattern, t_gc *gc, int *out_count)
{
	DIR		*dir;
	char	**matches;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = read_and_collect_matches(dir, pattern, gc, out_count);
	closedir(dir);
	return (matches);
}

char	**expand_wildcard(const char *pattern, t_gc *gc)
{
	int		count;
	char	**matches;

	matches = collect_matches(pattern, gc, &count);
	if (count == 0)
		return (NULL);
	return (matches);
}
