# zoid -- streaming sequence bloom filter for contaminant removal

## TODO

 - remove `files` from `bloom_list_t`?
 - hash of bits for validation?
 - move create/load binary functions to sequence bloom library 
 - valgrind; write destroy methods
 - iterate through filters in parallel

## GDB Tips

If you would like to look at bits in the bit field in GDB, `x\1000t
bloom->bits` works well.

## FAQ

**Why is it named "zoid"?**

Why not Zoid(berg)[?](http://en.wikipedia.org/wiki/Futurama)



