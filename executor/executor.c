#include "minishell.h"

int	fork_and_execute_builtin(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers();
		handle_redirections(cmd->redirs);
		exit_code = run_builtin(cmd, sh);
		gc_clear(&sh->gc);
		gc_clear(&sh->env_gc);
		exit(exit_code);
	}
	g_in_prompt = 0;
	waitpid(pid, &status, 0);
	g_in_prompt = 1;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (handle_signal_termination(status));
}

static void	execute_child_process(t_cmd *cmd, t_minishell *sh, char *full_path)
{
	reset_signal_handlers();
	handle_redirections(cmd->redirs);
	execve(full_path, cmd->argv, sh->envp);
	perror("execve");
	gc_clear(&sh->gc);
	gc_clear(&sh->env_gc);
	exit(127);
}

// WIFEXITED returns true if the child process terminated normally
// WEXITSTATUS returns the exit status of the child process
static int	handle_empty_command(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;

	if (!cmd->redirs)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers();
		handle_redirections(cmd->redirs);
		gc_clear(&sh->gc);
		gc_clear(&sh->env_gc);
		exit(0);
	}
	g_in_prompt = 0;
	waitpid(pid, &status, 0);
	g_in_prompt = 1;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (handle_signal_termination(status));
}

static int	execute_simple(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	clean_argv(cmd->argv);
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0')
		return (handle_empty_command(cmd, sh));
	if (is_expandable_command(cmd->argv[0]))
		return (execute_expanded_command(cmd->argv[0], sh));
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin_dispatch(cmd, sh));
	full_path = resolve_command_path(cmd->argv[0], sh);
	if (!full_path)
		return (write(2, cmd->argv[0], ft_strlen(cmd->argv[0])),
			write(2, ": command not found\n", 21), 127);
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, sh, full_path);
	g_in_prompt = 0;
	waitpid(pid, &status, 0);
	g_in_prompt = 1;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (handle_signal_termination(status));
}

int	execute_tree(t_cmd_node *node, t_minishell *sh)
{
	int	left_status;

	if (!node)
		return (1);
	if (node->type == N_SIMPLE)
		return (sh->in_logical_or_pipe = 0, execute_simple(node->cmd, sh));
	if (node->type == N_PIPE)
		return (sh->in_logical_or_pipe = 1, execute_pipe(node, sh));
	else if (node->type == N_AND)
	{
		sh->in_logical_or_pipe = 1;
		left_status = execute_tree(node->left, sh);
		if (left_status == 0)
			return (execute_tree(node->right, sh));
		return (left_status);
	}
	else if (node->type == N_OR)
	{
		sh->in_logical_or_pipe = 1;
		left_status = execute_tree(node->left, sh);
		if (left_status != 0)
			return (execute_tree(node->right, sh));
		return (left_status);
	}
	return (1);
}
