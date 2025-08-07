#include "minishell.h"

static void	init_shell(t_minishell *sh, char **envp)
{
	sh->gc.head = NULL;
	sh->tokens = NULL;
	sh->last_exit_status = 0;
	sh->in_logical_or_pipe = 0;
	sh->envp = copy_envp(envp);
	increment_shlvl(sh, &sh->envp);
}

static void	run_shell_loop(t_minishell *sh, char *input)
{
	t_cmd_node	*cmd_tree;

	if (*input && !is_only_whitespace(input))
		add_history(input);
	sh->tokens = tokenize_input(input, sh);
	if (!sh->tokens || !validate_token_stream(sh->tokens, sh))
	{
		gc_clear(&sh->gc);
		return ;
	}
	if (DEBUG_MODE)
		debug_print_tokens(sh->tokens);
	cmd_tree = parse_input(sh->tokens, sh);
	if (cmd_tree)
	{
		if (DEBUG_MODE)
			debug_print_ast(cmd_tree);
		sh->last_exit_status = execute_tree(cmd_tree, sh);
	}
	gc_clear(&sh->gc);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;
	char		*input;

	(void)argv;
	if (argc != 1)
		return (0);
	g_in_prompt = 1;
	setup_signal_handlers();
	init_shell(&sh, envp);
	while (42)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		run_shell_loop(&sh, input);
		free(input);
	}
	printf("Exiting minishell.\n");
	free_envp(sh.envp);
	rl_clear_history();
	return (0);
}
