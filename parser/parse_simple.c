#include "minishell.h"

void	add_redir_to_list(t_redir **head, t_redir **tail, t_redir *new)
{
	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

t_cmd_node	*parse_simple_command(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*node;
	t_cmd		*cmd;
	t_redir		*redir_head;

	if (!*curr || (*curr)->type != T_WORD)
		return (NULL);
	cmd = gc_malloc(&sh->gc, sizeof(t_cmd));
	cmd->argv = gather_args_and_redirs(curr, sh, &redir_head);
	cmd->redirs = redir_head;
	node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
	node->type = N_SIMPLE;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
