#include "../../incs/mini_header.h"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
        ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	choose_signal(int s)
{
	static struct sigaction	sa;

	if (s == 1)
	{
		sa.sa_handler = signal_handler;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
        signal(SIGQUIT, SIG_IGN);
	}
	else if (s == 2)
	{
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
}

