# streaming sequence bloom filter for contaminant removal

Note: This was an experimental bloom filter sequence contaminant
remover I made, mostly to play around with C and bloom filters. Feel
free to hack and play around with; licensed under GPL.

## TODO

 - remove `files` from `bloom_list_t`?
 - hash of bits for validation?
 - move create/load binary functions to sequence bloom library 
 - valgrind; write destroy methods
 - iterate through filters in parallel

## GDB Tips

If you would like to look at bits in the bit field in GDB, `x\1000t
bloom->bits` works well.


