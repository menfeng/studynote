//pythonÔ´Âë½âÎö±Ê¼Ç
/*
summarize============================
trick1:*(freeblock)-->next_freeblock
trick2:usedpools[size+size]-->usedpools[size+size-2] when init
        pool = usedpools[size + size];
		pool != pool->nextpool==>There is a used pool for this size class.
trick3:sort usable_arenas

alloc order:
1.usedpools[size+size].freeblock
2.usable_arenas-->freepools
3.usable_arenas-->pool_address
4.newarena

pool=[xxxxxx,block,block....]
	freeblock:
		1.return it when alloc
		2.*(freeblock)-->next_freeblock
usedpools[size+size]=pool<->pool<->pool

arena
	freepools:unused pool	[used pool in in usedpools] freepool.nextpool=freepool
	pool_address:			[use it when freepools in None]

arenas:arena->arana->arena	[full+unused+usable]
unused_arena_objects		[not have alloc mem]
usable_arenas				[have alloc mem of ARENA_SIZE]


free order:
1.return block to pool
A.pool is not full before
	a.pool wasn't empty:leave it in usedpool
	b.pool is empty: unlink from usedpools;link the pool to freepools in it's arena 
		* 1. If all the pools are free, return the arena to
		*    the system free().[unused_arena_objects]
		* 2. If this is the only free pool in the arena,
		*    add the arena back to the `usable_arenas` list.
		* 3. If the "next" arena has a smaller count of free
		*    pools, we have to "slide this arena right" to
		*    restore that usable_arenas is sorted in order of
		*    nfreepools.
		* 4. Else there's nothing more to do.
B.pool is full before:link it to usepool



gc:
_PyObject_GC_Malloc->gc0+=1->collect_generations->collect
collect
1.update_refs
2.subtract_refs:!!!We're only interested in gc_refs for objects in the generation being collected, which can be recognized because only they have positive gc_refs.
3.move_unreachable(young, &unreachable);
4.gc_list_merge(young, old); Move reachable objects to next generation. 
5.move_finalizers(&unreachable, &finalizers);
6.move_finalizer_reachable(&finalizers);
7.m += handle_weakrefs(&unreachable, old);
8.delete_garbage(&unreachable, old);tp_clear
9.(void)handle_finalizers(&finalizers, old);//finalizers->garbage&old
10.clear_freelists
*/


//4 layers
//layer0(C libray malloc)
//	malloc realloc free

//layer1
//	PyMem_MALLOC:macro for efficiency 
//	PyMem_Malloc:func for c extension
//in pymem.h
PyAPI_FUNC(void *) PyMem_Malloc(size_t);
PyAPI_FUNC(void *) PyMem_Realloc(void *, size_t);
PyAPI_FUNC(void) PyMem_Free(void *);

#define PyMem_MALLOC(n)		((size_t)(n) > (size_t)PY_SSIZE_T_MAX ? NULL \
				: malloc((n) ? (n) : 1))
#define PyMem_REALLOC(p, n)	((size_t)(n) > (size_t)PY_SSIZE_T_MAX  ? NULL \
				: realloc((p), (n) ? (n) : 1))
#define PyMem_FREE		free

#define PyMem_New(type, n) \
  ( ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	( (type *) PyMem_Malloc((n) * sizeof(type)) ) )
#define PyMem_NEW(type, n) \
  ( ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	( (type *) PyMem_MALLOC((n) * sizeof(type)) ) )

#define PyMem_Resize(p, type, n) \
  ( (p) = ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	(type *) PyMem_Realloc((p), (n) * sizeof(type)) )
#define PyMem_RESIZE(p, type, n) \
  ( (p) = ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	(type *) PyMem_REALLOC((p), (n) * sizeof(type)) )

#define PyMem_Del		PyMem_Free
#define PyMem_DEL		PyMem_FREE
// in object.c
void *
PyMem_Malloc(size_t nbytes)
{
    return PyMem_MALLOC(nbytes);
}

void *
PyMem_Realloc(void *p, size_t nbytes)
{
    return PyMem_REALLOC(p, nbytes);
}

void
PyMem_Free(void *p)
{
    PyMem_FREE(p);
}

//layer2(PyObj_API)
//in obmalloc.c
#define uptr    Py_uintptr_t
typedef uintptr_t       Py_uintptr_t;
//block
#define ALIGNMENT               8               /* must be 2^N */
#define ALIGNMENT_SHIFT         3
#define ALIGNMENT_MASK          (ALIGNMENT - 1)

#define SMALL_REQUEST_THRESHOLD 512 
#define NB_SMALL_SIZE_CLASSES   (SMALL_REQUEST_THRESHOLD / ALIGNMENT)
/*
  * Request in bytes     Size of allocated block      Size class idx
 * ----------------------------------------------------------------
 *        1-8                     8                       0
 *        9-16                   16                       1
 *       17-24                   24                       2
 *       25-32                   32                       3
 *       33-40                   40                       4
 *       41-48                   48                       5
 *       49-56                   56                       6
 *       57-64                   64                       7
 *       65-72                   72                       8
 *        ...                   ...                     ...
 *      497-504                 504                      62
 *      505-512                 512                      63 
 *
 *      0, SMALL_REQUEST_THRESHOLD + 1 and up: routed to the underlying
 *      allocator.
 */

// Size class idx --> Size of allocated block
#define INDEX2SIZE(I) (((uint)(I) + 1) << ALIGNMENT_SHIFT)
// Request in bytes --> Size class idx
size = (uint)(nbytes - 1) >> ALIGNMENT_SHIFT;

//pool
#define SYSTEM_PAGE_SIZE        (4 * 1024)
#define SYSTEM_PAGE_SIZE_MASK   (SYSTEM_PAGE_SIZE - 1)
#define POOL_SIZE               SYSTEM_PAGE_SIZE        /* must be 2^N */
#define POOL_SIZE_MASK          SYSTEM_PAGE_SIZE_MASK

/* When you say memory, my mind reasons in terms of (pointers to) blocks */
typedef uchar block;

/* Pool for small blocks. */
struct pool_header {
    union { block *_padding;
            uint count; } ref;          /* number of allocated blocks    */
    block *freeblock;                   /* pool's free list head         */
    struct pool_header *nextpool;       /* next pool of this size class  */
    struct pool_header *prevpool;       /* previous pool       ""        */
    uint arenaindex;                    /* index into arenas of base adr */
    uint szidx;                         /* block size class index        */
    uint nextoffset;                    /* bytes to virgin block         */
    uint maxnextoffset;                 /* largest valid nextoffset      */
};

