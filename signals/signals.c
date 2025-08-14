#include "minishell.h"

int	g_in_prompt = 1;

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

void	reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
