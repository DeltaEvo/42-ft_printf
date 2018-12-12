/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:00:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/12 14:56:10 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt_f.h"
#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"
#include "ft/double_parts.h"
#include "ft/mem.h"
#include <stdio.h>

static void	print(t_ctx *ctx, uint32_t *bigint, uint32_t len, int32_t maxnums)
{
	int32_t		i;
	t_int_str	res;

	i = len;
	if (i)
		while (--i >= 0)
		{
			res = ft_uint_to_str(bigint[i]);
			if (maxnums == -1)
			{
				if ((uint32_t)i != len - 1)
					ctx->writer(ctx, '0', 9 - res.len);
				ctx->write(ctx, res.str, res.len);
			}
			else
			{
				if (res.len != 9)
					ctx->writer(ctx, '0', ft_min(9 - res.len, maxnums));
				if ((9 - res.len) < maxnums)
					ctx->write(ctx, res.str, ft_min(res.len, maxnums));
				return ;
			}
			if (maxnums != -1)
				maxnums -= 9;
		}
}

static int	print_float(t_ctx *ctx, t_fmt *fmt, uint32_t *bigi,
		uint32_t ints[3])
{
	t_int_str		r;
	const uint16_t	p_e = ints[1] / 9;
	const uint16_t	l_e = ints[1] % 9;
	uint32_t		c_len;

	c_len = (!l_e || nb_len(bigi[*ints - 1]) == l_e) + nb_len(bigi[*ints - 1])
		+ ((int32_t)*ints - p_e - 1) * 9 - l_e + (fmt->precision ?
	fmt->precision + 1 : 0) + (ints[2] || (fmt->flags & (PF_SPACE | PF_PLUS)));
	(!(fmt->flags & PF_ZERO) ? pad_start(c_len, fmt, ctx, 0) : 0);
	(ints[2] ? ctx->write(ctx, "-", 1) : 0);
	if (!ints[2] && fmt->flags & (PF_SPACE | PF_PLUS))
		ctx->write(ctx, fmt->flags & PF_SPACE ? " " : "+", 1);
	(fmt->flags & PF_ZERO ? pad_start(c_len, fmt, ctx, 0) : 0);
	bigint_round(bigi, *ints, ints[1], fmt->precision);
	print(ctx, bigi + p_e + 1, *ints - p_e - 1, -1);
	r = ft_uint_to_str(bigi[p_e]);
	ctx->write(ctx, r.len > l_e ? r.str : "0", r.len > l_e ? r.len - l_e : 1);
	if (!fmt->precision)
		return (pad_end(c_len, fmt, ctx));
	ctx->write(ctx, ".", 1);
	if (l_e > r.len)
		ctx->writer(ctx, '0', ft_min(fmt->precision, l_e - r.len));
	ctx->write(ctx, r.str + (r.len - l_e), ft_min(fmt->precision, l_e));
	print(ctx, bigi, p_e, fmt->precision - ft_min(fmt->precision, l_e));
	return (pad_end(c_len, fmt, ctx));
}

static void	print_real(t_ctx *ctx, t_fmt *fmt, uint32_t *bigint, uint32_t i[2])
{
	const uint32_t	len = i[0];
	const uint32_t	sign = i[1];
	uint32_t		c_len;

	c_len = nb_len(bigint[len - 1]) + (len - 1) * 9
		+ (fmt->precision ? 1 + fmt->precision : 0)
		+ (sign || fmt->flags & (PF_SPACE | PF_PLUS));
	if (!(fmt->flags & PF_ZERO))
		pad_start(c_len, fmt, ctx, 0);
	if (sign)
		ctx->write(ctx, "-", 1);
	else if (fmt->flags & (PF_SPACE | PF_PLUS))
		ctx->write(ctx, fmt->flags & PF_SPACE ? " " : "+", 1);
	if (fmt->flags & PF_ZERO)
		pad_start(c_len, fmt, ctx, 0);
	print(ctx, bigint, len, -1);
	if (fmt->precision)
	{
		ctx->write(ctx, ".", 1);
		ctx->writer(ctx, '0', fmt->precision);
	}
	pad_end(c_len, fmt, ctx);
}

static int	print_special(t_fmt *fmt, t_ctx *ctx, t_dparts d, uint32_t len)
{
	if (d.p.exp)
	{
		len = 3 + (d.p.sign && !d.p.frac);
		if (fmt->width > len)
			ctx->writer(ctx, ' ', fmt->width - len);
		ctx->write(ctx, "-", !d.p.frac && d.p.sign);
		ctx->write(ctx, d.p.frac ? "nan" : "inf", 3);
	}
	else
	{
		len = 1 + (fmt->precision ? 1 + fmt->precision : 0) + d.p.sign;
		if (!(fmt->flags & PF_ZERO))
			pad_start(len, fmt, ctx, 0);
		if (d.p.sign)
			ctx->write(ctx, "-", 1);
		else if (fmt->flags & (PF_SPACE | PF_PLUS))
			ctx->write(ctx, fmt->flags & PF_SPACE ? " " : "+", 1);
		if (fmt->flags & PF_ZERO)
			pad_start(len, fmt, ctx, 0);
		ctx->write(ctx, "0.", !!fmt->precision + 1);
		if (fmt->precision)
			ctx->writer(ctx, '0', fmt->precision);
	}
	return (pad_end(len, fmt, ctx));
}

int			fmtf(t_fmt *fmt, t_ctx *ctx)
{
	t_dparts	d;
	uint32_t	bigint[50];
	int16_t		exp;
	uint64_t	frac;
	uint32_t	len;

	ft_memset(bigint, 0, sizeof(bigint));
	TRY(get_arg(fmt->param, fmt->length & PF_LU ? LDBL : DBL, &ctx->va));
	if (fmt->precision == -1)
		fmt->precision = 6;
	d.d = fmt->param->value.d;
	if ((d.p.exp == 0 && d.p.frac == 0) || d.p.exp == 2047)
		return (print_special(fmt, ctx, d, 0));
	exp = d.p.exp - EXP_BIAS - 52;
	frac = d.p.frac | 1L << 52;
	bigint[0] = frac % POW10;
	bigint[1] = frac / POW10;
	len = 2;
	bigint_pow(bigint, exp < 0 ? 5 : 2, exp < 0 ? -exp : exp, &len);
	if (exp > 0)
		print_real(ctx, fmt, bigint, (uint32_t[2]){ len, d.p.sign });
	else
		print_float(ctx, fmt, bigint, (uint32_t[3]){ len, -exp, d.p.sign });
	return (0);
}
