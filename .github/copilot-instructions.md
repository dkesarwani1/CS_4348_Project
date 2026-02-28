<!-- .github/copilot-instructions.md -->
# Copilot / AI agent instructions — CS_4348_Project

This project is a small single-binary C++ logger CLI. The guidance below focuses on the concrete, discoverable patterns an AI coding agent needs to be productive here.

1) Big picture
- **Type**: Single-purpose CLI program implemented in a single source file: `logger.c++`.
- **Purpose**: Read messages from standard input and append timestamped log entries to a file whose name is supplied as a single command-line argument (see `devlog.md`).
- **Data flow**: stdin -> parse lines -> format as `YYYY-MM-DD HH:MM [ACTION] MESSAGE` -> append to log file. The program exits when it sees the literal line `[QUIT]`.

2) Key files to open first
- `devlog.md` — developer notes that describe runtime behavior and the required output format.
- `logger.c++` — the implementation target (note: file currently exists but is empty in the repo; edits should implement the behavior described in `devlog.md`).
- `README.md` — minimal; no build instructions present.

3) Build & run (explicit commands)
- Compile on macOS / Linux using a recent g++/clang++:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o logger logger.c++
```

- Run example (create/append `mylog.txt`):

```bash
./logger mylog.txt
Type messages on stdin; program stops when you type [QUIT]
```

4) Expected runtime behavior and format (from `devlog.md`)
- Each written line must be: `YYYY-MM-DD HH:MM [ACTION] MESSAGE` (note minute precision).
- The program does not present a menu, perform encryption, or interactively validate inputs — it only reads stdin and writes formatted lines to the provided file.
- Termination condition: a line that equals `[QUIT]` (case-sensitive, exact match).

5) Project-specific conventions & gotchas
- The source file uses the extension `.c++` (not `.cpp`). Tools and CI may not expect that. Use explicit filenames for compile commands.
- No test harness or build system is present — add small manual tests or a tiny test script when adding functionality.
- Keep behavior strictly aligned with `devlog.md` (format and stop-condition are authoritative).

6) Helpful edits and examples an AI might produce
- Implement `logger.c++` that:
  - Parses `argv[1]` as the log filename (error if missing),
  - Reads stdin line-by-line, timestamps using local time, formats entries exactly, and appends to the log file, flushes per-line, and exits on `[QUIT]`.
- Add a small `tests/manual_test.sh` script (optional) that compiles the binary and runs it with sample stdin to verify output lines.

7) When to ask the human / triage questions
- Ask if the timestamp should be UTC or local time (devlog implies local but confirm if ambiguous).
- Ask whether seconds are required (devlog specifies minute precision — do not add seconds unless asked).
- Confirm whether log file should be rotated or truncated (devlog implies append-only).

8) Where to put changes
- Put the implementation in `logger.c++` (overwrite the empty file), and if you add tests or helper scripts, place them at repo root under `tests/`.

If anything above is unclear or you want me to implement `logger.c++` and a manual test harness now, say so and I will proceed.
