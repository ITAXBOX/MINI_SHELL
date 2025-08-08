#include "minishell.h"

char	*env_get(const char *key, char **envp)
{
	size_t	len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static int	setup_left_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	int	exit_code;

	reset_signal_handlers();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit_code = execute_tree(node->left, sh);
	gc_clear(&sh->gc);
	gc_clear(&sh->env_gc);
	exit(exit_code);
}

static int	setup_right_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	int	exit_code;

	reset_signal_handlers();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit_code = execute_tree(node->right, sh);
	gc_clear(&sh->gc);
	gc_clear(&sh->env_gc);
	exit(exit_code);
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
	g_in_prompt = 0;
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	g_in_prompt = 1;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
