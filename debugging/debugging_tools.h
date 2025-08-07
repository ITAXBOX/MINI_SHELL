#ifndef DEBUGGING_TOOLS_H
# define DEBUGGING_TOOLS_H

# include "minishell.h"

/* Enhanced debug visualizations */
void	debug_print_ast(t_cmd_node *node);
void	debug_print_tokens(t_token *token);

void	print_ast_tree(t_cmd_node *node, int depth, int is_last);

#endif
