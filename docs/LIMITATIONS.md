# Unsupported and unfinished work

No full parity claim. An implementation inherited from qTox is not automatically
an end-to-end verified feature on every platform.

## Protocol/platform restrictions

- qTox's conferences use the legacy Tox conference API. There are no owner/admin
  roles, kicks, bans, passwords, permissions or public/private room policies in
  that protocol. Modern c-toxcore NGC groups are a separate API and remain
  **missing** in this version. Interoperation with qTox conferences is preserved.
- Legacy conference history is persisted by the reference ChatHistory/SQLCipher
  implementation and supports the same local purge. Modern NGC groups are separate.
- One account is connected at a time. Background delivery to inactive profiles
  is unsupported. Unread counts represent messages received by the active
  profile; they are not a server-side inbox or remote read state.
- Android/iOS need a dedicated lifecycle, foreground service, permissions,
  notifications, file picker and adaptive frontend. The inherited experimental
  Android build files are not a supported Parley Android application.
- macOS/Web/Haiku upstream files are retained for source completeness but not
  supported or built in this migration.
- Tox has no server mailbox. Offline messages stay in the local outgoing queue
  until both peers can communicate; delivery is not guaranteed when the sender
  is shut down. Clearing local history cancels the remaining local queue, but
  cannot retract messages already sent over Tox.
- Calls and live transfers do not continue through an account switch. They end
  when the previous workers are disposed. Other clients retain their data.
- Resume is supported during the same transfer/session and through qTox's own
  broken-transfer logic; arbitrary download continuation across profile changes
  or process restarts is not certified.

## UI work still incomplete

The current frontend is native Qt Widgets using reusable qTox components and
Parley design tokens: rounded controls, layered gradients, larger chat rows,
focus states, unread badges, light/dark/AMOLED and desktop system-accent choices.

- Native Liquid Glass refraction/backdrop blur is **not implemented**. The
  fallback is deliberately opaque and does not blur the text/control tree.
- Full Material 3 Expressive components, Android wallpaper Dynamic Color,
  spring navigation and a mobile adaptive layout remain **unfinished**.
- Reduce motion is saved; the current surfaces use no mandatory animations.
  This does not claim a complete operating-system reduced-motion integration.
- A dedicated Calls dashboard, a cross-chat image gallery and a one-tap
  account switcher are unfinished. Calls work from real conversation headers;
  account selection works through Profile → Logout/Switch accounts.
- Archive/pin/mute, counters and local history purge work for private chats and
  legacy conferences. Deleting a conference chat keeps conference membership.
- Pin ordering is stable by the order in which chats were pinned. Manual drag
  reordering of pinned chats is not implemented. Existing qTox circle ordering
  and name/activity sort remain available.

## Data and security scope

- `.tox`, personal `.ini` settings and `.db` history are profile-specific;
  passwords use toxencryptsave and SQLCipher. Unencrypted profiles remain
  plaintext. Appearance, language, tray behaviour and log preferences are
  application-global. A password is not protection from a compromised OS.
- Unencrypted avatars are namespaced by the local identity. Encrypted avatars
  retain qTox's keyed per-identity names. Defaults for download and custom-node
  folders are per account. Explicitly shared download folders are user-selected
  external locations, not account-private storage.
- Full one-click history/files backup+restore is unfinished. Profile Export is
  an interoperable **identity-only** backup. A complete offline copy is possible
  with the application closed. Automatic profile overwrite is refused.
- History purge is transactional and clears session memory/search. Active file
  transfers must finish/cancel first. It removes database records, not arbitrary
  downloaded files or guaranteed forensic remnants on SSD/OS backups.
- Device call quality, camera capture, desktop notifications/tray integration,
  screen sharing and power/suspend/reconnect require platform hardware/manual
  checks listed in `VERIFICATION.md`.
- Spell checking is conditional on KF6 Sonnet; builds without that dependency
  do not provide it. Automatic update checking is disabled for this fork to
  avoid offering upstream qTox binaries as Parley upgrades.
