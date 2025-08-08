#include "minishell.h"

int	is_expandable_command(const char *cmd)
{
	int	i;
	int	has_space;

	if (!cmd)
		return (0);
	has_space = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == ' ' || cmd[i] == '\t')
		{
			has_space = 1;
			break ;
		}
		i++;
	}
	return (has_space);
}

int	execute_expanded_command(const char *cmd_str, t_minishell *sh)
{
	t_token		*tokens;
	t_cmd_node	*cmd_tree;
	int			exit_status;
	char		*input_copy;

	input_copy = gc_strdup(cmd_str, &sh->gc);
	if (!input_copy)
		return (1);
	tokens = tokenize_input(input_copy, sh);
	if (!tokens || !validate_token_stream(tokens, sh))
		return (1);
	cmd_tree = parse_input(tokens, sh);
	if (!cmd_tree)
		return (1);
	exit_status = execute_tree(cmd_tree, sh);
	return (exit_status);
}

int	handle_signal_termination(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

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
