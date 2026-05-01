This program is a classic implementation of the min heap data structure, also known as a priority queue. Elements are strings which are sorted lexicographically. The only out-of-place code is the `edit()` function, which reads 32 bytes into the element at the top of the heap:

```c
void edit(void) {
    if (size == 0) {
        puts("Queue is empty!");
        return;
    }

    puts("Message: ");
    read(fileno(stdin), array[0], 32);

    move_down(0);
}
```

This is odd since the number 32 doesn't appear anywhere else in the code. The size of the allocations are instead based on the size of the input strings:

```c
void insert(void) {
    puts("Message: ");
    char buffer[128] = { 0 };
    scanf("%127s", buffer);

    char *chunk = malloc(strlen(buffer) + 1);
    strcpy(chunk, buffer);

    // ...
}
```

A very short input message will allocate a chunk of less than 32 bytes, so this is an overflow, albeit not a very large one. To recap, the data in a `malloc()` chunk looks something like this:

```c
struct chunk {
	unsigned long prev_size;
	unsigned long size;
	// Pointer returned by malloc() is here
	struct tcache_entry *next;
	struct tcache_perthread_struct *key;
};
```

The `next` and `key` fields are used only when the chunk is freed and in the tcache. This is libc 2.31, the version that introduced tcache, so safe linking is not enabled. (But if it were, the challenge would still be solvable in almost the same way.)

It turns out that by sending in messages that are a single character long (the shortest available), chunks are created at every 32 bytes. A 32-byte read lets us overwrite the `prev_size` and `size` fields of the next chunk, but not the `next` field, which would be the traditional way to exploit tcache.

There is still something we can do, though. If we overwrite the `size` fields with a larger size and then free the chunk, `malloc()` will think the chunk is actually that size; when we request a chunk of the larger size, `malloc()` will return that chunk. So, we can allocate three chunks in a row, use an overwrite on the first to increase the size of the second, free and then reallocate the second, and now the second and third chunks are overlapping.

We can send our larger payload when reallocating the chunk so it overwrites the third chunk, and not just by a little bit this time. Now we can overwrite that `next` field and do a tcache poisoning attack to get an arbitrary read and leak the flag.

This is a well-known technique, so I won't fully explain how to do it here. The short explanation is that tcache stores freed chunks in a singly linked list, so by overwriting the linked list pointer we can add an arbitrary address to the list. Then we can call `malloc()` two more times and it will get returned. I recommend reading more about this if you aren't familiar with heap exploitation, as it's pretty fun.

That is the general order of `malloc()`, `free()`, read, and write calls to win. In some heap challenges we would already be done, since they let you do any of these on demand. This usually takes the form of an infinite loop that asks for an operation and then the arguments. This challenge is technically the same, but with limitations. You can only write into a chunk when first allocating it (except for the first 32 bytes), and you do not directly control the order chunks are freed. In practice, this can be worked around by carefully choosing the first bytes of each input message. That is what the `order` array in the solve script is for.

The only other concern is getting a heap leak. Luckily, you do not need any other leaks to solve the challenge, since the flag is already on the heap. This can be done by allocating two chunks 32 bytes apart, freeing the second, then writing 32 bytes of a printable character into the first and then printing it. This fills in the bytes right up until the `next` field (which points to the heap), so `puts()` on the first chunk prints it.
