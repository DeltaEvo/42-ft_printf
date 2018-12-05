/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_out.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:03:00 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 14:03:15 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITE_OUT_H
# define WRITE_OUT_H

# include "eval.h"
# include <sys/types.h>

typedef struct	s_out_data {
	ssize_t		nb;
	int			fd;
	char		buf[4096];
	uint16_t	idx;
}				t_out_wdata;

void			outflush(t_ctx *ctx);
void			outn(t_ctx *ctx, char *s, size_t n);
void			outr(t_ctx *ctx, char s, size_t n);

#endif