typedef struct pool_header *poolp;

#define ROUNDUP(x)              (((x) + ALIGNMENT_MASK) & ~ALIGNMENT_MASK)
#define POOL_OVERHEAD           ROUNDUP(sizeof(struct pool_header))

/* Round pointer P down to the closest pool-aligned address <= P, as a poolp */
#define POOL_ADDR(P) ((poolp)((uptr)(P) & ~(uptr)POOL_SIZE_MASK))

//arena
#define ARENA_SIZE              (256 << 10)     /* 256KB */
/* Record keeping for arenas. */
struct arena_object {
    uptr address;
    block* pool_address;
    uint nfreepools;
    uint ntotalpools;
    struct pool_header* freepools;
    struct arena_object* nextarena;
    struct arena_object* prevarena;
};
/* Array of objects used to track chunks of memory (arenas). */
static struct arena_object* arenas = NULL;
/* Number of slots currently allocated in the `arenas` vector. */
static uint maxarenas = 0;

/* The head of the singly-linked, NULL-terminated list of available
 * arena_objects.
 */
static struct arena_object* unused_arena_objects = NULL;

/* The head of the doubly-linked, NULL-terminated at each end, list of
 * arena_objects associated with arenas that have pools available.
 */
static struct arena_object* usable_arenas = NULL;
/* How many arena_objects do we initially allocate?
 * 16 = can allocate 16 arenas = 16 * ARENA_SIZE = 4MB before growing the
 * `arenas` vector.
 */
#define INITIAL_ARENA_OBJECTS 16

/* Allocate a new arena.  If we run out of memory, return NULL.  Else
 * allocate a new arena, and return the address of an arena_object
 * describing the new arena.  It's expected that the caller will set
 * `usable_arenas` to the return value.
 */
static struct arena_object* new_arena(void)

#define PTA(x)  ((poolp )((uchar *)&(usedpools[2*(x)]) - 2*sizeof(block *)))
#define PT(x)   PTA(x), PTA(x)

