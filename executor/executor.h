#ifndef EXECUTOR_H
# define EXECUTOR_H

int		run_builtin(t_cmd *cmd, t_minishell *sh);

char	*resolve_command_path(const char *cmd, t_minishell *sh);

int		fork_and_execute_builtin(t_cmd *cmd, t_minishell *sh);
int		execute_tree(t_cmd_node *node, t_minishell *sh);

int		builtin_cd(char **argv, t_minishell *sh);
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv, t_minishell *sh);
int		builtin_env(t_minishell *sh);

void	clean_argv(char **argv);
char	*env_get(const char *key, char **envp);
int		is_expandable_command(const char *cmd);
int		execute_expanded_command(const char *cmd_str, t_minishell *sh);	
int		handle_signal_termination(int status);
int		handle_heredoc(const char *delimiter, t_minishell *sh, int expand_vars);
int		execute_pipe(t_cmd_node *node, t_minishell *sh);

void	append_command_output(const char *cmd, char *res,
			size_t *j, t_minishell *sh);
void	handle_redirections(t_redir *redir_list);
#endif