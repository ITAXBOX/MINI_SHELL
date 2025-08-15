#include "minishell.h"

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

int	builtin_echo(char **argv)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_dash_n(argv[i]))
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

static int	handle_pwd_fallback(t_minishell *sh)
{
	char	*pwd_env;

	pwd_env = env_get("PWD", sh->envp);
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	perror("pwd");
	return (1);
}

int	builtin_pwd(t_minishell *sh)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	return (handle_pwd_fallback(sh));
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
