/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:36:32 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/12 17:49:20 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "ft_alloc.h"

size_t	ft_bsf(size_t word)
{
	size_t	i;

	i = 0;
    word = word & -word;
	word >>= 1;	// Review
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

// Is effectively log2
size_t	ft_bsr(size_t word)
{
	size_t	i;

	i = 0;
	word >>= 1;	// Review
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

size_t	ft_popcount(size_t word)
{
	size_t	i;

	i = 0;
	while (word)
	{
		word &= word - 1;
		i++;
	}
	return (i);
}

// There has to be a better way but alas norminette forces my hand
size_t	ft_membsr(size_t word[static META_COUNT], size_t start, size_t request, size_t count)
{
	size_t	i;
	size_t	run;
	size_t	offset;
	size_t	bitmap;

	run = 0;
	i = start / META_BIT;
	offset = start % META_BIT;
	bitmap = word[i] & (SIZE_MAX >> offset);
	if (bitmap != 0)
		return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
	run += META_BIT - offset;
	i++;
	while (run < request && i < count)
	{
		bitmap = word[i];
		if (bitmap != 0)
			return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
		run += META_BIT;
		i++;
	}
	return (start + run);
}

size_t	ft_membsr_not(size_t word[static META_COUNT], size_t start, size_t request, size_t count)
{
	size_t	i;
	size_t	run;
	size_t	offset;
	size_t	bitmap;

	run = 0;
	i = start / META_BIT;
	offset = start % META_BIT;
	bitmap = ~word[i] & (SIZE_MAX >> offset);
	if (bitmap != 0)
		return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
	run += META_BIT - offset;
	i++;
	while (run < request && i < count)
	{
		bitmap = ~word[i];
		if (bitmap != 0)
			return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
		run += META_BIT;
		i++;
	}
	return (start + run);
}

size_t	ft_bitset(size_t bitmap[static META_COUNT], size_t start, size_t value)
{
	size_t	i;
	size_t	run;
	size_t	offset;
	size_t	mask;

	mask = (SIZE_MAX >> offset);
	offset = start % META_BIT;
	i++;
	while (value != 0)
	{
		bitmap = bitmap[i];
		i++;
	}
	return (start + run);
}
