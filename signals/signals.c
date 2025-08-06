#include "minishell.h"

volatile sig_atomic_t	g_in_prompt = 1;

void	sigint_handler_prompt(int sig)
{
	if (sig == SIGINT && g_in_prompt)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, sigint_handler_prompt);
	signal(SIGQUIT, SIG_IGN);
}
