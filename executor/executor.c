#include "minishell.h"

static int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

static void	handle_redirections(t_redir *redir_list)
{
	int	fd;

	fd = -1;
	while (redir_list)
	{
		if (redir_list->type == T_REDIR_IN)
			fd = open(redir_list->file, O_RDONLY);
		else if (redir_list->type == T_REDIR_OUT)
			fd = open(redir_list->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir_list->type == T_REDIR_APPEND)
			fd = open(redir_list->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir_list->type == T_HEREDOC)
			fd = handle_heredoc(redir_list->file);
		if (fd < 0)
		{
			perror(redir_list->file);
			exit(1);
		}
		if (redir_list->type == T_REDIR_IN || redir_list->type == T_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir_list = redir_list->next;
	}
}

static int	fork_and_execute_builtin(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_redirections(cmd->redirs);
		exit(run_builtin(cmd, sh));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

// WIFEXITED returns true if the child process terminated normally
// WEXITSTATUS returns the exit status of the child process
static int	execute_simple(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	if (is_builtin(cmd->argv[0]))
		return (fork_and_execute_builtin(cmd, sh));
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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
	int	left_status;

	if (!node)
		return (1);
	if (node->type == N_SIMPLE)
		return (execute_simple(node->cmd, sh));
	if (node->type == N_PIPE)
		return (execute_pipe(node, sh));
	else if (node->type == N_AND)
	{
		left_status = execute_tree(node->left, sh);
		if (left_status == 0)
			return (execute_tree(node->right, sh));
		return (left_status);
	}
	else if (node->type == N_OR)
	{
		left_status = execute_tree(node->left, sh);
		if (left_status != 0)
			return (execute_tree(node->right, sh));
		return (left_status);
	}
	return (1);
}
