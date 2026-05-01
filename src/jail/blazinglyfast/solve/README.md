# blazinglyfast

This challenge is based on a known soundness hole in the Rust compiler, see [https://github.com/rust-lang/rust/issues/25860](https://github.com/rust-lang/rust/issues/25860).

## Specifics

Players are provided with a single Rust file which is compiled into a standalone binary and executed. The code is split into two modules:

* **host** (trusted)
* **usercode** (jail)

The **host** module defines two opaque wrapper types:

* `In`
* `Out`

Both are `#[repr(C)]` wrappers around private structs (`SecretIn` / `SecretOut`) that have the same layout, but cannot be accessed from the jail.

Internally, the secret struct contains:

* A function pointer (`gate`) which points to the win function
* A fixed cookie (`u64`)
* Several **derived values** (`folded`, `pair`) computed from the cookie via mixing functions

The **usercode** module exposes:

```rust
pub fn jail(input: In) -> Out
```

### Restrictions

The sandbox enforces:

* No field access or construction of `In` / `Out`
* `#![forbid(unsafe_code)]`
* Macros blocked via regex filtering
* No traits / impls / attributes (depending on filter)

## Validation

The host

1. Reinterprets `Out` back into its internal representation
2. Recomputes expected values from the cookie using mixing functions
3. Verifies all fields match exactly:

   * function pointer identity
   * cookie
   * multiple derived values (`folded`, `pair`)

The mixing functions look like:

```rust
fn mix_a(x: u64) -> u64 { ... }
fn mix_b(x: u64) -> u64 { ... }
fn mix_c(x: u64) -> u64 { ... }
```

These create dependent values stored inside the original `In`.

### Important Insight

The mixing functions do not change anything. They are added so partially forging the type without transmute, which I'm not sure is possible to begin with, is much harder. Walking through the mixing functions and how the result it checked shows that all a player needs to do is preserve every bit and just transmute the type.

## Intended Solution

Since:

* `In` and `Out` have identical memory layout
* Their fields are private and inaccessible
* `unsafe` is forbidden

The only viable path is a safe transmute.

This is achieved using the technique demonstrated in [https://github.com/Speykious/cve-rs](https://github.com/Speykious/cve-rs), which exploits:

* lifetime expansion via higher-ranked function pointers
* enum layout reuse to reinterpret memory

### Core Idea

1. Create an enum holding either `Box<In>` or `Box<Out>`
2. Take a mutable reference to the `Out` slot
3. Illegally extend its lifetime
4. Overwrite the enum with `Box<In>`
5. Read it back as `Box<Out>`

This results in a conversion of `In → Out`.

Because the transformation preserves bytes exactly:

* function pointer is unchanged
* cookie is unchanged
* all mixed values remain valid

And you get the flag. Yayyy!

