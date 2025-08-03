#include "minishell.h"

void env_unset(char **envp, const char *key)
{
	int i;

	i = env_find_index(envp, key);
	if (i < 0)
		return;
	free(envp[i]);
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
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
			env_set(&sh->envp, argv[i]);
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

int	run_builtin(t_cmd *cmd, t_minishell *sh)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd->argv));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(sh));
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, sh));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, sh));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, sh));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv, sh));
	return (1);
}
