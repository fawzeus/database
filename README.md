# Database

A simple C++17 SQLite-like database implementation with an interactive REPL (Read-Eval-Print Loop). The project is organized as **feature modules**, each owning `include/<module>/` (public headers), `src/` (implementation) and `test/` (unit tests). The code is currently a skeleton with the REPL, `.exit` metacommand and `Row`/`Value` model implemented; the rest of the modules are placeholders with TODOs.

## Features

- Interactive command-line interface
- C++17, compiled with `-Wall -g`
- Modular layout (`include/<module>/` + `src/` + `test/`), discovered automatically by the build
- Linux build via `make` or `./build.sh` (Windows `build.bat` is still available)

## Prerequisites

- Linux (or another POSIX shell)
- GCC/G++ with C++17 support (`g++ --version`)
- `make`

## Building

```bash
make                 # build all modules -> build/database
make check -s        # build and run every module's unit tests (quiet output)
make list-modules    # show discovered modules and file counts
./build.sh           # alternative one-shot build (same module discovery)
```

The **root `Makefile` is the single source of truth**: it discovers every `<module>/include`, `<module>/src` and `<module>/test` folder, adds the include roots to the compiler flags, and mirrors the layout under `build/` so object names never collide.

Each module also has a thin `Makefile` that only delegates to the root one, so you can work inside a single module while debugging:

```bash
cd sql
make          # build only sql objects + its test binaries
make check    # build and run only sql's tests, with live output
make clean    # remove only build/sql
```

No flags or build rules are duplicated in those module Makefiles.

### Include convention (layout B)

Public headers live in `<module>/include/<module>/` and are included with their **full logical path**:

```cpp
#include <common/utils.hpp>     // common/include/common/utils.hpp
#include <data/row.hpp>         // data/include/data/row.hpp
#include <sql/tokenizer.hpp>    // sql/include/sql/tokenizer.hpp
```

The doubled module name is intentional: the first is the physical module folder, the second is the public namespace inside it. That makes collisions impossible (`<data/utils.hpp>` and `<common/utils.hpp>` can coexist) and tells you at a glance which module owns a header. The Makefile passes `-I<module>/include` for every module, plus `-I<module>/src` so a module can still keep private headers included as `"header.hpp"`. Never use `../` in an include.

## Running

```bash
make run             # or: ./build/database
```

> Note: `app/src/main.cpp` currently has both the Row test and `repl::loop()`
> commented out, so the binary exits without output. Uncomment `repl::loop()`
> to use the REPL (type `.exit` to quit).

## Testing

Every module has a `test/` directory next to its `src/`. Each `*_test.cpp`
becomes its own binary, linked against all production sources except
`app/src/main.cpp`. Real tests currently cover `data`, `core`, `sql` and
`common`; the remaining modules have compile-safe placeholders with TODOs.

```bash
make check -s             # or: make test — builds and runs all unit tests
cd sql && make check -s   # only sql's tests, using sql/Makefile
```

Output is streamed **live**, module by module: a `Making check in <module>`
banner, a `building`/`running` line per test file, the test's own output, then
`[PASS]` (green) or `[FAIL]` (red) and a per-module `N test(s) passed, M failed`
count. The runner does not stop on the first failure — every module is checked,
then a final summary is printed and a non-zero exit code is returned if any
test failed.

## Cleaning

```bash
make clean           # removes the whole build/ directory
```

## Module map

```
.
├── app/       include/app/    (no public headers yet)
│             src/main.cpp     entry point
│             test/app_test.cpp  placeholder test
├── common/    include/common/utils.hpp     shared error codes (errorId_t)
│             test/utils_test.cpp           error-code uniqueness test
├── core/      include/core/repl.*, metacommand.*   REPL loop and .-commands
│             src/repl.cpp, metacommand.cpp
│             test/metacommand_test.cpp     real test
├── sql/       include/sql/sqlstatement.*   statement type + stub executor
│             include/sql/tokenizer.*       IN PROGRESS (Milestone 2)
│             include/sql/parser.*          PLACEHOLDER (Milestone 2-7)
│             src/sqlstatement.cpp, tokenizer.cpp, parser.cpp
│             test/sqlstatement_test.cpp    real test
├── catalog/   include/catalog/schema.hpp   PLACEHOLDER: ColumnDef, TableSchema
│             include/catalog/catalog.*     PLACEHOLDER: schema registry
│             src/catalog.cpp
│             test/catalog_test.cpp         skeleton test
├── data/      include/data/row.*           IMPLEMENTED: Value + Row
│             include/data/table.*          PLACEHOLDER: in-memory rows
│             src/row.cpp, table.cpp
│             test/row_test.cpp             full Row unit test
├── storage/   include/storage/pager.*      PLACEHOLDER (Milestone 5)
│             include/storage/serializer.*  PLACEHOLDER (Milestone 5)
│             include/storage/journal.*     PLACEHOLDER (Milestone 8)
│             src/pager.cpp, serializer.cpp, journal.cpp
│             test/storage_test.cpp         constants / TODO
├── index/     include/index/btree.*        PLACEHOLDER (Milestone 6)
│             include/index/cursor.*        PLACEHOLDER (Milestone 6)
│             src/btree.cpp, cursor.cpp
│             test/index_test.cpp           constants / TODO
├── engine/    include/engine/executor.*    PLACEHOLDER (Milestones 3-9)
│             src/executor.cpp
│             test/executor_test.cpp        placeholder test
├── build/                                  generated output (gitignored)
├── Makefile, build.sh                      build configuration
├── README.md                               this file
├── guide.md                                local: analysis + what to add next
├── plan.md                                 local: full 9-milestone roadmap
└── .vscode/                                 tasks.json + c_cpp_properties.json
```

| Module | Responsibility | Status |
|---|---|---|
| `app` | Program entry point | ✅ Minimal |
| `common` | Shared `errorId_t` codes | ✅ Done |
| `core` | REPL loop, `.exit` metacommand | ✅ Done |
| `sql` | Statement recognition; future tokenizer/parser | ⚠️ Stub + placeholders |
| `data` | `Value`/`Row` model; future `Table` | ✅ Row done, Table placeholder |
| `catalog` | Schema + table registry | 🧩 Placeholder |
| `storage` | Pages, serialization, journal | 🧩 Placeholder |
| `index` | B+ tree + cursor | 🧩 Placeholder |
| `engine` | Query execution | 🧩 Placeholder |

🧩 = files exist with headers/TODOs but no implementation yet.

## Local docs

`guide.md` and `plan.md` are gitignored — they are your personal working notes. `README.md` is the only documentation that gets committed.

## Development

- C++17, `-Wall` for warnings, `-g` for debug symbols.
- Public includes are rooted: `#include <common/utils.hpp>`, `#include <data/row.hpp>`. Private same-module headers still use `"header.hpp"`.
- Adding a module: create `<name>/include/<name>/`, `<name>/src/` **and `<name>/test/`**, then run `make list-modules` — no Makefile edits needed.
- Test files live in `<module>/test/*_test.cpp` and are auto-discovered by `make check`.
- `.vscode/c_cpp_properties.json` mirrors the Makefile include roots so IntelliSense resolves the same headers.
