# Summary

Luna is both a hardware and software project. It's a custom ISA, processing core, and systems programming language.
The programming language luna is not suitable for production use and is a toy language for this project.

NOTE: This is an almost exclusively hand-written project done very informally so the repo is a bit messy.

# Example Luna Code

## Hello World

```luna
const puts = fn@extern["puts"](str: *i8)
const main = fn() {
  puts("Hello world!")
}
```

## Structs

```luna
const pair = struct {
  a: i32,
  b: i32
}

const main = fn() {
  let p: pair = .pair {
    a: 1, b: 2
  }

  cstd::printf("pair: %d, %d\n", p.a, p.b)
}
```

## Submodules

```luna
const cstd = mod {
  const puts = fn@extern["puts"](str: *i8): i32
  const printf = fn@extern["printf"]@variadic(fmt: *i8): i32

  const nesting_example = mod {
    const magic: i32 = 100
  }
}

const main = fn() {
  cstd::puts("Hello world!")
  cstd::printf("Some value: %d\n", cstd::deep::stuff)
  cstd::printf("%d\n",abc + cstd::nesting_example::magic)
}
```

## More

More examples of luna code can be found in the `/examples` directory in this repo.
