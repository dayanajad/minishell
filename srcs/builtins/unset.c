#include "minishell.h"

int	builtin_unset(char **av, t_env **env)
{
	(void)av;
	(void)env;
	ft_putstr_fd("unset: not implemented yet\n", STDERR_FILENO);
	return (1);
}
