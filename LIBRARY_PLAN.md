# Turning this repo into a library

Plan only, nothing here is implemented yet. Goal: a year's repo contains just its solutions, and
this repo is consumed as a dependency instead of cloned/copied. The `aoc` script stops being
required: you create day folders yourself and the library handles the rest.

## Status

Done: sections 1 (per-challenge `AOC_ROOT_DIR`/`AOC_CHALLENGES_DIR`, set by `aoc_add_challenges`),
2 (headers under `Utils/AoCUtils/`) and 3 (`cmake/AoCUtils.cmake`, root `CMakeLists.txt` now just
calls `aoc_add_challenges(Challenges)`). Checked with a separate scratch project using a
different challenges folder. The sample project lives in `Example/` (its own `CMakeLists.txt`, pulling in the library with
`FetchContent` from `..`, plus an `aoc` stub that sets `AOC_PROJECT_DIR`), so the repo root is the
library only. Not done: sections 4-7 (scaffolding, config keywords, stub, docs/tag).

## Target shape

The library repo keeps `Utils/`, the template, and a CMake module. A year's repo is:

```
MyYear/
  CMakeLists.txt
  Challenges/
    Day01/Day01.cpp, input.txt
    ...
```

```cmake
cmake_minimum_required(VERSION 3.19)
project(AoC2026 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(AoCUtils GIT_REPOSITORY <this repo> GIT_TAG v1.0.0)
FetchContent_MakeAvailable(AoCUtils)

aoc_add_challenges(Challenges)   # optional keyword args, see "Configuration"
```

Pinning `GIT_TAG` keeps old years building against the version they were written for. Updating a
year is bumping the tag.

## Changes required

### 1. Remove the repo-root assumption (the real blocker)
- `AOC_ROOT_DIR` is currently a `PRIVATE` compile definition on `AoCUtilsLib`, pointing at this
  repo's root. In library form that is the library's checkout, not the user's project.
- Move it to each challenge target (set by `aoc_add_challenges`, pointing at the consumer's
  project root), or drop it and pass the challenge directory and input path explicitly.
- `Challenge.cpp` uses it for `rootDir`, `dir` and the input path, and `getFileInput` builds
  `AOC_ROOT_DIR/Challenges/<day>/input.txt`. All of these must use the per-challenge value.
- The `Challenges` folder name is hardcoded in both places: make it configurable.

### 2. Fix the include path
- Template currently does `#include "../Utils/AoCUtils.h"`, which only works inside this repo.
- Move headers under `Utils/AoCUtils/` (or `include/AoCUtils/`) and use
  `#include <AoCUtils/AoCUtils.h>`. The include directory comes from the `AoCUtilsLib` target,
  which already exposes it as `PUBLIC`.

### 3. Export the CMake logic
- Move `add_challenge`, the `Challenges/*` glob loop, `require_matplot`, and the Google
  Benchmark `FetchContent` out of the root `CMakeLists.txt` into a module
  (`cmake/AoCUtilsConfig.cmake` or similar) loaded when the library is added.
- Expose `aoc_add_challenges(<dir>)`. Consumers get Matplot++ (on-demand, as now) and Benchmark
  without duplicating any of it.
- Keep `CMAKE_BUILD_TYPE` defaulting to `Release`, but set it in the consumer's project (a
  library shouldn't force the build type of whatever includes it). Provide it as a documented
  line, or do it in the module only when the library is the top-level project.
- Use `target_include_directories`/`PUBLIC` links properly so `benchmark::benchmark` propagates.
- `CONFIGURE_DEPENDS` on the glob keeps new days picked up automatically.

### 4. Replace what `aoc` did

| `aoc` job | Replacement |
|---|---|
| `aoc -n 5` (scaffold) | If `Challenges/Day05/` exists with no `Day05.cpp`, generate it from the template at configure time. Alternative with no surprise file writes: an explicit `aoc_new_day(5)` CMake function/target. |
| `aoc 5` (build + run) | `cmake --build build --target Day05 && ./build/Day05`. Optionally generate `run_Day05` custom targets. In CLion each day is already a run target. |
| `aoc 5 --bench` / `--part` / `-v` | Unchanged: they are arguments to the binary (`./build/Day05 --bench`). |
| Day-number sugar (`5` -> `Day05`) | Optional: keep a thin script as a convenience, not a requirement. |

The template needs shipping with the library, and the scaffolding step needs to know where it is
(`${AoCUtils_SOURCE_DIR}/Challenge_Template`).

### 5. Configuration
Convention over configuration: defaults work with no config file. Overrides are keyword
arguments to `aoc_add_challenges`, since CMake cannot parse TOML and `string(JSON)` needs 3.19+:

```cmake
aoc_add_challenges(Challenges
    INPUT_FILE input.txt        # default
    TEMPLATE   my_template/     # default: the library's
    AUTO_SCAFFOLD ON            # default: generate missing <name>.cpp
)
```

The C++ side never reads a config file: the values reach it as compile definitions.

### 6. If the script stays (optional)
Run from the consumer's project, it must not `cd` to its own directory or assume `Challenges/`
and `Challenge_Template/` sit beside it. Suggested pattern: a ~10 line stub in each project that
finds the real script in the pinned checkout (`build/_deps/aoctils-src/aoc`), bootstrapping with
`cmake -B build` if the library has not been fetched yet, then `exec`s it with `--project-root`.
This keeps script, template and library on one version (the `GIT_TAG`).

### 7. Docs and versioning
- Rewrite the README around "add to your project" rather than "clone this repo".
- Tag releases (`v1.0.0`...). Template changes only affect days created after a tag bump,
  existing `DayXX.cpp` files keep what they were created with.
- First configure needs network access (FetchContent); later ones do not.

## Suggested order
1. Per-challenge root/dir definitions (section 1), so nothing depends on the repo location.
2. Header move and include path (section 2).
3. Extract the CMake module and `aoc_add_challenges` (section 3).
4. Auto-scaffold or `aoc_new_day`, and `run_<day>` targets (section 4).
5. Decide whether to keep a thin `aoc` stub (section 6).
6. Docs, then a first tag.

## Open questions
- Auto-generate missing day files at configure time, or require an explicit command?
- Keep a convenience script at all, or go CMake/IDE-only?
- Header-only/static library as now, or also support `find_package` for an installed copy?
