#include "minishell.h"

static void	print_indent(int depth, int is_last)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (is_last)
			printf("└── ");
		else
			printf("├── ");
	}
}

static const char	*get_node_type_name(t_node_type type)
{
	if (type == N_SIMPLE)
		return ("SIMPLE");
	else if (type == N_PIPE)
		return ("PIPE");
	else if (type == N_AND)
		return ("AND");
	else if (type == N_OR)
		return ("OR");
	return ("UNKNOWN");
}

static void	print_cmd_args(t_cmd *cmd, int depth)
{
	int	i;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	while (cmd->argv[i])
	{
		print_indent(depth + 1, !cmd->argv[i + 1] && !cmd->redirs);
		printf("arg[%d]: \"%s\"\n", i, cmd->argv[i]);
		i++;
	}
}

static void	print_cmd_redirs(t_redir *redir, int depth)
{
	const char	*redir_names[] = {"<", ">", ">>", "<<"};

	while (redir)
	{
		print_indent(depth + 1, !redir->next);
		if (redir->type >= T_REDIR_IN && redir->type <= T_HEREDOC)
			printf("redir: %s \"%s\"\n",
				redir_names[redir->type - T_REDIR_IN], redir->file);
		else
			printf("redir: type=%d file=\"%s\"\n", redir->type, redir->file);
		redir = redir->next;
	}
}

void	print_ast_tree(t_cmd_node *node, int depth, int is_last)
{
	if (!node)
		return ;
	print_indent(depth, is_last);
	printf("🌳 %s\n", get_node_type_name(node->type));
	if (node->type == N_SIMPLE)
	{
		print_cmd_args(node->cmd, depth);
		if (node->cmd->redirs)
			print_cmd_redirs(node->cmd->redirs, depth);
	}
	else
	{
		if (node->left)
		{
			print_indent(depth + 1, 0);
			printf("left:\n");
			print_ast_tree(node->left, depth + 2, 0);
		}
		if (node->right)
		{
			print_indent(depth + 1, 1);
			printf("right:\n");
			print_ast_tree(node->right, depth + 2, 1);
		}
	}
}
