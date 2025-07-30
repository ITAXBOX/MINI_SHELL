#include "minishell.h"

void	skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static char	*copy_word(const char *start, size_t len, t_minishell *sh)
{
	char	*word;

	word = gc_malloc(&sh->gc, len + 1);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

char	*extract_word(const char **s, t_minishell *sh)
{
	const char	*start;
	size_t		len;

	if (**s == '\'' || **s == '"')
		return (extract_quoted_word(s, sh));
	start = *s;
	while (**s && !ft_strchr(" \t|&<>", **s))
	{
		if (**s == '\'' || **s == '"')
			break ;
		(*s)++;
	}
	len = *s - start;
	return (copy_word(start, len, sh));
}

static char	*set_token_and_advance(const char **s, int len, t_minishell *sh)
{
	char	*op;

	op = gc_malloc(&sh->gc, len + 1);
	ft_memcpy(op, *s, len);
	op[len] = '\0';
	*s += len;
	return (op);
}

char	*extract_operator(const char **s, t_token_type *type, t_minishell *sh)
{
	if (ft_strncmp(*s, "&&", 2) == 0)
		return (*type = T_AND_IF, set_token_and_advance(s, 2, sh));
	if (ft_strncmp(*s, "||", 2) == 0)
		return (*type = T_OR_IF, set_token_and_advance(s, 2, sh));
	if (ft_strncmp(*s, ">>", 2) == 0)
		return (*type = T_REDIR_APPEND, set_token_and_advance(s, 2, sh));
	if (ft_strncmp(*s, "<<", 2) == 0)
		return (*type = T_HEREDOC, set_token_and_advance(s, 2, sh));
	if (**s == '|')
		return (*type = T_PIPE, set_token_and_advance(s, 1, sh));
	if (**s == '<')
		return (*type = T_REDIR_IN, set_token_and_advance(s, 1, sh));
	if (**s == '>')
		return (*type = T_REDIR_OUT, set_token_and_advance(s, 1, sh));
	return (NULL);
}
