/*
1. In this solution, memory is allocated using the mm_malloc function. The function takes a size_t size argument, which represents the size of the requested memory block.
2. In ALIGN it it used to calculate the adjusted aligned block size (asize): The requested size is aligned to the nearest multiple of alignment to ensure proper calculation and allocations of memory blocks
3. The size of the block is increased by the header and footer size which is 4 in order to account for storing of data
4. Then Find the index of the segregated free list to search for an appropriate block: The code calculates the log2 of the aligned size (asize) and adjusts it to obtain the index of the segregated free list to search for an appropriate block.
This is done using bitwise operations and the predefined arrays b and S. This part of the code is referenced from https://en.wikipedia.org/wiki/Binary_logarithm

5. Then the code searches for a fitting block by usin the fitchek function searches for a free block that can accommodate the requested size.
If it finds a suitable block, the function returns the pointer to that block else, it returns null

6. If a suitable block is found, the place function is called to allocate memory in the block. The function splits the block if the remaining size is sufficient to create a new block,
updates the header and footer of the block with the allocated status, and removes the block from the free list if necessary.

7.Extend the heap: If no suitable block is found, the heap is extended by the maximum of the aligned size or CHUNKSIZE. Keep in mind that instead of what the textbook shows, i automatically
selected the byte as 512 to reduce performance speed. the rest is executed using the extend_heap function,
which calls mem_sbrk to extend the heap by the requested size which is initialized afterwards to  new free block's header and footer, and calls the coalesce function to merge it with any adjacent free blocks.

8. mm_malloc function allocates memory in the most suitable block available in the segregated free lists, or extends the heap if necessary.
2021-14284 Taehyun Yang

 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE 512
#define Byte_LEN 15

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (unsigned int)val)

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~7)
#define GET_ALLOC(p) (GET(p) & 1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

// get the pred and succ pointers
#define PRED_PTR(bp) ((char *)(bp) + WSIZE)
#define SUCC_PTR(bp) ((char *)bp)

// get the pred and succ blocklist
#define PRED_BLKP(bp) (GET(PRED_PTR(bp)))
#define SUCC_BLKP(bp) (GET(SUCC_PTR(bp)))

// utility functions
static void *extend_heap(size_t size);
static void *coalesce(void *bp);
static void *place(char *bp, size_t size);
static void *fitchek(size_t size, int seg_idx);

const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000}; // list used to find log2 of an nbit int
const unsigned int S[] = {1, 2, 4, 8, 16};                     // another list
char *start_ptr;
char *heap_list;
int mm_init(void)
{

    if ((heap_list = mem_sbrk((Byte_LEN + 3) * WSIZE)) == (void *)-1)
        return -1;

    for (int i = 0; i < Byte_LEN; ++i)
        PUT(heap_list + i * WSIZE, NULL);
    // alignment padding
    PUT(heap_list + (Byte_LEN)*WSIZE, PACK(DSIZE, 1));       // prologue header
    PUT(heap_list + (Byte_LEN + 1) * WSIZE, PACK(DSIZE, 1)); // prologue footer
    PUT(heap_list + (Byte_LEN + 2) * WSIZE, PACK(0, 1));     // epilogue header

    start_ptr = heap_list;
    heap_list += (Byte_LEN)*WSIZE;
    // extend the empty heap with a free block of chunksize bytes
    if (extend_heap(CHUNKSIZE) == NULL)
        return -1;

    return 0;
}

void *mm_malloc(size_t size)    // mm_malloc allocates a block from the free list.
{                               // mostly copied from the textbook
    size_t asize = ALIGN(size); /* Adjusted aligned block size */
    size_t extendsize;          /* Amount to extend heap if no fit */
    char *bp;

    if (size == 0) /* Ignore spurious requests*/
        return NULL;

    register unsigned int r = 0; // faster method to search the free lists by finding the log 2 of the list
    for (int i = 4; i >= 0; i--)
    {
        if (asize & b[i])
        {
            asize >>= S[i];
            r |= S[i];
        }
    }
    int size_index = (int)r - 4;
    if (size_index < 0)
        size_index = 0;
    if (size_index >= 15)
        size_index = 14;

    asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    if ((bp = fitchek(asize, size_index)) != NULL) // search the free list for a fit
        return place(bp, asize);

    extendsize = MAX(asize, CHUNKSIZE); /* No fit found. Get more memory and place the block */
    if ((bp = extend_heap(extendsize)) == NULL)
        return NULL;

    bp = place(bp, asize);
    return bp;
}

static void free_block(char *bp) // empties a block
{

    if (PRED_BLKP(bp) && !SUCC_BLKP(bp)) // if the block is the last in the list
    {

        PUT(SUCC_PTR(PRED_BLKP(bp)), NULL);
    }

    if (SUCC_BLKP(bp) && PRED_BLKP(bp)) // if the block is in the middle of the list

    {
        PUT(SUCC_PTR(PRED_BLKP(bp)), SUCC_BLKP(bp));
        PUT(PRED_PTR(SUCC_BLKP(bp)), PRED_BLKP(bp));
    }
    PUT(SUCC_PTR(bp), NULL); // if the block is the first in the list
    PUT(PRED_PTR(bp), NULL);
}

