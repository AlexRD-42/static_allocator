/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_allocator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/16 17:12:46 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

t_alloc	init_alloc(void *data, size_t memory_size, size_t block_size)
{
	bool	valid;
	size_t	meta_size;
	void	*meta;
	void	*meta_end;

	valid = (block_size % WORD_SIZE == 0)
		&& (block_size >= WORD_SIZE)
		&& (memory_size % (block_size * CHAR_BIT) == 0)
		&& (memory_size >= 2 * WORD_BIT * block_size);
	if (valid == false)
		return ((t_alloc){0});
	meta_size = memory_size / (block_size * CHAR_BIT);
	memory_size -= 2 * meta_size;
	meta = (uint8_t *)data + memory_size;
	meta_end = (uint8_t *)data + memory_size + meta_size;
	return ((t_alloc){data, meta, meta_end, memory_size, block_size, memory_size / block_size});
}

// For pointers within range, it scans metadata_end until its terminator byte
// UB for invalid PTR
void	ft_free(t_vec *vec, t_alloc *alloc)
{
	size_t	start;
	size_t	length;

	start = (size_t)vec->start - (size_t)alloc->data;
	start /= alloc->block_size;
	length = ((size_t)vec->end - (size_t)vec->start);
	length /= alloc->block_size;
	if (vec == NULL)
		return ;
	ft_bitset(alloc->metadata, start, length + 1, 0);
}

// Size of metadata is 1 bit per memory block
void	*ft_alloc(size_t bytes, t_alloc *alloc)
{
	size_t	p1;
	size_t	p2;
	size_t	blocks;

	blocks = ((bytes + (alloc->block_size - 1)) / alloc->block_size);
	if (bytes == 0 || bytes >= alloc->memory_size || blocks > alloc->block_count)
		return (NULL);
	p1 = ft_bitfind(alloc->metadata, 0, alloc->block_count, 0);
	while (p1 <= alloc->block_count - blocks)
	{
		p2 = ft_bitfind(alloc->metadata, p1, p1 + blocks, 1); // Scans for the first set bit
		if (p2 - p1 >= blocks)	// Bit pos delta, when p2 == SIZE_MAX also breaks
			break ;
		p1 = ft_bitfind(alloc->metadata, p2, alloc->block_count, 0); // Scans for the first unset bit
	}
	if (p1 == SIZE_MAX || p1 + blocks > alloc->block_count)
		return (NULL);
	ft_bitset(alloc->metadata, p1, p1 + blocks, 1);
	return (alloc->data + p1 * alloc->block_size);
}
