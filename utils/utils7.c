#include "minishell.h"

static int	ft_count_digits(int n)
{
	long	l;
	int		answer;

	l = n;
	answer = 0;
	if (n == 0)
		return (1);
	if (l < 0)
	{
		answer = 1;
		l = -l;
	}
	while (l != 0)
	{
		l /= 10;
		answer++;
	}
	return (answer);
}

char	*ft_itoa(int n, t_minishell *sh)
{
	char	*answer;
	int		digits_num;
	long	l;
	int		sign;

	sign = 0;
	digits_num = ft_count_digits(n);
	answer = gc_malloc(&sh->gc, sizeof(char) * (digits_num + 1));
	if (!answer)
		return (NULL);
	answer[digits_num--] = '\0';
	l = n;
	if (n < 0)
	{
		l = -l;
		sign = 1;
	}
	while (digits_num >= sign)
	{
		answer[digits_num--] = (l % 10) + '0';
		l /= 10;
	}
	if (n < 0)
		answer[digits_num] = '-';
	return (answer);
}

void	increment_shlvl(t_minishell *sh, char ***envp)
{
	char	*old_val;
	int		level;
	char	*new_val;
	char	*shlvl_entry;

	old_val = env_get("SHLVL", *envp);
	level = 1;
	if (old_val)
	{
		level = ft_atoi(old_val);
		if (level < 0)
			level = 0;
		level++;
	}
	if (level > 999)
		level = 1;
	new_val = ft_itoa(level, sh);
	if (!new_val)
		return ;
	shlvl_entry = gc_strjoin("SHLVL=", new_val, &sh->gc);
	if (!shlvl_entry)
		return ;
	env_set(envp, shlvl_entry);
}
