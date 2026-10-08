# AGENTS.md

## Purpose
This repository is a learning workspace made up of several educational projects and assignments, mainly from CMU CS:APP and the Operating Systems: Three Easy Pieces (OSTEP) materials. The code is intentionally task-oriented and not a single production application.

## Primary references
- [README.md](README.md)
- [CMU-CSAPP-master/README.md](CMU-CSAPP-master/README.md)
- [ostep-code-master/README.md](ostep-code-master/README.md)
- [ostep-homework-master/README.md](ostep-homework-master/README.md)
- [code/codeReadme.md](code/codeReadme.md)

## Repository structure
- [CMU-CSAPP-master](CMU-CSAPP-master): CS:APP lab materials, C, assembly, pointer, system call, and memory examples.
- [ostep-code-master](ostep-code-master): reference code for OSTEP chapters and exercises.
- [ostep-homework-master](ostep-homework-master): homework folders organized by topic such as threads, scheduling, filesystems, and VM.
- [code](code): local practice code and supplemental examples.
- [red_green](red_green): notes and personal documentation.

## Working conventions
- Always check the nearest README or assignment instructions before changing code.
- Keep patches narrow and scoped to the exact lab, exercise, or example being worked on.
- Prefer minimal, idiomatic C changes that match the surrounding style and constraints of the source file.
- Do not introduce framework, dependency, or build-system changes unless the task explicitly requires them.
- Validate with the smallest relevant command for that subproject, such as a local `make`, `gcc`, or a focused test harness already provided by the assignment.
- Avoid broad repo-wide refactors; this workspace contains many independent educational exercises.
- Preserve the original learning intent of the code; do not “optimize” away required behavior or assignment constraints.

## OpenAI-compatible integration guidance
When a task involves an LLM or OpenAI-compatible API integration, prefer portable, provider-agnostic behavior:
- keep the base URL and API key configurable
- prefer standard OpenAI-style request shapes, especially `POST /chat/completions`
- keep model names, endpoint paths, and response parsing configurable rather than hard-coded
- avoid assuming vendor-specific features unless the task explicitly requires them

## Good agent behavior
- Start by locating the exact assignment folder and its README before editing.
- Prefer reading just the relevant files, then patching the minimal code needed.
- If build output is unclear, inspect the local Makefile or compile command in the target directory before making assumptions.
- Keep explanations brief and grounded in the actual project structure of this workspace.
