# Source and license provenance

Parley desktop is a maintained adaptation of TokTok/qTox, not a claim that
qTox's large backend was independently recreated in one patch.

- Imported repository: https://github.com/TokTok/qTox
- Imported revision: `592c2d3506487c42ffee13e95d67f6efcee2394e`
- Base Parley/HAU revision: `73ca3c5d4981ec48df0d9336b8b9778316e9b512`
- Import date: 2026-11-01
- Upstream desktop version: 1.18.6
- This adaptation: Parley 0.2.0 development, no full-parity certification.

The original HAU identity/bootstrap implementation is preserved, unchanged,
in `legacy/foundation/`; it is not the application launched by the root build.
It could create identities but had no chat, contacts, files or calls. Reusing
qTox's tested Qt/C++ implementation preserves its interoperable Tox protocol,
SQLCipher migrations, AV codecs, transfer engine and detailed settings.

The combined application is GPL-3.0-or-later. See root `LICENSE`; upstream
source headers, contributor list, translation credits, `LICENSES/` (where
present), `res.qrc` and embedded third-party notices retain their attribution.
The foundation's original MIT notice is retained in `legacy/foundation/LICENSE`.
No proprietary relicensing of the combined application is permitted.

Redistributors must provide the exact corresponding source, including Parley
changes, build scripts, dependency revisions and license notices. See
`docs/BUILDING.md`. Source and binary distributions should travel together.
Upstream references and documentation retained under `doc/` describe qTox;
Parley's authoritative feature and verification status is under `docs/`.

Internal CMake target and resource names still use `qtox` to minimize drift.
The executable is `Parley` (`Parley.exe` on Windows). No qTox updater is enabled.
