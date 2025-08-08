#include "minishell.h"

int	is_var_char(char c, int first)
{
	if (first && c == '?')
		return (1);
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
		return (1);
	if (!first && (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char	*extract_var_name(const char *input, size_t *i, t_minishell *sh)
{
	const char	*start;
	size_t		len;
	char		*name;

	start = &input[*i];
	if (input[*i] == '?')
	{
		(*i)++;
		return (gc_strdup("?", &sh->gc));
	}
	len = 0;
	while (input[*i] && is_var_char(input[*i], 0))
	{
		(*i)++;
		len++;
	}
	name = gc_malloc(&sh->gc, len + 1);
	ft_strncpy(name, start, len);
	name[len] = '\0';
	return (name);
}

void	append_var_value(char *name, char *res, size_t *j, t_minishell *sh)
{
	char	*val;
	size_t	k;

	val = get_var_value(name, sh);
	if (!val)
		return ;
	k = 0;
	while (val[k])
		res[(*j)++] = val[k++];
}

size_t	calculate_needed_size(const char *input, t_minishell *sh)
{
	size_t	needed_size;
	size_t	i;
	char	*name;

	needed_size = ft_strlen(input) * 2;
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && is_var_char(input[i + 1], 1))
		{
			i++;
			name = extract_var_name(input, &i, sh);
			if (get_var_value(name, sh))
				needed_size += ft_strlen(get_var_value(name, sh));
		}
		else if (input[i] == '`')
		{
			needed_size += 1024;
			i++;
		}
		else
			i++;
	}
	return (needed_size + 1);
}
