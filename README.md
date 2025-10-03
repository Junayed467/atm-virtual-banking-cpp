# ATM Virtual Banking System (C++)

Modernized, testable C++ implementation of a simple ATM simulator with **OOP + inheritance**:
- `BankAccount` (base), `SavingAccount` (interest), `CheckingAccount` (check fee)
- Safe domain API (`bool deposit/withdraw`) with **no UI in the model**
- CLI ATM driver (`atm_app`) with **input validation** and normalized choices
- Minimal tests runnable via CTest (no external framework)

> Originally a student VS Code project; refactored for maintainability, CI, and portability.

## Features
- Deposit, Withdraw, Transfer (between Savings/Checking), Write Check (with fee)
- Savings interest accrual over N months (simple nominal % per year)
- Clean separation of concerns: domain layer vs CLI

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/atm_app
```

Run tests:
```bash
ctest --test-dir build --output-on-failure
```

## Design Notes
- **Domain layer** is header-only for simplicity; no `std::cout` inside.
- **Error handling** via return `bool` and caller-side messages; this keeps the model reusable.
- **Extensibility**: swap the in-memory user map for persistence (files/SQLite) without touching ATM logic.

## Roadmap
- File-backed persistence or SQLite
- Multi-user auth and account registry
- Monthly compounding option
- JSON config for fees/rates

