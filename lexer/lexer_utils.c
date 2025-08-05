#include "minishell.h"

static char	*append_string(char *dest, const char *src, t_minishell *sh)
{
	char	*result;
	size_t	dest_len;
	size_t	src_len;
	size_t	i;
	size_t	j;

	if (!dest)
		return (gc_strdup(src, &sh->gc));
	if (!src)
		return (dest);
	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	result = gc_malloc(&sh->gc, dest_len + src_len + 1);
	i = -1;
	while (++i < dest_len)
		result[i] = dest[i];
	j = 0;
	while (j < src_len)
	{
		result[i + j] = src[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

static char	*process_unquoted_part(const char **s, t_minishell *sh)
{
	const char	*start;
	char		*part;
	size_t		len;

	start = *s;
	while (**s && !ft_strchr(" \t|&<>()\'\"", **s))
	{
		if (**s == '\\')
			*s += 2;
		else
			(*s)++;
	}
	len = *s - start;
	if (len > 0)
	{
		part = copy_word(start, len, sh);
		part = expand_variables(part, sh);
		return (part);
	}
	return (NULL);
}

char	*extract_word(const char **s, t_minishell *sh)
{
	char		*result;
	char		*part;

	result = NULL;
	while (**s && !ft_strchr(" \t|&<>()", **s))
	{
		if (**s == '\'' || **s == '"')
		{
			part = extract_quoted_word(s, sh);
			if (!part)
				return (NULL);
			result = append_string(result, part, sh);
		}
		else
		{
			part = process_unquoted_part(s, sh);
			if (part)
				result = append_string(result, part, sh);
		}
	}
	if (!result)
		result = gc_strdup("", &sh->gc);
	return (result);
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
	if (**s == '(')
		return (*type = T_PAREN_L, set_token_and_advance(s, 1, sh));
	if (**s == ')')
		return (*type = T_PAREN_R, set_token_and_advance(s, 1, sh));
	return (NULL);
}
