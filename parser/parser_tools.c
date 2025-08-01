#include "minishell.h"

t_cmd_node	*parse_simple_command(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*node;
	t_cmd		*cmd;
	size_t		argc;

	if (!*curr || (*curr)->type != T_WORD)
		return (NULL);
	cmd = gc_malloc(&sh->gc, sizeof(t_cmd));
	argc = count_args(*curr);
	cmd->argv = gather_args(curr, argc, sh);
	cmd->redirs = collect_redirs(curr, sh);
	node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
	node->type = N_SIMPLE;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_redir	*collect_redirs(t_token **curr, t_minishell *sh)
{
	t_redir	*head;
	t_redir	*tail;
	t_redir	*new;

	head = NULL;
	tail = NULL;
	while (*curr && ((*curr)->type >= T_REDIR_IN && (*curr)->type <= T_HEREDOC))
	{
		new = parse_redirection(curr, sh);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

t_redir	*parse_redirection(t_token **curr, t_minishell *sh)
{
	t_redir	*redir;

	if (!*curr || (*curr)->type < T_REDIR_IN || (*curr)->type > T_HEREDOC)
		return (NULL);
	if (!(*curr)->next || (*curr)->next->type != T_WORD)
		return (NULL);
	redir = gc_malloc(&sh->gc, sizeof(t_redir));
	redir->type = (*curr)->type;
	redir->file = (*curr)->next->value;
	redir->next = NULL;
	*curr = (*curr)->next->next;
	return (redir);
}

size_t	count_args(t_token *token)
{
	size_t	count;

	count = 0;
	while (token && token->type == T_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	**gather_args(t_token **token_ptr, size_t argc, t_minishell *sh)
{
	char	**argv;
	size_t	i;

	argv = gc_malloc(&sh->gc, sizeof(char *) * (argc + 1));
	i = 0;
	while (*token_ptr && (*token_ptr)->type == T_WORD && i < argc)
	{
		argv[i++] = (*token_ptr)->value;
		*token_ptr = (*token_ptr)->next;
	}
	argv[i] = NULL;
	return (argv);
}
