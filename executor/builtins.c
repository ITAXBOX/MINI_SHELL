#include "minishell.h"

static int	builtin_cd(char **argv)
{
	const char	*path;

	path = argv[1];
	if (!path)
		path = getenv("HOME");
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

static int	builtin_env(t_minishell *sh)
{
	int	i;

	i = 0;
	while (sh->envp[i])
	{
		printf("%s\n", sh->envp[i]);
		i++;
	}
	return (0);
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strncmp(cmd, "echo", 4) == 0 ||
		ft_strncmp(cmd, "cd", 2) == 0 ||
		ft_strncmp(cmd, "pwd", 3) == 0 ||
		ft_strncmp(cmd, "export", 6) == 0 ||
		ft_strncmp(cmd, "unset", 5) == 0 ||
		ft_strncmp(cmd, "env", 3) == 0 ||
		ft_strncmp(cmd, "exit", 4) == 0
	);
}

int	run_builtin(t_cmd *cmd, t_minishell *sh)
{
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		return (builtin_echo(cmd->argv));
	else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
		return (builtin_env(sh));
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (builtin_cd(cmd->argv));
	else if (ft_strncmp(cmd->argv[0], "export", 6) == 0)
		return (builtin_export(cmd->argv, sh));
	else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0)
		return (builtin_unset(cmd->argv, sh));
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0)
		return (builtin_exit(cmd->argv, sh));
	return (1);
}
