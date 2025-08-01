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