static void insert_block(char *bp)
{
    int seg_index = GET_SIZE(HDRP(bp));
    register unsigned int r = 0; // result of log2(v) will go here
    for (int i = 4; i >= 0; i--) // unroll for speed...
    {
        if (seg_index & b[i])
        {
            seg_index >>= S[i];
            r |= S[i];
        }
    }
    int x = (int)r - 4;
    if (x < 0)
        x = 0;
    if (x >= 15)
        x = 14;

    char *start = start_ptr + x * WSIZE; // use the index found to quickly find the starting pointer
    void *succ = start;

    while (SUCC_BLKP(succ))
    {
        succ = (char *)SUCC_BLKP(succ);
        if ((unsigned int)succ >= (unsigned int)bp)
        {

            char *tmp = succ;
            succ = (char *)PRED_BLKP(succ);
            PUT(SUCC_PTR(succ), bp);
            PUT(PRED_PTR(bp), succ);
            PUT(SUCC_PTR(bp), tmp);
            PUT(PRED_PTR(tmp), bp);

            return;
        }
    }

    PUT(SUCC_PTR(succ), bp);
    PUT(PRED_PTR(bp), succ);
    PUT(SUCC_PTR(bp), NULL);
}

void mm_free(void *bp) // frees a block and uses boundary-tag coalescing to merge it with any adjacent free blocks in constant time.
{                      // copied directly from textbook
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

void *mm_realloc(void *ptr, size_t size)
{

    if (ptr == NULL)
        return mm_malloc(size);

    else if (size == 0)
    {
        mm_free(ptr);
        return NULL;
    }
    size_t asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    size_t old_size = GET_SIZE(HDRP(ptr));
    void *newptr;

    if (old_size == asize)
        return ptr;

    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    char *next_bp = NEXT_BLKP(ptr);
    size_t total_size = old_size;

    if (prev_alloc && !next_alloc && (old_size + next_size >= asize))
    {
        total_size += next_size;
        free_block(next_bp);
        PUT(HDRP(ptr), PACK(total_size, 1));
        PUT(FTRP(ptr), PACK(total_size, 1));
        place(ptr, total_size);
    }
    else if (!next_size && asize >= old_size)
    {
        size_t extend_size = asize - old_size;
        if ((long)(mem_sbrk(extend_size)) == -1)
            return NULL;

        PUT(HDRP(ptr), PACK(total_size + extend_size, 1));
        PUT(FTRP(ptr), PACK(total_size + extend_size, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));
        place(ptr, asize);
    }
    else
    {
        newptr = mm_malloc(asize);
        if (newptr == NULL)
            return NULL;
        memcpy(newptr, ptr, MIN(old_size, size));
        mm_free(ptr);
        return newptr;
    }
    return ptr;
}

static void *extend_heap(size_t size)
{
    char *bp;
    // allocate an even number of words to maintain alignment

    if ((long)(bp = mem_sbrk(size)) == -1)
    {
        return NULL;
    }

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         // free block header
    PUT(FTRP(bp), PACK(size, 0));         // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // new epilogue header

    // coallesce if the previous block was free
    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
    { // case 1
        insert_block(bp);
        return bp;
    }
    else if (prev_alloc && !next_alloc)
    { // case 2
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        free_block(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc)
    { // case 3
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        free_block(PREV_BLKP(bp));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else
    {                              // case 4
        free_block(PREV_BLKP(bp)); // delete both blocks
        free_block(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    insert_block(bp); // insert the new block
    return bp;
}

static void *fitchek(size_t size, int seg_idx)
{
    while (seg_idx < 15)
    {
        char *start = start_ptr + seg_idx * WSIZE;
        char *bp = (char *)SUCC_BLKP(start);
        while (bp)
        {
            if ((size_t)GET_SIZE(HDRP(bp)) >= size)
                return bp;
            bp = (char *)SUCC_BLKP(bp);
        }
        seg_idx++;
    }
    return NULL;
}

static void *place(char *bp, size_t asize)
{
    size_t b_size = GET_SIZE(HDRP(bp));
    size_t r_size = b_size - asize;

    if (!GET_ALLOC(HDRP(bp)))
        free_block(bp);
    if (r_size >= 2 * DSIZE)
    {

        if (asize <= 64)
        {
            PUT(HDRP(bp), PACK(asize, 1));
            PUT(FTRP(bp), PACK(asize, 1));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(r_size, 0));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(r_size, 0));
            coalesce(NEXT_BLKP(bp));
        }
        else
        {
            PUT(HDRP(bp), PACK(r_size, 0));
            PUT(FTRP(bp), PACK(r_size, 0));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(asize, 1));
            insert_block(bp);
            return NEXT_BLKP(bp);
        }
    }
    else
    {
        PUT(HDRP(bp), PACK(b_size, 1));
        PUT(FTRP(bp), PACK(b_size, 1));
    }
    return bp;
}
