#include "minishell.h"

static void	read_heredoc_lines(const char *delimiter, t_minishell *sh,
	int expand_vars, int write_fd)
{
	char	*line;
	char	*processed_line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: here-document delimited by end-of-file\n", 50);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand_vars)
			processed_line = expand_variables(line, sh);
		else
			processed_line = gc_strdup(line, &sh->gc);
		write(write_fd, processed_line, ft_strlen(processed_line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(const char *delimiter, t_minishell *sh, int expand_vars)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	read_heredoc_lines(delimiter, sh, expand_vars, pipefd[1]);
	close(pipefd[1]);
	return (pipefd[0]);
}
