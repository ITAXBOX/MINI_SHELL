#include "minishell.h"

static int	needs_fork(const char *cmd, t_redir *redirs, int in_logical_or_pipe)
{
	if (!cmd)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "unset") == 0)
		return (0);
	if (ft_strcmp(cmd, "export") == 0)
	{
		if (redirs)
			return (0);
		if (in_logical_or_pipe == 1)
			return (1);
		return (0);
	}
	return (1);
}

int	execute_builtin_dispatch(t_cmd *cmd, t_minishell *sh)
{
	if (needs_fork(cmd->argv[0], cmd->redirs, sh->in_logical_or_pipe))
		return (fork_and_execute_builtin(cmd, sh));
	else
		return (run_builtin(cmd, sh));
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	char	a;

	a = (char) c;
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
