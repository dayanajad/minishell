#include "minishell.h"

int	builtin_cd(char **av, t_shell *shell)
{
	(void)av;
	(void)shell;
	ft_putstr_fd("cd: not implemented yet\n", STDERR_FILENO);
	return (1);
}
