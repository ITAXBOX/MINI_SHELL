#include "minishell.h"

void	print_cmd_node(t_cmd_node *node)
{
	int	i;

	if (!node || node->type != N_SIMPLE || !node->cmd)
	{
		printf("Invalid command node\n");
		return ;
	}
	printf("Parsed Command:\n");
	i = 0;
	while (node->cmd->argv[i])
	{
		printf("  argv[%d]: %s\n", i, node->cmd->argv[i]);
		i++;
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
