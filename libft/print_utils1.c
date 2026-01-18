/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:11:33 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/05 00:48:01 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_char(char c, int *count)
{
	ft_putchar_fd(c, 1);
	(*count)++;
}

void	print_string(char *s, int *count)
{
	int	i;

	if (!s)
	{
		ft_putstr_fd("(null)", 1);
		(*count) += 6;
		return ;
	}
	i = 0;
	while (s[i])
	{
		ft_putchar_fd(s[i], 1);
		(*count)++;
		i++;
	}
}

void	print_int(int n, int *count)
{
	char	*itoa;

	itoa = ft_itoa(n);
	if (!itoa)
		return ;
	ft_putstr_fd(itoa, 1);
	(*count) += ft_strlen(itoa);
	free(itoa);
}

static int	unsigned_len(unsigned int n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	print_unsigned(unsigned int n, int *count)
{
	ft_putnbr_unsigned(n);
	(*count) += unsigned_len(n);
}
