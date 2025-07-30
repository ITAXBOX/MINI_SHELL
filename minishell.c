#include "minishell.h"

static void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s | Type: %d\n", token->value, token->type);
		token = token->next;
	}
}

int	main(void)
{
	t_minishell	sh;
	char		*input;

	sh.gc.head = NULL;
	sh.tokens = NULL;
	sh.last_exit_status = 0;
	while (42)
	{
		input = readline(PROMPT);
		if (!input)
			break;
		sh.tokens = tokenize_input(input, &sh);
		if (!sh.tokens)
		{
			printf("Lexer error.\n");
			free(input);
			continue;
		}
		print_token_list(sh.tokens);
		gc_clear(&sh.gc);
		free(input);
	}
	printf("Exiting minishell.\n");
	return (0);
}
