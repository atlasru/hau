# Verification report

Run on 2026-11-01, Linux x86_64, Ubuntu 24.04, GCC 13.3.0, Qt 6.4.2,
SQLCipher 4.5.6, c-toxcore v0.2.23 with ToxAV enabled. Release build.

## Automated results

The latest complete run passed **40/40 CTest suites**. Two environment-dependent
QTest cases explicitly skipped: desktop notification backend setup (no D-Bus
session bus) and the public bootstrap request. The public bootstrap
HTTP check is an explicit opt-in QTest case and was skipped; deterministic
localhost HTTP response/failure/retry tests run by default. This is not evidence
that the public bootstrap endpoint is reachable from this environment.

- Actual two-profile create/switch/reload, distinct Tox IDs, contact/private-note
  isolation, unread/archive/mute/pin persistence and independent notification/proxy
  settings. Includes wrong-password rejection, encrypted settings content and
  destruction of the old Core before the next login's create handler executes.
- Account download/custom-bootstrap directory isolation.
- Current encrypted identity export and failure preserving the live identity.
- Persistent SQLCipher history clear, empty session/search after clear and new
  messages starting at index zero; reopening ChatHistory cannot restore old data.
- Offline history-disabled session clear.
- Pin precedence in both name/activity sorts; archive/deleted/contact filters;
  text search through the archive.
- **Real localhost interoperability:** Parley Core and a separate plain
  c-toxcore peer connect, send private messages both ways, join a legacy Tox
  conference, and deliver a conference message. A standard GUI qTox instance
  was not operated interactively; the wire-level peer uses its standard library.
- **Real file transfer:** 128 KiB payload compared byte-for-byte on the peer,
  followed by a second offer cancelled by Parley and observed by the remote peer.
- Existing qTox coverage includes friend requests/presence, message dispatch,
  offline queue, notification generation, SQLCipher schemas/migrations,
  serializer, file pause/progress/widgets, image/EXIF/video frames and bootstrap
  list parsing. Some reference tests mock the relevant transport; the new
  transport test above deliberately uses actual UDP/Tox traffic.

Exact output is shipped in `verification/ctest.log` and `LastTest.log` in the
review archive. Test count is suites, not individual assertions.

## Visual/build checks

- Native Linux ELF application built and launched offscreen through real
  create/login/switch UI flows. Actual account and settings screenshots rendered
  and inspected; Designer XML parsed successfully. Shared controls have visible
  keyboard focus and accessible names. This is not a full screen-reader audit.
- Linux artifact requires the documented Qt6/FFmpeg/SQLCipher/OpenAL/system
  libraries. It is not a standalone AppImage.
- Windows MSYS2/UCRT64 build, test and runtime packaging workflow supplied.
  **No Windows build was executed or certified here.** The existing foundation
  MSVC workflow cannot validate the migrated application and has been replaced.
- Android, iOS, macOS, Web and Haiku artifacts were not built.

## Remaining manual/hardware checks

- Live microphone/camera capture, two-device audio/video call quality, group
  audio, screen capture and hardware-device removal/reselection.
- OS desktop notifications, tray behaviour, OS accessibility integration and
  suspend/resume/background handling on each supported desktop platform.
- Real internet DHT/proxy/TCP relay connectivity, public-client GUI acceptance,
  offline/resume and network-change scenarios beyond the local tests.
- End-to-end encrypted password change/removal, full device-local backup/restore,
  high-volume histories and long-running performance/memory profiling.
- Full Liquid Glass/Material 3 Expressive/mobile frontend and modern NGC groups.

Passing these tests does not establish full qTox or cross-platform feature parity.
