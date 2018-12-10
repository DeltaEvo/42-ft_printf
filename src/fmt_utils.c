/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:02:11 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/10 14:04:38 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"

void	pad_start(size_t len, t_fmt *fmt, t_ctx *ctx, uint8_t prec_c)
{
	if (!(fmt->flags & PF_MINUS) && len < fmt->width)
		ctx->writer(ctx, fmt->flags & PF_ZERO
		&& !(prec_c && fmt->precision != -1) ? '0' : ' ', fmt->width - len);
}

void	pad_end(size_t len, t_fmt *fmt, t_ctx *ctx)
{
	if (fmt->flags & PF_MINUS && len < fmt->width)
		ctx->writer(ctx, ' ', fmt->width - len);
}

uintmax_t	downcast(uint8_t length, uintmax_t nb, uint8_t unsign)
{
	if (unsign)
	{
		if (length & PF_HH)
			return ((unsigned char)nb);
		if (length & PF_H)
			return ((unsigned short)nb);
	}
	else
	{
		if (length & PF_HH)
			return ((char)nb);
		if (length & PF_H)
			return ((short)nb);
	}
	return (nb);
}

t_types	type_for_length(uint8_t length, uint8_t unsign)
{
	if (length & PF_L)
		return (unsign ? ULONG : LONG);
	if (length & PF_LL)
		return (unsign ? ULLONG : LLONG);
	if (length & PF_J)
		return (unsign ? UIMAX : IMAX);
	if (length & PF_Z)
		return (unsign ? USIZET : SIZET);
	return (unsign ? UINT : INT);
}
