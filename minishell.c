#include "minishell.h"

int	main(void)
{
	t_minishell	sh;
	char		*input;
	t_cmd_node	*cmd_tree;

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
		cmd_tree = parse_input(sh.tokens, &sh);
		if (cmd_tree)
			print_cmd_node(cmd_tree);
		gc_clear(&sh.gc);
		free(input);
	}
	printf("Exiting minishell.\n");
	return (0);
}
