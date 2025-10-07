# ATM Virtual Banking System (C++)

A modern, testable C++ ATM simulator with a clean separation between **domain logic** and a **CLI**.  
Accounts include `BankAccount` (base), `SavingAccount` (interest), and `CheckingAccount` (check fee).

[MIT License](LICENSE)

---

## Highlights

- **Clean design**: Domain layer has **no I/O**; pure methods like `deposit/withdraw/transfer` return `bool` + leave messaging to the caller.
- **Two account types**: Savings (interest over N months), Checking (per-check fee).
- **Portable build**: CMake-based, no non-standard dependencies.
- **Tests**: Minimal unit tests via CTest; easy to extend.

