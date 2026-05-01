# Spelling Bee write up

This challenge involves popping a shell from within a forth interpreter

# forth info

The basic idea of the forth programming language is that all data is represented
by a stack. All operations/functions (called words) perform a set of operations that
manipulate this stack. 

Some basic words like `pop` (pop off stack), `dup` (duplicate top entry), and `+` (replace top two entries with their sum) are provided by the interpreter

Users can define their own words using other words by invoking `: [name]` and inputting a sequence of words to be called when `name` is invoked. The `;` word is used to end the definition of a new word

Example:

```
: another-add + ;
```

Defines a word `another-add` equivalent to the add primitive

# solve


Words are represented internally as `word_t`, these contain two useful fields for exploitation.

The first field is the `code` field, which contains a pointer to a function that takes 
a `(void*)` as its only parameter. This function is used to interpret the word

The second field is the `param` field, which contains arbitrary data used to assist `code`

When a word is ran, the interpreter calls `code(param)`.

Primitive words are implemented with c functions like `_add` or `_pop` in their `code` fields

User defined words are implemented with the function `docol`, which takes a list
of `word_t *` in their `param` and sets up the return stack to call each word in this list

We are given the address of a function called `dosys`, which is a wrapper around `system`.
The idea behind this challenge is to corrupt a word so that its `code` field points to `dosys`, and
its param field points a string like "/bin/sh"

In order to do this, we have to utilize a use after free vulnerability caused by the primitive word `forget`

During initialization, a dictionary containing definitions for basic words is created. 

The dictionary `dict` is a linked list of dictionary entries of type `dict_t`
When a word is looked up, the interpreter traverses this list for an `dict_t` containing a matching `name` field and
return that entry's `word` field

## allocating words

When a word is compiled, the following sequence of events is triggered (line 406):

* The interpreter allocates a buffer for the name of the word and stores the name in it
* The interpreter allocates a buffer to store the sequence of `word_t *` words associated with this word
    -  This will be `(16 * sizeof(word_t *))=128` bytes initially, and gets sized up as necessary
* As words are entered and looked up, the interpreter appends their locations to the word buffer
* Once interpreting is done, a final END word is added to the word buffer
* The interpreter allocates a `word_t`, and sets its `code` field to `docol` and `param` field to the word buffer
* `add_word` is called, which allocates a `dict_t *` containing the word name and information, and appends it to the dictionary

In total, there are 4 allocations in the following order

* 2-128 byte name
* 40 byte `word_t` 
* 128 (usually) byte word buffer
* 32 byte `dict_t`

## deallocating words

When a word is freed using `forget`, the following sequence of events is triggered (`delete_word`)

* If the word had `param` allocated, deallocate `param` (based on `WF_MALLOC_PARAM`)
* Free the word
* If the dictionary entry's name was allocated, free it
* Remove the dictionary entry from the dictionary
* Free the dictionary entry

Most importantly, when a word is forgotten it is still available to other words.
If we define a word `A` that uses a word `B`, and then forget `B`, `A` will still
interpret the memory at `B` when it gets executed.

We can deallocate `B` and then modify `B` to contain the `dosys` pointer before calling `A`
to pop a shell

```forth
: victim ;
: win victimAAAAAAAAAAAAAAAAAA ;
forget victimAAAAAAAAAAAAAAAAAA
```

When we run this sequence of events, our tcache will look like this

```
Bin of size 0x30 -> string "victimAAAAAAAAAAAAAAAAAA" -> old dict entry -> old word
Bin of size 0x90 -> old word param / word list (128 bytes)

```

What we want to do now is get an allocation for a word name that points to the old word's location,
so we can overwrite the code field with the leaked pointer. We also want to get an allocation for a word name that points to the old world param so we can set it to a string like "/bin/sh"

Do this by defining a new word named "/bin/sh/;#...", padded to be 127 bytes

When the new word is defined, the chunk in the 0x90 bin is assigned to the name, and the first
two chunks in the 0x30 bin allocated, leaving the victim word remaining

Define another word named "`[padding][leaked dosys addr]`" where padding is 24 bytes long to overwrite the `flags`, `length`, and `referenced_by` fields of the word so that `code` becomes dosys.

The name buffer will point to the victim word, and succesfully overwrite it.

With this done, calling `win` will trigger `do_sys("/bin/sh;#...")` and pop a shell
