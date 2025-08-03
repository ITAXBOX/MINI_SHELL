#include "minishell.h"

static t_token	*create_token(t_minishell *sh, char *val, t_token_type type)
{
	t_token	*tok;

	tok = gc_malloc(&sh->gc, sizeof(t_token));
	tok->value = val;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

static t_token	*get_next_token(const char **s, t_minishell *sh)
{
	char			*word;
	char			*op;
	t_token_type	type;

	if (**s == '\'' || **s == '"' || !ft_strchr("|&<>() \t", **s))
	{
		word = extract_word(s, sh);
		if (!word)
			return (NULL);
		return (create_token(sh, word, T_WORD));
	}
	op = extract_operator(s, &type, sh);
	if (!op)
		return (NULL);
	return (create_token(sh, op, type));
}

static void	add_token_to_list(
	t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
		(*tail)->next = new_token;
	*tail = new_token;
}

t_token	*tokenize_input(const char *input, t_minishell *sh)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_token;

	head = NULL;
	tail = NULL;
	while (*input)
	{
		skip_whitespace(&input);
		if (!*input)
			break ;
		new_token = get_next_token(&input, sh);
		if (!new_token)
		{
			printf("Syntax error: unclosed quotes or invalid token\n");
			sh->last_exit_status = 2;
			return (NULL);
		}
		add_token_to_list(&head, &tail, new_token);
	}
	return (head);
}
