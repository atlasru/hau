# HAU 0.1.0 architecture

- `ProfileStore` resolves Qt AppDataLocation, creates or loads one UUID profile, validates metadata, and atomically writes `profile.tox` with `QSaveFile`. Identity is never regenerated for an existing profile with missing/corrupt state.
- `Database` owns a named Qt SQLite connection. A transactional `schema_migrations` table records version 1. There are no empty chat tables.
- `ToxService` is a dedicated `QThread`. Its `run()` owns the sole `Tox*` through RAII. It reads savedata, creates toxcore, saves a fresh identity before publishing the ID, bootstraps from a centralized node list, iterates with toxcore's requested interval, saves periodically and on shutdown. The UI thread requests shutdown with an atomic flag and joins the worker. No QML or SQLite code calls raw toxcore.
- `AppViewModel` receives typed queued signals from the worker on the UI thread. It exposes profile, ID, connection state and errors to QML. `Main.qml` reads these properties and invokes only the copy command.
- Startup: initialize Qt/logging → profile → database → worker → QML. Shutdown: stop/join worker → close database → destroy services. Failures display an error without replacing user identity.
- The identity file contains secret private key material. Logs contain status and error codes, never savedata or private keys. Storage encryption is deferred.