static poolp usedpools[2 * ((NB_SMALL_SIZE_CLASSES + 7) / 8) * 8] = {
    PT(0), PT(1), PT(2), PT(3), PT(4), PT(5), PT(6), PT(7)
#if NB_SMALL_SIZE_CLASSES > 8
    , PT(8), PT(9), PT(10), PT(11), PT(12), PT(13), PT(14), PT(15)
#if NB_SMALL_SIZE_CLASSES > 16
    , PT(16), PT(17), PT(18), PT(19), PT(20), PT(21), PT(22), PT(23)
#if NB_SMALL_SIZE_CLASSES > 24
    , PT(24), PT(25), PT(26), PT(27), PT(28), PT(29), PT(30), PT(31)
#if NB_SMALL_SIZE_CLASSES > 32
    , PT(32), PT(33), PT(34), PT(35), PT(36), PT(37), PT(38), PT(39)
#if NB_SMALL_SIZE_CLASSES > 40
    , PT(40), PT(41), PT(42), PT(43), PT(44), PT(45), PT(46), PT(47)
#if NB_SMALL_SIZE_CLASSES > 48
    , PT(48), PT(49), PT(50), PT(51), PT(52), PT(53), PT(54), PT(55)
#if NB_SMALL_SIZE_CLASSES > 56
    , PT(56), PT(57), PT(58), PT(59), PT(60), PT(61), PT(62), PT(63)
#if NB_SMALL_SIZE_CLASSES > 64
#error "NB_SMALL_SIZE_CLASSES should be less than 64"
#endif /* NB_SMALL_SIZE_CLASSES > 64 */
#endif /* NB_SMALL_SIZE_CLASSES > 56 */
#endif /* NB_SMALL_SIZE_CLASSES > 48 */
#endif /* NB_SMALL_SIZE_CLASSES > 40 */
#endif /* NB_SMALL_SIZE_CLASSES > 32 */
#endif /* NB_SMALL_SIZE_CLASSES > 24 */
#endif /* NB_SMALL_SIZE_CLASSES > 16 */
#endif /* NB_SMALL_SIZE_CLASSES >  8 */
};
static struct arena_object*
new_arena(void)
{
    struct arena_object* arenaobj;
    uint excess;        /* number of bytes above pool alignment */
    void *address;
    int err;

#ifdef PYMALLOC_DEBUG
    if (Py_GETENV("PYTHONMALLOCSTATS"))
        _PyObject_DebugMallocStats();
#endif
    if (unused_arena_objects == NULL) {
        uint i;
        uint numarenas;
        size_t nbytes;

        /* Double the number of arena objects on each allocation.
         * Note that it's possible for `numarenas` to overflow.
         */
        numarenas = maxarenas ? maxarenas << 1 : INITIAL_ARENA_OBJECTS;
        if (numarenas <= maxarenas)
            return NULL;                /* overflow */
#if SIZEOF_SIZE_T <= SIZEOF_INT
        if (numarenas > PY_SIZE_MAX / sizeof(*arenas))
            return NULL;                /* overflow */
#endif
        nbytes = numarenas * sizeof(*arenas);
        arenaobj = (struct arena_object *)realloc(arenas, nbytes);
        if (arenaobj == NULL)
            return NULL;
        arenas = arenaobj;

        /* We might need to fix pointers that were copied.  However,
         * new_arena only gets called when all the pages in the
         * previous arenas are full.  Thus, there are *no* pointers
         * into the old array. Thus, we don't have to worry about
         * invalid pointers.  Just to be sure, some asserts:
         */
        assert(usable_arenas == NULL);
        assert(unused_arena_objects == NULL);

        /* Put the new arenas on the unused_arena_objects list. */
        for (i = maxarenas; i < numarenas; ++i) {
            arenas[i].address = 0;              /* mark as unassociated */
            arenas[i].nextarena = i < numarenas - 1 ?
                                   &arenas[i+1] : NULL;
        }

        /* Update globals. */
        unused_arena_objects = &arenas[maxarenas];
        maxarenas = numarenas;
    }

    /* Take the next available arena object off the head of the list. */
    assert(unused_arena_objects != NULL);
    arenaobj = unused_arena_objects;
    unused_arena_objects = arenaobj->nextarena;
    assert(arenaobj->address == 0);
#ifdef ARENAS_USE_MMAP
    address = mmap(NULL, ARENA_SIZE, PROT_READ|PROT_WRITE,
                   MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    err = (address == MAP_FAILED);
#else
    address = malloc(ARENA_SIZE);
    err = (address == 0);
#endif    
    if (err) {
        /* The allocation failed: return NULL after putting the
         * arenaobj back.
         */
        arenaobj->nextarena = unused_arena_objects;
        unused_arena_objects = arenaobj;
        return NULL;
    }
    arenaobj->address = (uptr)address;

    ++narenas_currently_allocated;
#ifdef PYMALLOC_DEBUG
    ++ntimes_arena_allocated;
    if (narenas_currently_allocated > narenas_highwater)
        narenas_highwater = narenas_currently_allocated;
#endif
    arenaobj->freepools = NULL;
    /* pool_address <- first pool-aligned address in the arena
       nfreepools <- number of whole pools that fit after alignment */
    arenaobj->pool_address = (block*)arenaobj->address;
    arenaobj->nfreepools = ARENA_SIZE / POOL_SIZE;
    assert(POOL_SIZE * arenaobj->nfreepools == ARENA_SIZE);
    excess = (uint)(arenaobj->address & POOL_SIZE_MASK);
    if (excess != 0) {
        --arenaobj->nfreepools;
        arenaobj->pool_address += POOL_SIZE - excess;
    }
    arenaobj->ntotalpools = arenaobj->nfreepools;

    return arenaobj;
}

void *
PyObject_Malloc(size_t nbytes)
{
    block *bp;
    poolp pool;
    poolp next;
    uint size;

#ifdef WITH_VALGRIND
    if (UNLIKELY(running_on_valgrind == -1))
        running_on_valgrind = RUNNING_ON_VALGRIND;
    if (UNLIKELY(running_on_valgrind))
        goto redirect;
#endif

    /*
     * Limit ourselves to PY_SSIZE_T_MAX bytes to prevent security holes.
     * Most python internals blindly use a signed Py_ssize_t to track
     * things without checking for overflows or negatives.
     * As size_t is unsigned, checking for nbytes < 0 is not required.
     */
    if (nbytes > PY_SSIZE_T_MAX)
        return NULL;

    /*
     * This implicitly redirects malloc(0).
     */
    if ((nbytes - 1) < SMALL_REQUEST_THRESHOLD) {
        LOCK();
        /*
         * Most frequent paths first
         */
        size = (uint)(nbytes - 1) >> ALIGNMENT_SHIFT;
        pool = usedpools[size + size];
        if (pool != pool->nextpool) {
            /*
             * There is a used pool for this size class.
             * Pick up the head block of its free list.
             */
            ++pool->ref.count;
            bp = pool->freeblock;
            assert(bp != NULL);
            if ((pool->freeblock = *(block **)bp) != NULL) {
                UNLOCK();
                return (void *)bp;
            }
            /*
             * Reached the end of the free list, try to extend it.
             */
            if (pool->nextoffset <= pool->maxnextoffset) {
                /* There is room for another block. */
                pool->freeblock = (block*)pool +
                                  pool->nextoffset;
                pool->nextoffset += INDEX2SIZE(size);
                *(block **)(pool->freeblock) = NULL;
                UNLOCK();
                return (void *)bp;
            }
            /* Pool is full, unlink from used pools. */
            next = pool->nextpool;
            pool = pool->prevpool;
            next->prevpool = pool;
            pool->nextpool = next;
            UNLOCK();
            return (void *)bp;
        }

        /* There isn't a pool of the right size class immediately
         * available:  use a free pool.
         */
        if (usable_arenas == NULL) {
            /* No arena has a free pool:  allocate a new arena. */
#ifdef WITH_MEMORY_LIMITS
            if (narenas_currently_allocated >= MAX_ARENAS) {
                UNLOCK();
                goto redirect;
            }
#endif
            usable_arenas = new_arena();
            if (usable_arenas == NULL) {
                UNLOCK();
                goto redirect;
            }
            usable_arenas->nextarena =
                usable_arenas->prevarena = NULL;
        }
        assert(usable_arenas->address != 0);

        /* Try to get a cached free pool. */
        pool = usable_arenas->freepools;
        if (pool != NULL) {
            /* Unlink from cached pools. */
            usable_arenas->freepools = pool->nextpool;

            /* This arena already had the smallest nfreepools
             * value, so decreasing nfreepools doesn't change
             * that, and we don't need to rearrange the
             * usable_arenas list.  However, if the arena has
             * become wholly allocated, we need to remove its
             * arena_object from usable_arenas.
             */
            --usable_arenas->nfreepools;
            if (usable_arenas->nfreepools == 0) {
                /* Wholly allocated:  remove. */
                assert(usable_arenas->freepools == NULL);
                assert(usable_arenas->nextarena == NULL ||
                       usable_arenas->nextarena->prevarena ==
                       usable_arenas);

                usable_arenas = usable_arenas->nextarena;
                if (usable_arenas != NULL) {
                    usable_arenas->prevarena = NULL;
                    assert(usable_arenas->address != 0);
                }
            }
            else {
                /* nfreepools > 0:  it must be that freepools
                 * isn't NULL, or that we haven't yet carved
                 * off all the arena's pools for the first
                 * time.
                 */
                assert(usable_arenas->freepools != NULL ||
                       usable_arenas->pool_address <=
                       (block*)usable_arenas->address +
                           ARENA_SIZE - POOL_SIZE);
            }
        init_pool:
            /* Frontlink to used pools. */
            next = usedpools[size + size]; /* == prev */
            pool->nextpool = next;
            pool->prevpool = next;
            next->nextpool = pool;
            next->prevpool = pool;
            pool->ref.count = 1;
            if (pool->szidx == size) {
                /* Luckily, this pool last contained blocks
                 * of the same size class, so its header
                 * and free list are already initialized.
                 */
                bp = pool->freeblock;
                pool->freeblock = *(block **)bp;
                UNLOCK();
                return (void *)bp;
            }
            /*
             * Initialize the pool header, set up the free list to
             * contain just the second block, and return the first
             * block.
             */
            pool->szidx = size;
            size = INDEX2SIZE(size);
            bp = (block *)pool + POOL_OVERHEAD;
            pool->nextoffset = POOL_OVERHEAD + (size << 1);
            pool->maxnextoffset = POOL_SIZE - size;
            pool->freeblock = bp + size;
            *(block **)(pool->freeblock) = NULL;
            UNLOCK();
            return (void *)bp;
        }

        /* Carve off a new pool. */
        assert(usable_arenas->nfreepools > 0);
        assert(usable_arenas->freepools == NULL);
        pool = (poolp)usable_arenas->pool_address;
        assert((block*)pool <= (block*)usable_arenas->address +
                               ARENA_SIZE - POOL_SIZE);
        pool->arenaindex = usable_arenas - arenas;
        assert(&arenas[pool->arenaindex] == usable_arenas);
        pool->szidx = DUMMY_SIZE_IDX;
        usable_arenas->pool_address += POOL_SIZE;
        --usable_arenas->nfreepools;

        if (usable_arenas->nfreepools == 0) {
            assert(usable_arenas->nextarena == NULL ||
                   usable_arenas->nextarena->prevarena ==
                   usable_arenas);
            /* Unlink the arena:  it is completely allocated. */
            usable_arenas = usable_arenas->nextarena;
            if (usable_arenas != NULL) {
                usable_arenas->prevarena = NULL;
                assert(usable_arenas->address != 0);
            }
        }

        goto init_pool;
    }

    /* The small block allocator ends here. */

redirect:
    /* Redirect the original request to the underlying (libc) allocator.
     * We jump here on bigger requests, on error in the code above (as a
     * last chance to serve the request) or when the max memory limit
     * has been reached.
     */
    if (nbytes == 0)
        nbytes = 1;
    return (void *)malloc(nbytes);
}
void
PyObject_Free(void *p)
{
    poolp pool;
    block *lastfree;
    poolp next, prev;
    uint size;
#ifndef Py_USING_MEMORY_DEBUGGER
    uint arenaindex_temp;
#endif

    if (p == NULL)      /* free(NULL) has no effect */
        return;

#ifdef WITH_VALGRIND
    if (UNLIKELY(running_on_valgrind > 0))
        goto redirect;
#endif

    pool = POOL_ADDR(p);
    if (Py_ADDRESS_IN_RANGE(p, pool)) {
        /* We allocated this address. */
        LOCK();
        /* Link p to the start of the pool's freeblock list.  Since
         * the pool had at least the p block outstanding, the pool
         * wasn't empty (so it's already in a usedpools[] list, or
         * was full and is in no list -- it's not in the freeblocks
         * list in any case).
         */
        assert(pool->ref.count > 0);            /* else it was empty */
        *(block **)p = lastfree = pool->freeblock;
        pool->freeblock = (block *)p;
        if (lastfree) {
            struct arena_object* ao;
            uint nf;  /* ao->nfreepools */

            /* freeblock wasn't NULL, so the pool wasn't full,
             * and the pool is in a usedpools[] list.
             */
            if (--pool->ref.count != 0) {
                /* pool isn't empty:  leave it in usedpools */
                UNLOCK();
                return;
            }
            /* Pool is now empty:  unlink from usedpools, and
             * link to the front of freepools.  This ensures that
             * previously freed pools will be allocated later
             * (being not referenced, they are perhaps paged out).
             */
            next = pool->nextpool;
            prev = pool->prevpool;
            next->prevpool = prev;
            prev->nextpool = next;

            /* Link the pool to freepools.  This is a singly-linked
             * list, and pool->prevpool isn't used there.
             */
            ao = &arenas[pool->arenaindex];
            pool->nextpool = ao->freepools;
            ao->freepools = pool;
            nf = ++ao->nfreepools;

            /* All the rest is arena management.  We just freed
             * a pool, and there are 4 cases for arena mgmt:
             * 1. If all the pools are free, return the arena to
             *    the system free().
             * 2. If this is the only free pool in the arena,
             *    add the arena back to the `usable_arenas` list.
             * 3. If the "next" arena has a smaller count of free
             *    pools, we have to "slide this arena right" to
             *    restore that usable_arenas is sorted in order of
             *    nfreepools.
             * 4. Else there's nothing more to do.
             */
            if (nf == ao->ntotalpools) {
                /* Case 1.  First unlink ao from usable_arenas.
                 */
                assert(ao->prevarena == NULL ||
                       ao->prevarena->address != 0);
                assert(ao ->nextarena == NULL ||
                       ao->nextarena->address != 0);

                /* Fix the pointer in the prevarena, or the
                 * usable_arenas pointer.
                 */
                if (ao->prevarena == NULL) {
                    usable_arenas = ao->nextarena;
                    assert(usable_arenas == NULL ||
                           usable_arenas->address != 0);
                }
                else {
                    assert(ao->prevarena->nextarena == ao);
                    ao->prevarena->nextarena =
                        ao->nextarena;
                }
                /* Fix the pointer in the nextarena. */
                if (ao->nextarena != NULL) {
                    assert(ao->nextarena->prevarena == ao);
                    ao->nextarena->prevarena =
                        ao->prevarena;
                }
                /* Record that this arena_object slot is
                 * available to be reused.
                 */
                ao->nextarena = unused_arena_objects;
                unused_arena_objects = ao;

                /* Free the entire arena. */
#ifdef ARENAS_USE_MMAP
                munmap((void *)ao->address, ARENA_SIZE);
#else
                free((void *)ao->address);
#endif
                ao->address = 0;                        /* mark unassociated */
                --narenas_currently_allocated;

                UNLOCK();
                return;
            }
            if (nf == 1) {
                /* Case 2.  Put ao at the head of
                 * usable_arenas.  Note that because
                 * ao->nfreepools was 0 before, ao isn't
                 * currently on the usable_arenas list.
                 */
                ao->nextarena = usable_arenas;
                ao->prevarena = NULL;
                if (usable_arenas)
                    usable_arenas->prevarena = ao;
                usable_arenas = ao;
                assert(usable_arenas->address != 0);

                UNLOCK();
                return;
            }
            /* If this arena is now out of order, we need to keep
             * the list sorted.  The list is kept sorted so that
             * the "most full" arenas are used first, which allows
             * the nearly empty arenas to be completely freed.  In
             * a few un-scientific tests, it seems like this
             * approach allowed a lot more memory to be freed.
             */
            if (ao->nextarena == NULL ||
                         nf <= ao->nextarena->nfreepools) {
                /* Case 4.  Nothing to do. */
                UNLOCK();
                return;
            }
            /* Case 3:  We have to move the arena towards the end
             * of the list, because it has more free pools than
             * the arena to its right.
             * First unlink ao from usable_arenas.
             */
            if (ao->prevarena != NULL) {
                /* ao isn't at the head of the list */
                assert(ao->prevarena->nextarena == ao);
                ao->prevarena->nextarena = ao->nextarena;
            }
            else {
                /* ao is at the head of the list */
                assert(usable_arenas == ao);
                usable_arenas = ao->nextarena;
            }
            ao->nextarena->prevarena = ao->prevarena;

            /* Locate the new insertion point by iterating over
             * the list, using our nextarena pointer.
             */
            while (ao->nextarena != NULL &&
                            nf > ao->nextarena->nfreepools) {
                ao->prevarena = ao->nextarena;
                ao->nextarena = ao->nextarena->nextarena;
            }

            /* Insert ao at this point. */
            assert(ao->nextarena == NULL ||
                ao->prevarena == ao->nextarena->prevarena);
            assert(ao->prevarena->nextarena == ao->nextarena);

            ao->prevarena->nextarena = ao;
            if (ao->nextarena != NULL)
                ao->nextarena->prevarena = ao;

            /* Verify that the swaps worked. */
            assert(ao->nextarena == NULL ||
                      nf <= ao->nextarena->nfreepools);
            assert(ao->prevarena == NULL ||
                      nf > ao->prevarena->nfreepools);
            assert(ao->nextarena == NULL ||
                ao->nextarena->prevarena == ao);
            assert((usable_arenas == ao &&
                ao->prevarena == NULL) ||
                ao->prevarena->nextarena == ao);

            UNLOCK();
            return;
        }
        /* Pool was full, so doesn't currently live in any list:
         * link it to the front of the appropriate usedpools[] list.
         * This mimics LRU pool usage for new allocations and
         * targets optimal filling when several pools contain
         * blocks of the same size class.
         */
        --pool->ref.count;
        assert(pool->ref.count > 0);            /* else the pool is empty */
        size = pool->szidx;
        next = usedpools[size + size];
        prev = next->prevpool;
        /* insert pool before next:   prev <-> pool <-> next */
        pool->nextpool = next;
        pool->prevpool = prev;
        next->prevpool = pool;
        prev->nextpool = pool;
        UNLOCK();
        return;
    }

#ifdef WITH_VALGRIND
redirect:
#endif
    /* We didn't allocate this address. */
    free(p);
}

//layer3(object specific memory:int dict list string ...)



//in objimpl

//all negative
#define _PyGC_REFS_UNTRACKED                    (-2)
#define _PyGC_REFS_REACHABLE                    (-3)
#define _PyGC_REFS_TENTATIVELY_UNREACHABLE      (-4)

/* Get an object's GC head */
#define AS_GC(o) ((PyGC_Head *)(o)-1)
/* Get the object given the GC head */
#define FROM_GC(g) ((PyObject *)(((PyGC_Head *)g)+1))
/*** Global GC state ***/
struct gc_generation {
    PyGC_Head head;
    int threshold; /* collection threshold */
    int count; /* count of allocations or collections of younger
                  generations */
};
#define NUM_GENERATIONS 3
#define GEN_HEAD(n) (&generations[n].head)
/* linked lists of container objects */
static struct gc_generation generations[NUM_GENERATIONS] = {
    /* PyGC_Head,                               threshold,      count */
    {{{GEN_HEAD(0), GEN_HEAD(0), 0}},           700,            0},
    {{{GEN_HEAD(1), GEN_HEAD(1), 0}},           10,             0},
    {{{GEN_HEAD(2), GEN_HEAD(2), 0}},           10,             0},
};

PyGC_Head *_PyGC_generation0 = GEN_HEAD(0);
static int enabled = 1; /* automatic collection enabled? */
/* true if we are currently running the collector */
static int collecting = 0;
/* list of uncollectable objects */
static PyObject *garbage = NULL;
/* This is the number of objects who survived the last full collection. It
   approximates the number of long lived objects tracked by the GC.
   (by "full collection", we mean a collection of the oldest generation).
*/
static Py_ssize_t long_lived_total = 0;
/* This is the number of objects who survived all "non-full" collections,
   and are awaiting to undergo a full collection for the first time.
*/
static Py_ssize_t long_lived_pending = 0;

/* Tell the GC to track this object.  NB: While the object is tracked the
 * collector it must be safe to call the ob_traverse method. */
#define _PyObject_GC_TRACK(o) do { \
    PyGC_Head *g = _Py_AS_GC(o); \
    if (g->gc.gc_refs != _PyGC_REFS_UNTRACKED) \
        Py_FatalError("GC object already tracked"); \
    g->gc.gc_refs = _PyGC_REFS_REACHABLE; \
    g->gc.gc_next = _PyGC_generation0; \
    g->gc.gc_prev = _PyGC_generation0->gc.gc_prev; \
    g->gc.gc_prev->gc.gc_next = g; \
    _PyGC_generation0->gc.gc_prev = g; \
    } while (0);
