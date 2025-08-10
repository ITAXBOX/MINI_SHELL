#include "minishell.h"

static char	*expand_cd_path(const char *path, t_minishell *sh)
{
	if (!path)
	{
		path = env_get("HOME", sh->envp);
		if (!path)
			return (write(2, "cd: HOME not set\n", 17), NULL);
		return (gc_strdup(path, &sh->gc));
	}
	else if (path[0] == '-' && path[1] == '\0')
	{
		path = env_get("OLDPWD", sh->envp);
		if (!path)
			return (write(2, "cd: OLDPWD not set\n", 19), NULL);
		printf("%s\n", path);
		return (gc_strdup(path, &sh->gc));
	}
	else if (path[0] == '~')
		return (expand_tilde_path(path, sh));
	return (gc_strdup(path, &sh->gc));
}

static void	handle_getcwd_failure(char *pwd_env, t_minishell *sh)
{
	char	*last_slash;
	char	*new_pwd;
	size_t	len;
	int		i;

	last_slash = NULL;
	i = 0;
	while (pwd_env[i])
	{
		if (pwd_env[i] == '/')
			last_slash = &pwd_env[i];
		i++;
	}
	if (last_slash && last_slash != pwd_env)
	{
		len = last_slash - pwd_env;
		new_pwd = gc_malloc(&sh->gc, len + 1);
		ft_strncpy(new_pwd, pwd_env, len);
		new_pwd[len] = '\0';
		env_set(&sh->envp, gc_strjoin("PWD=", new_pwd, &sh->gc), &sh->env_gc);
	}
	else
		env_set(&sh->envp, gc_strdup("PWD=/", &sh->gc), &sh->env_gc);
}

static char	*get_current_directory(t_minishell *sh)
{
	char	*current_dir;
	char	*pwd_env;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		pwd_env = env_get("PWD", sh->envp);
		if (pwd_env)
			current_dir = gc_strdup(pwd_env, &sh->gc);
	}
	return (current_dir);
}

static int	update_pwd_vars(char *current_dir, t_minishell *sh, int should_free)
{
	char	*new_dir;
	char	*pwd_env;

	new_dir = getcwd(NULL, 0);
	env_set(&sh->envp, gc_strjoin("OLDPWD=", current_dir, &sh->gc),
		&sh->env_gc);
	if (!new_dir)
	{
		perror("cd: getcwd");
		pwd_env = env_get("PWD", sh->envp);
		if (pwd_env)
			handle_getcwd_failure(pwd_env, sh);
		if (should_free)
			free(current_dir);
		return (0);
	}
	env_set(&sh->envp, gc_strjoin("PWD=", new_dir, &sh->gc), &sh->env_gc);
	if (should_free)
		free(current_dir);
	free(new_dir);
	return (0);
}

int	builtin_cd(char **argv, t_minishell *sh)
{
	char	*current_dir;
	char	*expanded_path;
	int		is_from_getcwd;

	current_dir = get_current_directory(sh);
	if (!current_dir)
		return (perror("cd: getcwd"), 1);
	is_from_getcwd = (getcwd(NULL, 0) != NULL);
	if (is_from_getcwd)
		free(getcwd(NULL, 0));
	expanded_path = expand_cd_path(argv[1], sh);
	if (!expanded_path)
	{
		if (is_from_getcwd)
			free(current_dir);
		return (1);
	}
	if (chdir(expanded_path) != 0)
	{
		if (is_from_getcwd)
			free(current_dir);
		return (perror("cd: chdir"), 1);
	}
	return (update_pwd_vars(current_dir, sh, is_from_getcwd));
}
