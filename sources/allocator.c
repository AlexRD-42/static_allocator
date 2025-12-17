/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:57:50 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/16 17:21:44 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

size_t	ft_find_fit(size_t blocks, size_t *metadata, size_t block_count)
{
	size_t	p1;
	size_t	p2;

	p1 = ft_bitfind(metadata, 0, block_count, 0);
	while (p1 <= block_count - blocks)
	{
		p2 = ft_bitfind(metadata, p1, p1 + blocks, 1); // Scans for the first set bit
		if (p2 - p1 >= blocks)	// Bit pos delta, when p2 == SIZE_MAX also breaks
			break ;
		p1 = ft_bitfind(metadata, p2, block_count, 0); // Scans for the first unset bit
	}
	if (p1 == SIZE_MAX || p1 + blocks > block_count)
		return (SIZE_MAX);
	return (p1);
}
