/*
1.Hash table
	a. Unused.  me_key == me_value == NULL
	b. Active.  me_key != NULL and me_key != dummy and me_value != NULL
	c. Dummy.  me_key == dummy and me_value == NULL

2.lookdict
	(1)hash
	(2)Unused||me_key==key	:return entry
	(3)Dummy				:freeslot=entry
	(4)Active&&equal		:return entry
	(5)
		for (perturb = hash; ; perturb >>= PERTURB_SHIFT) {
			i = (i << 2) + i + perturb + 1;
			if Unused		:return freeslot or entry
			if me_key==key	:return entry
			if Active&&equa	:return entry
			if Dummy		:check freeslot
3.insert
	(1)find
	(2)set
	(3) if (!(mp->ma_used > n_used && mp->ma_fill*3 >= (mp->ma_mask+1)*2))
			return 0;
		return dictresize(mp, (mp->ma_used > 50000 ? 2 : 4) * mp->ma_used);

4.smalltable
	#define PyDict_MINSIZE 8
	PyDictEntry ma_smalltable[PyDict_MINSIZE];

5.pool
	#define PyDict_MAXFREELIST 80
	static PyDictObject *free_list[PyDict_MAXFREELIST];

 */

//PyDictEntry
typedef struct {
    /* Cached hash code of me_key.  Note that hash codes are C longs.
     * We have to use Py_ssize_t instead because dict_popitem() abuses
     * me_hash to hold a search finger.
     */
    Py_ssize_t me_hash;
    PyObject *me_key;
    PyObject *me_value;
} PyDictEntry;

//PyDictObject
typedef struct _dictobject PyDictObject;
struct _dictobject {
    PyObject_HEAD
    Py_ssize_t ma_fill;  /* # Active + # Dummy */
    Py_ssize_t ma_used;  /* # Active */

    /* The table contains ma_mask + 1 slots, and that's a power of 2.
     * We store the mask instead of the size because the mask is more
     * frequently needed.
     */
    Py_ssize_t ma_mask;

    /* ma_table points to ma_smalltable for small tables, else to
     * additional malloc'ed memory.  ma_table is never NULL!  This rule
     * saves repeated runtime null-tests in the workhorse getitem and
     * setitem calls.
     */
    PyDictEntry *ma_table;
    PyDictEntry *(*ma_lookup)(PyDictObject *mp, PyObject *key, long hash);
    PyDictEntry ma_smalltable[PyDict_MINSIZE];
};

//create
PyObject *
PyDict_New(void)
{
    register PyDictObject *mp;
    if (dummy == NULL) { /* Auto-initialize dummy */
        dummy = PyString_FromString("<dummy key>");
        if (dummy == NULL)
            return NULL;
#ifdef SHOW_CONVERSION_COUNTS
        Py_AtExit(show_counts);
#endif
#ifdef SHOW_ALLOC_COUNT
        Py_AtExit(show_alloc);
#endif
#ifdef SHOW_TRACK_COUNT
        Py_AtExit(show_track);
#endif
    }
    if (numfree) {
        mp = free_list[--numfree];
        assert (mp != NULL);
        assert (Py_TYPE(mp) == &PyDict_Type);
        _Py_NewReference((PyObject *)mp);
        if (mp->ma_fill) {
            EMPTY_TO_MINSIZE(mp);
        } else {
            /* At least set ma_table and ma_mask; these are wrong
               if an empty but presized dict is added to freelist */
            INIT_NONZERO_DICT_SLOTS(mp);
        }
        assert (mp->ma_used == 0);
        assert (mp->ma_table == mp->ma_smalltable);
        assert (mp->ma_mask == PyDict_MINSIZE - 1);
#ifdef SHOW_ALLOC_COUNT
        count_reuse++;
#endif
    } else {
        mp = PyObject_GC_New(PyDictObject, &PyDict_Type);
        if (mp == NULL)
            return NULL;
        EMPTY_TO_MINSIZE(mp);
#ifdef SHOW_ALLOC_COUNT
        count_alloc++;
#endif
    }
    mp->ma_lookup = lookdict_string;
#ifdef SHOW_TRACK_COUNT
    count_untracked++;
#endif
#ifdef SHOW_CONVERSION_COUNTS
    ++created;
#endif
    return (PyObject *)mp;
}

