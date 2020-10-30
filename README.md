One way to determine the overall effectiveness of different cache configurations is by comparing their cycle counts on
the same memory trace.  Using gcc.trace with, the following lists various cache configurations in order of ascending cycle 
count:

    512 4 32 write-allocate write-back fifo (Total cycles: 8652306)
    256 4 16 write-allocate write-back fifo (Total cycles: 9831818)
    1024 8 16 no-write-allocate write-through fifo (Total cycles: 22467979)
    128 8 32 no-write-allocate write-through fifo (Total cycles: 23568015)
    8192 128 16 write-allocate write-through fifo (Total cycles: 24548855)
    256 16 64 no-write-allocate write-through lru (Total cycles: 23747695)
    128 32 64 write-allocate write-through lru (Total cycles: 25454600)
    1 1 4 write-allocate write-through lru (Total cycles: 68633215)

Overall, in our experiments fifo had a lower cycle count than lru.  The cycle counts were all within 13% of each other,
with the exception of write-allocate/write-back with fifo, which had significantly lower cycle counts, and the smallest
possible cache (1 set, 1 block, 4 bytes per block) took much more cycles.
It makes sense that the smallest cache was the least efficient, since every load/store is a miss unless the trace right
before was in the same block of memory (the cache could only hold one trace).

A fully-associative cache theoretically should have the best performance/highest hit rate because it is impossible for 
conflict misses to occur (since there is only 1 set).  However, since it requires all tags to be checked, it is 
expensive and impractical to actually implement on hardware.
A direct-mapped cache is effectively the opposite (many sets of 1 block each).
Set-associative caches are in between fully-associative and direct-mapped, so we experimented with various 
configurations of set-associative caches to find the most efficient one.  

Fifo configurations with write-allocate and write-back had the lowest cycle count.  As the number of sets increases,
the total cycles decreased significantly, but after 2048 sets, the cycle count began to level out:

    1024 4 32 write-allocate write-back fifo (Total cycles: 6849382)
    2048 4 32 write-allocate write-back fifo (Total cycles: 5586391)
    4096 4 32 write-allocate write-back fifo (Total cycles: 5387236)
    8192 4 32 write-allocate write-back fifo (Total cycles: 5375239)

Increasing the number of blocks (and leaving everything else unchanged) also slightly lowered the cycle count:
    
    2048 8 32 write-allocate write-back fifo (Total cycles: 5376841)
    4096 8 32 write-allocate write-back fifo (Total cycles: 5368042)
    8192 8 32 write-allocate write-back fifo (Total cycles: 5360044)

Increasing the number of bytes per block (and leaving everything else unchanged) led to a higher cycle count:

    2048 4 64 write-allocate write-back fifo (Total cycles: 6009900)
    4096 4 64 write-allocate write-back fifo (Total cycles: 5979505)
    
and decreasing the number of bytes per block to 16 decreases the cycle count, but further decreasing to 4 does the
opposite:
    
    2048 8 16 write-allocate write-back fifo (Total cycles: 5048859)
    2048 8 4 write-allocate write-back fifo (Total cycles: 5125311)
    
We think the cache configuration with the best overall effectiveness is write-allocate write-back fifo with 2048 sets, 
8 blocks, and 16 bytes per block.  However, the best cache depends on the trace, so while this is effective for 
gcc.trace, it may not be the best inn all scenarios.


We divided the work for this project very evenly.  Initially, we both discussed the file structure for the cache.  
Tad wrote get_input, all the methods for stores, found a lot of bugs (especially in createBlock and 
getSet), worked on the cycle count, and cleaned up/formatted our code.  
Sam wrote the Cache class, the main method, all the methods for loads, checkMemoryTrace, cacheFull, addBlock, 
createBlock, updateBlockOrder, and replace.
