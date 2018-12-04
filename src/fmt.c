#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"
#include "ft/str.h"
#include <stdlib.h>

void pad_start(size_t len, t_fmt *fmt, t_ctx *ctx)
{
	if (!(fmt->flags & PF_MINUS) && len < fmt->width)
		ctx->writer(ctx, fmt->flags & PF_ZERO ? '0' : ' ', fmt->width - len);
}

void pad_end(size_t len, t_fmt *fmt, t_ctx *ctx)
{
	if (fmt->flags & PF_MINUS && len < fmt->width)
		ctx->writer(ctx, ' ', fmt->width - len);
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
	pad_start(1, fmt, ctx);
	ctx->write(ctx, fmt->end - 1, 1);
	pad_end(1, fmt, ctx);
	return (0);
}

int	fmt(t_fmt *fmt, t_ctx *ctx)
{
	pad_start(1, fmt, ctx);
	ctx->write(ctx, "%", 1);
	pad_end(1, fmt, ctx);
	return (0);
}

void	write_wchar(t_ctx *ctx, const wchar_t *s)
{
	char	buffer[MB_CUR_MAX];
	size_t	len;

	while (*s)
	{
		len = wctomb(buffer, *s++);
		ctx->write(ctx, buffer, len);
	}
}

int	fmts(t_fmt *fmt, t_ctx *ctx)
{
	size_t len;

	TRY(get_arg(fmt->param, PTR, &ctx->va));
	if (fmt->param->value.p)
		len = fmt->length & PF_L ? wcslen(fmt->param->value.p) : ft_strlen(fmt->param->value.p);
	else
		len = 6;
	pad_start(len, fmt, ctx);
	if (fmt->param->value.p)
		if (fmt->length & PF_L)
			write_wchar(ctx, fmt->param->value.p);
		else
			ctx->write(ctx, fmt->param->value.p, len);
	else
		ctx->write(ctx, "(null)", 6);
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

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 0), &ctx->va));
	res = ft_int_to_str(fmt->param->value.i);
	if (fmt->flags & PF_ZERO && (res.str[0] == '-'))
		ctx->write(ctx, res.str, offset = 1);
	else
		offset = 0;
	if (fmt->flags & PF_ZERO && fmt->flags & (PF_PLUS | PF_SPACE) && res.str[0] != '-')
		ctx->write(ctx, fmt->flags & PF_PLUS ? "+" : " ", 1);
	pad_start(res.len + !!(fmt->flags & (PF_SPACE | PF_PLUS)), fmt, ctx);
	if (!(fmt->flags & PF_ZERO) && fmt->flags & (PF_PLUS | PF_SPACE) && res.str[0] != '-')
		ctx->write(ctx, fmt->flags & PF_PLUS ? "+" : " ", 1);
	ctx->write(ctx, res.str + offset, res.len - offset);
	pad_end(res.len + !!(fmt->flags & (PF_SPACE | PF_PLUS)), fmt, ctx);
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

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	res = ft_uint_to_str8(fmt->param->value.i);
	pad_start(res.len + (fmt->flags & PF_HASH ? res.str[0] != '0' : 0), fmt, ctx);
	if (fmt->flags & PF_HASH && res.str[0] != '0')
		ctx->write(ctx, "0", 1);
	ctx->write(ctx, res.str, res.len);
	pad_end(res.len + (fmt->flags & PF_HASH ? res.str[0] != '0' : 0), fmt, ctx);
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

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	res = ft_uint_to_str(fmt->param->value.i);
	pad_start(res.len, fmt, ctx);
	ctx->write(ctx, res.str, res.len);
	pad_end(res.len, fmt, ctx);
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

	TRY(get_arg(fmt->param, type_for_length(fmt->length, 1), &ctx->va));
	if (!prefix)
		prefix = fmt->flags & PF_HASH && fmt->param->value.i;
	res = ft_uint_to_str16(fmt->param->value.i, maj);
	if (!(fmt->flags & PF_ZERO))
		pad_start(res.len + (prefix ? 2 : 0), fmt, ctx);
	if (prefix)
		ctx->write(ctx, maj ? "0X" : "0x", 2);
	if (fmt->flags & PF_ZERO)
		pad_start(res.len + (prefix ? 2 : 0), fmt, ctx);
	ctx->write(ctx, res.str, res.len);
	pad_end(res.len + (prefix ? 2 : 0), fmt, ctx);
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
	size_t	len;

	TRY(get_arg(fmt->param, INT, &ctx->va));
	if (fmt->length & PF_L)
		len = wctomb(buffer, (wchar_t)fmt->param->value.i);
	else
	{
		buffer[0] = fmt->param->value.i;
		len = 1;
	}	
	pad_start(len, fmt, ctx);
	ctx->write(ctx, buffer, len);
	pad_end(len, fmt, ctx);
	return (0);
}

int	fmtcu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length |= PF_L;
	return (fmtc(fmt, ctx));
}
