#include "minishell.h"

static char	**copy_match_list(char **old, int old_size, int new_cap, t_gc *gc)
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

static char	**read_and_collect_matches(DIR *dir, const char *pattern
		, t_gc *gc, int *out_count)
{
	struct dirent	*entry;
	char			**matches;
	int				count;
	int				capacity;

	capacity = 16;
	matches = gc_malloc(gc, sizeof(char *) * (capacity + 1));
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			if (count >= capacity)
			{
				capacity *= 2;
				matches = copy_match_list(matches, count, capacity, gc);
			}
			matches[count++] = gc_strdup(entry->d_name, gc);
		}
		entry = readdir(dir);
	}
	matches[count] = NULL;
	*out_count = count;
	return (matches);
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
