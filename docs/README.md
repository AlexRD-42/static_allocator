# Stack Allocator
This is a very lightweight (zero dependencies) custom memory allocator designed with portability in mind.
It uses 2 bits of metadata per block, so for a regular 8 byte block system it uses 3.125% of the data.

It's designed with speed in mind, through the use of efficient bit manipulation techniques (explained in-depth below) 

## Project restrictions
This was designed for personal use in 42 projects, that most of the time forbid the use of external functions.
Without the use of assembly operations like LZCNT (leading zero count) or BSR (bit scan reverse), this project would've been doomed to inefficiency.
Luckily however, compiler optimizations have pattern recognition which I was able to leverage to turn this around.

With clang -O1 -march=native for example, most of the bit primitive functions compile to a single line of assembly. 
Another benefit of this is portability: because the compiler recognizes the pattern, it's able to emit the most efficient instruction available for that processor.
And when it doesn't exist, it has the naive fallback.

## How to Use
Stack allocator 


## Glossary
MSB:		Most Significant Bit, is the bit that represents the biggest exponent of two 2^x (where x is the bit size of the value)
LSB:		Least Significant Bit, is the bit that represents the smallest exponent of two 2^0
WORD:		The size_t type of the platform, sized to the register size of the CPU
METADATA:	Data about data, used to represent the use state of your memory

## Bit Operations
The core of this project is in the bit manipulation operations.
Each bit of the metadata represents a block, and because in C we don't have access to the individual bits of a value, we need techniques to affect individual bits efficiently.

Here are some of the techniques used in this project:

* Bit masking:
This is very common when you want to effect changes to certain bits. For example, lets say we want to read the value of the 4th bit of 0b10011001. What we'd do is first apply a mask to the whole value that would remove all other bits, so basically all zeros except 1 in the 4th position. Then we AND the result:
	0b10011001
   &0b00001000
   =0b00001000
This isolates the bit we want to read, which is exactly what we want. This is also very useful for ranges. In bitset and bitfind operations, for example, we frequently have bit indexes that are not word aligned and therefore need bit masking.

Let's say we want to find the first set bit in a 32 bit word, starting from bit 12 but searching at most until bit 24. This means we need to mask the start of the word and the end:
Range:    start=V          V=end
	0b10011001101110011000101110010011
   &0b00000000001111111111100000000000
   =0b00000000001110011000100000000000

This allows us to 

* Bit Flipping
Another common technique is to use XOR to flip a bit. In bitfind for example, we want to search for the first bit equal to bit. When the bit is 1, we search for the first bit equal to 1. The efficient search is to find the first word that isn't equal to zero, because then logically one of the bits isn't set. But how do we do that when we want to search for a zero instead? We flip the number so that every zero is a one and vice versa, and perform the search like we normally would for bit == 1. To do this without adding a branch like (if bit == 0, ~word), we can use a XOR mask:

    0b00100110
   ^0b11111111
   =0b11011001

## Known Problems
At the moment the only thing this project doesn't support are processors where the natural word size isn't power of two. It's not because
it wouldn't be possible, more so that a lot of the efficient operations rely on having the word size be a power of two and it'd be too much code duplication for exotic architectures.

There is no protection against invalid frees.
The only check that is performed is to see if the freed pointer belongs to the allocator. 
This means that passing an invalid pointer is UB and will probably corrupt metadata state. 
It would've been possible to protect against this, but it'd require another bit of metadata use, increasing metadata size by 50% and it kind of goes against the whole philosophy of this project of being a lean and fast allocator