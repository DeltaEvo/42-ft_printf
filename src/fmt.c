/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:51:38 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 14:13:26 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"
#include "ft/str.h"
#include <stdlib.h>

int		invalid_arg(t_fmt *fmt, t_ctx *ctx)
{
	pad_start(1, fmt, ctx, 0);
	ctx->write(ctx, fmt->end - 1, 1);
	pad_end(1, fmt, ctx);
	return (0);
}

int		fmt(t_fmt *fmt, t_ctx *ctx)
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
		if ((r = ft_wctomb(buffer, *s++)) < 1 || (size_t)r > len)
			break ;
		ctx->write(ctx, buffer, r);
		len -= r;
	}
}

int		fmts(t_fmt *fmt, t_ctx *ctx)
{
	size_t	len;

	TRY(get_arg(fmt->param, PTR, &ctx->va));
	if (fmt->param->value.p)
	{
		if (fmt->precision != -1)
			len = (fmt->length & PF_L ? ft_wcstrnlen(fmt->param->value.p,
			fmt->precision) : ft_strnlen(fmt->param->value.p, fmt->precision));
		else
			len = (fmt->length & PF_L ? ft_wcstrlen(fmt->param->value.p)
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

int		fmtc(t_fmt *fmt, t_ctx *ctx)
{
	char	buffer[MB_CUR_MAX];
	int		len;

	TRY(get_arg(fmt->param, INT, &ctx->va));
	if (fmt->length & PF_L)
	{
		if ((len = ft_wctomb(buffer, (wchar_t)fmt->param->value.i)) < 1)
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
