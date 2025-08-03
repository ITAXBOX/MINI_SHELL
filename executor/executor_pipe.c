#include "minishell.h"

static int	setup_left_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_tree(node->left, sh));
}

static int	setup_right_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_tree(node->right, sh));
}

int	execute_pipe(t_cmd_node *node, t_minishell *sh)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	left_pid = fork();
	if (left_pid == 0)
		setup_left_child(pipefd, node, sh);
	right_pid = fork();
	if (right_pid == 0)
		setup_right_child(pipefd, node, sh);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
