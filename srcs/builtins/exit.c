#include "minishell.h"

int	builtin_exit(char **av, t_shell *shell)
{
	(void)av;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	shell->should_exit = 1;
	return (0);
}
