#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;
	char		*input;
	t_cmd_node	*cmd_tree;

	(void)argv;
	if (argc == 1)
	{
		sh.gc.head = NULL;
		sh.tokens = NULL;
		sh.last_exit_status = 0;
		sh.envp = copy_envp(envp, &sh.gc);
		while (42)
		{
			input = readline(PROMPT);
			if (!input)
				break;
			sh.tokens = tokenize_input(input, &sh);
			if (!sh.tokens || !validate_token_stream(sh.tokens, &sh))
			{
				free(input);
				gc_clear(&sh.gc);
				continue;
			}
			print_token_list(sh.tokens);
			cmd_tree = parse_input(sh.tokens, &sh);
			if (cmd_tree)
			{
				print_cmd_node(cmd_tree);
				sh.last_exit_status = execute_tree(cmd_tree, &sh);
			}
			gc_clear(&sh.gc);
			free(input);
		}
		printf("Exiting minishell.\n");
	}
	return (0);
}
