/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:59:50 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 11:54:20 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

ssize_t	ft_printf(char const *fmt, ...)
{
	va_list	ap;
	ssize_t	r;

	va_start(ap, fmt);
	r = ft_vprintf(fmt, ap);
	va_end(ap);
	return (r);
}

ssize_t	ft_vprintf(char const *fmt, va_list ap)
{
	return (ft_vdprintf(STDOUT_FILENO, fmt, ap));
}

ssize_t	ft_dprintf(int fd, char const *fmt, ...)
{
	va_list	ap;
	ssize_t	r;

	va_start(ap, fmt);
	r = ft_vdprintf(fd, fmt, ap);
	va_end(ap);
	return (r);
}

ssize_t	ft_asnprintf(char **out, char const *fmt, ...)
{
	va_list	ap;
	ssize_t	r;

	va_start(ap, fmt);
	r = ft_vasnprintf(out, fmt, ap);
	va_end(ap);
	return (r);
}
