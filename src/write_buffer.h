/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:07:44 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 14:08:36 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITE_BUFFER_H
# define WRITE_BUFFER_H

# include "eval.h"
# include <sys/types.h>

typedef struct	s_buffer_wdata {
	ssize_t		nb;
	char		*buffer;
	size_t		len;
	char		buf[4096];
	uint16_t	idx;
}				t_buffer_wdata;

void			bufferflush(t_ctx *ctx);
void			buffern(t_ctx *ctx, char *s, size_t n);
void			bufferr(t_ctx *ctx, char s, size_t n);

#endif
