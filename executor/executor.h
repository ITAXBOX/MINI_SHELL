#ifndef EXECUTOR_H
# define EXECUTOR_H

char	*resolve_command_path(const char *cmd, t_minishell *sh);

int		execute_tree(t_cmd_node *node, t_minishell *sh);

#endif