#include "minishell.h"

int	builtin_export(char **av, t_env **env)
{
	(void)av;
	(void)env;
	ft_putstr_fd("export: not implemented yet\n", STDERR_FILENO);
	return (1);
}
