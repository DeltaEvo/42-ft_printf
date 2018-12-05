/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:51:38 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 15:13:10 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"
#include "ft/str.h"
#include <stdlib.h>
#include <string.h>

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

int	invalid_arg(t_fmt *fmt, t_ctx *ctx)
{
	pad_start(1, fmt, ctx, 0);
	ctx->write(ctx, fmt->end - 1, 1);
	pad_end(1, fmt, ctx);
	return (0);
}

int	fmt(t_fmt *fmt, t_ctx *ctx)
{
	pad_start(1, fmt, ctx, 0);
	ctx->write(ctx, "%", 1);
	pad_end(1, fmt, ctx);
	return (0);
}

void	write_wchar(t_ctx *ctx, const wchar_t *s, size_t len)
{
	char	buffer[MB_CUR_MAX];
	int		r;

	while (*s && len)
	{
		if ((r = wctomb(buffer, *s++)) < 1 || (size_t)r > len)
			break ;
		ctx->write(ctx, buffer, r);
		len -= r;
	}
}

size_t	wstr_len(const wchar_t *s)
{
	size_t	len;
	char	buffer[MB_CUR_MAX];
	int		r;

	len = 0;
	while (*s)
	{
		if ((r = wctomb(buffer, *s++)) < 1)
			break ;
		len += r;
	}
	return (len);
}

size_t	wstr_nlen(const wchar_t *s, size_t n)
{
	size_t	len;
	char	buffer[MB_CUR_MAX];
	int		r;

	len = 0;
	while (*s && n)
	{
		if ((r = wctomb(buffer, *s++)) < 1 || (size_t)r > n)
			break ;
		len += r;
		n -= r;
	}
	return (len);
}

int	fmts(t_fmt *fmt, t_ctx *ctx)
{
	size_t	len;

	TRY(get_arg(fmt->param, PTR, &ctx->va));
	if (fmt->param->value.p)
	{
		if (fmt->precision != -1)
			len = (fmt->length & PF_L ? wstr_nlen(fmt->param->value.p,
				fmt->precision) : strnlen(fmt->param->value.p, fmt->precision));
		else
			len = (fmt->length & PF_L ? wstr_len(fmt->param->value.p)
					: ft_strlen(fmt->param->value.p));
	}
	else
		len = fmt->precision == -1 ? 6 : fmt->precision;
	pad_start(len, fmt, ctx, 0);
	if (fmt->param->value.p)
		if (fmt->length & PF_L)
			write_wchar(ctx, fmt->param->value.p, len);
		else
			ctx->write(ctx, fmt->param->value.p, len);
	else
		ctx->write(ctx, "(null)", len);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtsu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmts(fmt, ctx));
}

int	fmtd(t_fmt *fmt, t_ctx *ctx)
{
	t_int_str	res;
	size_t		offset;
	size_t		len;

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 0), &ctx->va));
	res = ft_int_to_str(downcast(fmt->length, fmt->param->value.i, 0));
	len = res.len;
	if (fmt->precision != -1 && (size_t)fmt->precision > len)
		len = fmt->precision + (res.str[0] == '-');
	len += res.str[0] != '-' && fmt->flags & (PF_SPACE | PF_PLUS);
	len -= !!(fmt->precision == 0 && res.str[0] == '0');
	if (fmt->flags & PF_ZERO && (res.str[0] == '-'))
		ctx->write(ctx, res.str, offset = 1);
	else
		offset = 0;
	if (fmt->flags & PF_ZERO && fmt->flags & (PF_PLUS | PF_SPACE) &&
			res.str[0] != '-')
		ctx->write(ctx, fmt->flags & PF_PLUS ? "+" : " ", 1);
	pad_start(len, fmt, ctx, 1);
	if (!(fmt->flags & PF_ZERO) && fmt->flags & (PF_PLUS | PF_SPACE) &&
			res.str[0] != '-')
		ctx->write(ctx, fmt->flags & PF_PLUS ? "+" : " ", 1);
	if (res.str[0] == '-' && !offset)
		ctx->write(ctx, "-", offset = 1);
	if (fmt->precision != -1 && fmt->precision > res.len)
		ctx->writer(ctx, '0', fmt->precision - res.len + offset);
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str + offset, res.len - offset);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtdu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmtd(fmt, ctx));
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
		ctx->writer(ctx, '0', fmt->precision - res.len - !!(fmt->flags & PF_HASH));
	if (!(fmt->precision == 0 && res.str[0] == '0'))
		ctx->write(ctx, res.str, res.len);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtou(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmto(fmt, ctx));
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

int	fmtuu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmtu(fmt, ctx));
}

int	fmtx_impl(t_fmt *fmt, t_ctx *ctx, uint8_t maj, uint8_t prefix)
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

int	fmtx(t_fmt *fmt, t_ctx *ctx)
{
	return (fmtx_impl(fmt, ctx, 0, 0));
}

int	fmtxu(t_fmt *fmt, t_ctx *ctx)
{
	return (fmtx_impl(fmt, ctx, 1, 0));
}

int	fmtp(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmtx_impl(fmt, ctx, 0, 1));
}

int	fmtc(t_fmt *fmt, t_ctx *ctx)
{
	char	buffer[MB_CUR_MAX];
	int		len;

	TRY(get_arg(fmt->param, INT, &ctx->va));
	if (fmt->length & PF_L)
	{
		if ((len = wctomb(buffer, (wchar_t)fmt->param->value.i)) < 1)
			return (0);
	}
	else
	{
		buffer[0] = fmt->param->value.i;
		len = 1;
	}
	pad_start(len, fmt, ctx, 0);
	ctx->write(ctx, buffer, len);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtcu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmtc(fmt, ctx));
}