/*				
A-->A
A<--A
__________
A-->A1-->A
A<--A1<--A
______________
A-->A1-->A2-->A
A<--A1<--A2<--A
*/

/* GC information is stored BEFORE the object structure. */
typedef union _gc_head {
    struct {
        union _gc_head *gc_next;
        union _gc_head *gc_prev;
        Py_ssize_t gc_refs;
    } gc;
    long double dummy;  /* force worst-case alignment */
} PyGC_Head;
//in gcmodule.c
/* Get an object's GC head */
#define AS_GC(o) ((PyGC_Head *)(o)-1)

/* Get the object given the GC head */
#define FROM_GC(g) ((PyObject *)(((PyGC_Head *)g)+1))


static void
gc_list_init(PyGC_Head *list)
{
    list->gc.gc_prev = list;
    list->gc.gc_next = list;
}

/* append list `from` onto list `to`; `from` becomes an empty list */
static void
gc_list_merge(PyGC_Head *from, PyGC_Head *to)
{
    PyGC_Head *tail;
    assert(from != to);
    if (!gc_list_is_empty(from)) {
        tail = to->gc.gc_prev;
        tail->gc.gc_next = from->gc.gc_next;
        tail->gc.gc_next->gc.gc_prev = tail;
        to->gc.gc_prev = from->gc.gc_prev;
        to->gc.gc_prev->gc.gc_next = to;
    }
    gc_list_init(from);
}

