#include "minishell.h"

int	is_only_whitespace(const char *s)
{
	const char	*p;

	p = s;
	skip_whitespace(&p);
	return (*p == '\0');
}

char	**resize_argv(char **old_argv, size_t old_size
		, size_t new_capacity, t_minishell *sh)
{
	char	**new_argv;
	size_t	i;

	new_argv = gc_malloc(&sh->gc, sizeof(char *) * (new_capacity + 1));
	i = 0;
	while (i < old_size)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
	return (new_argv);
}

int	match_pattern(const char *pattern, const char *text)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			while (*text)
			{
				if (match_pattern(pattern, text))
					return (1);
				text++;
			}
			return (0);
		}
		else if (*pattern == *text)
		{
			pattern++;
			text++;
		}
		else
			return (0);
	}
	return (*text == '\0');
}

void	syntax_error(const char *token, t_minishell *sh)
{
	write(2, "minishell: syntax error near unexpected token `", 48);
	write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
	sh->last_exit_status = 2;
}

int	is_valid_identifier(const char *s)
{
	int	i;

	i = 0;
	if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
