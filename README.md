# Embedded C Smart Home Simulation

Small, educational smart-home simulator written in C. This repo demonstrates a simple console-based simulation of lights, fan control, door lock (password), and a simulated temperature sensor.

**Highlights**
- Portable C (POSIX & Windows) with simple build instructions
- Save/load settings to a local file (simulated EEPROM)
- GitHub Actions build workflow

## Build (Linux / macOS)

Requirements: `gcc` (or `clang`)

```bash
gcc -std=c11 -Wall -Wextra -O2 -o smart_home_sim smart_home_sim.c
./smart_home_sim
```

## Build (Windows - PowerShell)

Requirements: `gcc` from MinGW/WSL or MSYS2

Open PowerShell in the repo root and run:

```powershell
# Use bundled build script
.\build.ps1
```

## Project layout

- `smart_home_sim.c` — main source file
- `.github/workflows/ci.yml` — CI build workflow
- `Makefile` — simple build/clean targets
- `build.ps1` — Windows build helper

## Contributing

Contributions welcome — see `CONTRIBUTING.md` for details.

## License

This project is available under the MIT License — see `LICENSE`.
