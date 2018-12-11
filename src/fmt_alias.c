/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_alias.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 12:19:42 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 12:22:46 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt.h"
#include "parse.h"

int	fmtsu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmts(fmt, ctx));
}

int	fmtdu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmtd(fmt, ctx));
}

int	fmtou(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmto(fmt, ctx));
}

int	fmtuu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmtu(fmt, ctx));
}

int	fmtcu(t_fmt *fmt, t_ctx *ctx)
{
	fmt->length = PF_L;
	return (fmtc(fmt, ctx));
}
