#include "minishell.h"

static int	check_syntax_errors(t_token *token, t_token *prev, int *paren_count, t_minishell *sh)
{
	if ((token->type == T_AND_IF || token->type == T_OR_IF || token->type == T_PIPE)
		&& (!prev || !token->next || prev->type == token->type || token->next->type == token->type))
		return (printf("Syntax error: unexpected token near '%s'\n", token->value), sh->last_exit_status = 2, 0);
	if (token->type == T_PAREN_L)
		(*paren_count)++;
	else if (token->type == T_PAREN_R)
		(*paren_count)--;
	if (*paren_count < 0)
		return (printf("Syntax error: unexpected ')'\n"), sh->last_exit_status = 2, 0);
	if ((token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
		&& (!token->next || token->next->type != T_WORD))
		return (printf("Syntax error: redirection without target\n"), sh->last_exit_status = 2, 0);
	return (1);
}

int	validate_token_stream(t_token *token, t_minishell *sh)
{
	t_token	*prev;
	int		paren_count;

	prev = NULL;
	paren_count = 0;
	if (!token)
		return (0);
	while (token)
	{
		if (!check_syntax_errors(token, prev, &paren_count, sh))
			return (0);
		prev = token;
		token = token->next;
	}
	if (paren_count != 0)
		return (printf("Syntax error: unmatched parentheses\n"), sh->last_exit_status = 2, 0);
	return (1);
}
