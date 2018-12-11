/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:58:32 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 13:36:31 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FMT_H
# define FMT_H

# include "eval.h"

typedef int	(t_pf)(t_fmt *fmt, t_ctx *ctx);

int			fmt(t_fmt *fmt, t_ctx *ctx);
int			fmts(t_fmt *fmt, t_ctx *ctx);
int			fmtsu(t_fmt *fmt, t_ctx *ctx);
int			fmtp(t_fmt *fmt, t_ctx *ctx);
int			fmtd(t_fmt *fmt, t_ctx *ctx);
int			fmtdu(t_fmt *fmt, t_ctx *ctx);
int			fmti(t_fmt *fmt, t_ctx *ctx);
int			fmto(t_fmt *fmt, t_ctx *ctx);
int			fmtou(t_fmt *fmt, t_ctx *ctx);
int			fmtu(t_fmt *fmt, t_ctx *ctx);
int			fmtuu(t_fmt *fmt, t_ctx *ctx);
int			fmtx(t_fmt *fmt, t_ctx *ctx);
int			fmtxu(t_fmt *fmt, t_ctx *ctx);
int			fmtc(t_fmt *fmt, t_ctx *ctx);
int			fmtcu(t_fmt *fmt, t_ctx *ctx);
int			fmtf(t_fmt *fmt, t_ctx *ctx);
int			invalid_arg(t_fmt *fmt, t_ctx *ctx);

int			read_args(t_fmt *f, t_ctx *ctx);
t_types		type_for_length(uint8_t length, uint8_t unsign);
uintmax_t	downcast(uint8_t length, uintmax_t nb, uint8_t unsign);
int			pad_end(size_t len, t_fmt *fmt, t_ctx *ctx);
int			pad_start(size_t len, t_fmt *fmt, t_ctx *ctx, uint8_t prec_c);

# define TIDX(i) ((i) - '%')

static t_pf		*g_pf[] =
{
	[TIDX('%')] = fmt,
	[TIDX('s')] = fmts,
	[TIDX('S')] = fmtsu,
	[TIDX('p')] = fmtp,
	[TIDX('d')] = fmtd,
	[TIDX('D')] = fmtdu,
	[TIDX('i')] = fmtd,
	[TIDX('o')] = fmto,
	[TIDX('O')] = fmtou,
	[TIDX('u')] = fmtu,
	[TIDX('U')] = fmtuu,
	[TIDX('x')] = fmtx,
	[TIDX('X')] = fmtxu,
	[TIDX('c')] = fmtc,
	[TIDX('C')] = fmtcu,
	[TIDX('f')] = fmtf,
	[TIDX('F')] = fmtf
};

# define TRY(expr) if(expr == -1) return (-1)

#endif
