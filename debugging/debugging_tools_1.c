#include "minishell.h"

void	debug_print_ast(t_cmd_node *node)
{
	printf("\n" "═══════════════════════════════════════\n");
	printf("🚀 ABSTRACT SYNTAX TREE VISUALIZATION\n");
	printf("═══════════════════════════════════════\n");
	if (!node)
	{
		printf("❌ Empty AST\n");
		printf("═══════════════════════════════════════\n\n");
		return ;
	}
	print_ast_tree(node, 0, 1);
	printf("═══════════════════════════════════════\n\n");
}

void	debug_print_tokens(t_token *token)
{
	const char	*token_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "AND_IF", "OR_IF",
		"PAREN_L", "PAREN_R"
	};
	int			count;

	printf("\n" "═══════════════════════════════════════\n");
	printf("🪙  TOKEN STREAM VISUALIZATION\n");
	printf("═══════════════════════════════════════\n");
	count = 0;
	while (token)
	{
		printf("🔗 [%2d] %-12s │ \"%s\"\n", count,
			token_names[token->type], token->value);
		token = token->next;
		count++;
	}
	printf("═══════════════════════════════════════\n");
	printf("📊 Total tokens: %d\n", count);
	printf("═══════════════════════════════════════\n\n");
}
