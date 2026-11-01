# Parley

A desktop Tox messenger based on the working TokTok/qTox implementation.
Windows/Linux are desktop targets. Android and iOS are not supported builds.

**Development build. Full qTox parity is not certified.** See the
[feature matrix](docs/QTOX_FEATURE_MATRIX.md),
[verification results](docs/VERIFICATION.md) and
[limitations](docs/LIMITATIONS.md) for exact scope.

## Working backend

Private messages, offline outgoing queue, delivery receipts, typing, searchable
SQLCipher history, Tox friend requests, legacy conferences, audio/video calls,
screen sharing, actual file transfers, avatars, presence, profile encryption,
multiple local accounts, proxies and advanced network/AV/privacy controls all
use the imported qTox implementation. No mock call or transfer backend exists.

Parley adds persistent archive/pin/mute/deleted-chat states, unread badges, local
history purge that clears session memory too, account state isolation and a
shared desktop design system with light/dark/AMOLED palettes and system accent
choices. Glass highlights use an opaque fallback; this is not native iOS blur.

## Use

- Create/import a `.tox` identity on the account screen. Password is optional.
- Open **Profile → Logout / Switch accounts** to choose another identity without
  restarting. The old network and AV workers are destroyed before selection.
- Right-click a contact for pin, archive, mute, clear history or delete chat.
- **Delete chat** clears local records and hides the conversation; it keeps the
  Tox contact. **Contacts (including deleted chats)** restores access. A new
  incoming message makes a deleted chat visible again.
- **Remove friend** removes the actual Tox contact, with confirmation and the
  option to remove history. Other clients' history cannot be remotely deleted.
- **Archived chats** in the filter menu shows the archive. Text search includes
  archived conversations. Pin order and unread counters survive restart.
- **Settings → User Interface** contains theme and motion controls.

Downloaded files are not erased by deleting chat history. Finish/cancel active
transfers before clearing history. Do not share exported `.tox` identities.

## Build and distribution

See [build instructions](docs/BUILDING.md). Root CMake builds a native Qt Widgets
application, not the old QML foundation. All applicable qTox settings screens
remain available. `scripts/build-linux.sh` pins c-toxcore v0.2.23 including its
submodule and runs the test suite. CI packages Linux and Windows artifacts.

Use Profile → Export for an interoperable identity backup. This exports the
identity and contacts, **not** messages/downloads. For complete offline backups,
close Parley and copy its entire data directory. Import HAU's old
`HAU/profiles/<uuid>/profile.tox` through the importer using a distinct basename.
Automatic conversion of the old foundation metadata/database is unnecessary
because it contained no messages, and is not performed.

## License

GPL-3.0-or-later, derived from qTox. Original attribution and dependency notices
are preserved. See [provenance](docs/UPSTREAM.md) and `LICENSE`.
