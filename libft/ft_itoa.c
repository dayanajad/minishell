/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:38:07 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/05/22 21:36:12 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_len(int n)
{
	size_t	len;

	len = 1;
	while (n / 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	fill_newstr(char *str, int n, size_t len, int sign)
{
	str[len] = '\0';
	while (n >= 10)
	{
		str[--len] = n % 10 + '0';
		n /= 10;
	}
	str[--len] = n + '0';
	if (sign)
		str[--len] = '-';
}

char	*ft_itoa(int n)
{
	int		sign;
	size_t	len;
	char	*newstr;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	sign = 0;
	if (n < 0)
	{
		sign = 1;
		n = -n;
	}
	len = get_len(n) + sign;
	newstr = malloc(len + 1);
	if (!newstr)
		return (NULL);
	fill_newstr(newstr, n, len, sign);
	return (newstr);
}
