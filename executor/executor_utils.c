#include "minishell.h"

static char	*join_path_and_cmd(const char *dir
	, const char *cmd, t_minishell *sh)
{
	char	*with_slash;
	char	*full;

	with_slash = gc_strjoin(dir, "/", &sh->gc);
	full = gc_strjoin(with_slash, cmd, &sh->gc);
	return (full);
}

static char	*try_path_dir(const char *start
	, size_t len, const char *cmd, t_minishell *sh)
{
	char	*dir;
	char	*full;

	dir = gc_malloc(&sh->gc, len + 1);
	ft_strncpy(dir, start, len);
	dir[len] = '\0';
	full = join_path_and_cmd(dir, cmd, sh);
	if (access(full, X_OK) == 0)
		return (full);
	return (NULL);
}

static size_t	get_path_segment_len(const char *start)
{
	char	*end;

	end = ft_strchr(start, ':');
	if (end)
		return ((size_t)(end - start));
	return (ft_strlen(start));
}

static char	*internal_resolve_path(const char *cmd, t_minishell *sh)
{
	char	*path;
	char	*start;
	size_t	len;
	char	*result;

	path = env_get("PATH", sh->envp);
	if (!path)
		return (NULL);
	start = path;
	while (start && *start)
	{
		len = get_path_segment_len(start);
		result = try_path_dir(start, len, cmd, sh);
		if (result)
			return (result);
		if (start[len] == ':')
			start += len + 1;
		else
			start += len;
	}
	return (NULL);
}

char	*resolve_command_path(const char *cmd, t_minishell *sh)
{
	if (!cmd || ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (gc_strdup(cmd, &sh->gc));
		else
			return (NULL);
	}
	return (internal_resolve_path(cmd, sh));
}
