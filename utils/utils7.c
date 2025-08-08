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
	env_set(envp, shlvl_entry, &sh->env_gc);
}

void	init_default_env_vars(t_minishell *sh, char ***envp)
{
	char	*pwd_val;
	char	*underscore_val;
	char	*current_dir;
	char	*pwd_entry;
	char	*underscore_entry;

	pwd_val = env_get("PWD", *envp);
	if (!pwd_val)
	{
		current_dir = getcwd(NULL, 0);
		if (current_dir)
		{
			pwd_entry = gc_strjoin("PWD=", current_dir, &sh->gc);
			if (pwd_entry)
				env_set(envp, pwd_entry, &sh->env_gc);
			free(current_dir);
		}
	}
	underscore_val = env_get("_", *envp);
	if (!underscore_val)
	{
		underscore_entry = gc_strdup("_=/usr/bin/env", &sh->gc);
		if (underscore_entry)
			env_set(envp, underscore_entry, &sh->env_gc);
	}
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}
