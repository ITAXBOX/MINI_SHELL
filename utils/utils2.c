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

char	**copy_envp(char **envp)
{
	int		count;
	int		i;
	char	**copy;

	count = 0;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
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

static char	**env_add_new(char **envp, const char *entry)
{
	int		count;
	int		j;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (envp);
	j = 0;
	while (j < count)
	{
		new_env[j] = envp[j];
		j++;
	}
	new_env[count] = ft_strdup(entry);
	new_env[count + 1] = NULL;
	free(envp);
	return (new_env);
}

void	env_set(char ***envp_ptr, const char *entry)
{
	char	**envp;
	char	*equal;
	int		idx;
	char	key[256];

	envp = *envp_ptr;
	equal = ft_strchr(entry, '=');
	if (!equal || equal == entry)
		return ;
	ft_strncpy(key, entry, equal - entry);
	key[equal - entry] = '\0';
	idx = env_find_index(envp, key);
	if (idx >= 0)
	{
		free(envp[idx]);
		envp[idx] = ft_strdup(entry);
	}
	else
		*envp_ptr = env_add_new(envp, entry);
}
