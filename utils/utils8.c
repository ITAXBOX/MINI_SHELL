#include "minishell.h"

char	*expand_tilde_path(const char *path, t_minishell *sh)
{
	const char	*home;

	home = env_get("HOME", sh->envp);
	if (!home)
		home = "/";
	if (path[1] == '\0')
		return (gc_strdup(home, &sh->gc));
	else if (path[1] == '/')
		return (gc_strjoin(home, path + 1, &sh->gc));
	else
		return (gc_strdup(path, &sh->gc));
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_current_directory(t_minishell *sh)
{
	char	*current_dir;
	char	*pwd_env;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		pwd_env = env_get("PWD", sh->envp);
		if (pwd_env)
			current_dir = gc_strdup(pwd_env, &sh->gc);
	}
	return (current_dir);
}
