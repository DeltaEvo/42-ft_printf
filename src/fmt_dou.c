/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_dou.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 12:28:15 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 13:37:23 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"

int	fmtd(t_fmt *fmt, t_ctx *ctx)
{
	t_int_str	res;
	size_t		len;
	uint8_t		neg;

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 0), &ctx->va));
	res = ft_int_to_str(downcast(fmt->length, fmt->param->value.i, 0));
	neg = res.str[0] == '-';
	len = res.len;
	if (fmt->precision != -1 && (size_t)fmt->precision > len - neg)
		len = fmt->precision + neg;
	len += res.str[0] != '-' && fmt->flags & (PF_SPACE | PF_PLUS);
	len -= !!(fmt->precision == 0 && res.str[0] == '0');
	if (!(fmt->flags & PF_ZERO))
		pad_start(len, fmt, ctx, 1);
	if (neg)
		ctx->write(ctx, res.str, 1);
	else if (fmt->flags & (PF_PLUS | PF_SPACE))
		ctx->write(ctx, fmt->flags & PF_PLUS ? "+" : " ", 1);
	if (fmt->flags & PF_ZERO)
		pad_start(len, fmt, ctx, 1);
	if (fmt->precision != -1 && fmt->precision > res.len - neg)
		ctx->writer(ctx, '0', fmt->precision - res.len + neg);
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str + neg, res.len - neg);
	return (pad_end(len, fmt, ctx));
}

int	fmto(t_fmt *fmt, t_ctx *ctx)
{
	t_int_str8	res;
	size_t		len;

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	res = ft_uint_to_str8(downcast(fmt->length, fmt->param->value.i, 1));
	len = res.len;
	if (fmt->precision != -1 && (size_t)fmt->precision > len)
		len = fmt->precision;
	len += (fmt->flags & PF_HASH ? res.str[0] != '0' : 0);
	len -= !!(fmt->precision == 0 && res.str[0] == '0');
	pad_start(len, fmt, ctx, 1);
	if (fmt->flags & PF_HASH && (res.str[0] != '0' || fmt->precision == 0))
		ctx->write(ctx, "0", 1);
	if (fmt->precision != -1 && fmt->precision > res.len)
		ctx->writer(ctx, '0', fmt->precision - res.len -
				!!(fmt->flags & PF_HASH));
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str, res.len);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtu(t_fmt *fmt, t_ctx *ctx)
{
	t_int_str	res;
	size_t		len;

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	res = ft_uint_to_str(downcast(fmt->length, fmt->param->value.i, 1));
	len = res.len;
	if (fmt->precision != -1 && (size_t)fmt->precision > len)
		len = fmt->precision;
	len -= !!(fmt->precision == 0 && res.str[0] == '0');
	pad_start(len, fmt, ctx, 1);
	if (fmt->precision != -1 && fmt->precision > res.len)
		ctx->writer(ctx, '0', fmt->precision - res.len);
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str, res.len);
	pad_end(len, fmt, ctx);
	return (0);
}
