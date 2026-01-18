/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:40:44 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/05 00:45:09 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	handle_specifier(char specifier, va_list *args, int *count)
{
	if (specifier == 'c')
		print_char(va_arg(*args, int), count);
	else if (specifier == 's')
		print_string(va_arg(*args, char *), count);
	else if (specifier == 'p')
		print_pointer(va_arg(*args, void *), count);
	else if (specifier == 'd' || specifier == 'i')
		print_int(va_arg(*args, int), count);
	else if (specifier == 'u')
		print_unsigned(va_arg(*args, unsigned int), count);
	else if (specifier == 'x')
		print_hex(va_arg(*args, unsigned int), 0, count);
	else if (specifier == 'X')
		print_hex(va_arg(*args, unsigned int), 1, count);
	else if (specifier == '%')
		print_char('%', count);
}

void	process_format(const char *format, va_list *args, int *count)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			handle_specifier(format[i + 1], args, count);
			i++;
		}
		else
		{
			ft_putchar_fd(format[i], 1);
			(*count)++;
		}
		i++;
	}
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, format);
	process_format(format, &args, &count);
	va_end(args);
	return (count);
}
