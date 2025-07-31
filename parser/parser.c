#include "minishell.h"

static size_t	count_args(t_token *token)
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

static char	**gather_args(t_token **token_ptr, size_t argc, t_minishell *sh)
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
	cmd->redirs = NULL;
	node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
	node->type = N_SIMPLE;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_cmd_node	*parse_input(t_token *tokens, t_minishell *sh)
{
	t_token	*curr;

	curr = tokens;
	return (parse_simple_command(&curr, sh));
}
