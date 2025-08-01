#include "minishell.h"

void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("  redir: type=%d, file=%s\n", redir->type, redir->file);
		redir = redir->next;
	}
}

void	print_simple_cmd(t_cmd_node *node)
{
	int	i;

	printf("Parsed Command:\n");
	i = 0;
	while (node->cmd->argv && node->cmd->argv[i])
	{
		printf("  argv[%d]: %s\n", i, node->cmd->argv[i]);
		i++;
	}
	if (node->cmd->redirs)
		print_redirs(node->cmd->redirs);
}

void	print_cmd_node(t_cmd_node *node)
{
	if (!node)
		return ;
	if (node->type == N_SIMPLE)
		print_simple_cmd(node);
	else if (node->type == N_PIPE)
	{
		printf("PIPE NODE:\n");
		print_cmd_node(node->left);
		print_cmd_node(node->right);
	}
	else if (node->type == N_AND)
	{
		printf("AND NODE:\n");
		print_cmd_node(node->left);
		print_cmd_node(node->right);
	}
	else if (node->type == N_OR)
	{
		printf("OR NODE:\n");
		print_cmd_node(node->left);
		print_cmd_node(node->right);
	}
}

void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s | Type: %d\n", token->value, token->type);
		token = token->next;
	}
}
