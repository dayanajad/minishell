/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:11:47 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/05 00:48:56 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_unsigned(unsigned int n)
{
	char	c;

	if (n >= 10)
		ft_putnbr_unsigned(n / 10);
	c = (n % 10) + '0';
	ft_putchar_fd(c, 1);
}

void	ft_puthex(unsigned int n, int is_upper)
{
	char	c;

	if (n >= 16)
		ft_puthex(n / 16, is_upper);
	if ((n % 16) < 10)
		c = (n % 16) + '0';
	else
	{
		if (is_upper)
			c = (n % 16) - 10 + 'A';
		else
			c = (n % 16) - 10 + 'a';
	}
	ft_putchar_fd(c, 1);
}

void	ft_putptr(uintptr_t n)
{
	char	c;

	if (n >= 16)
		ft_putptr(n / 16);
	if ((n % 16) < 10)
		c = (n % 16) + '0';
	else
		c = (n % 16) - 10 + 'a';
	ft_putchar_fd(c, 1);
}
