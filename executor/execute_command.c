#include "minishell.h"

static void	run_child_command(const char *cmd, int *pipefd, t_minishell *sh)
{
	char	*argv[4];

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	argv[0] = "/bin/sh";
	argv[1] = "-c";
	argv[2] = (char *)cmd;
	argv[3] = NULL;
	execve("/bin/sh", argv, sh->envp);
	exit(127);
}

static char	*read_command_output(int fd, t_minishell *sh)
{
	char	buffer[4096];
	ssize_t	bytes_read;
	char	*result;
	size_t	total_len;
	char	*new_result;

	result = gc_strdup("", &sh->gc);
	total_len = 0;
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		new_result = gc_malloc(&sh->gc, total_len + ft_strlen(buffer) + 1);
		if (total_len > 0)
			ft_strcpy(new_result, result);
		else
			new_result[0] = '\0';
		ft_strcat(new_result, buffer);
		result = new_result;
		total_len += ft_strlen(buffer);
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
	return (result);
}

static char	*execute_command_substitution(const char *cmd, t_minishell *sh)
{
	int		pipefd[2];
	pid_t	pid;
	char	*result;
	int		status;

	if (pipe(pipefd) == -1)
		return (gc_strdup("", &sh->gc));
	pid = fork();
	if (pid == 0)
		run_child_command(cmd, pipefd, sh);
	else if (pid > 0)
	{
		close(pipefd[1]);
		result = read_command_output(pipefd[0], sh);
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		return (result);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (gc_strdup("", &sh->gc));
}

void	append_command_output(const char *cmd, char *res,
	size_t *j, t_minishell *sh)
{
	char	*output;
	size_t	k;

	output = execute_command_substitution(cmd, sh);
	if (!output)
		return ;
	k = 0;
	while (output[k])
		res[(*j)++] = output[k++];
}
