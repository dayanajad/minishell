/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_readline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/02/07 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	*readline_active_ref(void)
{
	static volatile sig_atomic_t	active = 0;

	return (&active);
}

int	get_readline_active(void)
{
	return (*readline_active_ref());
}

void	set_readline_active(int active)
{
	*readline_active_ref() = active;
}
