/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:01:57 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/14 12:50:58 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ALLOC_H
# define FT_ALLOC_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <limits.h>

enum e_sizes
{
	WORD_SIZE = sizeof(size_t),
	WORD_BIT = WORD_SIZE * CHAR_BIT,
};

size_t	ft_bsf(size_t word);
size_t	ft_bsr(size_t word);
size_t	ft_bsr(size_t word);
size_t	ft_bitfind(const size_t *word, size_t start, size_t end, bool bit);
void	ft_bitset(size_t *bitmap, size_t start, size_t end);
void	ft_bitclr(size_t *bitmap, size_t start, size_t end);
size_t	ft_popcount(size_t word);

typedef struct s_alloc
{
	uint8_t *const	data;
	size_t *const	metadata;
	size_t *const	metadata_end;
	const size_t	memory_size;
	const size_t	block_size;
}	t_alloc;

typedef struct s_bitptr
{
	void		*ptr;
	uint32_t	index;
	uint32_t	offset;
}	t_bitptr;

typedef struct s_bitpos
{
	uint32_t	index;		// Array index
	uint32_t	offset;		// Bit offset (max 64)
}	t_bitpos;

typedef struct s_vec
{
	void	*start;
	void	*ptr;
	void	*end;
}	t_vec;

typedef struct s_vec2
{
	void		*data;
	uint32_t	offset;
	uint32_t	capacity;
}	t_vec2;

#endif
