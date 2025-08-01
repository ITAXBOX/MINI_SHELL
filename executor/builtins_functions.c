#include "minishell.h"

void	env_unset(char **envp, const char *key)
{
	int i;

	i = env_find_index(envp, key);
	if (i < 0)
		return;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, t_minishell *sh)
{
	int	exit_code;

	write(1, "exit\n", 5);
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			write(2, "minishell: exit: numeric argument required\n", 43);
			exit(255);
		}
		if (argv[2])
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			return (1);
		}
		exit_code = ft_atoi(argv[1]) % 256;
		if (exit_code < 0)
			exit_code += 256;
	}
	else
		exit_code = sh->last_exit_status;
	exit(exit_code);
}

int	builtin_export(char **argv, t_minishell *sh)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '='))
			env_set(&sh->envp, argv[i], &sh->gc);
		i++;
	}
	return (0);
}

int	builtin_unset(char **argv, t_minishell *sh)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		env_unset(sh->envp, argv[i]);
		i++;
	}
	return (0);
}
