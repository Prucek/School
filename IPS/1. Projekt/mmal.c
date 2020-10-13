/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2020
 * Kostra: Ales Smrcka
 * Implementace : Peter Rucek, xrucek00
 */

#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
#include <string.h> // memcpy

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block 
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE (128*1024)

#endif // NDEBUG

Arena *first_arena = NULL;

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    size = (size - 1) / PAGE_SIZE * PAGE_SIZE + PAGE_SIZE;
    return size;
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
    assert(req_size > sizeof(Arena) + sizeof(Header));

    Arena *a = mmap(NULL, req_size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    if (a == MAP_FAILED)
        return NULL;
    a->next = NULL;
    a->size = req_size;
    return a;
}

/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    if (first_arena == NULL)
    {
        first_arena = a;
        return;
    }
    Arena *tmp = first_arena;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = a;
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    assert(size > 0);

    hdr->asize = 0;
    hdr->size = size;
    hdr->next = (Header *) &first_arena[1];
}

/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    assert(hdr->asize == 0);
    assert(size > 0);

    if (hdr->size >= size + 2*sizeof(Header))
        return true;
    else
        return false;
}

/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    assert(hdr->size >= req_size + 2*sizeof(Header));
    
    Header *new = (Header *) (((char *)hdr) + sizeof(Header) + req_size);
    hdr_ctor(new,hdr->size - sizeof(Header) - req_size);
    new->next = hdr->next;
    hdr->next = new;
    hdr->size = req_size;
    return new;
}

/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    assert(left->next == right);

    if (left == right) // not assert because Headers are in cyclic list, if freeing the only header in arena, hdr == hdr_get_prev(hdr) = hdr->next
        return false;
    if (((char *)left) + sizeof(Header) + left->size != (char *) right)
        return false;
    if (left->asize == 0 && right->asize == 0)
        return true;
    else
        return false;    
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);

    left->size = left->size + right->size + sizeof(Header);
    left->next = right->next;
}

/**
 * Finds the first free block that fits to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *first_fit(size_t size)
{
    assert(size > 0);

    Header *tmp = (Header *) &first_arena[1];
    Header *first = tmp;
    while(tmp->size < size || tmp->asize != 0)
    {
        tmp = tmp->next;
        if (first == tmp)
            return NULL;
    }
    return tmp;
}

/**
 * Search the header which is the predecessor to the hdr. Note that if 
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    assert(first_arena != 0);

    Header *tmp = hdr;
    while(tmp->next != hdr)
    {
        tmp = tmp->next;
    }

    assert(tmp->next == hdr);

    return tmp;
}

/**
 * Allocate memory. Use first-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    if (size == 0)
        return NULL;
    size_t al_size = (size-1) / sizeof(Header) * sizeof(Header) + sizeof(Header); //aligning to sizeof(Header)

    if (first_arena == NULL) // firts allocaion, create arena
    {
        first_arena = arena_alloc(/*al_size + sizeof(Header)*/ allign_page(al_size));
        if (first_arena == NULL)
            return NULL;
        Header *h = (Header*) &first_arena[1];
        hdr_ctor(h,first_arena->size - sizeof(Arena) - sizeof(Header));
    }

    Header *hdr = first_fit(al_size);

    if (hdr == NULL) // no place in current arena
    {
        Arena *a = arena_alloc(/*al_size + sizeof(Header)*/ allign_page(al_size));
        if (a == NULL)
            return NULL;
        arena_append(a);
        hdr = (Header*) &a[1];
        hdr_ctor(hdr,a->size - sizeof(Arena) - sizeof(Header));
        Header *prev = hdr_get_prev(hdr->next);
        prev->next = hdr;
    }

    if (hdr_should_split(hdr,al_size))
        hdr_split(hdr,al_size);  
        
    hdr->asize = size;
    
    return (void *) &hdr[1];
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
    assert(ptr != NULL);

    Header *hdr = &((Header *)ptr)[-1];
    hdr->asize = 0;

    if (hdr_can_merge(hdr,hdr->next))
        hdr_merge(hdr,hdr->next);
        
    if (hdr_can_merge(hdr_get_prev(hdr),hdr))
        hdr_merge(hdr_get_prev(hdr),hdr);
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space or NULL if size equals to 0.
 * @post header_of(return pointer)->size == size
 */
void *mrealloc(void *ptr, size_t size)
{
    if (ptr == NULL || size == 0)
        return NULL;

    Header *hdr = &((Header *)ptr)[-1];
    if (size > hdr->size)
    {
        void * new = mmalloc(size);
        memcpy(new,ptr,((Header *)ptr)->size);
        mfree(ptr);

        Header *tmp = &((Header *)new)[-1];
        assert(tmp->asize == size);

        return new;
    }
    else
    {
        hdr->asize = size;

        assert( hdr->asize == size);

        return (void *) &hdr[1];
    }
}