//look
static PyDictEntry *
lookdict(PyDictObject *mp, PyObject *key, register long hash)
{
    register size_t i;
    register size_t perturb;
    register PyDictEntry *freeslot;
    register size_t mask = (size_t)mp->ma_mask;
    PyDictEntry *ep0 = mp->ma_table;
    register PyDictEntry *ep;
    register int cmp;
    PyObject *startkey;

    i = (size_t)hash & mask;
    ep = &ep0[i];
    if (ep->me_key == NULL || ep->me_key == key)
        return ep;

    if (ep->me_key == dummy)
        freeslot = ep;
    else {
        if (ep->me_hash == hash) {
            startkey = ep->me_key;
            Py_INCREF(startkey);
            cmp = PyObject_RichCompareBool(startkey, key, Py_EQ);
            Py_DECREF(startkey);
            if (cmp < 0)
                return NULL;
            if (ep0 == mp->ma_table && ep->me_key == startkey) {
                if (cmp > 0)
                    return ep;
            }
            else {
                /* The compare did major nasty stuff to the
                 * dict:  start over.
                 * XXX A clever adversary could prevent this
                 * XXX from terminating.
                 */
                return lookdict(mp, key, hash);
            }
        }
        freeslot = NULL;
    }

    /* In the loop, me_key == dummy is by far (factor of 100s) the
       least likely outcome, so test for that last. */
    for (perturb = hash; ; perturb >>= PERTURB_SHIFT) {
        i = (i << 2) + i + perturb + 1;
        ep = &ep0[i & mask];
        if (ep->me_key == NULL)
            return freeslot == NULL ? ep : freeslot;
        if (ep->me_key == key)
            return ep;
        if (ep->me_hash == hash && ep->me_key != dummy) {
            startkey = ep->me_key;
            Py_INCREF(startkey);
            cmp = PyObject_RichCompareBool(startkey, key, Py_EQ);
            Py_DECREF(startkey);
            if (cmp < 0)
                return NULL;
            if (ep0 == mp->ma_table && ep->me_key == startkey) {
                if (cmp > 0)
                    return ep;
            }
            else {
                /* The compare did major nasty stuff to the
                 * dict:  start over.
                 * XXX A clever adversary could prevent this
                 * XXX from terminating.
                 */
                return lookdict(mp, key, hash);
            }
        }
        else if (ep->me_key == dummy && freeslot == NULL)
            freeslot = ep;
    }
    assert(0);          /* NOT REACHED */
    return 0;
}

//set
int
PyDict_SetItem(register PyObject *op, PyObject *key, PyObject *value)
{
    register long hash;

    if (!PyDict_Check(op)) {
        PyErr_BadInternalCall();
        return -1;
    }
    assert(key);
    assert(value);
    if (PyString_CheckExact(key)) {
        hash = ((PyStringObject *)key)->ob_shash;
        if (hash == -1)
            hash = PyObject_Hash(key);
    }
    else {
        hash = PyObject_Hash(key);
        if (hash == -1)
            return -1;
    }
    return dict_set_item_by_hash_or_entry(op, key, hash, NULL, value);
}

static int
dict_set_item_by_hash_or_entry(register PyObject *op, PyObject *key,
                               long hash, PyDictEntry *ep, PyObject *value)
{
    register PyDictObject *mp;
    register Py_ssize_t n_used;

    mp = (PyDictObject *)op;
    assert(mp->ma_fill <= mp->ma_mask);  /* at least one empty slot */
    n_used = mp->ma_used;
    Py_INCREF(value);
    Py_INCREF(key);
    if (ep == NULL) {
        if (insertdict(mp, key, hash, value) != 0)
            return -1;
    }
    else {
        if (insertdict_by_entry(mp, key, hash, ep, value) != 0)
            return -1;
    }
    /* If we added a key, we can safely resize.  Otherwise just return!
     * If fill >= 2/3 size, adjust size.  Normally, this doubles or
     * quaduples the size, but it's also possible for the dict to shrink
     * (if ma_fill is much larger than ma_used, meaning a lot of dict
     * keys have been * deleted).
     *
     * Quadrupling the size improves average dictionary sparseness
     * (reducing collisions) at the cost of some memory and iteration
     * speed (which loops over every possible entry).  It also halves
     * the number of expensive resize operations in a growing dictionary.
     *
     * Very large dictionaries (over 50K items) use doubling instead.
     * This may help applications with severe memory constraints.
     */
    if (!(mp->ma_used > n_used && mp->ma_fill*3 >= (mp->ma_mask+1)*2))
        return 0;
    return dictresize(mp, (mp->ma_used > 50000 ? 2 : 4) * mp->ma_used);
}

static int
insertdict(register PyDictObject *mp, PyObject *key, long hash, PyObject *value)
{
    register PyDictEntry *ep;

    assert(mp->ma_lookup != NULL);
    ep = mp->ma_lookup(mp, key, hash);
    if (ep == NULL) {
        Py_DECREF(key);
        Py_DECREF(value);
        return -1;
    }
    return insertdict_by_entry(mp, key, hash, ep, value);
}

static int
insertdict_by_entry(register PyDictObject *mp, PyObject *key, long hash,
                    PyDictEntry *ep, PyObject *value)
{
    PyObject *old_value;

    MAINTAIN_TRACKING(mp, key, value);
    if (ep->me_value != NULL) {
        old_value = ep->me_value;
        ep->me_value = value;
        Py_DECREF(old_value); /* which **CAN** re-enter */
        Py_DECREF(key);
    }
    else {
        if (ep->me_key == NULL)
            mp->ma_fill++;
        else {
            assert(ep->me_key == dummy);
            Py_DECREF(dummy);
        }
        ep->me_key = key;
        ep->me_hash = (Py_ssize_t)hash;
        ep->me_value = value;
        mp->ma_used++;
    }
    return 0;
}

