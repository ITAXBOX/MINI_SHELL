#include "minishell.h"

int	builtin_cd(char **argv, t_minishell *sh)
{
	const char	*path;
	char		*current_dir;
	char		*new_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (perror("cd: getcwd"), 1);
	path = argv[1];
	if (!path)
	{
		path = env_get("HOME", sh->envp);
		if (!path)
			path = "/";
	}
	if (chdir(path) != 0)
		return (perror("cd: chdir"), 1);
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
		return (perror("cd: getcwd"), 1);
	env_set(&sh->envp, gc_strjoin("OLDPWD=", current_dir, &sh->gc));
	env_set(&sh->envp, gc_strjoin("PWD=", new_dir, &sh->gc));
	free(current_dir);
	free(new_dir);
	return (0);
}

static int	is_dash_n(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv, t_minishell *sh)
{
	int		i;
	int		newline;
	char	*expanded;

	i = 1;
	newline = 1;
	while (argv[i] && is_dash_n(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		expanded = expand_variables(argv[i], sh);
		printf("%s", expanded);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_pwd(char **argv)
{
	char	*cwd;

	if (argv[1])
	{
		write(2, "pwd: ", 5);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": command not found\n", 20);
		return (127);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	builtin_env(t_minishell *sh)
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
