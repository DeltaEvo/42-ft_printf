/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:00:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 16:27:28 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"
#include "ft/double_parts.h"
#include <stdio.h>

#define EXP_BIAS (1023)
#define POW10 (1000000000)

static void	bigint_pow(uint32_t *bigint, uint8_t n, uint16_t exp, uint32_t *len)
{
	uint32_t	carry;
	uint64_t	t;
	int32_t		i;

	while (exp--)
	{
		carry = 0;
		i = -1;
		while ((uint32_t)++i < *len)
		{
			t = (uint64_t)bigint[i] * n + carry;
			bigint[i] = t % POW10;
			carry = t / POW10;
		}
		if (carry)
			bigint[(*len)++] = carry;
	}
}

static void	print(t_ctx *ctx, uint32_t *bigint, uint32_t len, int32_t maxnums)
{
	int32_t		i;
	t_int_str	res;

	i = len;
	if (i)
		while (--i >= 0)
		{
			res = ft_uint_to_str(bigint[i]);
			if (maxnums == -1 || maxnums > 9)
			{
				if ((uint32_t)i != len - 1)
					ctx->writer(ctx, '0', 9 - res.len);
				ctx->write(ctx, res.str, res.len);
			}
			else
			{
				if (maxnums > res.len)
					ctx->writer(ctx, '0', maxnums - res.len);
				ctx->write(ctx, res.str, maxnums);
				return ;
			}
			if (maxnums != -1)
				maxnums -= 9;
		}
}

static void	round(uint32_t *bigint, uint32_t len, uint16_t exp,
		uint32_t precision)
{
	const uint32_t	pos = exp - precision;
	uint32_t		pow;
	uint32_t		i;

	if (pos / 9 > len)
		return ;
	bigint += pos / 9;
	len -= pos / 9;
	i = 0;
	pow = 5;
	while (++i < pos % 9)
		pow *= 10;
	*bigint += pow;
}

size_t		nb_len(uint64_t n)
{
	size_t	len;

	len = 1;
	while (n /= 10)
		len++;
	return (len);
}

static void	print_float(t_ctx *ctx, t_fmt *fmt, uint32_t *bigint,
		uint32_t ints[3])
{
	t_int_str		res;
	const uint32_t	len = ints[0];
	const uint16_t	exp = ints[1];
	const uint16_t	p_exp = exp / 9;
	const uint16_t	l_exp = exp % 9;
	uint32_t		c_len;

	c_len = (bigint[p_exp] ? nb_len(bigint[p_exp]) - l_exp
		: 1) + (fmt->precision ? fmt->precision + 1 : 0)
		+ (ints[2] || fmt->flags & (PF_SPACE | PF_PLUS));
	if (!(fmt->flags & PF_ZERO))
		pad_start(c_len, fmt, ctx, 0);
	if (ints[2])
		ctx->write(ctx, "-", 1);
	else if (fmt->flags & (PF_SPACE | PF_PLUS))
		ctx->write(ctx, fmt->flags & PF_SPACE ? " " : "+", 1);
	if (fmt->flags & PF_ZERO)
		pad_start(c_len, fmt, ctx, 0);
	round(bigint, len, exp, fmt->precision);
	print(ctx, bigint + p_exp + 1, len - p_exp - 1, -1);
	res = ft_uint_to_str(bigint[p_exp]);
	if (res.len - l_exp)
		ctx->write(ctx, res.str, res.len - l_exp);
	else
		ctx->write(ctx, "0", 1);
	ctx->write(ctx, ".", 1);
	if (l_exp > fmt->precision)
		ctx->write(ctx, res.str + (res.len - l_exp), fmt->precision);
	else
	{
		ctx->write(ctx, res.str + (res.len - l_exp), l_exp);
		print(ctx, bigint, p_exp, fmt->precision - l_exp);
	}
	pad_end(c_len, fmt, ctx);
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

int			fmtf(t_fmt *fmt, t_ctx *ctx)
{
	t_dparts	d;
	uint32_t	bigint[50];
	int16_t		exp;
	uint64_t	frac;
	uint32_t	len;

	TRY(get_arg(fmt->param, fmt->length & PF_LU ? LDBL : DBL, &ctx->va));
	if (fmt->precision == -1)
		fmt->precision = 6;
	d.d = fmt->param->value.d;
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
