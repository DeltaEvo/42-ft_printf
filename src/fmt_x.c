/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_x.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 12:23:53 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 12:26:45 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"

static int	fmtx_impl(t_fmt *fmt, t_ctx *ctx, uint8_t maj, uint8_t prefix)
{
	t_int_str16	res;
	size_t		len;

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	if (!prefix)
		prefix = fmt->flags & PF_HASH && fmt->param->value.i;
	res = ft_uint_to_str16(downcast(fmt->length, fmt->param->value.i, 1), maj);
	len = res.len;
	if (fmt->precision != -1 && (size_t)fmt->precision > len)
		len = fmt->precision;
	len += (prefix ? 2 : 0);
	len -= !!(fmt->precision == 0 && res.str[0] == '0');
	if (!(fmt->flags & PF_ZERO))
		pad_start(len, fmt, ctx, 1);
	if (prefix)
		ctx->write(ctx, maj ? "0X" : "0x", 2);
	if (fmt->flags & PF_ZERO)
		pad_start(len, fmt, ctx, 1);
	if (fmt->precision != -1 && fmt->precision > res.len)
		ctx->writer(ctx, '0', fmt->precision - res.len);
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str, res.len);
	pad_end(len, fmt, ctx);
	return (0);
}

int			fmtx(t_fmt *fmt, t_ctx *ctx)
{
	return (fmtx_impl(fmt, ctx, 0, 0));
}

int			fmtxu(t_fmt *fmt, t_ctx *ctx)
{
	return (fmtx_impl(fmt, ctx, 1, 0));
}

int			fmtp(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmtx_impl(fmt, ctx, 0, 1));
}
