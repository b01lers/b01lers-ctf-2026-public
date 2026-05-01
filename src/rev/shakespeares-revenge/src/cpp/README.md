# ShakespeareLang C++ (Flex/Bison)

This directory contains a C++ rewrite of the SPL interpreter using:

- Flex (`lexer.l`) for tokenization
- Bison (`parser.y`) for grammar parsing
- A C++ runtime/interpreter in `src/`

## Build

```bash
make
```

This produces `shakespeare_cpp`.

Style selection flags are disabled by default in release builds. To enable
`--input-style` and `--output-style` support, build with:

```bash
make SPL_ENABLE_IO_STYLES=1
```

## Run

```bash
./shakespeare_cpp run ../shakespearelang/tests/sample_plays/hello_world.spl
```

When built with `SPL_ENABLE_IO_STYLES=1`, supported options are:

- `--input-style=basic|interactive`
- `--output-style=basic|verbose|debug`

## Syscall Phrase

The C++ interpreter supports this SPL sentence:

- `Revere your player <character>.`

Behavior:

- Looks up `<character>`'s stack.
- Treats the top value as the syscall number.
- Pops the syscall number plus the correct number of arguments for that syscall.
- Arguments are popped in order from directly under the syscall number.
- Executes Linux `syscall(2)` and stores the return value in that character's current value.

## Reference Phrase

The C++ interpreter also supports:

- `Reference <character>.`

Behavior:

- Targets the spoken-to character (the usual second-person target).
- Sets that character's current value to a pointer to `<character>`'s stack storage.
- If `<character>` has an empty stack, the pointer value is `0`.
