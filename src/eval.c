/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:37:19 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 12:18:35 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "fmt.h"
#include "parse.h"
#include "ft/str.h"
#include <alloca.h>

static void	re_eval_fmt(t_farg *arg, t_fmt *f, t_ctx *ctx)
{
	char	*c;
	t_pf	*fn;

	ctx->va.lock = 0;
	while (arg)
	{
		resolve_arg(arg, &ctx->va);
		arg = arg->next;
	}
	while (f)
	{
		read_args(f, ctx);
		fn = g_pf[TIDX(f->end[-1])];
		if (fn)
			fn(f, ctx);
		c = f->end;
		f = f->next;
		if (f)
			ctx->write(ctx, c, f->begin - c - 1);
	}
}

static int	exec(t_parse *res, t_fmt *f, const t_farg *arg_begin, t_ctx *ctx)
{
	t_pf		*fn;

	if (!f->param)
		f->param = arg_at_index(arg_begin, res->param);
	if (!f->precisionarg && res->precision != -1)
		f->precisionarg = arg_at_index(arg_begin, res->precision);
	if (!f->widtharg && res->width != -1)
		f->widtharg = arg_at_index(arg_begin, res->width);
	if (read_args(f, ctx) == -1)
		return (-1);
	if (TIDX(f->end[-1]) < 0 ||
			(uint32_t)TIDX(f->end[-1]) > sizeof(g_pf) / sizeof(*g_pf))
	{
		ctx->write(ctx, f->begin, f->end - f->begin);
		return (0);
	}
	fn = g_pf[TIDX(f->end[-1])];
	if (fn)
		return (fn(f, ctx));
	else
		return (invalid_arg(f, ctx));
}

static void	next_arg(t_farg **arg, t_farg *next, t_fmt *fmt, t_parse *res)
{
	(*arg)->next = next;
	*next = (t_farg) {
		.idx = (*arg)->idx + 1,
		.type = NONE,
		.next = NULL
	};
	if ((int32_t)next->idx == res->param)
		fmt->param = next;
	if ((int32_t)next->idx == res->precision)
		fmt->precisionarg = next;
	if ((int32_t)next->idx == res->width)
		fmt->widtharg = next;
	*arg = next;
}

static void	next_fmt(t_fmt **begin, t_fmt **f, t_fmt *next)
{
	if (*begin)
		(*f)->next = next;
	else
		*begin = next;
	*next = (t_fmt){
		.next = NULL,
		.param = NULL,
		.precisionarg = NULL,
		.widtharg = NULL
	};
	*f = next;
}

void		eval_fmt(char *fmt, t_ctx *ct)
{
	const char		*c = fmt;
	t_fmt			*f[2];
	t_parse			res;
	t_farg			*arg;
	t_farg			arg_b;

	f[0] = NULL;
	arg_b = (t_farg) { .idx = 0, .type = NONE };
	arg = (t_farg *)&arg_b;
	while ((c = ft_strchr(c, '%')))
	{
		if (!ct->va.lock)
			ct->write(ct, f[0] ? f[1]->end : fmt, c - (f[0] ? f[1]->end : fmt));
		next_fmt(f, f + 1, alloca(sizeof(t_fmt)));
		res = parse((char **)&c, f[1], &ct->idx);
		res.max -= arg->idx;
		while (res.max-- > 0)
			next_arg(&arg, alloca(sizeof(t_farg)), f[1], &res);
		if (exec(&res, f[1], &arg_b, ct) == -1 && !ct->va.lock && (f[0] = f[1]))
			ct->va.lock = 1;
	}
	if (ct->va.lock)
		re_eval_fmt((t_farg *)&arg_b, f[0], ct);
	ct->write(ct, f[0] ? f[1]->end : fmt, ft_strlen(f[0] ? f[1]->end : fmt));
}
