# Clean-code notes (portfolio polish)

Applied in this repo:

- Ownership migrated to `std::unique_ptr` for teams, matches, players, staff, and match events
- Fixed Mandžukić jersey number (`150000` → `17`)
- Added `#include <exception>` in `Exceptions.h`
- Removed unused `MainWindow::setupUi()` declaration
- Layered layout: `src/logic` (Qt-free) vs `src/gui`

Further optional improvements (not blocking publication):

1. Add `const` where getters return non-mutating views more consistently (e.g. `getZwyciezca() const`)
2. Seed RNG via `std::mt19937` instead of `std::rand` / `srand`
3. Extract squad data to JSON/CSV and parse at startup (currently hardcoded in `budujSklady`)
4. Add a headless console target that links only `src/logic`
5. Unit tests (Catch2/GoogleTest) for table sorting and knockout pairing
6. Rename `DrużynaWyjatek` to ASCII `DruzynaWyjatek` for MSVC/source-encoding safety
