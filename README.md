# Euro 2012 Tournament Simulator

Modern C++17 tournament engine with a Qt 6 desktop UI that simulates **UEFA Euro 2012** — group stage, knockout bracket, squads, match events, and live statistics.

The domain logic is **Qt-free** and talks to the GUI only through a clean public API (`std::string`, `std::vector`, POD structs). That separation keeps the simulator reusable and easy to extend (CLI, tests, or another front-end).

---

## Features

| Area | What it does |
|------|----------------|
| **Group stage** | Round-robin (every team plays every other) in groups A–D; points, GF/GA, GD tie-breakers |
| **Knockout bracket** | Quarter-finals → semi-finals → final; draws resolved via simplified penalty shoot-out |
| **Match engine** | Minute-by-minute loop (90') generating polymorphic events: shots, fouls, cards, corners, substitutions |
| **Squads & staff** | Historical Euro 2012-inspired line-ups; player & coaching-staff class hierarchies |
| **Statistics** | Group tables, top scorers, champion announcement |
| **Qt GUI** | Dark-themed Widgets UI: run stages, browse tables, coloured match log |

---

## Architecture

```
src/
├── main.cpp                 # QApplication entry
├── logic/                   # Pure C++17 domain (no Qt)
│   ├── Turniej.*            # Groups + knockout orchestration
│   ├── Mecz.*               # Match simulation loop
│   ├── Druzyna.*            # Team ownership of players/staff
│   ├── Zawodnik.*           # Player hierarchy (GK, CB, FB, MF, ST)
│   ├── ZdarzenieMeczowe.*   # Match-event hierarchy
│   ├── CzlonekSztabu.*      # Coaching staff hierarchy
│   └── Exceptions.h         # Custom exception tree
└── gui/
    ├── MainWindow.*         # Qt Widgets front-end
    └── MainWindow.ui        # Designer form
```

**Layering rule:** `gui` depends on `logic`; `logic` never includes Qt headers.

---

## C++ concepts demonstrated

- **OOP** — three inheritance trees (players, events, staff) with pure virtual methods and polymorphic dispatch
- **STL** — `std::vector`, `std::array`, `std::pair`, `std::sort` / `std::find_if`, lambdas
- **Modern memory** — `std::unique_ptr` ownership for teams, matches, players, staff, and events
- **Rule of Five / deleted copy** — owning types are non-copyable where appropriate
- **Exceptions** — domain-specific hierarchy rooted at `EuroWyjatek` (`std::exception`)
- **Separation of concerns** — simulation core vs. presentation (cout capture → HTML log in GUI)

---

## Requirements

- **CMake** ≥ 3.16
- **C++17** compiler (MSVC, GCC, or Clang)
- **Qt 6** (Widgets module)

Optional: Qt Creator + `euro2012.pro` (qmake) if you prefer that workflow.

---

## Build & Run (CMake)

### Windows (PowerShell)

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvcXXXX_64"
cmake --build build --config Release
.\build\Release\Euro2012Simulator.exe
```

Adjust `CMAKE_PREFIX_PATH` to your local Qt 6 kit.

### Linux / macOS

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"   # or your Qt path
cmake --build build -j
./build/Euro2012Simulator
```

### Qt Creator (qmake)

1. Open `euro2012.pro`
2. Select a Qt 6 kit
3. Build & Run

---

## Demo flow (GUI)

1. Launch **Euro2012Simulator**.
2. On the **Group Stage** tab, click **ROZPOCZNIJ FAZĘ GRUPOWĄ**.
3. Inspect the coloured match log and switch groups in the combo box to refresh the table.
4. Open the **Knockout** tab and start the cup phase.
5. Read the champion banner and the top scorers table.

### Example console-style output (captured into the GUI log)

```text
=== Grupa A - Wyniki Meczy ===

  Polska             1 : 2 Niemcy
  [23'] Strzal - GOL! Robert Lewandowski
  ...

=== Tabela: Grupa A ===
Niemcy          Pkt: 7 | G+: 5 G-: 2 Bilans: 3
...

--- FINAL EURO 2012 ---
  Hiszpania          2 : 1 Wlochy
  AWANS: Hiszpania

MISTRZ EURO 2012: Hiszpania
```

*(Scores are stochastic — each run produces a different tournament.)*

---

## Project hygiene

| File | Purpose |
|------|---------|
| `.gitignore` | Ignores `build/`, `cmake-build-*/`, binaries, Qt/IDE junk |
| `.clang-format` | LLVM-based style, 100-col, C++17 |
| `CMakeLists.txt` | Target-based build, `-Wall -Wextra` (or `/W4` on MSVC) |

Format sources:

```bash
clang-format -i src/logic/*.{h,cpp} src/gui/*.{h,cpp} src/main.cpp
```

---

## Author

**Jakub Guzikowski**
