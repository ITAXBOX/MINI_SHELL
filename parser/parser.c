#include "minishell.h"

static t_cmd_node	*parse_pipeline_group_or_cmd(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*node;

	if (*curr && (*curr)->type == T_PAREN_L)
	{
		*curr = (*curr)->next;
		node = parse_logical(curr, sh);
		if (!node || !*curr || (*curr)->type != T_PAREN_R)
			return (NULL);
		*curr = (*curr)->next;
	}
	else
	{
		node = parse_simple_command(curr, sh);
		if (!node)
			return (NULL);
	}
	return (node);
}

static t_cmd_node	*parse_pipeline(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*left;
	t_cmd_node	*right;
	t_cmd_node	*pipe_node;

	left = parse_pipeline_group_or_cmd(curr, sh);
	if (!left)
		return (NULL);
	while (*curr && (*curr)->type == T_PIPE)
	{
		*curr = (*curr)->next;
		right = parse_pipeline_group_or_cmd(curr, sh);
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

static t_cmd_node	*create_logic_node(t_cmd_node *left, t_cmd_node *right,
	t_token_type op_type, t_minishell *sh)
{
	t_cmd_node	*logic_node;

	logic_node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
	if (op_type == T_AND_IF)
		logic_node->type = N_AND;
	else
		logic_node->type = N_OR;
	logic_node->left = left;
	logic_node->right = right;
	logic_node->cmd = NULL;
	return (logic_node);
}

t_cmd_node	*parse_logical(t_token **curr, t_minishell *sh)
{
	t_cmd_node		*left;
	t_cmd_node		*right;
	t_token_type	op_type;

	left = parse_pipeline(curr, sh);
	if (!left)
		return (NULL);
	while (*curr && ((*curr)->type == T_AND_IF || (*curr)->type == T_OR_IF))
	{
		op_type = (*curr)->type;
		*curr = (*curr)->next;
		right = parse_pipeline(curr, sh);
		if (!right)
			return (NULL);
		left = create_logic_node(left, right, op_type, sh);
	}
	return (left);
}

t_cmd_node	*parse_input(t_token *tokens, t_minishell *sh)
{
	t_token	*curr;

	curr = tokens;
	return (parse_logical(&curr, sh));
}
