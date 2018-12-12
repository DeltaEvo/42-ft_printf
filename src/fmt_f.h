/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_f.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 12:12:51 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/12 12:21:13 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FMT_F_H
# define FT_PRINTF_FMT_F_H

# include <stdint.h>
# include <stddef.h>

# define EXP_BIAS (1023)
# define POW10 (1000000000)

void		bigint_pow(uint32_t *bigint, uint8_t n, uint16_t exp, uint32_t *le);
void		bigint_round(uint32_t *bigint, uint32_t len, uint16_t exp,
		uint32_t precision);
size_t		nb_len(uint64_t n);
uint32_t	ft_min(uint32_t a, uint32_t b);

#endif
