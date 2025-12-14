/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:36:32 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/14 12:33:30 by adeimlin         ###   ########.fr       */
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

// With O1 optimizes to popcount
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

// Returns 0 for word == 0, and 1 based indexing
// With O1 optimizes to lzcnt or bsr
size_t	ft_bsr(size_t word)
{
	size_t	i;

	i = 0;
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

// If end is not word aligned, it will scan past end until alignment
size_t	ft_bitfind(const size_t *word, size_t start, size_t end, bool bit)
{
	size_t			cur;
	size_t			bitmap;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	invert = (size_t)-(!bit);

	cur = start + WORD_BIT - (start % WORD_BIT);
	bitmap = (word[start / WORD_BIT] ^ invert) & start_mask;
	if (bitmap != 0)
		return (cur - 1 - (ft_bsr(bitmap) - 1)); // the compiler needs the -1
	while (cur < end)
	{
		bitmap = word[cur / WORD_BIT] ^ invert;
		if (bitmap != 0)
			return (cur + (WORD_BIT - 1) - (ft_bsr(bitmap) - 1));
		cur += WORD_BIT;
	}
	return (SIZE_MAX);
}

// Bitset and bitclr are kept separate to reduce branching
void	ft_bitset(size_t *bitmap, size_t start, size_t end)
{
	const size_t	word_start = start / WORD_BIT;
	const size_t	word_end = (end - 1) / WORD_BIT;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	end_mask = SIZE_MAX << (-end % WORD_BIT);
	const size_t	diff_mask = (size_t)-(word_start != word_end);

	start = word_start + 1;
	bitmap[word_start] |= start_mask & (end_mask | diff_mask);
	while (start < word_end)
		bitmap[start++] = SIZE_MAX;
	bitmap[word_end] |= end_mask & diff_mask;
}

void	ft_bitclr(size_t *bitmap, size_t start, size_t end)
{
	const size_t	word_start = start / WORD_BIT;
	const size_t	word_end = (end - 1) / WORD_BIT;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	end_mask = SIZE_MAX << (-end % WORD_BIT);
	const size_t	diff_mask = (size_t)-(word_start != word_end);

	start = word_start + 1;
	bitmap[word_start] &= ~(start_mask & (end_mask | diff_mask));
	while (start < word_end)
		bitmap[start++] = 0;
	bitmap[word_end] &= ~(end_mask & diff_mask);
}
