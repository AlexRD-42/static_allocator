/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/14 15:51:45 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

bool	bit_read(size_t word, size_t index)
{
	return (word & ((size_t)1 << (WORD_BIT - 1 - index % WORD_BIT)));
}

size_t	bit_write(size_t word, size_t index, bool bit)
{
	size_t	mask;

	mask = (size_t)1 << (WORD_BIT - 1 - (index % WORD_BIT));
	if (bit)
		return (word | mask);
	else
		return (word & ~mask);
}

// For pointers within range, it scans metadata_end until its terminator byte
// UB for invalid PTR
void	ft_free(void *ptr, t_alloc *alloc)
{
	ptrdiff_t		start;
	size_t			end;
	size_t			bit_mask;
	const size_t	block_count = alloc->memory_size / alloc->block_size;

	if (ptr == NULL)
		return ;
	start = ((uint8_t *)ptr - alloc->data);
	if (start < 0 || (size_t)start >= alloc->memory_size)
		return ;
	start /= alloc->block_size;
	end = ft_bitfind(alloc->metadata_end, (size_t)start, block_count, 1);
	if (end > block_count)
		return ;
	ft_bitclr(alloc->metadata, (size_t)start, end);
	bit_mask = ~((size_t)1 << (WORD_BIT - 1 - (end % WORD_BIT)));
	alloc->metadata_end[end / WORD_BIT] &= bit_mask;
}

// Size of metadata is 1 bit per memory block
void	*ft_alloc(size_t bytes, t_alloc *alloc)
{
	size_t			p1;
	size_t			p2;
	size_t			blocks;
	const size_t	block_count = alloc->memory_size / alloc->block_size;

	if (bytes == 0 || bytes == SIZE_MAX)
		return (NULL);
	blocks = ((bytes + (alloc->block_size - 1)) / alloc->block_size);	// Review: Having block_size be dynamic means compiler can't optimize division
	p1 = ft_bitfind(alloc->metadata, 0, block_count, 0);
	while (p1 != SIZE_MAX)
	{
		p2 = ft_bitfind(alloc->metadata, p1, p1 + blocks, 1); // Scans for the first set bit
		if (p2 - p1 >= blocks)	// Bit delta, when p2 == SIZE_MAX also breaks
			break ;
		p1 = ft_bitfind(alloc->metadata, p2, block_count, 0); // Scans for the first unset bit
	}
	if ((p1 | p2) == SIZE_MAX)
		return (NULL);
	p2 = p1 + blocks;
	ft_bitset(alloc->metadata, p1, p1 + blocks);
	alloc->metadata_end[p2 / WORD_BIT] |= (size_t)1 << (WORD_BIT - 1 - p2 % WORD_BIT);
	return (alloc->data + p1 * alloc->block_size);
}
