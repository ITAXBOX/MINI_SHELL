#include "minishell.h"

static t_token	*create_token(t_minishell *sh, char *val, t_token_type type)
{
	t_token	*tok;

	tok = gc_malloc(&sh->gc, sizeof(t_token));
	tok->value = val;
	tok->type = type;
	tok->was_quoted = 0;
	tok->next = NULL;
	return (tok);
}

static int	has_quotes_in_word(const char *s)
{
	while (*s && !ft_strchr("|&<>() \t", *s))
	{
		if (*s == '\'' || *s == '"')
			return (1);
		if (*s == '\\' && *(s + 1))
			s++;
		s++;
	}
	return (0);
}

static t_token	*get_next_token(const char **s, t_minishell *sh)
{
	char			*word;
	t_token_type	type;
	t_token			*token;
	const char		*start;

	if (**s == '\'' || **s == '"' || !ft_strchr("|&<>() \t", **s))
	{
		start = *s;
		word = extract_word(s, sh);
		if (!word)
			return (NULL);
		token = create_token(sh, word, T_WORD);
		token->was_quoted = has_quotes_in_word(start);
		return (token);
	}
	word = extract_operator(s, &type, sh);
	if (!word)
		return (NULL);
	return (create_token(sh, word, type));
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
			g_exit_status = 2;
			return (NULL);
		}
		add_token_to_list(&head, &tail, new_token);
	}
	return (head);
}