//dictresize
/*
Restructure the table by allocating a new table and reinserting all
items again.  When entries have been deleted, the new table may
actually be smaller than the old one.
*/
static int
dictresize(PyDictObject *mp, Py_ssize_t minused)
{
    Py_ssize_t newsize;
    PyDictEntry *oldtable, *newtable, *ep;
    Py_ssize_t i;
    int is_oldtable_malloced;
    PyDictEntry small_copy[PyDict_MINSIZE];

    assert(minused >= 0);

    /* Find the smallest table size > minused. */
    for (newsize = PyDict_MINSIZE;
         newsize <= minused && newsize > 0;
         newsize <<= 1)
        ;
    if (newsize <= 0) {
        PyErr_NoMemory();
        return -1;
    }

    /* Get space for a new table. */
    oldtable = mp->ma_table;
    assert(oldtable != NULL);
    is_oldtable_malloced = oldtable != mp->ma_smalltable;

    if (newsize == PyDict_MINSIZE) {
        /* A large table is shrinking, or we can't get any smaller. */
        newtable = mp->ma_smalltable;
        if (newtable == oldtable) {
            if (mp->ma_fill == mp->ma_used) {
                /* No dummies, so no point doing anything. */
                return 0;
            }
            /* We're not going to resize it, but rebuild the
               table anyway to purge old dummy entries.
               Subtle:  This is *necessary* if fill==size,
               as lookdict needs at least one virgin slot to
               terminate failing searches.  If fill < size, it's
               merely desirable, as dummies slow searches. */
            assert(mp->ma_fill > mp->ma_used);
            memcpy(small_copy, oldtable, sizeof(small_copy));
            oldtable = small_copy;
        }
    }
    else {
        newtable = PyMem_NEW(PyDictEntry, newsize);
        if (newtable == NULL) {
            PyErr_NoMemory();
            return -1;
        }
    }

    /* Make the dict empty, using the new table. */
    assert(newtable != oldtable);
    mp->ma_table = newtable;
    mp->ma_mask = newsize - 1;
    memset(newtable, 0, sizeof(PyDictEntry) * newsize);
    mp->ma_used = 0;
    i = mp->ma_fill;
    mp->ma_fill = 0;

    /* Copy the data over; this is refcount-neutral for active entries;
       dummy entries aren't copied over, of course */
    for (ep = oldtable; i > 0; ep++) {
        if (ep->me_value != NULL) {             /* active entry */
            --i;
            insertdict_clean(mp, ep->me_key, (long)ep->me_hash,
                             ep->me_value);
        }
        else if (ep->me_key != NULL) {          /* dummy entry */
            --i;
            assert(ep->me_key == dummy);
            Py_DECREF(ep->me_key);
        }
        /* else key == value == NULL:  nothing to do */
    }

    if (is_oldtable_malloced)
        PyMem_DEL(oldtable);
    return 0;
}

//del
int
PyDict_DelItem(PyObject *op, PyObject *key)
{
    register PyDictObject *mp;
    register long hash;
    register PyDictEntry *ep;
    PyObject *old_value, *old_key;

    if (!PyDict_Check(op)) {
        PyErr_BadInternalCall();
        return -1;
    }
    assert(key);
    if (!PyString_CheckExact(key) ||
        (hash = ((PyStringObject *) key)->ob_shash) == -1) {
        hash = PyObject_Hash(key);
        if (hash == -1)
            return -1;
    }
    mp = (PyDictObject *)op;
    ep = (mp->ma_lookup)(mp, key, hash);
    if (ep == NULL)
        return -1;
    if (ep->me_value == NULL) {
        set_key_error(key);
        return -1;
    }
    old_key = ep->me_key;
    Py_INCREF(dummy);
    ep->me_key = dummy;
    old_value = ep->me_value;
    ep->me_value = NULL;
    mp->ma_used--;
    Py_DECREF(old_value);
    Py_DECREF(old_key);
    return 0;
}

//dealloc
static void
dict_dealloc(register PyDictObject *mp)
{
    register PyDictEntry *ep;
    Py_ssize_t fill = mp->ma_fill;
    PyObject_GC_UnTrack(mp);
    Py_TRASHCAN_SAFE_BEGIN(mp)
    for (ep = mp->ma_table; fill > 0; ep++) {
        if (ep->me_key) {
            --fill;
            Py_DECREF(ep->me_key);
            Py_XDECREF(ep->me_value);
        }
    }
    if (mp->ma_table != mp->ma_smalltable)
        PyMem_DEL(mp->ma_table);
    if (numfree < PyDict_MAXFREELIST && Py_TYPE(mp) == &PyDict_Type)
        free_list[numfree++] = mp;
    else
        Py_TYPE(mp)->tp_free((PyObject *)mp);
    Py_TRASHCAN_SAFE_END(mp)
}


