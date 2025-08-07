#include "minishell.h"

static char	**add_single_arg(char **argv, char *value, t_gather_data *data)
{
	argv = resize_if_needed(argv, &data->i, &data->capacity, data->sh);
	argv[data->i++] = value;
	return (argv);
}

static t_redir	*create_redirection(t_token **token_ptr, t_minishell *sh)
{
	t_redir	*redir;

	if (!(*token_ptr)->next || (*token_ptr)->next->type != T_WORD)
		return (NULL);
	redir = gc_malloc(&sh->gc, sizeof(t_redir));
	redir->type = (*token_ptr)->type;
	if ((*token_ptr)->type == T_HEREDOC)
	{
		redir->file = (*token_ptr)->next->value;
		redir->heredoc_fd = handle_heredoc(redir->file, sh, 1);
	}
	else
	{
		redir->file = (*token_ptr)->next->value;
		redir->heredoc_fd = -1;
	}
	redir->next = NULL;
	*token_ptr = (*token_ptr)->next->next;
	return (redir);
}

static t_gather_data	init_gather_data(t_minishell *sh, t_redir **redir_head,
		t_redir **redir_tail)
{
	t_gather_data	data;

	data.capacity = 16;
	data.i = 0;
	data.sh = sh;
	*redir_head = NULL;
	*redir_tail = NULL;
	return (data);
}

static char	**process_word_token(char **argv, t_token **token_ptr,
		t_gather_data *data)
{
	char	**expanded;

	expanded = NULL;
	if (ft_strchr((*token_ptr)->value, '*'))
		expanded = expand_wildcard((*token_ptr)->value, &data->sh->gc);
	if (expanded)
		argv = add_expanded_args(argv, expanded, data);
	else
		argv = add_single_arg(argv, (*token_ptr)->value, data);
	*token_ptr = (*token_ptr)->next;
	return (argv);
}

char	**gather_args_and_redirs(t_token **token_ptr, t_minishell *sh,
		t_redir **redir_head)
{
	char			**argv;
	t_gather_data	data;
	t_redir			*redir_tail;

	data = init_gather_data(sh, redir_head, &redir_tail);
	argv = gc_malloc(&sh->gc, sizeof(char *) * (data.capacity + 1));
	while (*token_ptr && ((*token_ptr)->type == T_WORD
			|| ((*token_ptr)->type >= T_REDIR_IN
				&& (*token_ptr)->type <= T_HEREDOC)))
	{
		if ((*token_ptr)->type == T_WORD)
			argv = process_word_token(argv, token_ptr, &data);
		else
			add_redir_to_list(redir_head, &redir_tail,
				create_redirection(token_ptr, sh));
	}
	argv[data.i] = NULL;
	return (argv);
}
