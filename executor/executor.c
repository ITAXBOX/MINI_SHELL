#include "minishell.h"

static void	handle_redirections(t_redir *redir_list)
{
	int	fd;

	while (redir_list)
	{
		fd = -1;
		if (redir_list->type == T_REDIR_IN)
			fd = open(redir_list->file, O_RDONLY);
		else if (redir_list->type == T_REDIR_OUT)
			fd = open(redir_list->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir_list->type == T_REDIR_APPEND)
			fd = open(redir_list->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (redir_list->type == T_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir_list = redir_list->next;
	}
}

// WIFEXITED returns true if the child process terminated normally
// WEXITSTATUS returns the exit status of the child process
static int	execute_simple(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	if (is_builtin(cmd->argv[0]))
		return run_builtin(cmd, sh);
	full_path = resolve_command_path(cmd->argv[0], sh);
	if (!full_path)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 21);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(cmd->redirs);
		execve(full_path, cmd->argv, sh->envp);
		perror("execve");
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_tree(t_cmd_node *node, t_minishell *sh)
{
	if (!node)
		return (1);
	if (node->type == N_SIMPLE)
		return execute_simple(node->cmd, sh);
	return (1);
}