PyObject *
_PyObject_GC_New(PyTypeObject *tp)
{
    PyObject *op = _PyObject_GC_Malloc(_PyObject_SIZE(tp));
    if (op != NULL)
        op = PyObject_INIT(op, tp);
    return op;
}
PyObject *
_PyObject_GC_Malloc(size_t basicsize)
{
    PyObject *op;
    PyGC_Head *g;
    if (basicsize > PY_SSIZE_T_MAX - sizeof(PyGC_Head))
        return PyErr_NoMemory();
    g = (PyGC_Head *)PyObject_MALLOC(
        sizeof(PyGC_Head) + basicsize);
    if (g == NULL)
        return PyErr_NoMemory();
    g->gc.gc_refs = GC_UNTRACKED;
    generations[0].count++; /* number of allocated GC objects */
    if (generations[0].count > generations[0].threshold &&
        enabled &&
        generations[0].threshold &&
        !collecting &&
        !PyErr_Occurred()) {
        collecting = 1;
        collect_generations();
        collecting = 0;
    }
    op = FROM_GC(g);
    return op;
}
static Py_ssize_t
collect_generations(void)
{
    int i;
    Py_ssize_t n = 0;

    /* Find the oldest generation (highest numbered) where the count
     * exceeds the threshold.  Objects in the that generation and
     * generations younger than it will be collected. */
    for (i = NUM_GENERATIONS-1; i >= 0; i--) {
        if (generations[i].count > generations[i].threshold) {
            /* Avoid quadratic performance degradation in number
               of tracked objects. See comments at the beginning
               of this file, and issue #4074.
            */
            if (i == NUM_GENERATIONS - 1
                && long_lived_pending < long_lived_total / 4)
                continue;
            n = collect(i);
            break;
        }
    }
    return n;
}

