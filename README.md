The goals here to build a HoTT dependent type theory proof checker system with a 'visual lean' tool. The the long-term plans drei-fold:
1) HoTT kernel core (w/ TUI), implementing basic dependent type theory with a Natural Numbers game example
2) Visual lean (local envrionment in WASM, dragging and dropping, connecting)
3) Generate a wiki structure for collaboration and ultimately to house future research
3.X) Connect with other platforms, feature creep, &cd


1. HoTT kernel core + TUI

* [C++23, header-only core via INTERFACE lib; templates where they buy zero-cost abstractions]
* [tiny trusted kernel: term AST + typecheck + definitional equality + normalization (β/ι/δ); no IO/network/plugins in kernel]
* [core IR: canonical + versioned serialization of checked terms (e.g. CBOR/protobuf); content-hash per def/theorem]
* [incremental: dependency DAG + hash cache; edit -> recompute minimal affected closure]
* [holes/metavariables: partial proofs compile; goal list derived from elaboration constraints]
* [TUI: goals/context viewer, normalization preview, “why failed” trace (expected/got diff + blame slice)]
* [determinism: lockfile for deps + reproducible build manifest (kernel version + hashes)]

1b) Natural Numbers Game (demo/teaching track)

* [Nat core: 0/succ + recursor + induction; curated lemma set as “levels”]
* [step trace: replayable proof states; time-travel debugging for failures]
* [structural search: goal-shape fingerprinting (binder structure + head symbols) to suggest lemmas]
* [export: save solutions as checked artifacts + readable script/trace for sharing]
* [perf target: sub-500ms edit->feedback on typical levels]

2. Visual Lean (local browser workspace)

* [WASM via Emscripten; same kernel compiled to wasm; local-first/offline by default]
* [canvas = semantic graph: nodes = defs/theorems/goals; edges = uses/depends/rewrite/instance]
* [drag/drop blocks: definition, theorem, lemma, example, hole, import, scoped “assume”]
* [multi-representation per object: core term/IR <-> pretty view <-> block/tactic view <-> graph view]
* [instant recheck: artifact cache in IndexedDB/local FS; incremental updates on graph edits]
* [fast suggestions: structural suggestions first (lemmas/rewrites/tactics); optional model-ranking later behind same protocol]

3. Local wiki DB -> server collaboration

* [local wiki store: content-addressed objects (hash -> checked term + metadata + deps + docs)]
* [publish bundle: source + checked artifacts + lockfile + build manifest; server re-check CI before accept]
* [semantic diffs: compare by object hash + dependency impact (what breaks / what changed)]
* [graph-backed discovery: prereqs/used-by/related via dependency graph + structural similarity index]
* [collab: merge conflicts detected by meaning (same name, different hash); review tools show breakage clusters]

3.X) Interop + expansion (later)

* [stable seams: versioned core IR + JSON protocol for editor/agents + certificate-first external tools]
* [adapters: export/import statements/defs; external solvers provide traces/certs; kernel stays minimal]
* [AI/tools are optional clients: suggestions via protocol; never trusted for soundness]

Principles

* [speed/latency as a feature: C++ core, cached checks, minimal recomputation]
* [small trusted base: kernel boring + stable; everything else evolves fast]
* [portable + reproducible: local-first, pinned deps, deterministic artifacts]
* [UX > bloat: same math object viewable as code/terms/blocks/graph]
