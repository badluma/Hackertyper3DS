# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
make clean        # Remove build artifacts
make              # Build .3dsx and .elf (no CIA)
make cia          # Build everything including .cia (requires bannertool and makerom in PATH)
```

**Prerequisites:** `DEVKITARM` env var must point to your devkitARM installation. CIA builds additionally need `bannertool` and `makerom`.

## Architecture

Single-file C app (`source/main.c`) targeting 3DS ARMv6K hardware via libctru. `source/functions.h` and `source/functions.c` exist but are empty stubs.

**Runtime loop:**
- `aptMainLoop()` drives the main loop; `gspWaitForVBlank()` paces it
- Dual-screen: `PrintConsole topConsole` (top screen) and `bottomConsole` (bottom screen); switch active console with `consoleSelect()`
- ANSI escape codes work for color (`\x1b[32m` etc.) and clear (`\x1b[2J`)

**Global state:**
- `color` — index into `colors[]` array of ANSI codes
- `commandMode` — `enum LanguageMode { BASH, PYTHON, C }`
- `autoRun` — when true, `buttonPressed()` sleeps 10–100ms then returns true without waiting for input

**Controls:**
- Any face/d-pad/shoulder button → print two random commands
- `L+X` → cycle color
- `L+A` → cycle language mode
- `SELECT` → toggle autoRun spam mode
- `START` → exit

**CIA packaging:** `app.rsf` defines title metadata, permissions, and system call access. Assets used: `assets/icon.png` (app icon/SMDH), `assets/banner.png` + `assets/audio.wav` (CIA banner).

## Adding Commands

Each language has a `const char*` array at the top of `main.c` (`bashCommands`, `pythonCommands`, `cCommands`). Add strings to extend. `getRandomCommand()` picks randomly by index; `printRandomCommands()` guarantees the two displayed strings differ.