static void
update_refs(PyGC_Head *containers)
{
    PyGC_Head *gc = containers->gc.gc_next;
    for (; gc != containers; gc = gc->gc.gc_next) {
        assert(gc->gc.gc_refs == GC_REACHABLE);
        gc->gc.gc_refs = Py_REFCNT(FROM_GC(gc));
        assert(gc->gc.gc_refs != 0);
    }
}
static void
subtract_refs(PyGC_Head *containers)
{
    traverseproc traverse;
    PyGC_Head *gc = containers->gc.gc_next;
    for (; gc != containers; gc=gc->gc.gc_next) {
        traverse = Py_TYPE(FROM_GC(gc))->tp_traverse;
        (void) traverse(FROM_GC(gc),
                       (visitproc)visit_decref,
                       NULL);
    }
}
static int
visit_decref(PyObject *op, void *data)
{
    assert(op != NULL);
    if (PyObject_IS_GC(op)) {
        PyGC_Head *gc = AS_GC(op);
        /* We're only interested in gc_refs for objects in the
         * generation being collected, which can be recognized
         * because only they have positive gc_refs.
         */
        assert(gc->gc.gc_refs != 0); /* else refcount was too small */
        if (gc->gc.gc_refs > 0)
            gc->gc.gc_refs--;
    }
    return 0;
}


static void
move_unreachable(PyGC_Head *young, PyGC_Head *unreachable)
{
    PyGC_Head *gc = young->gc.gc_next;

    /* Invariants:  all objects "to the left" of us in young have gc_refs
     * = GC_REACHABLE, and are indeed reachable (directly or indirectly)
     * from outside the young list as it was at entry.  All other objects
     * from the original young "to the left" of us are in unreachable now,
     * and have gc_refs = GC_TENTATIVELY_UNREACHABLE.  All objects to the
     * left of us in 'young' now have been scanned, and no objects here
     * or to the right have been scanned yet.
     */

    while (gc != young) {
        PyGC_Head *next;

        if (gc->gc.gc_refs) {
            /* gc is definitely reachable from outside the
             * original 'young'.  Mark it as such, and traverse
             * its pointers to find any other objects that may
             * be directly reachable from it.  Note that the
             * call to tp_traverse may append objects to young,
             * so we have to wait until it returns to determine
             * the next object to visit.
             */
            PyObject *op = FROM_GC(gc);
            traverseproc traverse = Py_TYPE(op)->tp_traverse;
            assert(gc->gc.gc_refs > 0);
            gc->gc.gc_refs = GC_REACHABLE;
            (void) traverse(op,
                            (visitproc)visit_reachable,
                            (void *)young);
            next = gc->gc.gc_next;
            if (PyTuple_CheckExact(op)) {
                _PyTuple_MaybeUntrack(op);
            }
        }
        else {
            /* This *may* be unreachable.  To make progress,
             * assume it is.  gc isn't directly reachable from
             * any object we've already traversed, but may be
             * reachable from an object we haven't gotten to yet.
             * visit_reachable will eventually move gc back into
             * young if that's so, and we'll see it again.
             */
            next = gc->gc.gc_next;
            gc_list_move(gc, unreachable);
            gc->gc.gc_refs = GC_TENTATIVELY_UNREACHABLE;
        }
        gc = next;
    }
}
/* A traversal callback for move_unreachable. */
static int
visit_reachable(PyObject *op, PyGC_Head *reachable)
{
    if (PyObject_IS_GC(op)) {
        PyGC_Head *gc = AS_GC(op);
        const Py_ssize_t gc_refs = gc->gc.gc_refs;

        if (gc_refs == 0) {
            /* This is in move_unreachable's 'young' list, but
             * the traversal hasn't yet gotten to it.  All
             * we need to do is tell move_unreachable that it's
             * reachable.
             */
            gc->gc.gc_refs = 1;
        }
        else if (gc_refs == GC_TENTATIVELY_UNREACHABLE) {
            /* This had gc_refs = 0 when move_unreachable got
             * to it, but turns out it's reachable after all.
             * Move it back to move_unreachable's 'young' list,
             * and move_unreachable will eventually get to it
             * again.
             */
            gc_list_move(gc, reachable);
            gc->gc.gc_refs = 1;
        }
        /* Else there's nothing to do.
         * If gc_refs > 0, it must be in move_unreachable's 'young'
         * list, and move_unreachable will eventually get to it.
         * If gc_refs == GC_REACHABLE, it's either in some other
         * generation so we don't care about it, or move_unreachable
         * already dealt with it.
         * If gc_refs == GC_UNTRACKED, it must be ignored.
         */
         else {
            assert(gc_refs > 0
                   || gc_refs == GC_REACHABLE
                   || gc_refs == GC_UNTRACKED);
         }
    }
    return 0;
}
static void
gc_list_move(PyGC_Head *node, PyGC_Head *list)
{
    PyGC_Head *new_prev;
    PyGC_Head *current_prev = node->gc.gc_prev;
    PyGC_Head *current_next = node->gc.gc_next;
    /* Unlink from current list. */
    current_prev->gc.gc_next = current_next;
    current_next->gc.gc_prev = current_prev;
    /* Relink at end of new list. */
    new_prev = node->gc.gc_prev = list->gc.gc_prev;
    new_prev->gc.gc_next = list->gc.gc_prev = node;
    node->gc.gc_next = list;
}
/* This is the main function.  Read this to understand how the
 * collection process works. */
