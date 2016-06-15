## Allocator Library
Inspirace
CppCon 2015 - Andrei Alexandrescu - "std::allocator..." (https://youtu.be/LIb3L4vKZ7U)

#### Abstract

Jedno z hlavních poselství výše uvedené přednášky je, že současná implementace std::allocatoru je nedostatečná.
Ideálně by měla existovat základní sada alokátorů, které je možno do sebe skládat za účelem sestavení alokátoru dle specifických potřeb.
Naše sada alokátorů bude shodná se sadou uvedenou v přednášce (v přednášce se o nich začíná mluvit 25:37).

Hlavní náplní projektu bude implementovat tyto alokátory: fallback allocator, stack allocator, freelist, prefix/suffix allocator, allocator pro statistiky, allocator pro bitmapped block, segregator.

V rámci projektu provedeme také benchmark implementovaných alokátorů mezi sebou a benchmark oproti std::allocator<T> (či jiným běžně používaným) pro různě velké paměťové bloky.
Jakožto vedlejší, nepovinný, cíl by bylo zajímavé provést benchmark tradičního pool allocatoru (se zabudovaným freelistem) oproti bitmapped_block allocatoru.

#### How to compile

```
git submodule update --init --recursive
mkdir build
cd build
ccmake ..
make
```
