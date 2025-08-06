#ifndef EXECUTOR_H
# define EXECUTOR_H

int		run_builtin(t_cmd *cmd, t_minishell *sh);

char	*resolve_command_path(const char *cmd, t_minishell *sh);

int		fork_and_execute_builtin(t_cmd *cmd, t_minishell *sh);
int		execute_tree(t_cmd_node *node, t_minishell *sh);

int		builtin_cd(char **argv, t_minishell *sh);
int		builtin_echo(char **argv, t_minishell *sh);
int		builtin_pwd(char **argv);
int		builtin_env(t_minishell *sh);

char	*env_get(const char *key, char **envp);
int		handle_heredoc(const char *delimiter);
int		execute_pipe(t_cmd_node *node, t_minishell *sh);

#endif