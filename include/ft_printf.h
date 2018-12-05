/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:31:25 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 14:31:33 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <sys/types.h>

ssize_t	ft_printf(char const *fmt, ...);
ssize_t	ft_vprintf(char const *fmt, va_list ap);
ssize_t	ft_dprintf(int fd, char const *fmt, ...);
ssize_t	ft_vdprintf(int fd, char const *fmt, va_list ap);
ssize_t	ft_asnprintf(char **out, char const *fmt, ...);
ssize_t	ft_vasnprintf(char **out, char const *fmt, va_list ap);

#endif
