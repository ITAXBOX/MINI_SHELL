#include "minishell.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (sign * result);
}

char	**copy_envp(char **envp, t_gc *gc)
{
	int		i;
	int		j;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = gc_malloc(gc, sizeof(char *) * (i + 1));
	j = 0;
	while (j < i)
	{
		copy[j] = gc_strdup(envp[j], gc);
		j++;
	}
	copy[i] = NULL;
	return (copy);
}

int	env_find_index(char **envp, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	**env_add_new(char **envp, const char *entry, t_gc *gc)
{
	int		count;
	int		j;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = gc_malloc(gc, sizeof(char *) * (count + 2));
	j = 0;
	while (j < count)
	{
		new_env[j] = envp[j];
		j++;
	}
	new_env[count] = gc_strdup(entry, gc);
	new_env[count + 1] = NULL;
	return (new_env);
}

void	env_set(char ***envp_ptr, const char *entry, t_gc *gc)
{
	char	**envp;
	char	*equal;
	int		idx;

	envp = *envp_ptr;
	equal = ft_strchr(entry, '=');
	if (!equal)
		return ;
	idx = env_find_index(envp, entry);
	if (idx >= 0)
	{
		envp[idx] = gc_strdup(entry, gc);
		return ;
	}
	*envp_ptr = env_add_new(envp, entry, gc);
}
