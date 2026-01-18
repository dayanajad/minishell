/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:34:26 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/05 00:48:17 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	hex_len(uintptr_t n)
{
	int	len;

	len = 1;
	while (n >= 16)
	{
		n /= 16;
		len++;
	}
	return (len);
}

void	print_hex(unsigned int n, int is_upper, int *count)
{
	ft_puthex(n, is_upper);
	(*count) += hex_len(n);
}

void	print_pointer(void *ptr, int *count)
{
	uintptr_t	addr;

	if (!ptr)
	{
		ft_putstr_fd("(nil)", 1);
		(*count) += 5;
		return ;
	}
	addr = (uintptr_t)ptr;
	ft_putstr_fd("0x", 1);
	(*count) += 2;
	ft_putptr(addr);
	(*count) += hex_len(addr);
}
