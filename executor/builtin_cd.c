#include "minishell.h"

static char	*expand_tilde_path(const char *path, t_minishell *sh)
{
	const char	*home;

	home = env_get("HOME", sh->envp);
	if (!home)
		home = "/";
	if (path[1] == '\0')
		return (gc_strdup(home, &sh->gc));
	else if (path[1] == '/')
		return (gc_strjoin(home, path + 1, &sh->gc));
	else
		return (gc_strdup(path, &sh->gc));
}

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

static int	update_pwd_vars(char *current_dir, t_minishell *sh)
{
	char	*new_dir;

	new_dir = getcwd(NULL, 0);
	if (!new_dir)
	{
		free(current_dir);
		return (perror("cd: getcwd"), 1);
	}
	env_set(&sh->envp, gc_strjoin("OLDPWD=", current_dir, &sh->gc));
	env_set(&sh->envp, gc_strjoin("PWD=", new_dir, &sh->gc));
	free(current_dir);
	free(new_dir);
	return (0);
}

int	builtin_cd(char **argv, t_minishell *sh)
{
	char	*current_dir;
	char	*expanded_path;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (perror("cd: getcwd"), 1);
	expanded_path = expand_cd_path(argv[1], sh);
	if (!expanded_path)
	{
		free(current_dir);
		return (1);
	}
	if (chdir(expanded_path) != 0)
	{
		free(current_dir);
		return (perror("cd: chdir"), 1);
	}
	return (update_pwd_vars(current_dir, sh));
}
