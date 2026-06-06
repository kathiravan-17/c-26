# C++ Quick Revision — Senior Engineer Notes
> `#pragma once` · Header Guards · lvalue · rvalue · `std::move`

---

## 1. Header Guard vs `#pragma once`

### The Problem They Solve
Same header included twice = redefinition errors. Both prevent this.

### Header Guard
```cpp
#ifndef CONFIG_H   // if not defined
#define CONFIG_H   // define it

void init();

#endif             // end
```

### `#pragma once`
```cpp
#pragma once       // one line, done

void init();
```

### Key Difference
| | Header Guard | `#pragma once` |
|---|---|---|
| Standard | ✅ ISO C++ | ❌ Compiler extension |
| Lines | 3 lines | 1 line |
| Symlink safe | ✅ Yes | ❌ No |
| Name collision | ❌ Possible | ✅ Impossible |

### When to Use
```
#pragma once  → modern apps, MSVC/GCC/Clang
Header Guard  → embedded systems, public libraries, symlink heavy codebases
```

### Symlink Trap
```
/project/include/config.h   ← real file
/project/src/config.h       ← symlink (same file, different path)

#pragma once  → sees 2 paths = includes twice = 💥 error
Header Guard  → macro already defined = skips = ✅ safe
```

---

## 2. lvalue vs rvalue

### One Question Rule
> "Can I take the address with `&` ?"

```cpp
int x = 42;
&x;    // ✅ YES → x is lvalue
&42;   // ❌ NO  → 42 is rvalue
```

### lvalue — Has Name, Has Home
```cpp
int x = 42;           // x = lvalue
std::string s = "Hi"; // s = lvalue

// has name     ✅
// has address  ✅ (&x works)
// lives until scope { } ends ✅
```

### rvalue — No Name, No Home
```cpp
42          // rvalue — just a number
x + y       // rvalue — temporary result
"Hello"     // rvalue — temporary
getName()   // rvalue — function return
  
// no name     ❌
// no address  ❌ (&42 fails)
// dies immediately ❌
```

### Quick Test
```cpp
int x = 10;

x      // ✅ lvalue — has name
10     // ❌ rvalue — temporary
x + 1  // ❌ rvalue — temporary result
++x    // ✅ lvalue — returns x itself
x++    // ❌ rvalue — returns temp copy of old x
```

### Why `++x` is lvalue and `x++` is rvalue
```
++x → increment x → return x ITSELF       → lvalue ✅ (real variable)
x++ → copy x → increment x → return COPY  → rvalue ❌ (copy dies)
```

---

## 3. lvalue Reference `&` vs rvalue Reference `&&`

### lvalue Reference `&`
```cpp
std::string name = "Hello";

std::string& ref = name;      // ✅ binds to lvalue (named variable)
// std::string& ref = "Hi";   // ❌ ERROR — rvalue, no address

// ref IS name — same memory address
ref = "Modified";
std::cout << name;            // "Modified" — they are same!
```

### rvalue Reference `&&`
```cpp
std::string&& rref = "Hello"; // ✅ binds to rvalue (temporary)

std::string name = "Hello";
// std::string&& r = name;    // ❌ ERROR — name is lvalue
std::string&& r = std::move(name); // ✅ std::move converts lvalue → rvalue
```

### Key Difference
| | `T&` lvalue ref | `T&&` rvalue ref |
|---|---|---|
| Binds to | Named variables | Temporaries |
| Purpose | Alias, avoid copy | Enable move / steal |
| Example | `string& r = name` | `string&& r = std::move(name)` |

---

## 4. `std::move`

### The Core Problem
```cpp
std::string hello = "Hello World";

// hello = lvalue
// Move constructor = only accepts rvalue (&&)
// lvalue passed → copy constructor runs → expensive 😢
```

### What `std::move` Does
```cpp
// std::move is JUST a cast — nothing more
std::move(hello)
// same as:
static_cast<std::string&&>(hello)

// converts lvalue → rvalue reference
// now move constructor gets triggered → cheap 😎
```

> `std::move` does NOT move anything.
> It just lies to the compiler — "treat this lvalue as rvalue"

### Memory — Copy vs Move
```
string hello = "Hello World"

Stack              Heap
────────           ──────────────────────
hello:
  ptr  ──────────► [H e l l o  W o r l d]
  size = 11

──────────────────────────────────────────
AFTER COPY:  string copy = hello

hello:
  ptr  ──────────► [H e l l o  W o r l d]  ← original
copy:
  ptr  ──────────► [H e l l o  W o r l d]  ← NEW copy (expensive!)
  
Cost = O(n) — copies every byte

──────────────────────────────────────────
AFTER MOVE:  string moved = std::move(hello)

hello:
  ptr  = nullptr   [H e l l o  W o r l d]
  size = 0                ▲
moved:                    │ same buffer — no copy!
  ptr  ─────────────────┘
  size = 11

Cost = O(1) — just copies the pointer (8 bytes)
```

### Move Constructor Internally
```cpp
string(string&& other) noexcept {
    ptr      = other.ptr;      // steal pointer
    size     = other.size;     // steal size
    capacity = other.capacity; // steal capacity

    other.ptr      = nullptr;  // leave source empty but valid
    other.size     = 0;
    other.capacity = 0;
}
```

### After `std::move` — Source is Valid but Empty
```cpp
std::string hello = "Hello World";
std::string moved = std::move(hello);

std::cout << hello;        // ""  — empty but valid ✅
hello = "New value";       // ✅  — can reuse
```

### Copy vs Move Summary
| | Copy | Move |
|---|---|---|
| Heap allocation | ✅ Yes | ❌ No |
| Data copied | Every byte | Just pointer (8 bytes) |
| Original after | Full | Empty |
| Cost | O(n) | O(1) |
| 1GB string | Copies 1GB 😱 | Copies 8 bytes 😎 |

---

## 5. Print lvalue & rvalue Reference

```cpp
#include <iostream>
#include <string>

int main() {
    std::string lval     = "hello";
    std::string& l       = lval;           // lvalue reference
    std::string&& rvalue = "world";        // rvalue reference

    // cout
    std::cout << "lvalue ref : " << l      << "\n";
    std::cout << "rvalue ref : " << rvalue << "\n";

    // printf — needs .c_str()
    printf("lvalue ref : %s\n", l.c_str());
    printf("rvalue ref : %s\n", rvalue.c_str());

    // std::println C++23 — use {} not %s
    std::println("lvalue ref : {}", l);
    std::println("rvalue ref : {}", rvalue);

    return 0;
}
```

---

## Mental Models — Never Forget

```
lvalue  = person with a house      (name + address + permanent)
rvalue  = ghost                    (no name, no address, disappears)

T&      = handing over house key   (alias to real thing)
T&&     = catching ghost before it vanishes

std::move = giving homeowner a ghost costume
            move constructor sees ghost → steals the house
            new owner lives there permanently
```

```
Copy = photocopying a document  → 2 copies, double cost
Move = handing over original    → 1 copy, zero extra cost
```

```
++x = go to house, renovate, hand over HOUSE KEY  → lvalue
x++ = go to house, take PHOTO, renovate, hand over PHOTO → rvalue
```

---

## One Line Summaries

```
#pragma once    → 1 line, modern, breaks with symlinks
Header Guard    → 3 lines, portable, safe everywhere

lvalue          → has name, address, lives in scope
rvalue          → no name, no address, dies immediately

T&              → alias to named variable (lvalue)
T&&             → grabs temporary (rvalue)

std::move       → lies to compiler, converts lvalue → rvalue
                  triggers move constructor → steal instead of copy
```