static Py_ssize_t
collect(int generation)
{
    int i;
    Py_ssize_t m = 0; /* # objects collected */
    Py_ssize_t n = 0; /* # unreachable objects that couldn't be collected */
    PyGC_Head *young; /* the generation we are examining */
    PyGC_Head *old; /* next older generation */
    PyGC_Head unreachable; /* non-problematic unreachable trash */
    PyGC_Head finalizers;  /* objects with, & reachable from, __del__ */
    PyGC_Head *gc;
    double t1 = 0.0;

    if (delstr == NULL) {
        delstr = PyString_InternFromString("__del__");
        if (delstr == NULL)
            Py_FatalError("gc couldn't allocate \"__del__\"");
    }

    if (debug & DEBUG_STATS) {
        PySys_WriteStderr("gc: collecting generation %d...\n",
                          generation);
        PySys_WriteStderr("gc: objects in each generation:");
        for (i = 0; i < NUM_GENERATIONS; i++)
            PySys_WriteStderr(" %" PY_FORMAT_SIZE_T "d",
                              gc_list_size(GEN_HEAD(i)));
        t1 = get_time();
        PySys_WriteStderr("\n");
    }

    /* update collection and allocation counters */
    if (generation+1 < NUM_GENERATIONS)
        generations[generation+1].count += 1;
    for (i = 0; i <= generation; i++)
        generations[i].count = 0;

    /* merge younger generations with one we are currently collecting */
    for (i = 0; i < generation; i++) {
        gc_list_merge(GEN_HEAD(i), GEN_HEAD(generation));
    }

    /* handy references */
    young = GEN_HEAD(generation);
    if (generation < NUM_GENERATIONS-1)
        old = GEN_HEAD(generation+1);
    else
        old = young;

    /* Using ob_refcnt and gc_refs, calculate which objects in the
     * container set are reachable from outside the set (i.e., have a
     * refcount greater than 0 when all the references within the
     * set are taken into account).
     */
    update_refs(young);
    subtract_refs(young);

    /* Leave everything reachable from outside young in young, and move
     * everything else (in young) to unreachable.
     * NOTE:  This used to move the reachable objects into a reachable
     * set instead.  But most things usually turn out to be reachable,
     * so it's more efficient to move the unreachable things.
     */
    gc_list_init(&unreachable);
    move_unreachable(young, &unreachable);

    /* Move reachable objects to next generation. */
    if (young != old) {
        if (generation == NUM_GENERATIONS - 2) {
            long_lived_pending += gc_list_size(young);
        }
        gc_list_merge(young, old);
    }
    else {
        /* We only untrack dicts in full collections, to avoid quadratic
           dict build-up. See issue #14775. */
        untrack_dicts(young);
        long_lived_pending = 0;
        long_lived_total = gc_list_size(young);
    }

    /* All objects in unreachable are trash, but objects reachable from
     * finalizers can't safely be deleted.  Python programmers should take
     * care not to create such things.  For Python, finalizers means
     * instance objects with __del__ methods.  Weakrefs with callbacks
     * can also call arbitrary Python code but they will be dealt with by
     * handle_weakrefs().
     */
    gc_list_init(&finalizers);
    move_finalizers(&unreachable, &finalizers);
    /* finalizers contains the unreachable objects with a finalizer;
     * unreachable objects reachable *from* those are also uncollectable,
     * and we move those into the finalizers list too.
     */
    move_finalizer_reachable(&finalizers);

    /* Collect statistics on collectable objects found and print
     * debugging information.
     */
    for (gc = unreachable.gc.gc_next; gc != &unreachable;
                    gc = gc->gc.gc_next) {
        m++;
        if (debug & DEBUG_COLLECTABLE) {
            debug_cycle("collectable", FROM_GC(gc));
        }
    }

    /* Clear weakrefs and invoke callbacks as necessary. */
    m += handle_weakrefs(&unreachable, old);

    /* Call tp_clear on objects in the unreachable set.  This will cause
     * the reference cycles to be broken.  It may also cause some objects
     * in finalizers to be freed.
     */
    delete_garbage(&unreachable, old);

    /* Collect statistics on uncollectable objects found and print
     * debugging information. */
    for (gc = finalizers.gc.gc_next;
         gc != &finalizers;
         gc = gc->gc.gc_next) {
        n++;
        if (debug & DEBUG_UNCOLLECTABLE)
            debug_cycle("uncollectable", FROM_GC(gc));
    }
    if (debug & DEBUG_STATS) {
        double t2 = get_time();
        if (m == 0 && n == 0)
            PySys_WriteStderr("gc: done");
        else
            PySys_WriteStderr(
                "gc: done, "
                "%" PY_FORMAT_SIZE_T "d unreachable, "
                "%" PY_FORMAT_SIZE_T "d uncollectable",
                n+m, n);
        if (t1 && t2) {
            PySys_WriteStderr(", %.4fs elapsed", t2-t1);
        }
        PySys_WriteStderr(".\n");
    }

    /* Append instances in the uncollectable set to a Python
     * reachable list of garbage.  The programmer has to deal with
     * this if they insist on creating this type of structure.
     */
    (void)handle_finalizers(&finalizers, old);

    /* Clear free list only during the collection of the highest
     * generation */
    if (generation == NUM_GENERATIONS-1) {
        clear_freelists();
    }

    if (PyErr_Occurred()) {
        if (gc_str == NULL)
            gc_str = PyString_FromString("garbage collection");
        PyErr_WriteUnraisable(gc_str);
        Py_FatalError("unexpected exception during garbage collection");
    }
    return n+m;
}

