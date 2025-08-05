#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;
	char		*input;
	t_cmd_node	*cmd_tree;

	(void)argv;
	if (argc == 1)
	{
		setup_signal_handlers();
		sh.gc.head = NULL;
		sh.tokens = NULL;
		sh.last_exit_status = 0;
		sh.in_logical_or_pipe = 0;
		sh.envp = copy_envp(envp);
		while (42)
		{
			input = readline(PROMPT);
			if (!input)
				break;
			if (*input && !is_only_whitespace(input))
				add_history(input);
			sh.tokens = tokenize_input(input, &sh);
			if (!sh.tokens || !validate_token_stream(sh.tokens, &sh))
			{
				free(input);
				gc_clear(&sh.gc);
				continue;
			}
			// print_token_list(sh.tokens);
			cmd_tree = parse_input(sh.tokens, &sh);
			if (cmd_tree)
			{
				// print_cmd_node(cmd_tree);
				sh.last_exit_status = execute_tree(cmd_tree, &sh);
			}
			gc_clear(&sh.gc);
			free(input);
		}
		printf("Exiting minishell.\n");
		free_envp(sh.envp);
		rl_clear_history();
	}
	return (0);
}
