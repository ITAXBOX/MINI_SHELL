#include "minishell.h"

static char	*itoa_exit_status(int status, t_minishell *sh)
{
	char	buf[12];
	int		i;
	int		is_negative;
	int		j;

	i = 11;
	j = 0;
	while (j < 12)
		buf[j++] = '\0';
	is_negative = 0;
	if (status < 0)
	{
		is_negative = 1;
		status = -status;
	}
	if (status == 0)
		buf[--i] = '0';
	while (status > 0)
	{
		buf[--i] = '0' + (status % 10);
		status /= 10;
	}
	if (is_negative)
		buf[--i] = '-';
	return (gc_strdup(&buf[i], &sh->gc));
}

char	*get_var_value(const char *var, t_minishell *sh)
{
	char	*val;

	if (ft_strncmp(var, "?", 1) == 0 && var[1] == '\0')
		return (itoa_exit_status(sh->last_exit_status, sh));
	val = env_get(var, sh->envp);
	if (!val)
		return (gc_strdup("", &sh->gc));
	return (gc_strdup(val, &sh->gc));
}

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
	{
		if (**s == '\\')
			(*s)++;
		(*s)++;
	}
	if (**s != quote)
		return (NULL);
	len = *s - start;
	word = gc_malloc(&sh->gc, len + 1);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	if (**s == quote)
		(*s)++;
	if (quote == '"')
		return (expand_variables(word, sh));
	return (word);
}
