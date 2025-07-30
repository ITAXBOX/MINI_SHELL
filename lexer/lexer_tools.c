#include "minishell.h"

char	*extract_quoted_word(const char **s, t_minishell *sh)
{
	char		quote;
	const char	*start;
	size_t		len;
	char		*word;

	quote = **s;
	(*s)++;
	start = *s;
	while (**s && **s != quote)
		(*s)++;
	if (**s != quote)
		return (NULL);
	len = *s - start;
	word = gc_malloc(&sh->gc, len + 1);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	if (**s == quote)
		(*s)++;
	return (word);
}
