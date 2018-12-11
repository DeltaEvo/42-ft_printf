/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:02:11 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 13:36:54 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"

int			read_args(t_fmt *f, t_ctx *ctx)
{
	if (f->widtharg)
	{
		if (get_arg(f->widtharg, INT, &ctx->va) == -1)
			return (-1);
		if ((int)f->widtharg->value.i < 0)
		{
			f->width = -f->widtharg->value.i;
			f->flags |= PF_MINUS;
		}
		else
			f->width = f->widtharg->value.i;
	}
	if (f->precisionarg)
	{
		if (get_arg(f->precisionarg, INT, &ctx->va) == -1)
			return (-1);
		f->precision = f->precisionarg->value.i;
		if (f->precision < 0)
			f->precision = -1;
	}
	return (0);
}

int			pad_start(size_t len, t_fmt *fmt, t_ctx *ctx, uint8_t prec_c)
{
	if (!(fmt->flags & PF_MINUS) && len < fmt->width)
		ctx->writer(ctx, fmt->flags & PF_ZERO
		&& !(prec_c && fmt->precision != -1) ? '0' : ' ', fmt->width - len);
	return (0);
}

int			pad_end(size_t len, t_fmt *fmt, t_ctx *ctx)
{
	if (fmt->flags & PF_MINUS && len < fmt->width)
		ctx->writer(ctx, ' ', fmt->width - len);
	return (0);
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

t_types		type_for_length(uint8_t length, uint8_t unsign)
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
