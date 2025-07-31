#include "minishell.h"

static t_redir	*collect_redirs(t_token **curr, t_minishell *sh)
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

static t_cmd_node	*parse_pipeline(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*left;
	t_cmd_node	*right;
	t_cmd_node	*pipe_node;

	left = parse_simple_command(curr, sh);
	if (!left)
		return (NULL);
	while (*curr && (*curr)->type == T_PIPE)
	{
		*curr = (*curr)->next;
		right = parse_simple_command(curr, sh);
		if (!right)
			return (NULL);
		pipe_node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
		pipe_node->type = N_PIPE;
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

t_cmd_node	*parse_input(t_token *tokens, t_minishell *sh)
{
	t_token	*curr;

	curr = tokens;
	return (parse_pipeline(&curr, sh));
}
