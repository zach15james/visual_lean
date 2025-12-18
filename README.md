1. Kernel + TUI

* [C++23] core checker + normalizer (small, trusted)
* [TUI] inspect goals / terms / reductions (fast feedback)
* basic dependent type theory (enough to grow)

1b) Natural Numbers Game (demo)

* Nat + induction / recursion (small level set)
* use it as the first real end-to-end example

2. Visual Lean (local WASM workspace)

* [WASM/Emscripten] run locally in the browser
* drag/drop + connect objects (graph-like workspace)
* multiple views of the same thing: code/terms <-> blocks <-> visual

3. Local wiki database -> later server

* local library store (definitions/theorems + metadata)
* searchable + linkable + shareable later for collaboration/research

3.X) Later

* interop with other provers/tools/models (adapters, not in the kernel)

Principles

* fast + portable (not a giant toolchain)
* minimal core, better UX on top