/* Break reference cycles by clearing the containers involved.  This is
 * tricky business as the lists can be changing and we don't know which
 * objects may be freed.  It is possible I screwed something up here.
 */
static void
delete_garbage(PyGC_Head *collectable, PyGC_Head *old)
{
    inquiry clear;

    while (!gc_list_is_empty(collectable)) {
        PyGC_Head *gc = collectable->gc.gc_next;
        PyObject *op = FROM_GC(gc);

        assert(IS_TENTATIVELY_UNREACHABLE(op));
        if (debug & DEBUG_SAVEALL) {
            PyList_Append(garbage, op);
        }
        else {
            if ((clear = Py_TYPE(op)->tp_clear) != NULL) {
                Py_INCREF(op);
                clear(op);
                Py_DECREF(op);
            }
        }
        if (collectable->gc.gc_next == gc) {
            /* object is still alive, move it, it may die later */
            gc_list_move(gc, old);
            gc->gc.gc_refs = GC_REACHABLE;
        }
    }
}



/*
 * how to break cycle in list?
 */

//in object.h
#define Py_DECREF(op)                                   \
    do {                                                \
        if (_Py_DEC_REFTOTAL  _Py_REF_DEBUG_COMMA       \
        --((PyObject*)(op))->ob_refcnt != 0)            \
            _Py_CHECK_REFCNT(op)                        \
        else                                            \
        _Py_Dealloc((PyObject *)(op));                  \
    } while (0)

#define _Py_Dealloc(op) (                               \
    _Py_INC_TPFREES(op) _Py_COUNT_ALLOCS_COMMA          \
    (*Py_TYPE(op)->tp_dealloc)((PyObject *)(op)))

#define Py_XDECREF(op) do { if ((op) == NULL) ; else Py_DECREF(op); } while (0)
list_dealloc

//1.tp_clear
static int
list_clear(PyListObject *a)
{
    Py_ssize_t i;
    PyObject **item = a->ob_item;
    if (item != NULL) {
        /* Because XDECREF can recursively invoke operations on
           this list, we make it empty first. */
        i = Py_SIZE(a);
        Py_SIZE(a) = 0;
        a->ob_item = NULL;
        a->allocated = 0;
        while (--i >= 0) {
            Py_XDECREF(item[i]);
        }
        PyMem_FREE(item);
    }
    /* Never fails; the return value can be ignored.
       Note that there is no guarantee that the list is actually empty
       at this point, because XDECREF may have populated it again! */
    return 0;
}

//2.tp_dealloc
static void
list_dealloc(PyListObject *op)
{
    Py_ssize_t i;
    PyObject_GC_UnTrack(op);
    Py_TRASHCAN_SAFE_BEGIN(op)
    if (op->ob_item != NULL) {
        /* Do it backwards, for Christian Tismer.
           There's a simple test case where somehow this reduces
           thrashing when a *very* large list is created and
           immediately deleted. */
        i = Py_SIZE(op);
        while (--i >= 0) {
            Py_XDECREF(op->ob_item[i]);
        }
        PyMem_FREE(op->ob_item);
    }
    if (numfree < PyList_MAXFREELIST && PyList_CheckExact(op))
        free_list[numfree++] = op;
    else
        Py_TYPE(op)->tp_free((PyObject *)op);
    Py_TRASHCAN_SAFE_END(op)
}

//3.tp_free PyObject_GC_Del
void
PyObject_GC_Del(void *op)
{
    PyGC_Head *g = AS_GC(op);
    if (IS_TRACKED(op))
        gc_list_remove(g);
    if (generations[0].count > 0) {
        generations[0].count--;
    }
    PyObject_FREE(g);
}

//about get_referrers
//sys.getrefcount(object) Return the reference count of the object. The count returned is generally one higher than you might expect, because it includes the (temporary) reference as an argument to getrefcount().
/*
d={}
l=[]
d['0']=l
d['1']=l
d['3']=l
len(gc.get_referrers(l))
2
sys.getrefcount(l)
5
why?
	referrersvisit return 1
*/

#define Py_VISIT(op)                                                    \
    do {                                                                \
        if (op) {                                                       \
            int vret = visit((PyObject *)(op), arg);                    \
            if (vret)                                                   \
                return vret;                                            \
        }                                                               \
    } while (0)

static int
dict_traverse(PyObject *op, visitproc visit, void *arg)
{
    Py_ssize_t i = 0;
    PyObject *pk;
    PyObject *pv;

    while (PyDict_Next(op, &i, &pk, &pv)) {
        Py_VISIT(pk);
        Py_VISIT(pv);
    }
    return 0;
}

static int
referrersvisit(PyObject* obj, PyObject *objs)
{
    Py_ssize_t i;
    for (i = 0; i < PyTuple_GET_SIZE(objs); i++)
        if (PyTuple_GET_ITEM(objs, i) == obj)
            return 1;
    return 0;
}

static int
gc_referrers_for(PyObject *objs, PyGC_Head *list, PyObject *resultlist)
{
    PyGC_Head *gc;
    PyObject *obj;
    traverseproc traverse;
    for (gc = list->gc.gc_next; gc != list; gc = gc->gc.gc_next) {
        obj = FROM_GC(gc);
        traverse = Py_TYPE(obj)->tp_traverse;
        if (obj == objs || obj == resultlist)
            continue;
        if (traverse(obj, (visitproc)referrersvisit, objs)) {
            if (PyList_Append(resultlist, obj) < 0)
                return 0; /* error */
        }
    }
    return 1; /* no error */
}

static PyObject *
gc_get_referrers(PyObject *self, PyObject *args)
{
    int i;
    PyObject *result = PyList_New(0);
    if (!result) return NULL;

    for (i = 0; i < NUM_GENERATIONS; i++) {
        if (!(gc_referrers_for(args, GEN_HEAD(i), result))) {
            Py_DECREF(result);
            return NULL;
        }
    }
    return result;
}

