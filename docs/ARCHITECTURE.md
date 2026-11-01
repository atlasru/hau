# Parley desktop architecture

The root application is built from pinned qTox Qt/C++ sources. The old HAU QML
identity-only foundation remains in `legacy/foundation/` as historical source.

## Ownership and switching

`AppManager → Nexus → Profile → Core / CoreAV / History` owns the active session.
`Nexus` owns its `Widget` frontend and audio control. Switching emits
`profileAboutToChange`, removes URI handlers, destroys the UI, saves and drains
personal settings, destroys the old profile/workers/database, then opens the
account selector. A new profile is installed before its UI's IPC handlers are
registered. URI adapters are rebound to the new Core. QObject receiver contexts
scope connections to their actual lifetimes.

Only one account is active. A profile lock prevents concurrent identity writes.
The Tox network core, AV processing, camera worker, database and settings writer
retain qTox's threading model; network iteration does not run on the GUI thread.

## Persistence

Each account has its own `<name>.tox`, `<name>.ini`, `<name>.db`; `.tox` and `.ini`
use toxencryptsave when a password is set, while `.db` uses SQLCipher. The imported
SQL schema upgrader supports versions through 11. Personal settings v2 adds
`ParleyChats` (archive, mute, deletion, pin sequence, unread) and `ParleyAccount`
(network, notification, downloads and AV device choices). Missing values reset
to explicit defaults instead of inheriting a previous account's values.

Custom node lists live under `nodes/<profile>/`. Default downloads live under
`files/<profile>/`; explicit destinations can be shared at the user's request.
Plaintext avatars are isolated under the local public key. Encrypted avatars
retain qTox's keyed per-identity filenames. Language, appearance and tray
preferences are application-global. Savedata and settings commits use QSaveFile;
profile export is atomic and reads fresh in-memory identity data.

## Chat state

The same ChatHistory/SQLCipher implementation stores private and conference
history. Purge deletes the SQL records, cancels the remaining offline outgoing
queue and clears session/search state. It refuses active file transfers; those
must finish or cancel first. A generation token discards stale insertion callbacks.
Pin order is the first list sort key. Archive/deletion are filters, not protocol
operations. Incoming messages reopen deleted chats. Mute gates notifications,
sounds and window raising while retaining messages and unread counters.

## Design

`src/parley/designsystem.*` supplies reusable semantic styles to qTox's shared
Style cache. Existing forms and their real backend connections are retained.
The account selector uses a resizable layout instead of fixed coordinates.
Native backdrop blur/refraction and full mobile Material components remain
outside this desktop adaptation. See LIMITATIONS.md for explicit boundaries.
