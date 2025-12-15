#include <stddef.h>
#include "ft_alloc.h"

// If end is not word aligned, it will scan past end until alignment
size_t	ft_bitfind(const size_t *word, size_t start, size_t end, bool bit)
{
	size_t			cur;
	size_t			offset;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	invert = (size_t)-(!bit); // All ones or zero mask

	cur = start + WORD_BIT - (start % WORD_BIT);
	offset = ft_bsr((word[start / WORD_BIT] ^ invert) & start_mask);
	if (offset != WORD_BIT)
		return (cur - 1 - offset);
	while (cur < end)
	{
		offset = ft_bsr(word[cur / WORD_BIT] ^ invert);
		if (offset != WORD_BIT)
			return (cur + WORD_BIT - 1 - offset);
		cur += WORD_BIT;
	}
	return (SIZE_MAX);
}

size_t	ft_bitfind2(const size_t *word, size_t start, size_t end, bool bit)
{
	size_t			cur;
	size_t			offset;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	invert = (size_t)-(!bit); // All ones or zero mask

	offset = ft_bsr((word[start / WORD_BIT] ^ invert) & start_mask);
	cur = start + WORD_BIT - (start % WORD_BIT);
	while (cur < end && offset == WORD_BIT)
	{
		offset = ft_bsr(word[cur / WORD_BIT] ^ invert);
		cur += WORD_BIT;
	}
	if (offset != WORD_BIT)
		return (cur - 1 - offset);
	return (SIZE_MAX);
}

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

// Bitset and bitclr are kept separate to reduce branching
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

// MSB first
bool	bit_read(size_t word, size_t index)
{
	return (word & ((size_t)1 << (WORD_BIT - 1 - index % WORD_BIT)));
}

// MSB first
size_t	bit_write(size_t word, size_t index, bool bit)
{
	size_t	mask;

	mask = (size_t)1 << (WORD_BIT - 1 - (index % WORD_BIT));
	if (bit)
		return (word | mask);
	else
		return (word & ~mask);
}
