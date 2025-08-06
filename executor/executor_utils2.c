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

int	handle_heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	return (pipefd[0]);
}

static int	setup_left_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_tree(node->left, sh));
}

static int	setup_right_child(int *pipefd, t_cmd_node *node, t_minishell *sh)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
	g_in_prompt = 0;
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	g_in_prompt = 1;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
