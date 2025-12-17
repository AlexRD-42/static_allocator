### Finish the readme

### Create vector functions

### Avoid code duplication for stack/static/vector allocator
Currently static allocator has no struct requirements and ceil operations are bit shifts rather than divisions because the compiler recognizes them as power of two.

Moving this to a general function will:
1) introduces struct requirement for static allocator because of norminette 4 parameters limit;
2) prevents compiler from optimizing;
Ideally ceil should leverage the info that block_size is a multiple of word_size

### Introduce compaction techniques
Function that takes pointers to be preserved in a null terminated array, like so: {ptr1, ptr2, ..., NULL}
performs compaction and returns the new pointers

### Use a small neural network to determine best fit
Rather than using a first fit algorithm to allocate blocks, maybe a more sophisticated approach would be to train a neural network to search for fits that reduce fragmentation in the long run

