#ifndef EXECUTOR_H
# define EXECUTOR_H

void	env_unset(char **envp, const char *key);
int		builtin_exit(char **argv, t_minishell *sh);
int		builtin_export(char **argv, t_minishell *sh);
int		builtin_unset(char **argv, t_minishell *sh);

int		is_builtin(const char *cmd);
int		run_builtin(t_cmd *cmd, t_minishell *sh);

char	*resolve_command_path(const char *cmd, t_minishell *sh);

int		execute_tree(t_cmd_node *node, t_minishell *sh);

#endif