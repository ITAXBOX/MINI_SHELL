#include "minishell.h"

static void	open_and_dup(const char *file, int flags, int target_fd, int mode)
{
	int	fd;

	fd = open(file, flags, mode);
	if (fd < 0)
	{
		perror(file);
		exit(1);
	}
	dup2(fd, target_fd);
	close(fd);
}

static void	handle_file_redirections(t_redir *redir)
{
	if (redir->type == T_REDIR_IN)
		open_and_dup(redir->file, O_RDONLY, STDIN_FILENO, 0);
	else if (redir->type == T_REDIR_OUT)
		open_and_dup(redir->file, O_WRONLY | O_CREAT | O_TRUNC,
			STDOUT_FILENO, 0644);
}

static void	handle_special_redirections(t_redir *redir)
{
	if (redir->type == T_REDIR_APPEND)
		open_and_dup(redir->file, O_WRONLY | O_CREAT | O_APPEND,
			STDOUT_FILENO, 0644);
	else if (redir->type == T_HEREDOC)
	{
		if (redir->heredoc_fd < 0)
		{
			write(2, "minishell: heredoc error\n", 25);
			exit(1);
		}
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
	}
}

void	handle_redirections(t_redir *redir_list)
{
	while (redir_list)
	{
		if (redir_list->type == T_REDIR_IN
			|| redir_list->type == T_REDIR_OUT)
			handle_file_redirections(redir_list);
		else if (redir_list->type == T_REDIR_APPEND
			|| redir_list->type == T_HEREDOC)
			handle_special_redirections(redir_list);
		redir_list = redir_list->next;
	}
}
