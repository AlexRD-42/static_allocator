/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/12 16:51:24 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

void	ft_free(void *ptr)
{
	if (ptr == NULL)
		return ;
}

size_t	ft_first_fit(size_t metadata[static META_COUNT], size_t blocks, size_t count)
{
	const size_t	end = count * META_BIT;
	size_t			p1;
	size_t			p2;

	p1 = 0;
	while (p1 < end)
	{
		p2 = ft_membsr(metadata, p1, blocks, count);		// Scans for the first set bit
		if (p2 - p1 >= blocks)
			return (p1);
		p1 = ft_membsr_not(metadata, p2, blocks, count);	// Scans for the first unset bit
	}
	return (SIZE_MAX);
}

// Start is i + bitindex
static
void	*stt_allocate(uint8_t data[static BLOCK_COUNT], size_t metadata[static META_COUNT], size_t metadata_end[static META_COUNT], size_t blocks)
{
	const size_t	free_pos = ft_first_fit(metadata, blocks, META_COUNT);

	if (free_pos == SIZE_MAX)
		return (NULL);
	return (data + free_pos * META_BIT * BLOCK_SIZE);
}

// Assign blocks, calls free if necessary
void	*ft_alloc(size_t bytes)
{
	static uint8_t	data[MEMORY_SIZE];
	static size_t	metadata[MEMORY_SIZE / META_SIZE];
	static size_t	metadata_end[MEMORY_SIZE / META_SIZE];
	static size_t	bytes_free = MEMORY_SIZE;
	void			*ptr;

	bytes = (bytes + (bytes % BLOCK_SIZE)) / BLOCK_SIZE;
	ptr = stt_allocate(data, metadata, metadata_end, bytes);
	if (bytes_free < bytes)
		return (NULL);
	return
}
